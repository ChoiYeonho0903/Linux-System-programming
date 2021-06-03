#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd;
	int num=atoi(argv[1]);
	char buf[1] = "\n";

	if((fd = open(argv[3], O_WRONLY| O_CREAT | O_TRUNC, 0644))<0)
	{
		fprintf(stderr, "open error for %s\n", argv[3]);
		exit(1);
	}

	if(num>=0)
	{
		for(int i=0; i<num; i++)
		{
			for(int j=0; j<=i; j++)
			{
				write(fd, argv[2], 1);
			}
			write(fd, buf, 1);
		}
	}
	else
	{	
		num=num*(-1);
		for(int i=num; i>0; i--)
		{
			for(int j=i; j>0; j--)
			{
				write(fd, argv[2], 1);
			}
			write(fd, buf, 1);
		}
	}
	return 0;
}
