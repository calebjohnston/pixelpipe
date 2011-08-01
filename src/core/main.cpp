/**
 * @file
 * @author  Caleb Johnston <caleb.johnston@example.com>
 * @version 0.1
 *
 * @section LICENSE
 *
 * Object-order rendering pipeline
 * Copyright (C) 2011 by Caleb Johnston
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 * Root class for all functionality
 */

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <iostream>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>

#include "core/pixelpipe.h"
#include "logger/logger.h"
#include "logger/stdiowriter.h"

#ifndef TARGET_VERSION_MAJOR
#define TARGET_VERSION_MAJOR 99
#endif

#ifndef TARGET_VERSION_MINOR
#define TARGET_VERSION_MINOR 99
#endif

/**
 * A helper function to simplify the CLI processing
 */
template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(std::cout, " ")); 
    return os;
}

/**
 * Main function, process inputs
 *
 * @return integer status of the program of range [0,1]
 */
int main(int argc, char **argv)
{
	// figure out how to automate this output...
	std::cout << "PixelPipe version " << TARGET_VERSION_MAJOR << "." << TARGET_VERSION_MINOR << " of " << __DATE__ << " at " << __TIME__ << std::endl;
	std::cout << "Copyright (c) 2011 Caleb Johnston" << std::endl;
	std::cout << "The source code to PixelPipe is covered by the GNU GPL." << std::endl;
	std::cout << "See the LICENSE file for the conditions of the license." << std::endl;
	
	pipeline::LogWriter* logger = new pipeline::StdOutWriter();
	pipeline::Logger::SetIdentity("PixelPipe");
	pipeline::Logger::RegisterWriter(logger);
	
	int loggerLevel;
	try {
		po::options_description desc("Allowed options");
		desc.add_options()
		    ("help,H", "produce help message")
		    ("input-file,I", po::value<std::string>(), "input scene file")
		    // ("image-size,S", po::value< std::vector<double> >(), "[ XxY ] | [ X Y ]")
			("verbose,V", po::value<int>(&loggerLevel)->default_value(1), "Verbose logging?")
		;

        po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc, po::command_line_style::unix_style), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << "Usage: options_description [options]\n";
            std::cout << desc;
            return 0;
        }

        if (vm.count("input-file")) {
            std::cout << "Include paths are: " << vm["input-file"].as< std::vector<std::string> >() << "\n";
        }

		std::string levelLabel = "";
		if(loggerLevel >= 0){
			levelLabel = pipeline::Logger::LoggerLevelAsString(pipeline::Logger::INFO);
			pipeline::Logger::PushLevel(pipeline::Logger::INFO);
		}
		if(loggerLevel >= 1){
			levelLabel = pipeline::Logger::LoggerLevelAsString(pipeline::Logger::DEV);
			pipeline::Logger::PushLevel(pipeline::Logger::DEV);
		}
		if(loggerLevel >= 2){
			levelLabel = pipeline::Logger::LoggerLevelAsString(pipeline::Logger::DEBUG);
			pipeline::Logger::PushLevel(pipeline::Logger::DEBUG);
		}
		
		std::cout << "Verbose logging enabled. Level is " << levelLabel << " (" << loggerLevel << ")" << std::endl;
		
    }
    catch(std::exception& e) {
        std::cerr << " * error: " << e.what() << "\n";
        return 1;
    }
    catch(...) {
        std::cerr << "Exception of unknown type!\n";
    }	

	// start it up!
	pipeline::PixelPipeWindow* app = new pipeline::PixelPipeWindow();
	app->init();
	
	return app->run();
}
