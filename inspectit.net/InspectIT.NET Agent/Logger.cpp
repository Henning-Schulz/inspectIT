#include "Logger.h"

#include <stdio.h>
#include <stdarg.h>
#include <windows.h>
#include <iostream>

LoggerFactory loggerFactory;

Logger::Logger(char* name, LogLevel logLevel)
{
	this->name = name;
	this->logLevel = logLevel;
}


Logger::~Logger()
{
}

void Logger::debug(char* message, ...)
{
	if (logLevel >= LEVEL_DEBUG) {
		va_list ap;

		printf("DEBUG %s - ", name);

		va_start(ap, message);
		vprintf(message, ap);
		va_end(ap);

		printf("\n");
	}
}

void Logger::info(char* message, ...)
{
	if (logLevel >= LEVEL_INFO) {
		va_list ap;

		printf("INFO %s - ", name);

		va_start(ap, message);
		vprintf(message, ap);
		va_end(ap);

		printf("\n");
	}
}

void Logger::warn(char* message, ...)
{
	if (logLevel >= LEVEL_WARN) {
		va_list ap;

		printf("WARN %s - ", name);

		va_start(ap, message);
		vprintf(message, ap);
		va_end(ap);

		printf("\n");
	}
}

void Logger::error(char* message, ...)
{
	if (logLevel >= LEVEL_ERROR) {
		va_list ap;

		printf("ERROR %s - ", name);

		va_start(ap, message);
		vprintf(message, ap);
		va_end(ap);

		printf("\n");
	}
}

void Logger::log(LogLevel level, char* message, ...) {
	if (logLevel >= level) {
		va_list ap;

		printf("%s: ? - ", name);

		va_start(ap, message);
		vprintf(message, ap);
		va_end(ap);

		printf("\n");
	}
}

LoggerFactory::LoggerFactory()
{
	printf("Create LoggerFactory...\n");
	updateLogLevel();
	printf("LoggerFactory created\n");
}

LoggerFactory::~LoggerFactory()
{
}

void LoggerFactory::updateLogLevel() {
	char *level;
	size_t len;
	errno_t err = _dupenv_s(&level, &len, "LOG_LEVEL");
	if (err || len == 0) {
		logLevel = LEVEL_INFO;
		printf("No log level specified. Using INFO\n");
		return;
	}

	bool found = false;
	for (int i = 0; i < logLevelNames.size(); i++) {
		if (strcmp(level, logLevelNames.at(i)) == 0) {
			logLevel = static_cast<LogLevel>(i);
			printf("Log level is %s\n", logLevelNames.at(i));
			found = true;
			break;
		}
	}

	if (!found) {
		printf("Unknown log level %s. Using INFO\n", level);
		logLevel = LEVEL_INFO;
	}

	/*if (strcmp(level, "DEBUG") == 0) {
		logLevel = LEVEL_DEBUG;
		printf("Log level is DEBUG\n");
	}
	else if (strcmp(level, "INFO") == 0) {
		logLevel = LEVEL_INFO;
		printf("Log level is INFO\n");
	}
	else if (strcmp(level, "WARN") == 0) {
		logLevel = LEVEL_WARN;
		printf("Log level is WARN\n");
	}
	else {
		logLevel = LEVEL_ERROR;
		printf("Log level is ERROR\n");
	}*/

	free(level);
}

Logger LoggerFactory::createLogger(char* name)
{
	return Logger(name, logLevel);
}

LogLevel LoggerFactory::getLogLevel() {
	return logLevel;
}