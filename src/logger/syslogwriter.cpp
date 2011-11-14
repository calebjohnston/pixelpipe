/*
 *  SyslogWriter.cpp
 *  XMPPBase
 *
 *  Created by M. Hank Kiedrowski on 8/24/10.
 *  Copyright 2010 Clockwork Active Media Systems. All rights reserved.
 *
 */

#include "logger/syslogwriter.h"

#include <boost/thread/mutex.hpp>
#include <syslog.h>
#include <stdarg.h>


namespace pixelpipe {
	
boost::mutex SyslogWriter::_syslogOutMutex;

SyslogWriter::SyslogWriter( ) :
LogWriter( "Syslog Writer" )
{
}

SyslogWriter::~SyslogWriter( ) {
}

void SyslogWriter::Write( Logger::LEVEL level, const std::string& identity, const std::string& message ) {
	
	boost::mutex::scoped_lock lock( _syslogOutMutex );
	
	std::ostringstream m;
	m << "[" << Logger::LoggerLevelAsString( level ) << "] " << message;
	
	openlog( identity.c_str(), LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL5 );
	setlogmask( LOG_UPTO( LOG_NOTICE ) );
	syslog( _map_level( level ), "%s", m.str().c_str() );
	
}

int SyslogWriter::_map_level( const Logger::LEVEL& level ) {
	
	switch ( level ) {
		case Logger::ALERT:
			return LOG_ALERT;
		case Logger::CRIT:
			return LOG_CRIT;
		case Logger::ERROR:
			return LOG_ERR;
		case Logger::WARN:
			return LOG_WARNING;
		case Logger::INFO:
		case Logger::TODO:
		case Logger::DEV:
		case Logger::DEBUG:
			return LOG_NOTICE;
	}
	return LOG_DEBUG;
}

}	// namespace pixelpipe {