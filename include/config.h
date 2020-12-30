#ifndef CONFIG_H
#define CONFIG_H

#include "logger.h"

struct config_general {
	char *configfile;
};

struct config_logger {
	char *loglevel;
	char *logfile;
};

struct config_screencast {
	char *output_name;
};

struct xdpw_config {
	struct config_general conf;
	struct config_logger logger_conf;
	struct config_screencast screencast_conf;
};

void print_config(struct xdpw_config *config);
void destroy_config(struct xdpw_config *config);
void init_config(struct xdpw_config *config);

#endif
