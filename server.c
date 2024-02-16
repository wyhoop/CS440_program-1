/*
 * Written By: Wyatt Hooper
 * CS440 Assignment 1
 * February 15, 2024
 * This program is representative of a simple server in C. It 
 * opens up and listens on a user defined port and waits for a 
 * connection to be made by a client. It then sends the client 
 * terminal a Quote of the Day according to RFC 865 protocol. 
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <err.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

const int SERVER_PORT = 11001; /*Server Port Number*/
const int MAX_PENDING = 5;    /*Max Allowed in Queue*/
const int MAX_LINE    = 256;  /*Maximum Buffer Size*/

int
main(void)
{
    struct sockaddr_in sin;
    socklen_t new_len;
    int new_s, s;

    /* Open the file in read mode */
    FILE *file = fopen("quotes.txt", "r");

    if (file == NULL)
    {
	/* Handle File open errors */
        err(1, "Unable To Open File");
    }
   
    char *line = NULL;
    size_t length = 0;

    memset((char*)&sin, 0, sizeof sin); /* Used 0 to make the bytes null */
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    // Sets the port number for the server to listen on
    sin.sin_port = htons(SERVER_PORT); 
   
    /* Passive Open */
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) == -1)
	err(1, "unable to open socket");

    if ((bind(s, (struct sockaddr*)&sin, sizeof sin)) == -1)
	err(1, "Unable to bind socket");

    if ((listen(s, MAX_PENDING)) == -1)
	err(1, "Listen on socket failed");

    /* Get Next Connection */
    while (1) {
	new_len = sizeof sin;
	if ((new_s = accept(s, (struct sockaddr*)&sin, &new_len)) == -1) {
	    close(new_s);
	    err(1, "Accept Failed");
	}

	// This if block replaces the previous while loop that took in data.
	// I did this since we will be outputting using the contents of a 
	// file
	if (getline(&line, &length, file)!= -1)
	{	
	    /* This sends and outputs a quote */	
	    send(new_s, line, strlen(line), 0);
        }

	close(new_s); /* Closes the socket associated with new_s */

    }
   /* Close the file */ 
    fclose(file);
    /* NOT REACHED */
    close(s); /* Closes the socket associated with the server */

    return 0;
}
