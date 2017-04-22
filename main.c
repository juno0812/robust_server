#include <stdlib.h>
#include <stdio.h>

#include "configuration.h"
//#include "connection.h"

int main(int argc, char *argv[])  {
	cfg_t cfg;

	if(configuration(&cfg, argc, argv) < 0)  {
		fprintf(stderr, "[!] error with configuration\n");
		conf_cleanup(&cfg);
		exit(-1);
	}
	
	printf("in main: %s %d\n", cfg.host, cfg.port);
	//dispatch_connections(conf);
	
	//admin_cli();
	
	conf_cleanup(&cfg);
	exit(0);
}