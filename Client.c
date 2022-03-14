#include "client-server.h"

/*#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>
#define PORT 54321
#define SIZE 256
#define MAXSIZE 2000
#define ADDR "127.0.0.1"
*/

int main()
{

  char dir[SIZE];
  int sockfd=socket(AF_INET,SOCK_STREAM,0);
  if(sockfd == -1)
  {
   printf("Socket Creation Failed\n");
   exit(0);
  }
  printf("Socket is created sucessfully\n");

  char client_mess[SIZE];
  struct sockaddr_in server_addr;
  server_addr.sin_family= AF_INET;
  server_addr.sin_port= htons(PORT);
  server_addr.sin_addr.s_addr= inet_addr(ADDR);

  if(connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr))<0)
  {
   perror("Failed to connect.\n");
   exit(0);
  }
  printf("Sucessfully conected with server\n");


  char serv_resp[MAXSIZE];

  while(1)
  {

   printf("Enter the command: ");
   scanf("%s",client_mess);

   send(sockfd,client_mess,sizeof(client_mess),0);

   if(strcmp("bye",client_mess)==0)
   {
    printf("Bye!!\n");
    close(sockfd);
    exit(0);
   }


   if(strcmp("cd",client_mess)==0)
   {
    printf("Enter directory name :");
    scanf("%s",dir);
    send(sockfd,dir,sizeof(dir),0);
   }       

   memset(dir, '\0', sizeof dir);

   if(recv(sockfd,&serv_resp,sizeof(serv_resp),0)<0)
   {
    printf("Receive failed\n");
    exit(0); 
   }

   printf("Message from server: %s\n",serv_resp);

   memset(serv_resp, '\0', sizeof serv_resp);

  }
  close(sockfd);
  exit(0);
}
