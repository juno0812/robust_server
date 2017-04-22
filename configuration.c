#include <sys/types.h>	/* these three are for checking for conf file */
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>	 /* argv parsing */
#include <stdlib.h>  /* free() */
#include <stdio.h>	/* printing */
#include <errno.h>  /* errno */
#include <string.h>	/* stuffs */
#include <ctype.h>	/* isspace */

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
	printf("[>] host: %s\n", cfg_copy.host);
	printf("[>] port: %d\n", cfg_copy.port);
	
	*cfg = cfg_copy;
	return 0;
}

void init_cfg(cfg_t *cfg)  {
	if(DEFAULT_HOST == NULL)  {
		cfg->host = DEFAULT_HOST;
	}
	else  {
		cfg->host = malloc(strlen(DEFAULT_HOST) * sizeof(char) + 1);
		strcpy(cfg->host, DEFAULT_HOST);
	}
	cfg->port = DEFAULT_PORT;
}

int parse_conf_file(cfg_t *cfg)  {
	int fd = -1;
	int line_ret = -1;
	char *line = NULL;
	char *key = NULL, *value = NULL;

	/* try the conf file */
	if((fd = open(CONF_FILENAME, O_RDONLY)) < 0)  {
		if(errno == EACCES)  {
			fprintf(stderr, "[!] conf file doesn't exist or we're having access issues.  If you'd like to use a conf file, create a file in the current directory named \"%s\".  Proceeding without a conf file.\n", CONF_FILENAME);
			return 0;
		}
		else  {
			fprintf(stderr, "[!] issue opening conf file, \"%s\"\n", CONF_FILENAME);
			return -1;
		}
	}
	
	printf("[>] reading from conf file\n");
	
	/* Begin parsing file */
	while((line_ret = get_line(fd, line)) > 0)  {
		printf("poop\n");
		if(line == NULL)  {
			/* empty line */
			continue;
		}
		printf("fart\n");

		if(*line == '#')  {
			/* comment */
			continue;
		}
		printf("snot\n");

		if(get_key_value(line, key, value) < 0)  {
			fprintf(stderr, "[!] syntax issue in conf file\n");
			free(line);
			if(key != NULL)	free(key);
			if(value != NULL)	free(key);
			close(fd);
			return -1;
		}

		printf("key = %s | value = %s\n", key, value);

		if(strcmp(key, "host") == 0)  {
			if(cfg->host == NULL)  {
				cfg->host = malloc(strlen(value) * sizeof(char) + 1);
			}
			else  {
				cfg->host = realloc(cfg->host, strlen(value) * sizeof(char) + 1);
			}
			strcpy(cfg->host, value);
		}

		else if(strcmp(key, "port") == 0)  {
			cfg->port = atoi(value);
		}
		
		else  {
			fprintf(stderr, "[!] unknown conf key \"%s\"", key);
			free(line);
			if(key != NULL)	free(key);
			if(value != NULL)	free(value);
			close(fd);
			return -1;
		}
	}

	if(line != NULL)	free(line);
	if(key != NULL)	free(key);
	if(value != NULL)	free(value);

	close(fd);

	if(line_ret < 0)  {
		/* problem with get_line */
		fprintf(stderr, "[!] problem parsing conf file\n");
		return -1;
	}
	
	return 0;
}

int get_line(int fd, char *line)  {
	off_t curr = -1;
	int line_len = 0;
	char c;

	if(line != NULL)	free(line);

	curr = lseek(fd, 0, SEEK_CUR);
	printf("current pos = %lld\n", curr);

	while(read(fd, &c, 1) > 0)  {
		printf("char is %c\n", c);
		if(c == '\r' || c == '\n')  {
			break;
		}
		line_len++;
	}

	printf("line len = %d\n", line_len);

	if(line_len == 0)  {
		return 0;
	}

	&(*line) = malloc(line_len * sizeof(char) + 1);
	lseek(fd, curr, SEEK_SET);
	read(fd, &(*line), line_len);

	printf("line = %s\n", line);

	while(read(fd, &c, 1) > 0)  {
		if(isspace(c) == 0)  {
			lseek(fd, -1, SEEK_CUR);
			break;
		}
	}

	return line_len;
}

int get_key_value(char *line, char *key, char *value)  {
	int equal_pos = 0;

	if(key != NULL)	free(key);
	if(value != NULL)	free(value);

	/* get the key */
	for(equal_pos=0; equal_pos<strlen(line); equal_pos++)  {
		if(line[equal_pos] == '=')	break;
	}

	if(equal_pos == strlen(line) - 1)  {
		fprintf(stderr, "[!] no equal sign or in wrong place\n[!] line = %s", line);
		return -1;
	}

	key = malloc(equal_pos * sizeof(char) + 1);
	memset(key, 0, equal_pos * sizeof(char) + 1);
	memcpy(key, line, equal_pos);

	/* now get the value */
	equal_pos++;
	value = malloc(strlen(line + equal_pos) + 1);
	strcpy(value, line + equal_pos);

	return equal_pos;
}

int parse_argv(cfg_t *cfg, int argc, char *argv[])  {
	int option = 0;

	while((option = getopt(argc, argv, "p:h:")) != -1)  {
		switch(option)  {
			case 'p':
				cfg->port = atoi(optarg);
				break;
			case 'h':
				if(cfg->host == NULL)  {
					cfg->host = malloc(strlen(optarg) + 1);
				}
				else  {
					cfg->host = realloc(cfg->host, strlen(optarg) + 1);
				}
				strcpy(cfg->host, optarg);
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

void conf_cleanup(cfg_t *cfg)  {
	if(cfg->host != NULL)	free(cfg->host);
}