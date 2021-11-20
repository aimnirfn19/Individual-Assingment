#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>	//inet_addr

int main(int argc, char *argv[])
{
	//argument fail if not execute with port
	if(argc < 2)
	{
		printf("\n[*]No port provided. Program end...\n\n*****NOTES*****\n");
		printf("[*]Usage: %s 'PORT' \n\n ", argv[0]);
		return 1;
	}


	//declaration
	int createSocket , clientSocket ,port, x, n;
	struct sockaddr_in server , client;
	socklen_t clien;
	char buffer[256];

	//Create server socket
	createSocket = socket(AF_INET , SOCK_STREAM , 0);
	if (createSocket < 0 )
	{
		printf("[*]Could not create socket");
	}
	else
		printf("\n[*]Success create socket \n");

	//Prepare the sockaddr_in structure
	bzero((char *)&server, sizeof(server));
        port = atoi(argv[1]);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( port );

	//Bind server
	if( bind(createSocket,(struct sockaddr *)&server , sizeof(server)) < 0 )
	{
		puts("[*]bind failed\n");
		return 1;
	}
	else
	{
		printf("[*]bind done at port %d\n",port);
	}

	//Listen
	listen(createSocket , 3);

	//Client incoming connection
	puts("[*]Waiting for client connections...");

	//Client connected or not
	clien=sizeof(client);
	clientSocket = accept(createSocket, (struct sockaddr *)&client, &clien);
	if (clientSocket<0)
	{
		perror("[*]accept failed");
	}
	puts("[*]Connection accepted ");

	//read and send message to client
	for(;;)
	{
		bzero(buffer,255);
		n = read(clientSocket, buffer, sizeof(buffer));
		printf("From client: %s\t To client: ",buffer);
		if(n<0)
		{
			perror("[*]Error read");
		}
		bzero(buffer,255);
		n = 0;
		while ((buffer[n++] = getchar()) != '\n');
		n = write(clientSocket,buffer,255);
		if(n<0)
		{
			perror("[*]Error write");
		}
		if (strncmp("bye", buffer, 2) == 0) {
            	printf("\n[*]Server Diconnected...\n\n");
            	break;
		}
	}

	close(createSocket);
	return 0;
}
