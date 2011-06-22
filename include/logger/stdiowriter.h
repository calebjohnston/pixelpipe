/*
 *  StdOutWriter.h
 *  ECSLogger
 *
 *  Created by Jerry Belich on 8/11/10.
 *  Copyright 2010 Clockwork. All rights reserved.
 *
 */

#ifndef __PIPELINE_StdOutWriter_H
#define __PIPELINE_StdOutWriter_H

#include "logger/logwriter.h"

namespace boost {
	class mutex;
}

namespace pipeline {
	
class StdOutWriter : public LogWriter {
	
public:
	StdOutWriter();
	~StdOutWriter();

	void Write( Logger::LEVEL level, const std::string& identity, const std::string& message );
	
private:
	static boost::mutex _coutMutex;
	
};

}	// namespace pipeline

#endif	//__PIPELINE_StdOutWriter_H