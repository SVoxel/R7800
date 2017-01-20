
#ifndef _LOG_H
#define _LOG_H

#include <stdio.h>

#define LOG(level)	if (level <= Log::logLevel) Log::print


class Log {
public:
	static void open(const char *file, int level);
	static void openstderr();
	static void closeLog();
	static void print(const char *fmt, ...);

	static int logLevel;

private:
	static FILE *logFile;
};

#endif
