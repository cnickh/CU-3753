#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024

int main(){
	char op;
	int length, whence, new_offset;
	char buffer[BUFFER_SIZE];
    
	int file = open("/dev/nicksio_dev", O_RDWR);
	bool running = true;
	while(running){
		
		printf("\nMenu:\n'r' - read\n'w' - write\n's' - seek\n'e' - exit\n");	
		scanf("%c",&op);	
		
		switch(op){
			case 'r':
				printf("How many bytes to read?: ");
				scanf("%d", &length);
				read(file, buffer, length);
				printf("nicksO %s\n", buffer); 
				while(getchar() != '\n'); 
				break;

			case 'w':
				printf("nicksI: ");
				scanf("%s", buffer);
				write(file, buffer, BUFFER_SIZE); 
				while (getchar() != '\n'); 
				break;

			case 's':
				printf("Enter whence: ");
				scanf("%d", &whence);
				printf("Enter an offset value: ");
				scanf("%d", &new_offset);
				llseek(file, new_offset, whence);
				break;

			case 'e':
				printf("Exiting\n");
				running = false;
				break;

			default:
				printf("not a valid command\n");
				break;
		}
	}
	close(file);
	return 0;
}

