#include "logger.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int NUM_LEVELS = 6;

static const char *loglevels[] = {
	"QUIET",
	"ERROR",
	"WARN",
	"INFO",
	"DEBUG",
	"TRACE"
};

static struct logger_properties logprops;

FILE* open_logger_file(char *file_name) {
	if (file_name == NULL) {
		printf("Use stderr\n");
		return stderr;
	} else if (strcmp(file_name, "stderr") == 0) {
		printf("Use stderr\n");
		return stderr;
	} else {
		return fopen(file_name, "w");
	}
}

void destroy_logger() {
	if (logprops.dst != stderr) {
		fclose(logprops.dst);
	}
}

void init_logger(FILE *dst, enum LOGLEVEL level) {
	logprops.dst = dst;
	logprops.level = level;
}

enum LOGLEVEL get_loglevel(const char *level) {
	int i;
	for (i = 0; i < NUM_LEVELS; i++) {
		if (!strcmp(level, loglevels[i])) {
			return (enum LOGLEVEL) i;
		}
	}
	fprintf(stderr, "Could not understand log level %s\n", level);
	abort();
}

void logprint(enum LOGLEVEL level, char *msg, ...) {
	if (!logprops.dst) {
		fprintf(stderr, "Logger has been called, but was not initialized\n");
		abort();
	}

	if (level > logprops.level || level == QUIET) {
		return;
	}
	va_list args;

	char timestr[200];
	time_t t = time(NULL);
	struct tm *tmp = localtime(&t);

	if (strftime(timestr, sizeof(timestr), "%Y/%m/%d %H:%M:%S", tmp) == 0) {
		fprintf(stderr, "strftime returned 0");
		abort();
	}

	fprintf(logprops.dst, "%s", timestr);
	fprintf(logprops.dst, " ");
	fprintf(logprops.dst, "[%s]", loglevels[level]);
	fprintf(logprops.dst, " - ");

	va_start(args, msg);
	vfprintf(logprops.dst, msg, args);
	va_end(args);


	fprintf(logprops.dst, "\n");
	fflush(logprops.dst);
}
