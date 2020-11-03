#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static char buff[2048];
static char page[200];
static char path[210];

void slash2Colon(){
    for(int i = strlen(&page[7])+7; i>6; i--){
        if(page[i]=='/')
            page[i] = ':';
    }
}

char*  consumeHTTP(FILE* fp){
    char c = fgetc(fp);
    char* http;
    int counter=0;
    while(counter!=4){
        c = fgetc(fp);
        if(c =='\r' || c=='\n'){
            counter++;
        }
        else{
            counter = 0;
        }
        sprintf(http,"%c",c);
    }
}
int main(){
    // char buff[12];
    //
    // int size = sizeof(buff);
    // for(int i= 0; i<size; i++){
    //     buff[i] = 'e';
    // }
    // buff[11]  ='\0';
    // int len = strlen(buff);
    // printf("size: %d \t length:%d\n", size, len);




    // sprintf(page, "%s", "http://www.cse.scu.edu:~atkinson:teaching:sp19:175:");
    // slash2Colon();
    // sprintf(path, "./cache/%s", &page[7]);
    // printf("%s\n",path);


    FILE* fp = fopen("./cache/test.css","r");
    char c = fgetc(fp);
    int counter;
    while(c!=EOF){
        if(c =='\r' || c=='\n'){
            counter++;
        }
        else{
            counter = 0;
        }
        if(counter == 4){
            printf("\nend\n");
        }
        printf("%c",c);
        c = fgetc(fp);
    }
    fclose(fp);

}




//1 2 3 4 5 6 7 8 9 10 11 \n
