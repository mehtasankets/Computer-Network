#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>      //For htons() function

void set_socket(struct sockaddr_in *socket, int type, int host_short, int addr_type)
{
	socket -> sin_family = type;
	socket -> sin_port = htons(host_short); 		//converts host byte order to network byte order.
	socket -> sin_addr.s_addr = addr_type;
}

int main()
{
	int sid = 0, bid = 0, con = 0;
	int size = sizeof(struct sockaddr_in);
	char *data = (char *)malloc(256*sizeof(char));
	struct sockaddr_in server_socket, client_socket;
	
	if((sid = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("Connection error..\n");
		exit(1);
	}

	set_socket(&server_socket, AF_INET, 5000, INADDR_ANY);

	if((bid = bind(sid, (struct sockaddr *)&server_socket, sizeof(struct sockaddr))) == -1)
	{
		printf("Binding error..\n");
		exit(1);
	}

	if(listen(sid, 5) == -1)
	{
		printf("Listening error..\n");
		exit(1);
	}

	printf("I am waiting for client..\n");
	fflush(stdin);

	con = accept(sid, (struct sockaddr *)&client_socket, &size);
	recv(con, data, 255, 0);
	printf("received data is : %s\n", data);
	close(con);

	close(sid);
	return 0;
}
