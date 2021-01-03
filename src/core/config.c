#include "config.h"
#include "xdpw.h"

#include <dictionary.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <wordexp.h>
#include "iniparser.h"

#define SYSCONFDIR "/etc"

void print_config(struct xdpw_config *config) {
	printf("Configfile: %s\n",config->conf.configfile);
	printf("Logfile: %s\n",config->logger_conf.logfile);
	printf("Loglevel: %s\n",config->logger_conf.loglevel);
	printf("Output_Name: %s\n",config->screencast_conf.output_name);
}

void free_configstring(char **dest) {
	if (*dest != NULL) {
		free(*dest);
		*dest = NULL;
	}
}

void destroy_config(struct xdpw_config *config) {
	// logger
	free_configstring(&config->logger_conf.loglevel);
	free_configstring(&config->logger_conf.logfile);

	// screencast
	free_configstring(&config->screencast_conf.output_name);
}

void getstring_from_conffile(dictionary *d, char *key, char **dest, char *def) {
	if (*dest == NULL) {
		const char *c = iniparser_getstring(d, key, def);
		if (c != NULL) {
			// Allow keys without value as default
			if (strcmp(c, "") != 0) {
				printf("Config: Key %s, Value %s\n", key, c);
				*dest = strdup(c);
			} else {
				*dest = strdup(def);
			}
		}
	}
}

static bool file_exists(const char *path) {
	return path && access(path, R_OK) != -1;
}

static char *expand_path(char *path, bool realloc) {
		wordexp_t p;
		if (wordexp(path, &p, WRDE_UNDEF) == 0) {
			if (realloc) {
				free(path);
			}
			return strdup(p.we_wordv[0]);
		}

		if (realloc) {
			free(path);
		}
		return NULL;
}

void config_parse_file(struct xdpw_config *config) {
	dictionary *d = iniparser_load(config->conf.configfile);

	// logger
	getstring_from_conffile(d, "logger:loglevel", &config->logger_conf.loglevel, "ERROR");
	getstring_from_conffile(d, "logger:logfile", &config->logger_conf.logfile, "stderr");
	config->logger_conf.logfile = expand_path(config->logger_conf.logfile, true);

	// screencast
	getstring_from_conffile(d, "screencast:output_name", &config->screencast_conf.output_name, NULL);

	iniparser_freedict(d);
}

static char *get_config_path(void) {
	static const char *config_paths[] = {
		"$XDG_CONFIG_HOME/xdg-desktop-portal-wlr/$XDG_CURRENT_DESKTOP",
		"$XDG_CONFIG_HOME/xdg-desktop-portal-wlr/config",
		SYSCONFDIR "/xdg-desktop-portal-wlr/$XDG_CURRENT_DESKTOP",
		SYSCONFDIR "/xdg-desktop-portal-wlr/config",
	};

	char *config_home = getenv("XDG_CONFIG_HOME");
	if (!config_home || !*config_home) {
		config_paths[0] = "$HOME/.config/xdg-desktop-portal-wlr/$XDG_CURRENT_DESKTOP";
		config_paths[1] = "$HOME/.config/xdg-desktop-portal-wlr/config";
	}

	for (size_t i = 0; i < sizeof(config_paths) / sizeof(char *); ++i) {
		char *path = expand_path((char*)config_paths[i], false);
		if (file_exists(path)) {
			return path;
		}
	}

	return NULL;
}

void init_config(struct xdpw_config *config) {
	if (config->conf.configfile == NULL) {
		config->conf.configfile = get_config_path();
	}

	config_parse_file(config);
}
