#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>

int main(int argc , char *argv[])
{
	int createSocket, port, n;
	struct sockaddr_in server_addr;
	struct hostent *server;
	char buffer[256];

	if(argc<3)
	{
		printf("\nERROR\n");
		printf("[*]Usage: %s 'HOSTNAME' 'PORT'\n\n", argv[0]);
		return 0;
	}

	//Create client socket
	port = atoi(argv[2]);
	createSocket = socket(AF_INET , SOCK_STREAM , 0);
	if ( createSocket < 0)
	{
		printf("\n[*]Could not create socket");
	}
	else
		printf("\n[*]Success create socket\n");

	//search host(server) if available
	server = gethostbyname(argv[1]);
	if(server == NULL)
	{
		printf("Sorry no such host");
	}

	//sockaddr_in structure
	bzero((char *)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,(char *)&server_addr.sin_addr.s_addr, server->h_length);
	server_addr.sin_port = htons( port );

	//Connect to server
	if (connect(createSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		printf("[*]connect error \n\n");
		return 1;
	}
	
	puts("[*]Connected \n");

 	//send message
	for(;;)
	{
		printf("To server:");
		bzero(buffer,256);
		fgets(buffer,256,stdin);
		n = write(createSocket,buffer,strlen(buffer));
		if(n<0)
		{
			perror("[*]Error Write");
		}

		bzero(buffer,256);
		n = read(createSocket, buffer, 255);
		if(n<0)
		{
			perror("[*]Error Read");
		}
		printf("\tFrom Server:%s\n",buffer);
		if (strncmp(buffer, "bye", 3) == 0)
        	{
            		printf("[*]Client Disconnected...\n\n");
         		break;
        	}
	}

	close(createSocket);
	return 0;
}
