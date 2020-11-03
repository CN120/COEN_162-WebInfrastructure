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



int connectToHost (char* host, int portNum, char* pathToObject){
    char* hostname = host;
    char ip[20];

    int sockfd=0, i;
    struct hostent *he;
    struct in_addr **addr_list;
    struct sockaddr_in serv_addr;
    char message[150];
    char response[100000];

/* format and print out HTTP message */
    sprintf(message, "GET %s HTTP/1.1\r\nHOST: %s\r\n\r\n", pathToObject, host);

    printf("Get Request:\n\n\n");

    printf("%s", message);


/* Get IP address */
    if ( (he = gethostbyname( hostname ) ) == NULL){
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
    } while(sent<totals);

    /* receive the response */
    memset(response,0,sizeof(response));
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

    printf("Response:\n%s\n",response);

    close (sockfd);
    return 0;
}


/* MAIN function */
int main(int argc, char* argv[]){
    char* host;
    int hostPortNo;
    char* path;
    if(argc<2){
        host = "www.w3.org";
        hostPortNo = 80;
        path = "/Style/Examples/011/firstcss.en.html";
    }
    else if(argc<4){
        host = argv[1];
        path = argv[2];
        hostPortNo = 80;
    }
    else{
        host = argv[1];
        path = argv[2];
        hostPortNo = atoi(argv[3]);
    }

    connectToHost(host,hostPortNo,path);
}
