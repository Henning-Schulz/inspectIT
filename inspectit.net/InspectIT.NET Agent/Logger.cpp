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
	this->toFile = false;
}

Logger::Logger(char* name, LogLevel logLevel, bool toFile, FILE *logFile)
{
	this->name = name;
	this->logLevel = logLevel;
	this->toFile = toFile;
	this->logFile = logFile;
}


Logger::~Logger()
{
}

void Logger::log(LogLevel level, char* message, va_list ap)
{
	if (logLevel >= level) {
		if (toFile) {
			fprintf(logFile, "%s %s - ", logLevelNames[level], name);
			vfprintf(logFile, message, ap);
			fprintf(logFile, "\n");
			fflush(logFile);
		}
		else {
			printf("%s %s - ", logLevelNames[level], name);
			vprintf(message, ap);
			printf("\n");
		}
	}
}

void Logger::debug(char* message, ...)
{
	if (logLevel >= LEVEL_DEBUG) {
		va_list ap;
		va_start(ap, message);
		log(LEVEL_DEBUG, message, ap);
		va_end(ap);
	}
}

void Logger::info(char* message, ...)
{
	if (logLevel >= LEVEL_INFO) {
		va_list ap;
		va_start(ap, message);
		log(LEVEL_INFO, message, ap);
		va_end(ap);
	}
}

void Logger::warn(char* message, ...)
{
	if (logLevel >= LEVEL_WARN) {
		va_list ap;
		va_start(ap, message);
		log(LEVEL_WARN, message, ap);
		va_end(ap);
	}
}

void Logger::error(char* message, ...)
{
	if (logLevel >= LEVEL_ERROR) {
		va_list ap;
		va_start(ap, message);
		log(LEVEL_ERROR, message, ap);
		va_end(ap);
	}
}

void Logger::log(LogLevel level, char* message, ...) {
	if (logLevel >= level) {
		va_list ap;
		va_start(ap, message);
		log(level, message, ap);
		va_end(ap);
	}
}

LoggerFactory::LoggerFactory()
{
	printf("INFO LoggerFactory - Create LoggerFactory...\n");
	updateLogLevel();
	printf("INFO LoggerFactory - LoggerFactory created\n");
}

LoggerFactory::~LoggerFactory()
{
	if (logFile) {
		fclose(logFile);
	}
}

void LoggerFactory::updateLogLevel() {
	// No new loggers should be created during execution of this method
	std::unique_lock<std::shared_mutex> lock(mUpdate);

	char *level;
	size_t len;
	errno_t err = _dupenv_s(&level, &len, "LOG_LEVEL");
	if (err || len == 0) {
		logLevel = LEVEL_INFO;
		printf("WARN LoggerFactory - No log level specified. Using INFO\n");
	}
	else {
		bool found = false;
		for (int i = 0; i < logLevelNames.size(); i++) {
			if (strcmp(level, logLevelNames.at(i)) == 0) {
				logLevel = static_cast<LogLevel>(i);
				printf("INFO LoggerFactory - Log level is %s\n", logLevelNames.at(i));
				found = true;
				break;
			}
		}

		if (!found) {
			printf("WARN LoggerFactory - Unknown log level %s. Using INFO\n", level);
			logLevel = LEVEL_INFO;
		}

		free(level);
	}

	char *logToFile;
	err = _dupenv_s(&logToFile, &len, "LOG_TO_FILE");
	if (err || len == 0 || strcmp(logToFile, "true") != 0) {
		toFile = false;
		printf("INFO LoggerFactory - Logging to console.\n");
	}
	else {
		char* fileName;
		err = _dupenv_s(&fileName, &len, "LOG_FILE_NAME");

		if (err || len == 0) {
			toFile = false;
			printf("WARN LoggerFactory - No file name for logging specified. Logging to console.\n");
		}
		else {
			toFile = true;
			this->fileName = fileName;

			errno_t err = fopen_s(&logFile, fileName, "ab+");
			if (err || logFile == NULL) {
				printf("WARN LoggerFactory - Cannot open file %s!\n", fileName);
				printf("INFO LoggerFactory - Logging to console.\n");
				this->toFile = false;
			}

			printf("INFO LoggerFactory - Logging to file %s\n", fileName);
			free(fileName);
		}

		// fileName need to be freed in destructor
	}

	free(logToFile);
}

Logger LoggerFactory::createLogger(char* name)
{
	// Wait for termination of updateLockLevel() before creating new logger
	std::shared_lock<std::shared_mutex> lock(mUpdate);

	if (toFile) {
		return Logger(name, logLevel, true, logFile);
	}
	else {
		return Logger(name, logLevel);
	}
}

LogLevel LoggerFactory::getLogLevel() {
	return logLevel;
}

void LoggerFactory::staticLog(LogLevel level, char *loggerName, char *message, ...)
{
	if (logLevel >= level) {
		va_list ap;
		va_start(ap, message);

		if (toFile) {
			fprintf(logFile, "%s %s - ", logLevelNames[level], loggerName);
			vfprintf(logFile, message, ap);
			fprintf(logFile, "\n");
			fflush(logFile);
		}
		else {
			printf("%s %s - ", logLevelNames[level], loggerName);
			vprintf(message, ap);
			printf("\n");
		}

		va_end(ap);
	}
}

void LoggerFactory::staticLogWithoutNewLine(LogLevel level, char *loggerName, char *message, ...)
{
	if (logLevel >= level) {
		va_list ap;
		va_start(ap, message);

		if (toFile) {
			fprintf(logFile, "%s %s - ", logLevelNames[level], loggerName);
			vfprintf(logFile, message, ap);
			fflush(logFile);
		}
		else {
			printf("%s %s - ", logLevelNames[level], loggerName);
			vprintf(message, ap);
		}

		va_end(ap);
	}
}

void LoggerFactory::printf(char* message, ...)
{
	va_list ap;
	va_start(ap, message);

	if (toFile) {
		vfprintf(logFile, message, ap);
		fflush(logFile);
	}
	else {
		vprintf(message, ap);
	}

	va_end(ap);
}