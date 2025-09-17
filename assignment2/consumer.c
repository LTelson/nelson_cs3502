#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <time.h>


int main (int argc, char *argv[]) {
	int max_lines = -1; //Unlimited
	int verbose = 0;

	//Parse arguments (-n max_lines, -v verbose)
	int opt;
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
	while((read = getline(&line, &len, stdin)) != -1) {
		line_count++;
		char_count += read;

		if(verbose) {
			printf("%s", line);
		}

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
