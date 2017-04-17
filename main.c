#include "common.h"
#include "configuration.h"
#include "connection.h"

int main(int argc, char *argv[])  {
	conf_t conf = {0};

	conf = configuration(argc, argv);
	if(conf == 0)  {
		fprintf(stderr, "[!] error with configuration\n");
		exit(-1);
	}
	
	dispatch_connections(conf);
	
	admin_cli();
	
	exit(0);
}