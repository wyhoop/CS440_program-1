/* Wyatt Hooper
 * CS 440
 * Simple client example
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <err.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

const int SERVER_PORT = 11001;
// 512 is compliant with RFCX 865 recommendations
const int MAX_LINE    = 512;
int
main(int argc, char *argv[])
{
    struct sockaddr_in sin;
    char buff[MAX_LINE]; /* An array of bytes */
    struct hostent *hp;
    //char data_received;
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
    memset((char*)&sin, 0, sizeof sin); // Used 0 because we want to make the bytes null, 0 is an easy way to accomplish that
					// Zeroing out the memory occupied by sin.
    sin.sin_family = AF_INET;
    memcpy((char*)&sin.sin_addr.s_addr, hp -> h_addr, hp->h_length); /* Conceptually a for loop copying values into one array from the other byte by byte */
    sin.sin_port = htons(SERVER_PORT); // Convert to network byte order from big or little indian

    if ((s = socket(PF_INET, SOCK_STREAM, 0)) == -1)
	    err(1, "Unable to open socket");

    if (connect(s, (struct sockaddr*)&sin, sizeof sin) == -1) {
    	close(s);
	err(1, "Connect failed");
    }

    while((bytes_received = recv(s, buff, sizeof buff, 0))) {
	printf("Quote: %s\n", buff);
	}	
    
    /*NOT REACHED*/
    close(s);

    return 0;
}

// socket, binf, listen, accept
//
