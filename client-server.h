#define _GNU_SOURCE
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
#include<netinet/in.h>
#include<sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define SIZE 256
#define MAXSIZE 2000
#define PORT 54321
#define ADDR "127.0.0.1"
