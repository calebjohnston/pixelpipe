/*
 *  SyslogWriter.h
 *  XMPPBase
 *
 *  Created by M. Hank Kiedrowski on 8/24/10.
 *  Copyright 2010 Clockwork Active Media Systems. All rights reserved.
 *
 */

#ifndef __PIPELINE_SyslogWriter_H
#define __PIPELINE_SyslogWriter_H

#include "logger/logwriter.h"

namespace boost {
	class mutex;
}

namespace pixelpipe {
	
class SyslogWriter : public LogWriter {
	
private:
	static boost::mutex _syslogOutMutex;
	
public:
	SyslogWriter( );
	~SyslogWriter( );
	
	void Write( Logger::LEVEL level, const std::string& identity, const std::string& message );
	
private:
	int _map_level( const Logger::LEVEL& level );
};

}	// namespace pixelpipe

#endif	// __PIPELINE_SyslogWriter_H