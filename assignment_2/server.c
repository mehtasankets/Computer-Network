#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>      //For htons() function
#include <fcntl.h>

typedef struct frame
{
	int seq_no;
	int type;
	char payload[1024];
}frame;

void set_socket(struct sockaddr_in *socket, int type, int host_short, int addr_type)
{
	socket -> sin_family = type;
	socket -> sin_port = htons(host_short); 		//converts host byte order to network byte order.
	socket -> sin_addr.s_addr = htonl(addr_type);
}

void set_frame(frame *fr, int seq, int type_no, char *data)
{
	fr -> seq_no = seq;
	fr -> type = type_no;
	strcpy(fr -> payload, data);
}

int main()
{
	int sid = 0, bid = 0, fp, cnt = 0;
	char *send_data = (char *)malloc(1024*sizeof(char));
	char *receive_data = (char *)malloc(1024*sizeof(char));
	char *send_data1 = (char *)malloc(1024*sizeof(char));
	char *receive_data1 = (char *)malloc(1024*sizeof(char));
	int fd, count;
	struct sockaddr_in server_socket, client_socket;
	int size = sizeof(client_socket);
	frame *receive_frame = (frame *)malloc(sizeof(frame));
	frame send_frame;

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

	recvfrom(sid, receive_data, 1024, 0,(struct sockaddr *)&client_socket, &size);
	printf("received data is : %s\n", receive_data);
	fd = open(receive_data, O_RDONLY);

	while((count=read(fd, send_data, 1024)) != 0)
	{
		printf("I am inside the loop : %d\n", cnt);
		set_frame(&send_frame, cnt, 0, send_data);
		cnt++;
		sendto(sid, &send_frame, sizeof(send_frame), 0, (struct sockaddr *)&client_socket, size);
		while(1)
		{
			recvfrom(sid, receive_frame, 1024, 0,(struct sockaddr *)&client_socket, &size);
			if(receive_frame -> seq_no == cnt)
				break;
			sendto(sid, &send_frame, sizeof(send_frame), 0, (struct sockaddr *)&client_socket, size);
		}
	}
	printf("I am outside the loop : %d\n", count);
	strcpy(send_data, "ENDOFFILE");
	set_frame(&send_frame, cnt, 0, send_data);
	sendto(sid, &send_frame, sizeof(send_frame), 0, (struct sockaddr *)&client_socket, size);
	
	fcloseall();
	close(sid);
	close(fd);
	return 0;
}
