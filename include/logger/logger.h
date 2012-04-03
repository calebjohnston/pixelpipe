/*
 *  Logger.h
 *  ECSLogger
 *
 *  Created by M. Hank Kiedrowski on 8/3/10.
 *  Copyright 2010 Clockwork Active Media Systems. All rights reserved.
 *
 */

#ifndef __PIPELINE_Logger_H
#define __PIPELINE_Logger_H

#define LOG(level) \
pixelpipe::Logger::CanLog(level) && pixelpipe::Logger().Log(level)

#define DEV() \
pixelpipe::Logger::CanLog(pixelpipe::Logger::DEV) && pixelpipe::Logger().Log(pixelpipe::Logger::DEV)

#define DEBUG() \
pixelpipe::Logger::CanLog(pixelpipe::Logger::DEBUG) && pixelpipe::Logger().Log(pixelpipe::Logger::DEBUG)

#define INFO() \
pixelpipe::Logger::CanLog(pixelpipe::Logger::INFO) && pixelpipe::Logger().Log(pixelpipe::Logger::INFO)

#define ERROR() \
pixelpipe::Logger::CanLog(pixelpipe::Logger::ERROR) && pixelpipe::Logger().Log(pixelpipe::Logger::ERROR)

#define LOGIdent(level, identity) \
pixelpipe::Logger::CanLog(level) && pixelpipe::Logger().Log(level, identity)

#include <sstream>
#include <string>
#include <list>
#include <vector>

namespace pixelpipe {
	
class LogWriter;

class Logger  {
	
public:
	
	enum LEVEL {

		// most severe log level, program failing
		ALERT = 1,
		
		// action must be taken immediately
		CRIT = 2,
		
		// Error messages; something has gone wrong.
		ERROR = 4,
		
		// Possible error, possible misconfiguration
		WARN = 8,
		
		// Messages that may be of interest; verbose action logging.
		INFO = 16,
		
		// Debugging messages
		DEBUG = 32,
		
		// TODO 
		TODO = 64,
		
		// Development messages, should not be used in production
		DEV = 128
		
	};
		
	// if level is not set default will be used; FAIL through INFO
	static const int LEVEL_FLOOR = Logger::ALERT;
	static const int ALL = 255;
	static const int DEFAULT_REPORTING_LEVEL = 31;
	
	
protected:
	static int _level;
	static std::vector<int> _levelStack;
	static const int _maxWriters = 10;
	static std::list<LogWriter *> _registeredWriters;
	static std::string _identity;
	std::string * _local_identity;
	
	std::ostringstream _message;
	Logger::LEVEL _messageLevel;
	
	
public:
	static bool CanLog( Logger::LEVEL level );
	static const int SetLevel( const int level ); // sets logging level
	static const int GetLevel(); // returns the current level
	static void PushLevel( Logger::LEVEL level ); // pushes the passed log level onto the level stack
	static void PopLevel(); // pops the top level off of the log level stack
	
	std::ostringstream & Log( Logger::LEVEL level ); // logs a message using registered LogWriters(s)
	std::ostringstream & Log( Logger::LEVEL level, const std::string& identity );
	
	// Identity is used to label the use of the log, typically this should be the application name
	static void SetIdentity( const std::string& identity );
	static const std::string GetIdentity();
	
	static void RegisterWriter( LogWriter * writer ); // registers the passed LogWriter
	static void UnRegisterWriter( LogWriter * writer ); // un registers the passed LogWriter
	
	static std::string CurrentTime();
	static std::string LoggerLevelAsString( Logger::LEVEL level );
	static Logger::LEVEL StringAsLoggerLevel( std::string level_string );
	
	// returns the passed level OR'd with all lower levels
	static int LevelAndBelow( Logger::LEVEL level );
	
	Logger( );
	~Logger( );
	
	
protected:
	void WriteMessage(); // sends the current message to the RegisteredWriters
	std::string FormattedMessage();
	
};

}	// namespace pixelpipe

#endif	//__PIPELINE_Logger_H