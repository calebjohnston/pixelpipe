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

#include "logger/logger.h"

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
	std::cout << "FDL version " << TARGET_VERSION_MAJOR << "." << TARGET_VERSION_MINOR << " of " << __DATE__ << " at " << __TIME__ << std::endl;
	std::cout << "Copyright (c) 2011 Caleb Johnston" << std::endl;
	std::cout << "The source code to FDL is covered by the GNU GPL." << std::endl;
	std::cout << "See the LICENSE file for the conditions of the license." << std::endl;
	
	try {
        int opt;
		po::options_description desc("Allowed options");
		desc.add_options()
		    ("help,H", "produce help message")
		    ("input-file,I", po::value<std::string>(), "input scene file")
		    ("output-format,O", po::value<std::string>(), "output file format")
			("output-name,N", po::value<std::string>(), "output name with format")
		    ("image-size,S", po::value< std::vector<double> >(), "[ XxY ] | [ X Y ]")
			("verbose,V", po::value<int>(&opt)->default_value(1), "Verbose logging?")
		;

        po::variables_map vm;        
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << "Usage: options_description [options]\n";
            std::cout << desc;
            return 0;
        }

        if (vm.count("input-file")) {
            std::cout << "Include paths are: " << vm["input-file"].as< std::vector<std::string> >() << "\n";
        }

        if (vm.count("output-name")) {
            std::cout << "Output name is: " << vm["output-name"].as<std::string>() << "\n";
        }

        if (vm.count("output-format")) {
            std::cout << "Output format is: " << vm["output-format"].as<std::string>() << "\n";
        }

        if (vm.count("verbose")) {
            std::cout << "Verbosity enabled. Level is " << vm["verbose"].as<int>() << "\n";
        }

/* MUST ADD SOME MORE DECLARATIONS HERE!!! */

		pipeline::Logger out;
    }
    catch(std::exception& e) {
        std::cerr << " * error: " << e.what() << "\n";
        return 1;
    }
    catch(...) {
        std::cerr << "Exception of unknown type!\n";
    }

	// start it up!

    return 0;
}
