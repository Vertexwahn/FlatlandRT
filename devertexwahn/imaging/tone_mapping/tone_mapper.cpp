/*
 *  SPDX-FileCopyrightText: Copyright 2025 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/image.hpp"
#include "imaging/io/io.hpp"
#include "imaging/io/io_openexr.hpp"
#include "imaging/tone_mapping/tone_mapping.hpp"

#include "core/logging.hpp"

#include "boost/program_options.hpp"

#include <iostream>

using namespace boost::program_options;
using namespace de_vertexwahn;

class CommandLineOptions {
public:
    CommandLineOptions() {
        desc.add_options()
            ("filename", value<std::string>(), "Filename of an HDR input image")
            ("outfile", value<std::string>(), "Filename of LDR output file")
            ("operator", value<std::string>()->default_value("agx"),
                "Tone mapping operator: 'agx' (Blender-style filmic, default) "
                "or 'srgb' (plain sRGB encode)")
            ("help,h", "Help screen")
            ("version,v", "print version string");
    }

    void parse_command_line_options(int argc, char **argv) {
        try {
            store(parse_command_line(argc, argv, desc), vm);
            notify(vm);
        } catch (const error &ex) {
            LOG_ERROR_WITH_LOCATION("Error parsing command line arguments: {}", ex.what());
        }
    }

    bool handle_help() const {
        if (vm.count("help")) {
            std::cout << desc << std::endl;
            return true;
        }

        return false;
    }

    bool handle_version() const {
        if(vm.count("version")) {
            std::cout << "Tone Mapper 0.0.1" << std::endl;
            return true;
        }

        return false;
    }

    bool handle_standard_commands() const {
        if(handle_help()) {
            return true;
        }

        if(handle_version()) {
            return true;
        }

        return false;
    }

    std::string filename() const {
        return vm["filename"].as<std::string>();
    }

    std::string outfile() const {
        return vm["outfile"].as<std::string>();
    }

    std::string tone_mapping_operator() const {
        return vm["operator"].as<std::string>();
    }

private:
    boost::program_options::variables_map vm;
    boost::program_options::options_description desc{"Options"};
};

class ToneMapper {
public:
    ToneMapper() {}

    int run(int argc, char **argv) {
        LOG_INFO_WITH_LOCATION("Hello from ToneMapper");

        CommandLineOptions command_line_options;
        command_line_options.parse_command_line_options(argc, argv);
        bool handled = command_line_options.handle_standard_commands();
        if (handled) { return  0; }

        LOG_INFO_WITH_LOCATION("Infile: {}", command_line_options.filename());
        LOG_INFO_WITH_LOCATION("Outfile: {}", command_line_options.outfile());

        Image3f hdr_image = load_image_openexr(command_line_options.filename());

        const std::string op = command_line_options.tone_mapping_operator();
        LOG_INFO_WITH_LOCATION("Tone mapping operator: {}", op);
        Image3b ldr_image = (op == "srgb")
            ? srgb_operator(hdr_image)
            : agx_operator(hdr_image);

        store_image(command_line_options.outfile(), ldr_image);

        return 0;
    }
};

int main(int argc, char **argv) {
    ToneMapper tm;
    return tm.run(argc, argv);
}
