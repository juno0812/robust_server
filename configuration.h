#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "./lib/libconfuse/src/confuse.h"  /* conf file parsing lib */

#define CONF_FILENAME "default.conf"

/* default conf values */
char *conf_host = NULL;
int conf_port = 4444;
	
int configuration(cfg_t **cfg, int argc, char *argv[]);
void init_cfg(cfg_t **cfg);
int parse_conf_file(cfg_t **cfg);
int parse_argv(cfg_t **cfg, int, char *argv[]);
void conf_cleanup(cfg_t **cfg);
void print_usage(char*);

#endif