#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

typedef struct frame
{
	int seq_no;
	int type;
	char payload[1024];
}frame;

void set_socket(struct sockaddr_in *socket, int type, int host_short)
{
	socket -> sin_family = type;
	socket -> sin_port = htons(host_short); 		//converts host byte order to network byte order.
}

void set_frame(frame *fr, int seq, int type_no, char *data)
{
	fr -> seq_no = seq;
	fr -> type = type_no;
	strcpy(fr -> payload, data);
}


int main()
{
	int sid = 0, bid = 0, con = 0, expect = 0;
	char *send_data = (char *)malloc(1024*sizeof(char));
	char *receive_data = (char *)malloc(1024*sizeof(char));
	char *ans = (char *)malloc(1024*sizeof(char));
	char *send_data1 = (char *)malloc(1024*sizeof(char));
	char *receive_data1 = (char *)malloc(1024*sizeof(char));
	struct hostent *host;
	struct sockaddr_in server_socket;
	int size = sizeof(server_socket);
	frame *receive_frame = (frame *)malloc(sizeof(frame));
	frame send_frame;
	
	if((sid = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		printf("Connection error at client side..\n");
		exit(1);
	}

	set_socket(&server_socket, AF_INET, 6000);

	if (inet_aton("127.0.0.1", &server_socket.sin_addr)==0) 
	{
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}

	printf("Enter the name of the file you want to see : ");
	scanf("%s", send_data);
	int fd = open("sanket.mp3", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IXUSR);
	sendto(sid, send_data, 1024, 0, (struct sockaddr *)&server_socket, size);
	printf("================= Contents of the File =====================\n");
	while(1)
	{
		int send = 0;
		recvfrom(sid, receive_frame, 1024, 0, (struct sockaddr *)&server_socket, &size);
		strcpy(receive_data, receive_frame -> payload);
		printf("Received : %s -- %s\n", receive_data, receive_frame->payload);
		if(expect == receive_frame -> seq_no)
			send = ++expect;
		else
			send = expect;
		set_frame(&send_frame, expect, 1, "");
		sendto(sid, &send_frame, sizeof(send_frame), 0, (struct sockaddr *)&server_socket, size);
		if(!strcmp(receive_data, "ENDOFFILE"))
		{
			printf("============================================================\n");
			break;
		}
		else
			write(fd, receive_data, 1024);
	}
	fcloseall();
	close(sid);

	return 0;
}
