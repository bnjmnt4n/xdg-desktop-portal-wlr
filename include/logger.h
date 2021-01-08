#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

enum LOGLEVEL { UNSET, QUIET, ERROR, WARN, INFO, DEBUG, TRACE };

struct logger_properties {
	enum LOGLEVEL level;
	FILE *dst;
};

void init_logger(FILE *dst, enum LOGLEVEL level);
enum LOGLEVEL get_loglevel(const char *level);
const char** get_loglevels();
void logprint(enum LOGLEVEL level, char *msg, ...);

#endif
