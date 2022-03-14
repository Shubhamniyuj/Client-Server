#include "client-server.h"

/*#define _GNU_SOURCE
#include<stdio.h>
#include<bsd/string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/stat.h>
#include<time.h>
#include<pthread.h>
#define SIZE 256
#define MAXSIZE 2000

#include<sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define PORT 54321
*/
typedef struct client
{
int socketfd;
struct sockaddr_in client_addr;
char client_path[SIZE];
}node;


void *connection_handler(void *sock_desc);

int main()
{
  int n_clients;
 
  int sockfd=socket(AF_INET, SOCK_STREAM, 0);
    
  if (sockfd == -1)
  {
   printf("Socket creation failed");
   return 1;
  }
   printf("Socket created successfully\n");
   
   struct sockaddr_in server_addr; 
   server_addr.sin_family = AF_INET; 
   server_addr.sin_addr.s_addr = INADDR_ANY;
   server_addr.sin_port = htons(PORT); 
    
   if(bind(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr))<0)
  {
   perror("Binding failed.");
   return 1;
  }
   printf("Binded sucessfully\n");
   
   printf("Enter the number of clients: \n");
   scanf("%d",&n_clients);
   listen(sockfd, n_clients);

   int clientLen = sizeof(struct sockaddr_in);
        
   printf("Waiting for clients to connect...\n");
   for(int i=0;i<n_clients;i++)
  {
    node *ptr=(node *)malloc(MAXSIZE);           
    
    if((ptr->socketfd= accept(sockfd,(struct sockaddr *)&ptr->client_addr,(socklen_t*)&clientLen))<0)
    {	
      printf("Failed to accept connection");
      close(sockfd);
      free(ptr);
      exit(0);
    }




    printf("Connection accepted for Client[%d]\n",i+1);
             
          
    pthread_t * pthreads[n_clients];

    pthread_t tid[i];
    pthreads[i]= &tid[i];
              
    if(pthread_create(pthreads[i], NULL, connection_handler, (void *)ptr) < 0){

    printf("Couldn't create thread"); 
    close(ptr->socketfd);
    free(ptr);
    return -1;
               
    }
             
   }
 close(sockfd);
 pthread_exit(NULL); 
 exit(0); 
}
         


void *connection_handler(void *sock_desc)
{
   
  node *ptr= (node*)sock_desc;
  char client_message[SIZE];
  char serv_resp[MAXSIZE];
  char tmp[SIZE];
    
  strlcpy(ptr->client_path,get_current_dir_name(),MAXSIZE);
 
  while(1)
  {
        

    memset(client_message, '\0', sizeof client_message);
                   
    if(recv(ptr->socketfd, client_message, sizeof(client_message), 0)< 0)
    {
     printf("Could not receive");
     close(ptr->socketfd);
     return(NULL);
            
    }
    printf("Client replied : %s\n",client_message);
        
    if(strcmp("pwd",client_message)==0) //to execute pwd
    {
   
    strlcpy(serv_resp,ptr->client_path,sizeof(serv_resp));
    
    }
        
    else if(strcmp("ls",client_message)==0) //to execute ls
    {
     DIR *dir=opendir(ptr->client_path);
        
     if (dir == NULL) {
     strlcpy(serv_resp,"Could not open directory.",sizeof(serv_resp)); 
     }

     else
     {
      struct stat filestat; //time
      struct dirent* entity;
      entity= readdir(dir);
      while(entity != NULL)
      {
       stat(entity->d_name,&filestat);
        
       if(entity->d_type==4){
       strlcat(serv_resp,"dir:\t",sizeof(serv_resp));
       }
       else if(entity->d_type==8)
       strlcat(serv_resp,"file:\t",sizeof(serv_resp));
   
       strlcat(serv_resp,entity->d_name,sizeof(serv_resp));
       strlcat(serv_resp,"\t",sizeof(serv_resp));
       char time[SIZE];
        
       ctime_r(&filestat.st_mtime,time);
       strlcat(serv_resp,time,sizeof(serv_resp));
        
       memset(time, '\0', sizeof time);

       entity= readdir(dir);
       }
      } 
     closedir(dir);
        
    }      
  
      
    else if(strcmp("cd",client_message)==0) //to enter a specific dir
    {
        
    recv(ptr->socketfd,tmp, sizeof(tmp), 0);
     
    bzero(ptr->client_path,MAXSIZE);
       
    strlcpy(ptr->client_path,tmp,MAXSIZE);
      
    DIR *dirptr=opendir(tmp);
    if(dirptr==NULL)
     {
     strlcpy(serv_resp,"\nDirectory does not exist.",sizeof(serv_resp));
     strlcpy(ptr->client_path,get_current_dir_name(),MAXSIZE);
     }

    else
     {
     closedir(dirptr);
     bzero(tmp,SIZE);
     strlcpy(serv_resp,"Changed directory successfully",sizeof(serv_resp));
 
     }
      
    } 
       
    else if(strcmp("bye",client_message)==0) //to close connection
    {
     printf("Disconnected from client.\n");
     close(ptr->socketfd);
     return NULL;
    }
   
    else
    {
     strlcpy(serv_resp,"Enter a valid command",sizeof(serv_resp)); //if client enters any other command
        
    }
 
    if(send(ptr->socketfd,serv_resp, sizeof(serv_resp), 0)<0)
    {
     printf("Could not send.\n");
     close(ptr->socketfd);
     return NULL;
    }
             
    memset(serv_resp, '\0', sizeof serv_resp);
    
  }
    
  free(ptr);    
}
