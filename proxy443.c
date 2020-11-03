#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>




/*
 * Function:	connectToHost
 *
 * Description:	connects to specified host over given
            port number and returns the object specified
            in pathToObject.
 */
int connectToOutsideHost(char* host,int portNum, char* message, char* response){
    char ip[20];
    int sockfd=0, i;
    struct hostent *he;
    struct in_addr **addr_list;
    struct sockaddr_in serv_addr;



    /* format and print out HTTP message */
    printf("%s", message);


    /* Get IP address */
    if ( (he = gethostbyname(host)) == NULL){
        printf("ERROR: gethostbyname() failed\n");
        return 1;
    }
    addr_list = (struct in_addr **) he->h_addr_list;
    if(addr_list[0] != NULL){
        strcpy(ip , inet_ntoa(*addr_list[0]) );
        printf("%s\n",ip);
    }

/* open socket */
    if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0){
		printf ("Error: could not create socket \n");
		return 1;
	}

/* set address */
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons (portNum);
    printf("%s\n",ip);
    if (inet_pton (AF_INET, ip, &serv_addr.sin_addr) <= 0){
        printf ("ERROR: inet_pton error occured\n");
        return 1;
    }

/* connect */
    int consuc = connect (sockfd, (struct sockaddr *)&serv_addr, sizeof (serv_addr));
    if (consuc < 0){
        printf ("ERROR: Connection Failed \n");
        return 1;
    }

/* send the request message */
    int bytes;
    int total = strlen(message);
    int sent = 0;
    do{
        bytes = write(sockfd,message+sent,total-sent);
        if (bytes < 0)
            printf("ERROR: write error\n");
        if (bytes == 0)
            break;
        sent+=bytes;
    } while(sent<total);

    /* receive the response */
    memset(response,0,strlen(response)+1);
    total = sizeof(response)-1;
    int received = 0;
    bytes = 0;
    do {
        bytes = read(sockfd,response+received,total-received);
        if (bytes < 0){
            printf("ERROR: read error\n");
        }
        if (bytes == 0){
            break;
        }
        received+=bytes;
    } while (received<total);

    if(received >= total)
        printf("ERROR: response too large\n");


    //close (sockfd);
    return 0;
}




/*
 * Function:	connectToHost
 *
 * Description:	connects to specified host over given
            port number and returns the object specified
            in pathToObject.
 */
int connectToHost (char* host, char* pathToObject,int portNum, char* response){
    char ip[20];

    int sockfd=0, i;
    struct hostent *he;
    struct in_addr **addr_list;
    struct sockaddr_in serv_addr;
    char message[150];

/* format and print out HTTP message */
    sprintf(message, "GET %s HTTP/1.1\r\nHOST: %s\r\n\r\n", pathToObject, host);

    printf("Get Request:\n\n\n");

    printf("%s", message);


/* Get IP address */
    if ( (he = gethostbyname(host)) == NULL){
        printf("ERROR: gethostbyname() failed\n");
        return 1;
    }
    addr_list = (struct in_addr **) he->h_addr_list;
    if(addr_list[0] != NULL){
        strcpy(ip , inet_ntoa(*addr_list[0]) );
        printf("%s\n",ip);
    }

/* open socket */
    if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0){
		printf ("Error: could not create socket \n");
		return 1;
	}

/* set address */
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons (portNum);
    printf("%s\n",ip);
    if (inet_pton (AF_INET, ip, &serv_addr.sin_addr) <= 0){
        printf ("ERROR: inet_pton error occured\n");
        return 1;
    }

/* connect */
    int consuc = connect (sockfd, (struct sockaddr *)&serv_addr, sizeof (serv_addr));
    if (consuc < 0){
        printf ("ERROR: Connection Failed \n");
        return 1;
    }

/* send the request message */
    int bytes;
    int total = strlen(message);
    int sent = 0;
    do{
        bytes = write(sockfd,message+sent,total-sent);
        if (bytes < 0)
            printf("ERROR: write error\n");
        if (bytes == 0)
            break;
        sent+=bytes;
    } while(sent<total);

    /* receive the response */
    memset(response,0,sizeof(*response));
    total = sizeof(response)-1;
    int received = 0;
    bytes = 0;
    do {
        bytes = read(sockfd,response+received,total-received);
        if (bytes < 0){
            printf("ERROR: read error\n");
        }
        if (bytes == 0){
            break;
        }
        received+=bytes;
    } while (received < total);

    if(received >= total)
        printf("ERROR: response too large\n");

    printf("Response in browser");

    close (sockfd);
    return 0;
}



void proxy(char* hostVar, char* pathVar, int portNum, char* response){
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;
	char clientRequest[4096];
	// set up
	memset (&serv_addr, '0', sizeof (serv_addr));
	memset (clientRequest, '0', sizeof (clientRequest));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl (INADDR_ANY);
	serv_addr.sin_port = htons (portNum);

	// create socket, bind, and listen
	listenfd = socket (AF_INET, SOCK_STREAM, 0);
	bind (listenfd, (struct sockaddr*)&serv_addr, sizeof (serv_addr));
	listen (listenfd, 10);




	// accept and interact
    while(1){
    	connfd = accept (listenfd, (struct sockaddr*)NULL, NULL);
    // receive data and reply
    	if((read (connfd, clientRequest, sizeof (clientRequest))) > 0){
            printf("%s",clientRequest);

        //     sscanf(clientRequest, "%*s %s %*s %*s %s",pathVar, hostVar);
        //     pathVar = "/c_standard_library/c_function_printf.htm";
        //     hostVar[strlen(hostVar)]='\0';
        //     printf("this is the host: %s\n",hostVar);
        //     //printf("Host: %s\n Path: %s",hostVar, pathVar);
        //     memset(response, '\0', 6);
        //     write(connfd, response,6)
            sscanf(clientRequest, "%*s %*s %*s %*s %s", hostVar);
            if(connectToOutsideHost(hostVar, 443, clientRequest, response)==0){
                char* ok= "HTTP/1.1 200 OK";
                write(connfd, ok,strlen(ok)+1);
                read (connfd, clientRequest, sizeof (clientRequest));
                printf("%s",clientRequest);
            }

    	 }
        else{
            fprintf(stderr, "%s", "read error\n"); // Error message on stderr (using fprintf)
            write(connfd, "error",6);
        }
        connectToHost(hostVar, pathVar,portNum,response);
        write(connfd, response, sizeof(response));
    }
    close (connfd);
}

/*
 * Function:	Main
 *
 * Description:	controls running of thr program
 */
int main(int argc, char* argv[]){


    char host[16];
    int hostPortNo = 443;
    char path[100];
    char responseMessage[3000000];

    proxy(host, path, hostPortNo, responseMessage);


    //printf("%s %s\n",host, path);


    //parseRequest(message);
}
