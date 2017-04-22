#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#define CONF_FILENAME "default.conf"

/* default conf values */
#define DEFAULT_HOST NULL
#define DEFAULT_PORT 4321

typedef struct _cfg_t  {
	char *host;
	int port;
} cfg_t;
	
int configuration(cfg_t *cfg, int argc, char *argv[]);
void init_cfg(cfg_t *cfg);
int parse_conf_file(cfg_t *cfg);
int parse_argv(cfg_t *cfg, int, char *argv[]);
void conf_cleanup(cfg_t *cfg);
void print_usage(char*);
int get_line(int fd, char *line);
int get_key_value(char *line, char *key, char *value);

#endif