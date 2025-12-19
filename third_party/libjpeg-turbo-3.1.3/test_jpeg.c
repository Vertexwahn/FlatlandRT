#include <stdio.h>
#include <jpeglib.h>

int main() {
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    
    printf("libjpeg-turbo initialized successfully\n");
    printf("JPEG library version: %d\n", JPEG_LIB_VERSION);
    
    jpeg_destroy_compress(&cinfo);
    return 0;
}
