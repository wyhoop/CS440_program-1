/* Wyatt Hooper
 * CS 440 Assignment 1
 * February 21, 2024
 * This program is representative of a simple client in C. It
 * receives incoming data from the server listening on the port
 * 8017, then it stores the received data into a buffer and prints
 * the quote while the program is still receiving data. It should
 * conform to the RFC 865 protocol. 
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <err.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

const int SERVER_PORT = 8017;
const int MAX_LINE    = 512;
// 512 is recommended by RFC 865 
int
main(int argc, char *argv[])
{
    struct sockaddr_in sin;
    char buff[MAX_LINE]; /* An array of bytes */
    struct hostent *hp;
    char* hostname; /* Points to where it is stored */
    ssize_t bytes_received;
    int s;

    /* Process command line arguments*/
    if(argc == 2)
	// all good, grab argument
	hostname = argv[1];
    else 
	// not enough args, display error and terminate
	err(1, "Usage: client hostanme");

    /* Ask resolver for IP address of hostname */
    hp = gethostbyname(hostname);
    if(hp == NULL)
	errx(1, "Unknown Host %s", hostname);

    /* build address data structure*/
    // C-style cast
    memset((char*)&sin, 0, sizeof sin);
			
    sin.sin_family = AF_INET;
    memcpy((char*)&sin.sin_addr.s_addr, hp -> h_addr, hp->h_length);
    
    // Convert to network byte order from big or little endian
    sin.sin_port = htons(SERVER_PORT);

    if ((s = socket(PF_INET, SOCK_STREAM, 0)) == -1)
	    err(1, "Unable to open socket");

    if (connect(s, (struct sockaddr*)&sin, sizeof sin) == -1) {
    	close(s);
	err(1, "Connect failed");
    }

    while((bytes_received = recv(s, buff, sizeof buff, 0))) {
	printf("%s",buff);
    }	
    
    /*NOT REACHED*/
    close(s);

    return 0;
}

