/*
 *  LogWriter.cpp
 *  ECSLogger
 *
 *  Created by M. Hank Kiedrowski on 8/4/10.
 *  Copyright 2010 Clockwork Active Media Systems. All rights reserved.
 *
 */

#include "logger/logwriter.h"


namespace pixelpipe {
	
LogWriter::LogWriter( const std::string& type) {
	_type = type;
}

LogWriter::~LogWriter() {
}

const std::string LogWriter::type() {
	return _type;
}

}	// namespace pixelpipe
