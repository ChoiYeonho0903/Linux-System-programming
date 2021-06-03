#include <stdio.h>
#include <sys/types.h>
#include <utime.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int fd;

	if(access(argv[1], F_OK) == -1)
	{
		if((fd=open(argv[1], O_RDONLY|O_CREAT|O_EXCL, 0644))<0) {
			fprintf(stderr, "open error for %s\n", argv[1]);
		}
		else
			close(fd);
	}
	
	else if(access(argv[1], F_OK) == 0) {
		if(utime(argv[1], NULL) == -1) {
			fprintf(stderr, "utime error: %s\n", strerror(errno));
			return -1;
		}
	}
	return 0;
}
