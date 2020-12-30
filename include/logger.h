#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

enum LOGLEVEL { QUIET, ERROR, WARN, INFO, DEBUG, TRACE };

struct logger_properties {
	enum LOGLEVEL level;
	FILE *dst;
};

FILE* open_logger_file(char *file_name);
void destroy_logger();
void init_logger(FILE *dst, enum LOGLEVEL level);
enum LOGLEVEL get_loglevel(const char *level);
void logprint(enum LOGLEVEL level, char *msg, ...);

#endif
