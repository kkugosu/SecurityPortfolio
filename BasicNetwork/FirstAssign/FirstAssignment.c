#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define PREFIX_LEN 16
//const int PREFIX_LEN = 1;
void CheckSameSubnet(char *networkId, int mask, char *checkIp)
{
    unsigned long int k;
    int i;
    unsigned long int result;
    char ipBuffer[PREFIX_LEN];
    char str[PREFIX_LEN];
    long int source;
    long int target;
    char *token;

    strcpy(ipBuffer, checkIp);  
    //printf("targetIp: %s\n", ipBuffer);
    
    strcpy(str, networkId);
    //printf("sourceIP: %s\n", str);
    char delimiter[] = ".";
    
    token = strtok(str, delimiter);
    i = 3;
    result = 0;
    while (token != NULL) {
        k = pow(2, 8 * i);
        result = result + k * atoi(token);
        token = strtok(NULL, delimiter);
        i = i - 1;
    }
    source = result >> (32 - mask);

    token = strtok(ipBuffer, delimiter);
    i = 3;
    result = 0;
    while (token != NULL) {
        k = pow(2, 8 * i);
        result = result + k * atoi(token);
        token = strtok(NULL, delimiter);
        i = i - 1;
    }
    target = result >> (32 - mask);

    if ((target ^ source) == 0)
    {
        printf("same network");
    }
    else
    {
        printf("different network");
    }

    return;
}


void main()
{
    //char ipAddr[PREFIX_LEN] ;
    //char ipAddr[] = "192.168.2.10";

    int mask ;
    char ipAddr [PREFIX_LEN]; 
    char checkAddr [PREFIX_LEN];
    printf("Enter a source: ");
    scanf("%16s", ipAddr);
    printf("Enter a target: ");
    scanf("%16s", checkAddr);
    printf("Enter a mask : ");
    scanf("%d", &mask);
    CheckSameSubnet(ipAddr, mask, checkAddr);

    return;
}
