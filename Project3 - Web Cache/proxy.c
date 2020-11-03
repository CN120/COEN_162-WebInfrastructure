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

#define cacheSize 10
static char buff[2048];
static char response[10];
static char page[200];
static char path[210];
static char cache[cacheSize][200];
static int cacheCount = 0;
int buffLen = sizeof(buff);
int main (void);



void consumeHTTP(FILE**fp){
    char c;
    //char* http;
    int counter=0;
    while(counter!=4){
        c = fgetc(*fp);
        if(c =='\r' || c=='\n'){
            counter++;
            printf("%c",c);
        }
        else{
            counter = 0;
        }
        //sprintf(http,"%c",c);
    }
}

void alphaChars()
{
    char c;
    for(int i = strlen(page)-1;i>=0; i--){
        c = page[i];
        if( (c>='a' && c<='z') || (c>='A' && c<='Z')){

        }
        else{
            page[i]='_';
        }
    }
}

int checkCache(){
    for(int i=0; i<cacheSize; i++){
        if(strcmp(cache[i],path)==0){
            return i;
        }
    }
    return -1;
}

char* hostname_to_ip(char * hostname , char* ip)
{
    char *IPbuffer;
    struct hostent *host_entry;


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

    //printf("Hostname: %s\n", hostname);
    //printf("Host IP: %s\n", ip);

    return ip;
}

void printCache()
{
    printf("Current Cache list\n~~~~~~~~~~~~~~~~~~\n");
    for(int i=0; i<cacheCount; i++){
        printf("%s\n",cache[i]);
    }
}

/*********************
* main
*********************/
int main (void)
{
    int bytesRead;

    printf("\n\n==============================\n|      new program run       |\n==============================\n\n");
    char* ip;
    char hostname[50];
    char type[4];

    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    int buffLen = sizeof(buff) - 1;
    buff[2047] = '\0';


    // set up
    memset (&serv_addr, '\0', sizeof (serv_addr));
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
       //return 1;
    }
    listen (listenfd, 10);


    while (1){
       // accept and interact
       connfd = accept (listenfd, (struct sockaddr*)NULL, NULL);
       memset (buff, '\0', sizeof (buff));
       printf("---------------start exchange---------------------------------------------\n\n"); //defines start of exchange
       /* reads in GET from browser */
       if (read(connfd, buff, buffLen)<=0){
           printf("ERROR: bad read\n");
           close(connfd);
           continue;
       }


       /* parses GET request for the page and hostname */
       printf("-------------start GET--------------\n");
       sscanf(buff,"%s %s %*s %*s %s",type, page, hostname);
       alphaChars(); //converts 'page' to file writbale text
       sprintf(path, "./cache/%s.txt", page);  //defines the path where the cache file will be written
       //printf("Hostname:%s\n\n",hostname);

        /* prints the GET request */
        printf("%s\n",buff);
        printf("--------------end GET---------------\n"); //defines end to printed GET request
        /* prevents connections from the browser (aka www) */
        if(type[0]!='G' /*&& hostname[0]!='c'*/){
            printf("Error: not a get request\n");
            close (connfd);
            continue;    //skips rest of loop
        }




        /*
            This block should look for a page in cache then
            if found, open the file (whick contains )
         */
        int cacheLoc = checkCache();
        if(cacheLoc > -1){
            FILE* cachefp = fopen(cache[cacheLoc],"r"); //read from this the cache file
            printf("CACHE HIT\n");
            consumeHTTP(&cachefp);
            //write(connfd, "HTTP/1.1 200 OK", 15);

            memset(buff, '\0', sizeof (buff)); //clear buffer for next read
            while((fread(buff, 1, buffLen, cachefp))>0){
                //printf("%s\n",buff);
                write(connfd, buff, sizeof(buff));
                memset(buff, '\0', sizeof (buff)); //clear buffer for next read
            }
            close(connfd);
            fclose(cachefp);
            printf("\n----------------end exchange-----------------------------------------------\n\n\n\n\n\n"); //defines end of exchange
            continue;
        }

        /* setup to connect to outside site/server */
        ip = hostname_to_ip(hostname,ip);    //converts hostname to IP address
        int sockfd = 0; //open scokfd
       	struct sockaddr_in host_addr;
       	memset (&host_addr, '\0', sizeof (host_addr));

        /* open socket to communicate, terminate if failure */
       	if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0){
       		printf ("Error : Could not create socket \n");
       		return 1;
       	}
        /* set address to send to */
       	host_addr.sin_family = AF_INET;
       	host_addr.sin_port = htons (80);


        /* dont reallt know what this does but its neccesary */
       	if (inet_pton (AF_INET, ip, &host_addr.sin_addr) <= 0){
       		printf ("inet_pton error occured\n");
       		return 1;
       	}

        /* establish connection with host server */
       	if (connect (sockfd, (struct sockaddr *)&host_addr, sizeof (host_addr)) < 0){
       		printf ("Error : Connect Failed \n");
       		return 1;
       	}

        /* send GET request to host, and clear buffer */
       	write(sockfd, buff, strlen(buff)+1);        //send get
        memset (buff, '\0', sizeof (buff));  //clear buffer for response

        int shouldCache = 0;
        FILE* fp;
        if((bytesRead=read(sockfd, buff, buffLen))>0){
            sscanf(buff,"%*s %*s %s",response);
            if(strcmp(response, "OK")==0){
                fp = fopen(path,"w"); //this is the cache file

                shouldCache = 1;
                fprintf(fp,"%s",buff);
                sprintf(cache[cacheCount],"%s",path);
                cacheCount=(cacheCount+1)%cacheSize;
                //printf("\nRESPONSE\n%s\n",buff);
                printf("\n -> Page saved to cache <- \n\n");
            }
            if(write(connfd, buff, bytesRead) <0){
                printf("write error\n");
            }
            memset (buff, '\0', sizeof (buff));

        }

        /* cache page to file and save cached file name in array of caches filenames */




       	while((bytesRead = read(sockfd, buff, buffLen))>0){
            if(shouldCache==1){
                fprintf(fp,"%s",buff);
            }
            if(write(connfd, buff, strlen(buff)-1)<0){
                printf("Write error 2\n");
            }
            memset (buff, '\0', sizeof (buff));

        }

        printCache();
        fclose(fp);
        close(sockfd);
        close (connfd);
        printf("\n----------------end exchange-----------------------------------------------\n\n\n\n\n\n"); //defines end of exchange

        //return 0;
    }
     close (connfd);

    return 0;
}
