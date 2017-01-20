
#include <stdarg.h>
#include <time.h>
#include <stdio.h>
#include <time.h>
#include <memory.h>
#include <string.h>

#include <sys/timeb.h>

#include "log.h"

#define BUFSIZE		4096

#ifdef _WIN32
#define snprintf	_snprintf
#endif

FILE *Log::logFile = NULL;
int Log::logLevel = 20;

static const char *logTime()
{
    static char buf[128],buf2[10];
	static timeb tstruct;

	ftime(&tstruct);
	struct tm *tm = localtime(&(tstruct.time));
	strftime(buf, 127, "%Y/%m/%d %H:%M:%S", tm);
	sprintf(buf2,".%.3d",tstruct.millitm);
	strcat(buf,buf2);
    return buf;
}

void Log::open(const char *file, int level)
{
	logLevel = level;

	if (logFile)
		closeLog();

	bool b = (file && *file);
	if (b)
		logFile = fopen(file, "a+");
	if (!logFile) {
		logFile = stderr;
		if (b)
			LOG(1) ("Error open log file: %s\n", file);
	}
	//for test only!!!
	//logFile = stderr;
}

void Log::openstderr()
{
	if (logFile && logFile != stderr) {
		fclose(logFile);
		logFile = stderr;
	}
}

void Log::closeLog()
{
	if (logFile && logFile != stderr) {
		fclose(logFile);
		logFile = NULL;
	}
}

void Log::print(const char *fmt, ...)
{
	char buf[BUFSIZE];
	
	va_list args;
	va_start(args, fmt);

	snprintf(buf, sizeof(buf), "%s| %s", logTime(), fmt);
	vfprintf(logFile, buf, args);

	fflush(logFile);

	va_end(args);
}
