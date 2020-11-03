/**************************
*     socket example, server
*     Winter 2019
***************************/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
#include <netdb.h>


int main (int, char *[]);



char* hostname_to_ip(char * hostname , char* ip)
{
    char *IPbuffer;
    struct hostent *host_entry;

    // To retrieve hostname


    // To retrieve host information
    host_entry = gethostbyname(hostname);
    if (host_entry == NULL)
    {
        perror("gethostbyname");
        exit(1);
    }

    // To convert an Internet network
    // address into ASCII string
    ip = inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));

    printf("Hostname: %s\n", hostname);
    printf("Host IP: %s\n", ip);

    return ip;
}

/*********************
* main
*********************/
int main (int argc, char *argv[])
{
    char* ip;
    char hostname[200];
   int listenfd = 0, connfd = 0;
   struct sockaddr_in serv_addr;
   char buff[2048];

   // set up
   memset (&serv_addr, '0', sizeof (serv_addr));
   memset (buff, '\0', sizeof (buff));

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = htonl (INADDR_ANY);
   serv_addr.sin_port = htons (5500);

   // create socket, bind, and listen
   listenfd = socket (AF_INET, SOCK_STREAM, 0);
   int option = 1;
   setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
   if(bind (listenfd, (struct sockaddr*)&serv_addr, sizeof (serv_addr))<0){
       perror("Bind failed");
       return 1;
   }
   listen (listenfd, 10);

   // accept and interact
   connfd = accept (listenfd, (struct sockaddr*)NULL, NULL);
   while (1){


       if (read(connfd, buff, sizeof (buff))>0){
           printf("%s\n",buff);
       }

       sscanf(buff,"%*s %*s %*s %*s %s",hostname);
       printf("hostname:%s\n",hostname);
       if(strcmp("http.00.s.sophosxl.net", hostname)==0){
           printf("connected to sophos\n");
           return 1;
       }
       ip = hostname_to_ip(hostname,ip);

       printf("%s\n\n\n",ip);


        int sockfd = 0;
       	struct sockaddr_in host_addr;
       	memset (&host_addr, '0', sizeof (host_addr));
       	// open socket
       	if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
       	{
       		printf ("Error : Could not create socket \n");
       		return 1;
       	}
       	// set address
       	host_addr.sin_family = AF_INET;
       	host_addr.sin_port = htons (80);
       	//argv[2] is ip address
       	if (inet_pton (AF_INET, ip, &host_addr.sin_addr) <= 0)
       	{
       		printf ("inet_pton error occured\n");
       		return 1;
       	}
       	// connect
       	if (connect (sockfd, (struct sockaddr *)&host_addr, sizeof (host_addr)) < 0)
       	{
       		printf ("Error : Connect Failed \n");
       		return 1;
       	}
        //write(listenfd,"HTTP/1.1 200 OK")
       	write(sockfd, buff, strlen(buff)+1);        //send get
        //printf("hello?\n" );
        memset (buff, '0', sizeof (buff));  //clear buffer for response

       	while(read(sockfd, buff, sizeof (buff))>0){
            printf("%s\n",buff);
            write(connfd, buff, strlen(buff)+1);
            memset (buff, '0', sizeof (buff));
        }
        close(sockfd);
        close (connfd);
        return 0;
   }
     close (connfd);

   return 0;
}
