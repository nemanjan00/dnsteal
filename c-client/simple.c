#include<netdb.h>

int hostname_to_ip(char* hostname){
		 
	gethostbyname(hostname);
	 
	return 1;
}

int main(int argc, char **argv){
	char hostname[] = "sacuvaj.ga";

	hostname_to_ip(hostname);

	return 0;
}