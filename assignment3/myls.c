#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

char type(mode_t);
char *perm(mode_t);
void printstat_l(char *, struct stat*);
void printstat_i(char *, struct stat*);
void printstat_t(char *, struct stat*);
void ls_l(char *argv[]);
void ls_i(char *argv[]);
void ls_t(char *argv[]);
void ls(char *argv[]);

int main(int argc, char *argv[])
{
	char *dir;
	if(argc==1)
	{
		dir = ".";
		ls(&dir);
	}
	else if(strcmp(argv[1], "-i")==0)
	{
		if(argc==2)
		{
			dir = ".";
			ls_i(&dir);
		}
		else
			ls_i(&argv[2]);	
	}
	else if(strcmp(argv[1], "-l")==0)
	{
		if(argc==2)
		{
			dir = ".";
			ls_l(&dir);
		}
		else
			ls_l(&argv[2]);
	}
	else if(strcmp(argv[1], "-t")==0)
	{
		if(argc==2)
		{
			dir = ".";
			ls_t(&dir);
		}
		else
			ls_t(&argv[2]);	
	}
	else
	{
		ls(&argv[1]);
	}
	exit(0);
}

void ls(char *argv[])
{
	DIR *dp;
	char *dir;
	struct stat statbuf;
	struct dirent *d;
	char path[1024];

	if(lstat(argv[0], &statbuf)<0) {
		perror("stat error");
	}

	if(S_ISREG(statbuf.st_mode))
	{
		printf("%s\n", argv[0]);
		
	}
	else if(S_ISDIR(statbuf.st_mode))
	{
		dir = argv[0];

		if((dp=opendir(dir))==NULL)
			perror(dir);

		while((d=readdir(dp))!=NULL){
			if((strcmp(d->d_name,"..")!=0)&&strcmp(d->d_name,".")!=0)
			{
			printf("%s  ",  d->d_name);
			}
		}
		putchar('\n');
		closedir(dp);
	}

}
void ls_t(char *argv[])
{
	DIR *dp;
	char *dir;
	struct stat statbuf;
	struct stat statbuffer[100];
	struct stat temp2;
	struct dirent *d;
	char path[100][1024];
	char filename[100][1024];
	char temp1[1024];
	int filenum=0;

	if(lstat(argv[0], &statbuf)<0) {
		perror("stat error");
	}

	if(S_ISREG(statbuf.st_mode))
	{
		printf("%s\n", argv[0]);		
	}
	else if(S_ISDIR(statbuf.st_mode))
	{
		dir = argv[0];

		if((dp=opendir(dir))==NULL)
			perror(dir);

		while((d=readdir(dp))!=NULL){
			if((strcmp(d->d_name,"..")!=0)&&strcmp(d->d_name,".")!=0)
			{
			sprintf(path[filenum], "%s/%s", dir, d->d_name);
			sprintf(filename[filenum], "%s", d->d_name);
			if(lstat(path[filenum], &statbuffer[filenum])<0)
				perror("lsat error");
			filenum++;
			}
		}
		for(int i=0; i<filenum; i++)
		{
			for(int j=0; j<filenum-1-i; j++)
			{
				if(statbuffer[j].st_mtime<statbuffer[j+1].st_mtime) {
					strcpy(temp1, filename[j]);
					strcpy(filename[j], filename[j+1]);
					strcpy(filename[j+1], temp1);

					temp2 = statbuffer[j];
					statbuffer[j] = statbuffer[j+1];
					statbuffer[j+1] = temp2;
				}
			}
		}
		for(int i=0; i<filenum; i++)
		{
			printstat_t(filename[i], &statbuffer[i]);
		}
		putchar('\n');
		closedir(dp);
	}

}
void ls_i(char *argv[])
{
	DIR *dp;
	char *dir;
	struct stat statbuf;
	struct dirent *d;
	char path[1024];

	if(lstat(argv[0], &statbuf)<0) {
		perror("stat error");
	}

	if(S_ISREG(statbuf.st_mode))
	{
		printstat_i(argv[0], &statbuf);
		putchar('\n');
	}
	else if(S_ISDIR(statbuf.st_mode))
	{
		dir = argv[0];

		if((dp=opendir(dir))==NULL)
			perror(dir);

		while((d=readdir(dp))!=NULL){
			if((strcmp(d->d_name,"..")!=0)&&strcmp(d->d_name,".")!=0)
			{
				sprintf(path, "%s/%s", dir, d->d_name);
				if(lstat(path, &statbuf)<0)
					perror(path);
				printstat_i(d->d_name, &statbuf);
			}
		}
		putchar('\n');
		closedir(dp);
	}
}
void ls_l(char *argv[])
{
	DIR *dp;
	char *dir;
	struct stat statbuf;
	struct dirent *d;
	char path[1024];

	if(lstat(argv[0],&statbuf)<0) {
		perror("stat error");
	}

	if(S_ISREG(statbuf.st_mode))
	{
		printstat_l(argv[0], &statbuf);
		
	}
	else if(S_ISDIR(statbuf.st_mode))
	{
		dir = argv[0];

		if((dp=opendir(dir))==NULL)
			perror(dir);

		while((d=readdir(dp))!=NULL){
			if((strcmp(d->d_name,"..")!=0)&&strcmp(d->d_name,".")!=0)
			{
				sprintf(path, "%s/%s", dir, d->d_name);
				if(lstat(path, &statbuf)<0)
					perror(path);
				printstat_l(d->d_name, &statbuf);
			}
		}
		closedir(dp);
	}
}



void printstat_l(char *file, struct stat *st) {
	printf("%c%s ", type(st->st_mode), perm(st->st_mode));
	printf("%ld ", st->st_nlink);
	printf("%s %s ", getpwuid(st->st_uid)->pw_name, getgrgid(st->st_gid)->gr_name);
	printf("%6ld ", st->st_size);
	printf("%.12s ", ctime(&st->st_mtime)+4);
	printf("%s\n", file);
}

void printstat_i(char *file, struct stat *st) {
	printf("%ld ", st->st_ino);
	printf("%s  ", file);
}

void printstat_t(char *file, struct stat *st) {
	printf("%s  ", file);
}

char type(mode_t mode) {
	if(S_ISREG(mode))
		return('-');
	if(S_ISDIR(mode))
		return('d');
	if(S_ISCHR(mode))
		return('c');
	if(S_ISBLK(mode))
		return('b');
	if(S_ISLNK(mode))
		return('l');
	if(S_ISFIFO(mode))
		return('p');
	if(S_ISSOCK(mode))
		return('s');
}

char *perm(mode_t mode) 
{
	int i;
	char *perms=malloc(10);
	strcpy(perms, "---------");
	for(i=0;i<3;i++)
	{
		if(mode &(S_IRUSR>>i*3))
			perms[i*3]='r';
		if(mode &(S_IWUSR>>i*3))
			perms[i*3+1]='w';
		if(mode &(S_IXUSR>>i*3))
		{
			perms[i*3+2]='x';
		}

	}
	if(mode&S_ISUID)
	{
		if(perms[2]=='x')
			perms[2]='s';
		else
			perms[2]='S';
	}
	if(mode&S_ISGID)
	{
		if(perms[5]=='x')
			perms[5]='s';
		else
			perms[5]='S';

	}
	if(mode&S_ISVTX)
	{
		if(perms[8]=='x')
			perms[8]='t';
		else
			perms[8]='T';

	}
	return perms;
}
