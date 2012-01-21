#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

void set_socket(struct sockaddr_in *socket, int type, int host_short)
{
	socket -> sin_family = type;
	socket -> sin_port = htons(host_short); 		//converts host byte order to network byte order.
}

int main()
{
	int sid = 0, bid = 0, con = 0;
	char *send_data = (char *)malloc(1024*sizeof(char));
	char *receive_data = (char *)malloc(1024*sizeof(char));
	char *ans = (char *)malloc(1024*sizeof(char));
	char *send_data1 = (char *)malloc(1024*sizeof(char));
	char *receive_data1 = (char *)malloc(1024*sizeof(char));
	struct hostent *host;
	struct sockaddr_in server_socket;
	int size = sizeof(server_socket);
	
	if((sid = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		printf("Connection error at client side..\n");
		exit(1);
	}

	set_socket(&server_socket, AF_INET, 6000);

	if (inet_aton("10.1.11.123", &server_socket.sin_addr)==0) 
	{
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}

	printf("Do you want to browse server (yes/no) : ");
	scanf("%s",ans);
	if(strcmp(ans,"yes")==0 || strcmp(ans,"YES"))
	{
		int cnt = 0;
		sendto(sid,ans,1024,0, (struct sockaddr *)&server_socket, size);
		while(1)
		{
			recvfrom(sid, receive_data1,1024, 0, (struct sockaddr *)&server_socket, &size);
			if(strcmp(receive_data1, "ENDOFFILE")==0)
			{
				printf("============================================================\n");
				break;
			}
			else
			{
				printf ("%s",receive_data1);
			}	
		}
	}		
    

	printf("Enter the name of the file you want to see : ");
	scanf("%s", send_data);
	int fd = open(send_data, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IXUSR);
	sendto(sid, send_data, 1024, 0, (struct sockaddr *)&server_socket, size);
	printf("================= Contents of the File =====================\n");
	while(1)
	{
		recvfrom(sid, receive_data, 1024, 0, (struct sockaddr *)&server_socket, &size);
		printf("%s\n",receive_data);
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
