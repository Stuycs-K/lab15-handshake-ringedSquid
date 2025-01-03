#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pipe_networking.h"

int main() {
  int to_client;
  int from_client;

  //send random number every second
  while (1) {
	  from_client = server_handshake(&to_client);
	  
	  //While the pipe is open
	  srand(time(NULL));
	  int num = random();
	  while (write(to_client, &num, sizeof(int))) {
		  printf("SERVER (%d) : Sending %d\n", getpid(), num);
		  num = random();
		  sleep(1);
	  }
	  close(from_client);
  }

}
