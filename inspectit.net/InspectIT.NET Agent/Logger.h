#pragma once

enum LogLevel { LEVEL_DEBUG = 20, LEVEL_INFO = 10, LEVEL_ERROR = 0 };

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

