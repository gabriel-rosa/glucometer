#ifndef LOG_H
#define LOG_H

#include <string.h>
#include <stdio.h>

#ifdef WIN32
	#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
	#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define LOG_LEVEL 4 // 0 - Don't log
					// 1 - Log errors only
					// 2 - Log errors and warnings
					// 3 - Log errors, warnings and info
					// 4 - Log everything
#define LOG_DESTINATION stderr

#define LOG(level, type, ...) \
	if (level <= LOG_LEVEL) { \
		fprintf(LOG_DESTINATION, type); \
		fprintf(LOG_DESTINATION, "<%s in %s:%d> ", __FUNCTION__, __FILENAME__, __LINE__); \
		fprintf(LOG_DESTINATION, __VA_ARGS__); \
		fprintf(LOG_DESTINATION, "\n"); \
		fflush(LOG_DESTINATION); \
	}

#define LOG_ERROR(...) LOG(1, "ERROR\t", __VA_ARGS__)
#define LOG_WARNING(...) LOG(2, "WARNING\t", __VA_ARGS__)
#define LOG_INFO(...) LOG(3, "INFO\t", __VA_ARGS__)
#define LOG_DEBUG(...) LOG(4, "DEBUG\t", __VA_ARGS__)

#endif /* LOG_H */
