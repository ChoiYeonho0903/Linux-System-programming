#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int num=atoi(argv[1]);
	int fd1;
	int fd2;
	srand(time(NULL));
	char *buffer;
	char space = '\n';
	int temp;
	int random;
	char buf[100];
	int k=0;
	int *order=malloc(sizeof(int)*num);
	char *str;

	if((fd1 = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0666))<0)
	{
		fprintf(stderr, "open error for %s\n", argv[2]);
		exit(1);
	}
	else
	{
		for(int i=0; i<num; i++)
		{
			random=(rand()%100) + 1;
			buffer=(char*)malloc(sizeof(int));
			memset(buffer, 0, sizeof(int));
			sprintf(buffer, "%d", random);
			write(fd1, buffer, strlen(buffer));
			write(fd1, &space, 1);
		}
		if(lseek(fd1, 0, SEEK_SET)<0) 
		{
			fprintf(stderr, "lseek error\n");
		}
	}

	if((fd2 = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644))<0)
	{
		fprintf(stderr, "open error for %s\n", argv[3]);
		exit(1);
	}
	else
	{
		read(fd1, buf, 100);
		char *ptr = strtok(buf, "\n");
		while (ptr != NULL)
		{
			strcpy(str, ptr);
			order[k] = atoi(str);
			ptr = strtok(NULL, "\n");
			k++;
		}
		for(int i=0; i<num; i++)
		{
			for(int j=0; j<num-1; j++)
			{
				if(order[j] > order[j+1])
				{
					temp = order[j];
					order[j] = order[j+1];
					order[j+1] = temp;
				}
			}
		}
		for(int i=0; i<num; i++)
		{
			memset(buffer, 0, sizeof(int));
			sprintf(buffer, "%d", order[i]);
			write(fd2, buffer, strlen(buffer));
			write(fd2, &space, 1);
		}
	}

	free(buffer);
	return 0;
}
