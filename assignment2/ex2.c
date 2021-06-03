#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main (int argc, char *argv[])
{
	int fd1;
	int fd2;
	char *buf;
	char character;

	if((fd1 = open(argv[1], O_RDONLY)) < 0)
	{
		fprintf(stderr, "open error for %s\n", argv[1]);
		exit(1);
	}
	else
	{	
		buf = (char*)malloc(strlen(argv[1]));
		for(int i =0; i<(int)strlen(argv[1]); i++)
		{
			if(argv[1][i] >= 'A' && argv[1][i] <= 'Z')
			{
				buf[i]=argv[1][i]+32;
			}
			else if (argv[1][i] >='a' && argv[1][i] <= 'z')
			{
				buf[i]=argv[1][i]-32;
			}
			else
			{
				buf[i]=argv[1][i];
			}
		}
	}

	if((fd2 = open(buf, O_WRONLY | O_CREAT | O_TRUNC, 0644))<0)
	{
		fprintf(stderr, "open error for %s\n", argv[2]);
		exit(2);
	}
	
	while((read(fd1, &character, 1))>0)
	{
			if(character >= 'A' && character <= 'Z')
			{
				character += 32;
				write(fd2, &character, 1);
			}
			else if (character >='a' && character <= 'z')
			{
				character -= 32;
				write(fd2, &character, 1);
			}
			else
			{
				write(fd2, &character, 1);
			}
	}
	return 0;
}
