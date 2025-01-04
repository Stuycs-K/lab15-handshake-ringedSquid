#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include "pipe_networking.h"

void sighandle(int signo) {
	remove(WKP);
	exit(0);
}

int main() {
  int to_client;
  int from_client;
  int num_client = 0;
  signal(SIGINT, sighandle);

  //send random number every second
  while (num_client < 10) {
	  //wait for client
	  from_client = server_setup();
	  num_client++;
	  //Fork subserver
	  pid_t subserver = fork();
	  if (server_handshake_half(&to_client, from_client) != 0) {
		  fprintf(stderr, "error with handshake! %s\n", strerror(errno));
		  exit(1);
	  }

	  printf("SERVER (%d) : %dth client connected!\n", getpid(), num_client);

	  if (subserver == 0) {
		  char message[255];
		  char response[512];
		  while (read(from_client, message, 255) > -1) {
			  sleep(1);
			  printf("SERVER (%d) : From client (%d): %s\n", getpid(), num_client, message);
			  sprintf(response, "Why did you say: %s", message);
			  write(to_client, response, 512);
		  }
		  exit(0);
	  }
  }

}
