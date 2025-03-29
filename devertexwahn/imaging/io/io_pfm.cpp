// more details here regarding the PFM Portable FloatMap Image Format:
// https://www.pauldebevec.com/Research/HDR/PFM/

#include "imaging/io/io_pfm.hpp"

#include <fstream>

DE_VERTEXWAHN_BEGIN_NAMESPACE

//----------------------------------------------------------------------------
// The following code section has been copied.
// Copyright by Bryan Henderson, San Jose, CA April 2004.
// Contributed to the public domain by its author.
// Minor modifications by Julian Amann distributed under the same license conditions (public domain)
//----------------------------------------------------------------------------

enum endian {ENDIAN_BIG, ENDIAN_LITTLE};

static enum endian
thisMachineEndianness(void) {
/*----------------------------------------------------------------------------
   Endianness is a component of the format in which a machine represents
   a number in memory or a register.  It is the only component of the format
   that varies among typical machines.

   Big endianness is the natural format.  In this format, if an integer is
   4 bytes, to be stored at memory address 100-103, the most significant
   byte goes at 100, the next most significant at 101, and the least
   significant byte at 103.  This is natural because it matches the way
   humans read and write numbers.  I.e. 258 is stored as 0x00000102.

   Little endian is extremely common because it is used by IA32.  In the
   example above, the least significant byte goes first, so 258 would be
   stored as 0x02010000.

   You can extend this concept to floating point numbers, even though the
   bytes of a floating point number differ by more than significance.
-----------------------------------------------------------------------------*/
    short const testNumber = 0x0001;

    unsigned char * const storedNumber = (unsigned char *)&testNumber;
    enum endian endianness;

    if (storedNumber[0] == 0x01)
        endianness = ENDIAN_LITTLE;
    else
        endianness = ENDIAN_BIG;

    return endianness;
}

typedef union {
    unsigned char bytes[4];      /* as read from the file */
    float value;
    /* This is valid only if the pfmSample has the same endianness
       as the machine we're running on.
    */
} pfmSample;

static float
floatFromPfmSample(pfmSample   const sample,
                   enum endian const pfmEndianness) {
/*----------------------------------------------------------------------------
Type converter
-----------------------------------------------------------------------------*/
    static enum endian machineEndianness = thisMachineEndianness();

    if (machineEndianness == pfmEndianness) {
        return sample.value;
    } else {
        pfmSample rightEndianSample;
        unsigned int i, j;

        for (i = 0, j = sizeof(sample.bytes)-1;
             i < sizeof(sample.bytes);
             ++i, --j)

            rightEndianSample.bytes[i] = sample.bytes[j];

        return rightEndianSample.value;
    }
}

struct pfmHeader {
    unsigned int width;
    unsigned int height;
    bool color;
    float scaleFactor;
    enum endian endian;
};

static void
readPfmHeader(FILE *             const ifP,
              struct pfmHeader * const pfmHeaderP) {

    int firstChar;
    int secondChar;
    float scaleFactorEndian;

    firstChar = fgetc(ifP);
    if (firstChar == EOF)
        throw std::runtime_error("Error reading first character of PFM file");
    secondChar = fgetc(ifP);
    if (secondChar == EOF)
        throw std::runtime_error("Error reading second character of PFM file");

    if (firstChar != 'P' || (secondChar != 'F' && secondChar != 'f'))
        throw std::runtime_error("First two characters for a valid PFM file must be 'PF' or 'Pf'.");

    {
        int whitespace;

        whitespace = fgetc(ifP);
        if (whitespace == EOF)
            throw std::runtime_error("Error reading third character of PFM file");

        if (!isspace(whitespace))
            throw std::runtime_error("The 3rd character of the input file is not whitespace.");
    }
    {
        int rc;
        char whitespace;

        rc = fscanf(ifP, "%u %u%c",
                    &pfmHeaderP->width, &pfmHeaderP->height, &whitespace);

        if (rc == EOF)
            throw std::runtime_error("Error reading the width and height from input file.");
        else if (rc != 3)
            throw std::runtime_error("Invalid input file format where width and height "
                     "are supposed to be (should be two positive decimal "
                     "integers separated by a space and followed by "
                     "white space)");

        if (!isspace(whitespace))
            throw std::runtime_error("Invalid input file format -- some character instead of "
                     "white space after height");

        if (pfmHeaderP->width == 0)
            throw std::runtime_error("Invalid input file: image width is zero");
        if (pfmHeaderP->height == 0)
            throw std::runtime_error("Invalid input file: image height is zero");
    }
    {
        int rc;
        char whitespace;

        rc = fscanf(ifP, "%f%c", &scaleFactorEndian, &whitespace);

        if (rc == EOF)
            throw std::runtime_error("Error reading the scale factor from input file.");
        else if (rc != 2)
            throw std::runtime_error("Invalid input file format where scale factor "
                     "is supposed to be (should be a floating point decimal "
                     "number followed by white space");

        if (!isspace(whitespace))
            throw std::runtime_error("Invalid input file format -- '%c' instead of "
                     "white space after scale factor");
    }

    pfmHeaderP->color = (secondChar == 'F');
    /* 'PF' = RGB, 'Pf' = monochrome */

    if (scaleFactorEndian > 0.0) {
        pfmHeaderP->endian = ENDIAN_BIG;
        pfmHeaderP->scaleFactor = scaleFactorEndian;
    } else if (scaleFactorEndian < 0.0) {
        pfmHeaderP->endian = ENDIAN_LITTLE;
        pfmHeaderP->scaleFactor = - scaleFactorEndian;
    } else
        throw std::runtime_error("Scale factor/endianness in PFM header is 0");
}


//-----------------------------------------------------------------

ReferenceCounted<Image3f> load_image_pfm(std::string_view filename) {
    FILE* ifP = fopen(filename.data(), "rb");;
    struct pfmHeader pfmHeader;
    readPfmHeader(ifP, &pfmHeader);

    std::unique_ptr<pfmSample[]> pfmBuffer(new pfmSample[pfmHeader.width * pfmHeader.height * 3]);
    unsigned int rc = fread(pfmBuffer.get(), sizeof(pfmSample), pfmHeader.width * pfmHeader.height * 3, ifP);

    if (rc != pfmHeader.width * pfmHeader.height * 3) {
        throw std::runtime_error("Invalid pfm read");
    }

    ReferenceCounted<Image3f> image = make_reference_counted<Image3f>(pfmHeader.width, pfmHeader.height);

    auto endian = thisMachineEndianness();

    for (int y = 0; y < image->height(); ++y) {
        for (int x = 0; x < image->width(); ++x) {
            ColorRGB3f color{
                floatFromPfmSample(pfmBuffer[(y * image->width() + x) * 3 + 0], pfmHeader.endian),
                floatFromPfmSample(pfmBuffer[(y * image->width() + x) * 3 + 1], pfmHeader.endian),
                floatFromPfmSample(pfmBuffer[(y * image->width() + x) * 3 + 2], pfmHeader.endian),
            };
            image->set_pixel(x,image->height()-y-1,color);
        }
    }

    fflush(ifP);
    fclose(ifP);

    return image;
}

bool store_pfm(std::string_view filename, const Image3f &image) {
    std::ofstream file(filename.data(), std::ios::binary);
    file << "PF" << std::endl;
    file << image.width() << " " << image.height() << std::endl;
    file << "-1.0" << std::endl;

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            auto color = image.get_pixel(x,image.height()-y-1);
            file.write((char*) &color, sizeof(color));
        }
    }

    return false;
}

DE_VERTEXWAHN_END_NAMESPACE
