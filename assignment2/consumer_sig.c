#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <signal.h>

volatile sig_atomic_t shutdown_flag = 0; //Triggered by SIGINT
volatile sig_atomic_t stats_flag = 0; //Triggered by SIGUSR1

void handle_sigint(int sig){
        shutdown_flag = 1;
}

void handle_sigusr1(int sig){
        stats_flag = 1;
}



int main (int argc, char *argv[]) {
	int max_lines = -1; //Unlimited
	int verbose = 0;

	//Parse arguments (-n max_lines, -v verbose)
	int opt;


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
	while ((opt = getopt(argc, argv, "n:v")) != -1) {
		switch(opt) {
			case 'n':
			max_lines = atoi(optarg);
			if (max_lines <= 0) {
				fprintf(stderr, "Invalid max_lines: %s\n", optarg);
				return 1;
			}
			break;

			case 'v':
			verbose = 1;
			break;

			default:
			fprintf(stderr, "Usage: %s [-n max_lines] [-v]\n", argv[0]);
			return 1;
			}
		} //Close w-loop


	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int line_count = 0;
	int char_count = 0;

	//Read stdin line by line
	while(!shutdown_flag && (read = getline(&line, &len, stdin)) != -1) {
		line_count++;
		char_count += read;

		if(verbose) {
			printf("%s", line);
		}

		//Check if SIGUSR 1 was received
		if(stats_flag){
			fprintf(stderr, "Lines so far: %d, Characters: %d\n", line_count, char_count);
			stats_flag = 0; //Reset
		}

		//Stop after reaching max_lines
		if(max_lines > 0 && line_count >= max_lines){
			break;
		}
	} //Close w-loop


	free(line);

	//Print stats to stderr
	fprintf(stderr, "Lines: %d\n", line_count);
	fprintf(stderr, "Characters: %d\n", char_count);

	return 0;

} //Closes main
