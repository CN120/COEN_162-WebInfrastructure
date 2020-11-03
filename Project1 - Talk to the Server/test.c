// #include <sys/socket.h>
// #include <sys/types.h>
// #include <netinet/in.h>
// #include <netdb.h>
// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <errno.h>
// #include <arpa/inet.h>
//
// int main(){
//     struct hostent *server = gethostbyname("google.com");
//     //char* address = ;
//     printf("host: %s\n", inet_ntoa(server));
//
// }


#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int main()
{
    struct hostent *ghbn=gethostbyname("www.scu.edu");//change the domain name
    if (ghbn) {
        printf("Host Name->%s\n", ghbn->h_name);
        printf("IP ADDRESS->%s\n",inet_ntoa(*(struct in_addr *)ghbn->h_name) );
    }
}
