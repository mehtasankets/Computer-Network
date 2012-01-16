#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

int main()
{
	
	int i,sock, connected,recieved;  
	
	struct sockaddr_in server_addr,client_addr;    
	int sin_size;
	
	if((sock = socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		printf("Error occured in creating socket of server");
		exit(1);
	}
	server_addr.sin_family = AF_INET;         
	server_addr.sin_port = htons(5000);     
	server_addr.sin_addr.s_addr = INADDR_ANY; 
	
	if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))== -1)
	{
		printf("Error in server bind function");
		exit(1);
	}
	if (listen(sock, 5) == -1) 
	{      
		printf("Error in listen function");
		exit(1);
	}
	printf("\nserver Waiting for client");
	fflush(stdout);
	
	while(1)
	{
		sin_size = sizeof(struct sockaddr_in);
                connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);
		printf("Connected sucessfully......\n");
       

		while(1)
		{
			char send_data [1024],recv_data[1024];       
			memset(&send_data[0],0,sizeof(send_data));
			memset(&recv_data[0],0,sizeof(recv_data));

			recieved = recv(connected,recv_data,1024,0);
	        	recv_data[recieved] = '\0';
			for(i = 0 ; recv_data[i]!='\0'; i++)
			{
				send_data[i] = toupper(recv_data[i]);
			}
	                if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0)
	                {
		                close(connected);
		                break;
			}
			else
			printf("\n RECIEVED DATA = %s " , recv_data);
			
			send(connected, send_data,strlen(send_data), 0);  
			printf("\n Send data is  = %s ",send_data);
			
			fflush(stdout);
			
		}
					
	}
	close(sock);
	return 0;
}

