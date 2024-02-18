#include <stdio.h>
#include <stdlib.h>
#define TRUE   1
#define FALSE  0
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

#if 1
#define N 6
#define ip1 "127.0.0.1"
#define ip2 "127.0.0.1"
#define ip3 "127.0.0.1"
#define ip4 "127.0.0.1"
#define ip5 "127.0.0.1"
#define ip6 "127.0.0.1"
#endif
#define tablesize 100

//we have 6 different hash table in different server, they communicate with udp and tcp
//each hash table has "tablesize" number of index and each index have many number of values

struct table_element
{
    int element_value;
    struct table_element* link;
};

struct hash{
    struct table_element* link;
};

struct hash Hashtable[tablesize];
int current_server_idx = 0;

void addvalue(struct hash* table, int index, char value){
    struct table_element* tmp_pointer;
    struct table_element* pre_tmp_pointer = malloc(sizeof(struct table_element));
    if (table[index].link == NULL){
        struct table_element* new_element = malloc(sizeof(struct table_element));
        table[index].link = new_element;
        new_element -> element_value = value;
        return;
    }
    tmp_pointer = table[index].link;
    while (tmp_pointer != NULL){
        pre_tmp_pointer = tmp_pointer;
        tmp_pointer = tmp_pointer -> link;
    }
    struct table_element* new_element = malloc(sizeof(struct table_element));
    pre_tmp_pointer -> link = new_element;
    new_element -> element_value = value;
    return;
}

void cal_hash_key(int k, int *div, int *mod){
    *div = k / 6; 
    *mod = k % 6; 
}

void put(int key, char value){
    int table_idx, server_idx;

    cal_hash_key(key, &table_idx, &server_idx);

}

void get(int key){


}

void server_start(){


}

int main()
{
    
    int key = 11;
    char value = 't';
    
    server_start();
    put(key, value);
    get(key);
}