#define _GNU_SOURCE

#include <linux/kernel.h>

#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>




int main(){

	int result;
	int* ptr = &result;

	int a = 1;
	int b = 3;

	syscall(333);
	syscall(334,a,b,ptr);

	printf("%d\n",result);
	return 0;
}
