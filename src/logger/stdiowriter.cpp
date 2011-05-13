/*
 *  StdOutWriter.cpp
 *  ECSLogger
 *
 *  Created by Jerry Belich on 8/11/10.
 *  Copyright 2010 Clockwork. All rights reserved.
 *
 */

#include "logger/stdiowriter.h"

#include <iostream>

#include <boost/thread/mutex.hpp>

namespace fdl {
namespace logger {
	
boost::mutex StdOutWriter::_coutMutex;

StdOutWriter::StdOutWriter() :
LogWriter( "Standard Output" )
{
}

StdOutWriter::~StdOutWriter() {
}

void StdOutWriter::Write( Logger::LEVEL level, const std::string& identity, const std::string& message ) {
	boost::mutex::scoped_lock lock( _coutMutex );
	std::cout << "[" << Logger::CurrentTime() << " " << Logger::LoggerLevelAsString( level ) << "] " <<  message << std::endl;
}

}	// namespace logger
}	// namespace fdl