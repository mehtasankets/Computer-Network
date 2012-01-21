#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>      //For htons() function
#include <fcntl.h>

void set_socket(struct sockaddr_in *socket, int type, int host_short, int addr_type)
{
	socket -> sin_family = type;
	socket -> sin_port = htons(host_short); 		//converts host byte order to network byte order.
	socket -> sin_addr.s_addr = htonl(addr_type);
}

int main()
{
	int sid = 0, bid = 0, fp;
	char *send_data = (char *)malloc(1024*sizeof(char));
	char *receive_data = (char *)malloc(1024*sizeof(char));
	char *send_data1 = (char *)malloc(1024*sizeof(char));
	char *receive_data1 = (char *)malloc(1024*sizeof(char));
	int fd, count, cnt=0;
	struct sockaddr_in server_socket, client_socket;
	int size = sizeof(client_socket);

	if((sid = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		printf("Connection error..\n");
		exit(1);
	}

	set_socket(&server_socket, AF_INET, 6000, INADDR_ANY);

	if((bid = bind(sid, (struct sockaddr *)&server_socket, sizeof(struct sockaddr))) == -1)
	{
		printf("Binding error..\n");
		exit(1);
	}

	printf("I am waiting for client..\n");

	recvfrom(sid, receive_data1, 1024, 0,(struct sockaddr *)&client_socket, &size);
	printf("received data is : %s\n", receive_data1);

	if(strcmp(receive_data1,"yes")==0 || strcmp(receive_data1,"YES")==0)
	{
		system("ls>>root.txt");
		FILE * fd = fopen("root.txt","r");
		int r;
		while(!feof(fd))
		{
			fread(send_data1,1024,1,fd);
			sendto(sid, send_data1,1024, 0, (struct sockaddr *)&client_socket, size);
		}
		strcpy(send_data1, "ENDOFFILE");
		sendto(sid, send_data1,1024, 0, (struct sockaddr *)&client_socket, size);
	}

	recvfrom(sid, receive_data, 1024, 0,(struct sockaddr *)&client_socket, &size);
	printf("received data is : %s\n", receive_data);
	fd = open(receive_data, O_RDONLY);

	while((count=read(fd, send_data, 1024)) == 1024)
	{
		printf("I am inside the loop : %d\n", cnt++);
		sendto(sid, send_data, 1024, 0, (struct sockaddr *)&client_socket, size);
	}
	printf("I am outside the loop : %d\n", count);
	if(count > 0)
		sendto(sid, send_data, count, 0, (struct sockaddr *)&client_socket, size);
	strcpy(send_data, "ENDOFFILE");
	sendto(sid, send_data, 1024, 0, (struct sockaddr *)&client_socket, size);
	
	system("rm root.txt");

	fcloseall();
	close(sid);
	close(fd);
	return 0;
}
