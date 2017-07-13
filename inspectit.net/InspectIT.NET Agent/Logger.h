#pragma once

#include <vector>
#include <shared_mutex>

namespace inspectit {
	namespace logger {

		enum LogLevel { LEVEL_ERROR = 0, LEVEL_WARN, LEVEL_INFO, LEVEL_DEBUG };
		const std::vector<char*> logLevelNames = { "ERROR", "WARN", "INFO", "DEBUG" };

		class Logger
		{
		private:
			LogLevel logLevel;
			char* name = "";
			bool toFile = false;
			FILE *logFile;

			void log(LogLevel level, char* message, va_list ap);

		public:
			Logger(char* name, LogLevel logLevel);
			Logger(char* name, LogLevel logLevel, bool toFile, FILE *logFile);
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
			bool toFile = false;
			char* fileName = "";
			FILE *logFile;

			std::shared_mutex mUpdate;

			void updateLogLevel();

		public:
			LoggerFactory();
			~LoggerFactory();

			Logger createLogger(char* name);
			LogLevel getLogLevel();

			void staticLog(LogLevel level, char *loggerName, char *message, ...);
			void staticLogWithoutNewLine(LogLevel level, char *loggerName, char *message, ...);
			void printf(char* message, ...);
		};

	}
}

extern inspectit::logger::LoggerFactory loggerFactory;

