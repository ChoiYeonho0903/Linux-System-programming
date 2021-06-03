#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	struct stat statbuf;
	int mode = atoi(argv[1]);
	int num[4]={0};
	int sum=0;
	int pow=1;

	if(argc != 3)
	{
		fprintf(stderr, "usage:%s[file name]\n", argv[0]);
		return 1;
	}
	if(access(argv[2], F_OK)!=0)
	{
		printf("%s is not existed\n", argv[1]);
		return 1;
	}
	if(stat(argv[2], &statbuf)<0)
	{
		fprintf(stderr, "stat error %s\n", argv[2]);
	}
	
	num[4] = statbuf.st_mode>>12;
	for(int i=3; i>0; i--)
	{
		for(int j=0; j<i; j++)
		{
			pow *=10;
		}
		num[i] = mode/pow;
		mode -= num[i]*pow;
		pow=1;
	}
	num[0] = mode;

	sum+=(num[4]<<12);
	sum+=(num[3]<<9);
	sum+=(num[2]<<6);
	sum+=(num[1]<<3);
	sum+=num[0];
	if(chmod(argv[2], sum)!=0)
	{
		printf("fail change mode\n");
	}

	return 0;
}
