#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>

volatile sig_atomic_t shutdown_flag = 0; //Triggered by SIGINT
volatile sig_atomic_t stats_flag = 0; //Triggered by SIGUSR1

void handle_sigint(int sig){
	shutdown_flag = 1;
}

void handle_sigusr1(int sig){
	stats_flag = 1;
}

int main(int argc, char *argv[]) {
	FILE * input = stdin;
	int buffer_size=4096;
	char opt;

	struct sigaction sa_int, sa_usr1;

	//SIGINT Ctrl+C Shutdown
	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags=0;
	sigaction(SIGINT, &sa_int, NULL);

	//SIGUSR1 Print stats
	sa_usr1.sa_handler = handle_sigusr1;
	sigemptyset(&sa_usr1.sa_mask);
	sa_usr1.sa_flags = 0;
	sigaction(SIGUSR1, &sa_usr1, NULL);

	//Loops
	while ((opt = getopt(argc,argv,"f:b:")) != -1) {
		switch(opt) {
			case 'f':
			input = fopen(optarg,"r");
			if (!input) {
				perror("fopen");
				exit(1);
			}
			break;

			case 'b':
				buffer_size = atoi(optarg);
				if(buffer_size <=0) {
					fprintf(stderr,"Invalid buffer size\n");
					exit(1);
				}
				break;

			default:
			fprintf(stderr, "Usage: %s [-f filename] [-buffer_size]\n", argv[0]);
			exit(1);
	}
}
	
	char *buffer = malloc(buffer_size);
	if(!buffer) {
		perror("malloc");
		exit(1);
	}

	size_t bytes_read;
	while ((bytes_read = fread(buffer, 1, buffer_size, input)) > 0) {
		fwrite(buffer, 1, bytes_read, stdout);

		//Signal handling
		if(stats_flag){
			fprintf(stderr, "Bytes so far: %zu\n", bytes_read);
			stats_flag = 0; //Reset
		}
	}

	if (input != stdin) {
	fclose(input);
	}
	free(buffer);

	return 0;
}
