#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
	int numofchild = atoi(argv[1]);
	int *pid = malloc(sizeof(int)*numofchild);
	int count =1;
	int status,pidc;
	for(int i=0; i<numofchild; i++) {
		if((pidc = fork())==0) {
			break;	
		}
		else {
			pid[i]=pidc;
		}
	}

	if(pidc>0) {
		sleep(5);
		while(1){
			for(int i=0; i<=(numofchild-count); i++){
				printf("%d processs의 순서번호는 %d입니다.\n", pid[i], i+1);
			}
	
			for(int i=0; i<5; i++) {
				sleep(1);
				kill(pid[numofchild-count], SIGKILL);
				count++;
			}
	
			for(int i=0; i<5; i++) {
				pidc = wait(&status);
			}

			if(numofchild==count)
				break;
		}
		raise(SIGKILL);
	}
}
