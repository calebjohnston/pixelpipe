/*
 *  Logger.cpp
 *  ECSLogger
 *
 *  Created by M. Hank Kiedrowski on 8/4/10.
 *  Copyright 2010 Clockwork Active Media Systems. All rights reserved.
 *
 */



#include "logger/logger.h"
#include "logger/logwriter.h"

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <iostream>
#include <locale>

namespace pixelpipe {
	
int Logger::_level = Logger::DEFAULT_REPORTING_LEVEL;
std::vector<int> Logger::_levelStack( 1, Logger::DEFAULT_REPORTING_LEVEL );
std::list<LogWriter *> Logger::_registeredWriters;
std::string Logger::_identity = "Logger";

Logger::Logger() :
_local_identity( NULL ) {
}

Logger::~Logger() {
	WriteMessage();
	if( _local_identity != NULL ) {
		delete _local_identity;
	}
}

void Logger::SetIdentity( const std::string& identity ) {
	Logger::_identity = identity;
}

const std::string Logger::GetIdentity() {
	return Logger::_identity;
}

const int Logger::SetLevel( const int level ) {
	
	int old_level = Logger::_level;
	int l;
	
	if ( level >= Logger::LEVEL_FLOOR ) {
		l = level;
	}
	else {
		l = Logger::DEFAULT_REPORTING_LEVEL;
	}
	
	Logger::_level = l;
	
	// clear the level stack and set the current value
	Logger::_levelStack.clear();
	Logger::_levelStack.push_back( Logger::_level );
	return old_level;
}

const int Logger::GetLevel() {
	return Logger::_level;
}


void Logger::RegisterWriter( LogWriter * writer ) {
	std::list<LogWriter *>::iterator itr;
	
	for (itr = _registeredWriters.begin(); itr != _registeredWriters.end(); itr++) {
		if (writer->type() == (*itr)->type()) {
			return;
		}
	}
	Logger::_registeredWriters.push_back( writer );
}


void Logger::UnRegisterWriter( LogWriter * writer ) {
	
	Logger::_registeredWriters.remove( writer );
}


void Logger::PushLevel( Logger::LEVEL level ) {

	Logger::_levelStack.push_back( Logger::_level );
	
	Logger::_level = Logger::_level | level;
	
}


void Logger::PopLevel() {
	
	if( Logger::_levelStack.empty() == false ) {
		Logger::_level = Logger::_levelStack.back();
		Logger::_levelStack.pop_back();
	}
}


bool Logger::CanLog( Logger::LEVEL compare ) {
	bool isWithin = 0;
	
	if (!(_level >= Logger::LEVEL_FLOOR) || !(compare >= Logger::LEVEL_FLOOR)) {
		return false;
	}
	
	isWithin = ( ( _level & compare ) != 0 );
	
	return isWithin;
}


std::ostringstream& Logger::Log( Logger::LEVEL level ) {
	_messageLevel = level;

	return _message;
}

std::ostringstream& Logger::Log( Logger::LEVEL level, const std::string& identity ) {
	
	if( _local_identity == NULL ) {
		_local_identity = new std::string( identity );
	}
	return Log( level );
}


void Logger::WriteMessage() {
	
	for ( std::list<LogWriter *>::iterator it = Logger::_registeredWriters.begin(); it != Logger::_registeredWriters.end(); it++ ) {
		if( _local_identity != NULL ) {
			( *it )->Write( _messageLevel, *_local_identity, _message.str() );
		}
		else {
			( *it )->Write( _messageLevel, GetIdentity(), _message.str() );
		}
	}
	
}


std::string Logger::CurrentTime() {
	
	std::ostringstream dateTime;
	const boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
	boost::posix_time::time_facet* const f = new boost::posix_time::time_facet("%d-%b-%Y %H:%M:%S");
	dateTime.imbue(std::locale(dateTime.getloc(),f));
	dateTime << now;
	return dateTime.str();
}

std::string Logger::LoggerLevelAsString( Logger::LEVEL level ) {
	
	std::ostringstream debugLevel;
	debugLevel.flags( std::ios::right );

	switch ( level ) {
		case Logger::ALERT:
			debugLevel << "ALERT";
			break;
		case Logger::CRIT:
			debugLevel << "CRIT";
			break;
		case Logger::ERROR:
			debugLevel << "ERROR";
			break;
		case Logger::WARN:
			debugLevel << "WARN";
			break;
		case Logger::INFO:
			debugLevel << "INFO";
			break;
		case Logger::DEBUG:
			debugLevel << "DEBUG";
			break;
		case Logger::TODO:
			debugLevel << "TODO";
			break;
		case Logger::DEV:
			debugLevel << "DEV";
			break;
		case Logger::ALL:
			debugLevel << "ALL";
			break;
		default:
			break;
	}
	return debugLevel.str();
}


Logger::LEVEL Logger::StringAsLoggerLevel( std::string level_string ) {
	
	Logger::LEVEL l;
			
	if ( "ALERT" == level_string ) {
		l = Logger::ALERT;
	}

	if ( "CRIT" == level_string ) {
		l = Logger::CRIT;
	}
		
	if ( "ERROR" == level_string ) {
		l = Logger::ERROR;
	}
		
	if ( "WARN" == level_string ) {
		l = Logger::WARN;
	}
		
	if ( "INFO" == level_string ) {
		l = Logger::INFO;
	}
		
	if ( "DEBUG" == level_string ) {
		l = Logger::DEBUG;
	}
		
	if ( "TODO" == level_string ) {
		l = Logger::TODO;
	}
		
	if ( "DEV" == level_string ) {
		l = Logger::DEV;
	}
			
	return l;
	
}

int Logger::LevelAndBelow( Logger::LEVEL level ) {
	
	int l = level;
	
	if ( Logger::ALERT <= level ) {
		l = l | Logger::ALERT;
	}
	
	if ( Logger::CRIT <= level ) {
		l = l | Logger::CRIT;
	}
	
	if ( Logger::ERROR <= level ) {
		l = l | Logger::ERROR;
	}
	
	if ( Logger::WARN <= level ) {
		l = l | Logger::WARN;
	}
	
	if ( Logger::INFO <= level ) {
		l = l | Logger::INFO;
	}
	
	if ( Logger::DEBUG <= level ) {
		l = l | Logger::DEBUG;
	}
	
	if ( Logger::TODO <= level ) {
		l = l | Logger::TODO;
	}
	
	if ( Logger::DEV <= level ) {
		l = l | Logger::DEV;
	}
	
	return l;
	
}

}	// namespace pixelpipe