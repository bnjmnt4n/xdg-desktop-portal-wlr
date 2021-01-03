#ifndef CONFIG_H
#define CONFIG_H

#include "logger.h"
#include "screencast_common.h"

struct config_general {
	char *configfile;
};

struct config_logger {
	enum LOGLEVEL loglevel;
};

struct config_screencast {
	char *output_name;
	char *chooser_cmd;
	enum xdpw_chooser_types chooser_type;
};

struct xdpw_config {
	struct config_general conf;
	struct config_logger logger_conf;
	struct config_screencast screencast_conf;
};

void print_config(enum LOGLEVEL loglevel, struct xdpw_config *config);
void destroy_config(struct xdpw_config *config);
void init_config(struct xdpw_config *config);

#endif
