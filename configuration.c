#include <sys/types.h>	/* these three are for checking for conf file */
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>	 /* argv parsing */
#include <stdlib.h>  /* free() */
#include <errno.h>  /* errno */

#include "configuration.h"

int configuration(cfg_t *cfg, int argc, char *argv[])  {
	cfg_t cfg_copy;
	
	init_cfg(&cfg_copy);
	
	if(parse_conf_file(&cfg_copy) < 0)  {
		*cfg = cfg_copy;
		return -1;
	}
	
	if(parse_argv(&cfg_copy, argc, argv) < 0)  {
		*cfg = cfg_copy;
		return -1;
	}

	printf("[>] configuration:\n");
	cfg_print(cfg_copy, stdout);
	
	*cfg = cfg_copy;
	return 0;
}

void init_cfg(cfg_t *cfg)  {
	cfg.host = DEFAULT_HOST;
	cfg.port = DEFAULT_PORT;
}

int parse_conf_file(cfg_t *cfg)  {
	int fd = -1;

	/* try the conf file */
	if((fd = open(CONF_FILENAME, O_RDONLY)) < 0)  {
		if(errno == EACCES)  {
			fprintf(stderr, "[!] conf file doesn't exist or we're having access issues.  If you'd like to use a conf file, create a file in the current directory named \"%s\".  Proceeding without a conf file.\n", CONF_FILENAME);
			return 0;
		}
		else  {
			fprintf(stderr, "[!] issue opening conf file, \"%s\"", CONF_FILENAME);
			return -1;
		}
	}
	
	printf("[>] reading from conf file\n");
	
	/* Begin parsing file */
	
	
	return 0;
}

int parse_argv(cfg_t *cfg, int argc, char *argv[])  {
	int option = 0;

	while((option = getopt(argc, argv, "p:h:")) != -1)  {
		switch(option)  {
			case 'p':
				cfg_setint(*cfg, "port", atoi(optarg));
				break;
			case 'h':
				cfg_setstr(*cfg, "host", optarg);
				break;
			default:
				fprintf(stderr, "[!] unknown option \'%c\'\n", option);
				print_usage(argv[0]);
				return -1;
		}  /* end switch */
	}  /* end while getting all opts */

	return 0;
}

void print_usage(char *bin)  {
	printf("[!] Usage: %s [-h <interface>] [-p <port>]\n", bin);
}

void conf_cleanup(cfg_t **cfg)  {
	cfg_free(*cfg);
	
	if(conf_host != NULL)	free(conf_host);
}