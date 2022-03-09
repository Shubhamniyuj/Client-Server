#include<stdio.h>
#include<bsd/string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<dirent.h>
#include <sys/stat.h>
#include <time.h>
#include<pthread.h>
#define SIZE 256
#define MAXSIZE 2000

#define PORT 54321


void *connection_handler(void *sock_desc);

int main()
{
  struct sockaddr_in client;
  char client_message[SIZE];
  char serv_resp[MAXSIZE];
  char tmp[SIZE];
  
  int *new_sockp,n_clients,val;
    
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
   int new_sock;
        
   printf("Waiting for clients to connect...\n");
   for(int i=0;i<=n_clients;i++)
   {
               
    if(val== n_clients)
    printf("Server has reached maximum clients now...\n");

    new_sock= accept(sockfd,(struct sockaddr *)&client,(socklen_t*)&clientLen);
    printf("Connection accepted for Client[%d]\n",i+1);
             
    new_sockp[SIZE];
    *new_sockp= new_sock;
               
    pthread_t * pthreads[n_clients];

    pthread_t tid[i];
    pthreads[i]= &tid[i];
              
    if(pthread_create(pthreads[i], NULL, connection_handler, (void *)new_sockp) < 0){

    printf("Couldn't create thread");
    return -1;
               
    }
             
    val++;
                
   }
   close(sockfd);
}
         


void *connection_handler(void *sock_desc)
{
   int sock= *(int*)sock_desc;   
   char client_message[SIZE];
   char serv_resp[MAXSIZE];
   char tmp[SIZE];
 

   while(1)
   {
        
	
    memset(client_message, '\0', sizeof client_message);
                   
    if(recv(sock, client_message, sizeof(client_message), 0)< 0)
    {
     printf("Could not receive");
            
    }
     printf("Client replied : %s\n",client_message);
        
    if(strcmp("pwd",client_message)==0) //to execute pwd
    {
    if (getcwd(serv_resp, sizeof(serv_resp)) == NULL)
     perror("getcwd() error");
    }
        
    else if(strcmp("ls",client_message)==0) //to execute ls
    {
     DIR *dir=opendir(".");
        
     if (dir == NULL) {
     printf("Could not open dir\n");
     return NULL;
     }
	     
     struct stat filestat; //time
     struct dirent* entity;
     entity= readdir(dir);
     while(entity != NULL){
     stat(entity->d_name,&filestat);
        
     if(entity->d_type==DT_DIR){
     strlcat(serv_resp,"dir:\t",sizeof(serv_resp));
     }
     else if(entity->d_type==DT_REG)
     strlcat(serv_resp,"file:\t",sizeof(serv_resp));
   
     strlcat(serv_resp,entity->d_name,sizeof(serv_resp));
     strlcat(serv_resp,"\t",sizeof(serv_resp));
     strlcat(serv_resp,ctime(&filestat.st_mtime),sizeof(serv_resp));
     entity= readdir(dir);
     }
     }      
  
      
    else if(strcmp("cd",client_message)==0) //to enter a specific dir
    {
        
     recv(sock,tmp, sizeof(tmp), 0);
     int ch=chdir(tmp);
       
     memset(tmp, '\0', sizeof tmp);
     memset(client_message, '\0', sizeof client_message);
     if(ch<0)
     strlcpy(serv_resp,"Failed to change directory",sizeof(serv_resp));
     else
     {
     printf("Changed directory successfully\n");
     strlcpy(serv_resp,"Successfully changed directory",sizeof(serv_resp));
     }
     } 
       
     else if(strcmp("bye",client_message)==0) //to close connection
     {
      printf("Disconnected from client.\n");
      close(sock);  
      return NULL;
     }
     
    else
    {
         
     strlcpy(serv_resp,"Enter a valid command",sizeof(serv_resp)); //if client enters any other command
        
    }
 
    if(send(sock,serv_resp, sizeof(serv_resp), 0)<0)
    {
     printf("Could not send.\n");
     return NULL;
    }
             
    memset(serv_resp, '\0', sizeof serv_resp);
    }
    
    
}
