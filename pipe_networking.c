#include "pipe_networking.h"

void debug(char* s, int type) {
	if (DEBUG) {
		//server
		if (type == 1) {
			printf("SERVER (%d) : ", getpid());
		}
		//client
		if (type == 0) {
			printf("CLIENT (%d) : ", getpid());
		}
		printf("%s\n", s);
	}
}

//UPSTREAM = to the server / from the client
//DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a  connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
	int from_client;
	//make pipe
	debug("Creating WKP...", 1);
	mkfifo(WKP, 0666);
	//Wait for pipe
	debug("Waiting for SYN...", 1);
	from_client = open(WKP, O_RDONLY);
	//Delete pipe once connection is made
	debug("SYN received! Deleting WKP...", 1);
	remove(WKP);
	return from_client;
}

/*=========================
  server_handshake 
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {
  int from_client = server_setup();
  char buff[255];
  //Read in name of private pipe, receive SYN
  debug("Getting PP...", 1);
  read(from_client, buff, 255);
  debug("Opening PP...", 1);
  *to_client = open(buff, O_WRONLY);
  //Write to the private pipe, SYN_ACK
  int rand = (int)random();	  
  int rand2;
  debug("Sending SYN_ACK...", 1);
  write(*to_client, &rand, sizeof(int));
  debug("Waiting for ACK...", 1);
  read(from_client, &rand2, sizeof(int));
  if (rand2 == (rand+1)) {
	  debug("ACK received!", 1);
  }
  else {
	  debug("Error with ACK...", 1);
  }
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
	int from_server;
	char path[255];
	sprintf(path, "%d", getpid());
	debug("Creating PP...", 0);
	mkfifo(path, 0666);
	debug("Opening WKP...", 0);
	*to_server = open(WKP, O_WRONLY);
	debug("Sending SYN...", 0);
	write(*to_server, path, 255);
	debug("Waiting for SYN_ACK...", 0);
	from_server = open(path, O_RDONLY);
	int rand;
	debug("SYN_ACK Received! Sending ACK...", 0);
	read(from_server, &rand, sizeof(int));
	rand++;
	write(*to_server, &rand, sizeof(int));
	debug("Removing PP...", 0);
	remove(path);
	return from_server;
}
