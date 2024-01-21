#include <stdio.h>
#include <string.h>
#define PREFIX_LEN 16
//const int PREFIX_LEN = 1;
void GetBroadcastAddress(char *ipAddr, int mask, char *outputBuffer)
{
    char ipBuffer[PREFIX_LEN];
    strcpy(ipBuffer, ipAddr);  
    printf("Copied string: %s\n", ipBuffer);
    int i = 0;
    while (i < strlen(ipAddr)){
        
        i = i + 1;

    }

    char str[] = "This,is,a,sample,string";
    char delimiter[] = ",";

    char *token = strtok(str, delimiter);
    printf("%c\n",token[0]);
    printf("%c\n",token[3]);
    printf("%c\n",token[4]);
    printf("%c\n",token[5]);
    while (token != NULL) {
        printf("%s\n", token);
        token = strtok(NULL, delimiter);
    }

    return;
}


void main()
{
    //char ipAddr[PREFIX_LEN] ;
    //char ipAddr[] = "192.168.2.10";
    char* ipAddr = "192.168.2.10";
    char tmp[PREFIX_LEN];
    char mask = 24;
    char outputBuffer[16];
    GetBroadcastAddress(ipAddr, mask, outputBuffer);

    printf("%s\n",ipAddr);
    memset(ipAddr, 'c', 1*sizeof(float));
    printf("%s\n",ipAddr);
    printf("%li\n", sizeof(int));
    return;
}
