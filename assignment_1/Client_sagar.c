#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main()
{
	int sock, recieved;  
        struct hostent *host; //data type is used to represent an entry in the hosts database 
				//and provides the gethostbyname(),gethostbyaddr() method
        struct sockaddr_in server_addr;  

        host =gethostbyname("10.200.56.38"); // connecting to local host machine

        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{	
		printf("Error in client Socket\n");
		exit(1);
        }

        server_addr.sin_family = AF_INET;     
        server_addr.sin_port = htons(5000);   
	server_addr.sin_addr =*((struct in_addr *)host->h_addr); 

        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{	
		printf("Error in client Socket\n");
		exit(1);
	}	
	if (connect(sock, (struct sockaddr *)&server_addr,sizeof(struct sockaddr)) == -1)
	{
		printf("Error in client connect method.\n");
		exit(1);
	}
	printf("Connection Established....\n");
	while(1)
	{
	       char send_data[1024],recv_data[1024];
 
		printf("\nEnter (q or Q to quit) : ");
		gets(send_data);

		if(strcmp(send_data,"q")!=0 || strcmp(send_data,"Q")!=0)
		{
			send(sock,send_data,strlen(send_data),0);
		}
		else
		{
			send(sock,send_data,strlen(send_data),0);
			close(sock);
			break;
		}
		recieved=recv(sock,recv_data,1024,0);
		recv_data[recieved] = '\0';
		
		printf("Rececived Data is = %s \n",recv_data);
		if (strcmp(recv_data , "q")== 0 || strcmp(recv_data , "Q") == 0)
		{
			close(sock);
			exit(1);
		}
	}
       return 0;
}
