#include "common.h"
#include "configuration.h"
#include "connection.h"

int main(int argc, char *argv[])  {
	cfg_t conf = {0};

	if(configuration(&conf, argc, argv) < 0 {
		fprintf(stderr, "[!] error with configuration\n");
		conf_cleanup(&conf);
		exit(-1);
	}
	
	dispatch_connections(conf);
	
	admin_cli();
	
	conf_cleanup(&conf);
	exit(0);
}