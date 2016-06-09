#pragma once

#include <vector>

enum LogLevel { LEVEL_ERROR = 0, LEVEL_WARN, LEVEL_INFO, LEVEL_DEBUG };
const std::vector<char*> logLevelNames = { "ERROR", "WARN", "INFO", "DEBUG" };

class Logger
{
private:
	LogLevel logLevel;
	char* name = "";

public:
	Logger(char* name, LogLevel logLevel);
	~Logger();

	void debug(char* message, ...);
	void info(char* message, ...);
	void warn(char* message, ...);
	void error(char* message, ...);
	void log(LogLevel level, char* message, ...);
};

class LoggerFactory
{
private:
	LogLevel logLevel;

	void updateLogLevel();

public:
	LoggerFactory();
	~LoggerFactory();

	Logger createLogger(char* name);
	LogLevel getLogLevel();
};

extern LoggerFactory loggerFactory;

