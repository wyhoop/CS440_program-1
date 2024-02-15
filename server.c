#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <err.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

const int SERVER_PORT = 9876; /*Server Port Number*/
const int MAX_PENDING = 5;    /*Max Allowed in Queue*/
const int MAX_LINE    = 256;  /*Maximum Buffer Size*/

int
main(void)
{
    puts("HERE");
    char buff[MAX_LINE]; /*Buffer*/
    struct sockaddr_in sin;
    socklen_t new_len;
    int new_s, s;

    FILE *file = fopen("quotes.txt", "r");
    if (file == NULL)
    {
        err(1, "Unable To Open File");
    }
   
    char *line = NULL;
    size_t len = 0;

    memset((char*)&sin, 0, sizeof sin); /* Used 0 to make the bytes null */
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(SERVER_PORT); /* Sets the port number for the server to listen on */

    /* Passive Open */
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        err(1, "unable to open socket");
    puts("AFTER PASSIVE OPEN");
    if ((bind(s, (struct sockaddr*)&sin, sizeof sin)) == -1)
	err(1, "Unable to bind socket");
    puts("AFTER BIND");
    if ((listen(s, MAX_PENDING)) == -1)
	err(1, "Listen on socket failed");
    puts("AFTER LISTEN");
    /* Get Next Connection */
    while (1) {
	new_len = sizeof sin;
	if ((new_s = accept(s, (struct sockaddr*)&sin, &new_len)) == -1) {
	    close(new_s);
	    err(1, "Accept Failed");
	}

	/* while (recv(new_s, buff, sizeof buff, 0) > 0)
	    fputs(buff, stdout); */
	while(getline(&line, &len, file) != -1)
	{
	    printf(line, stdout);
	    puts("\n");
	}

    puts("AFTER RECV");
	close(new_s);

    }

    /* NOT REACHED */
    close(s);

    return 0;
}
