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
  signal(SIGINT, sighandle);
  signal(SIGPIPE, SIG_IGN);

  //send random number every second
  while (1) {
	  from_client = server_handshake(&to_client);
	  
	  //While the pipe is open
	  srand(time(NULL));
	  int num = random();
	  while (write(to_client, &num, sizeof(int)) > -1) {
		  printf("SERVER (%d) : Sending %d\n", getpid(), num);
		  num = random();
		  sleep(1);
	  }
	  close(from_client);
  }

}
