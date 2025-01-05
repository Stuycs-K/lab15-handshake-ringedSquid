#include "pipe_networking.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main() {

  int to_server;
  int from_server;
  char message[255];
  char response[512];
  sprintf(message, "Hello from %d!", getpid());

  from_server = client_handshake( &to_server );
  while (1) {
	  while (write(to_server, message, 255) > -1) {
		  printf("CLIENT (%d) : Sent message: %s\n", getpid(), message);
		  if (read(from_server, response, 512) < 0) {
			  exit(0);
		  }
		  printf("SERVER C(%d): %s\n", getpid(), response);
	  }
  }
}
