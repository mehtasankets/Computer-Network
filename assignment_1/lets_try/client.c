#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void set_socket(struct sockaddr_in *socket, int type, int host_short, char *addr_type)
{
	socket -> sin_family = type;
	socket -> sin_port = htons(host_short); 		//converts host byte order to network byte order.
	socket -> sin_addr = *((struct in_addr *)addr_type);
}

int main()
{
	int sid = 0, bid = 0, con = 0;
	char *send_data = (char *)malloc(16*sizeof(char));
	char *receive_data = (char *)malloc(16*sizeof(char));
	struct hostent *host;
	struct sockaddr_in server_socket;
	FILE *fp = fopen("output.jpg", "wb");

	host = gethostbyname("localhost");  			//requires netdb.h
	
	if((sid = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("Connection error at client side..\n");
		exit(1);
	}

	set_socket(&server_socket, AF_INET, 6000, host -> h_addr);
	server_socket.sin_family = AF_INET;     
	server_socket.sin_port = htons(6000);   
	server_socket.sin_addr =*((struct in_addr *)host->h_addr);   //requires netdb.h 

	connect(sid, (struct sockaddr *)&server_socket, sizeof(struct sockaddr));

	printf("Enter the name of the file you want to see : ");
	scanf("%s", send_data);
	send(sid, send_data, 16, 0);
	printf("================= Contents of the File =====================\n");
	while(1)
	{
		recv(sid, receive_data, 16, 0);
		if(!strcmp(receive_data, "ENDOFFILE"))
		{
			printf("============================================================\n");
			break;
		}
		else
			fprintf(fp,"%s", receive_data);
	}
	fcloseall();
	close(sid);

	return 0;
}
