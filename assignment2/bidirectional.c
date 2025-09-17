#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
	int pipe1[2]; //Parent to chikd
	int pipe2[2]; //Child to parent


	//Create both pipes
	if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
		perror("pipe");
		exit(1);
	}

	//Fork child
	pid_t pid = fork();
	if(pid < 0) {
		perror("fork");
		exit(1);
	}

	if(pid == 0){
		close(pipe1[1]); //Closing unsued endsd
		close(pipe2[0]);

		char buffer[100];
		ssize_t n = read(pipe1[0], buffer, sizeof(buffer)-1);
		if(n>0){
			buffer[n] = '\0'; //Null termination
			printf("Child received: %s\n", buffer);
		}

		char *response = "Hello from child";
		write(pipe2[1], response, strlen(response));

		close(pipe1[0]);
		close(pipe2[1]);

		exit(0);
	}

	else{
		close(pipe1[0]);
		close(pipe2[1]);

		char *message = "Hello from parent"; //Send message to child
		write(pipe1[1], message, strlen(message));

		char buffer[100]; //Read response from child
		ssize_t n = read(pipe2[0], buffer, sizeof(buffer)-1);
		if(n>0) {
			buffer[n] = '\0';
			printf("Parent received: %s\n", buffer);
		}

		close(pipe1[1]); //Close used ends
		close(pipe2[0]);

		wait(NULL); //Wait for child to finish
	}


} //End main
