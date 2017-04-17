#include <sys/types.h>	/* these three are for checking for conf file */
#include <sys/stat.h>
#include <fcntl.h>

#include <argp.h>	/* argv parsing */

#include "configuration.h"

void init_cfg(cfg_t **cfg)  {
	
	cfg_opt_t opts[] = {
		CFG_SIMPLE_STR("host", &conf_host),
		CFG_INT("port", &conf_port),
		CFG_END()
	};
	
	*cfg = cfg_init(opts, 0);
}
int parse_conf_file(cfg_t **cfg)  {
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
	
	cfg_parse(*cfg, CONF_FILENAME);
	return 0;
}

int parse_argv(cfg_t **cfg)  {
	struct arguments  {
		char *args[2];	/* TODO: magic number */
		int port;
		char *host;
	};
}

int configuration(cfg_t **cfg, int argc, char *argv[])  {
	cfg_t *cfg_copy;
	
	init_cfg(&cfg_copy);
	
	if(parse_conf_file(&cfg_copy) < 0)  {
		*cfg = *cfg_copy;
		return -1;
	}
	
	if(parse_argv(&cfg_copy, argc, argv) < 0)  {
		*cfg = *cfg_copy;
		return -1;
	}
	
	
	*cfg = *cfg_copy;
	return 0;
}

void conf_cleanup(cfg_t **cfg)  {
	cfg_free(*cfg);
	
	if(conf_host != NULL)	free(conf_host);
}