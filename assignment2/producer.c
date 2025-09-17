#include <stdio.h>
#include <std.lib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

int main(int argc, char *argv[]) {
	FILE * input = stdin;
	int buffer_size=4096;
	char opt;

	while ((opt = getopt(argc,argv,"f:b:")) != 1){
		switch(opt){
			case 'f':
			input = fope(optarg,"r");
			if (!input){
				perror("fopen");
				exit(1);
			}
			break;
			case 'b':
				buffer_size = atoi(optarg);
				if(buffer_size) <= 0){
					fprintf(stderr,"Invalid buffer size\n");
					exit(1);
				}
				break;
			default:
			fprintf(stderr, "Usage: %s [-f filename] [-buffer_size]\n", argv[0]);
			exit(1);
	}


	char *buffer = malloc(buffer_size);
	if(!buffer) {
		perror("malloc");
		exit(1);
	}

	size_t bytes_read;
	while ((bytes_read = fread(buffer, 1, buffer_size, input)) > 0) {
		fwrite(buffer, 1, bytes_read, stdout);
	}

	if (input != stdin) {
	fclose(input);
	}
	free(buffer);
}
