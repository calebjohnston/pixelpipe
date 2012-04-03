/*
 *  LogWriter.h
 *  ECSLogger
 *
 *  Created by M. Hank Kiedrowski on 8/4/10.
 *  Copyright 2010 Clockwork Active Media Systems. All rights reserved.
 *
 */

#ifndef __PIPELINE_LogWriter_H
#define __PIPELINE_LogWriter_H

#include <string>
#include "logger/logger.h"

namespace pixelpipe {

class LogWriter {
	
private:
	std::string _type;
	
public:
	LogWriter( const std::string& type );
	virtual ~LogWriter();
	virtual void Write( Logger::LEVEL level, const std::string& identity, const std::string& message ) = 0;
	const std::string type();

};

}	// namespace pixelpipe

#endif	//__PIPELINE_LogWriter_H