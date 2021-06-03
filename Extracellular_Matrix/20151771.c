#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/time.h>

FILE *fp;

int dx[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
int dy[8] = {-1, -1, -1, 0, 1, 1, 1, 0};
int matrix[10000][10000]={0,};
int matrix_dup[10000][10000]={0,};
int size_x;
int size_y;
struct timeval startTime, endTime;
int execution_time;

struct thread_data {
	int t_start;
	int t_work;
};

void *parrallel_thread(void *arg);
void file_open(char *input);

int main(int argc, char *argv[]) {
	FILE *fp_sub;
	int action;
	int generation, process_num, thread_num;
	char c;
	int count;
	char buf[256];
	while(1) {
		printf("(1) Program termination\n(2) Sequential excution\n(3) Parallel excution with child process\n(4) Parallel excution with thread\n");
		scanf("%d", &action);
		if(action==1) {
			printf("Program termination\n");
			exit(0);
		}
		else if(action==2) {
			//이전 파일 삭제
			system("find . -name \"*gen*\" -delete");
			printf("How many generations will you run?");
			scanf("%d", &generation);
			//실행시간 측정 시작
			gettimeofday(&startTime, NULL);
			file_open(argv[1]);
			for(int k=0; k<generation; k++) {
				for(int i=1; i<=size_y; i++) {
					for(int j=1; j<=size_x; j++) {
						count =0;
						for(int l=0; l<8; l++) {
							if(matrix[i+dy[l]][j+dx[l]]==1) {
								count++;
							}
						}
						if(matrix[i][j]==1 && (count<=2 || count>=7))
							matrix_dup[i][j]=0;
						else if(matrix[i][j]==1 && (count>=3 && count <=6))
							matrix_dup[i][j]=1;
						else if(matrix[i][j]==0 && count==4)
							matrix_dup[i][j]=1;
						else
							matrix_dup[i][j]=0;
					}
				}
				for(int i=1; i<=size_y; i++) {
					for(int j=1; j<=size_x; j++) {
						matrix[i][j]=matrix_dup[i][j];
					}
				}
				//세대별 중간, 결과 파일
				if(k==(generation-1)) {
					fp_sub = fopen("output.matrix", "w");
				}
				else {
					memset(buf, 0, sizeof(buf));
					sprintf(buf, "gen_%d.matrix", k+1);
					fp_sub = fopen(buf, "w");
				}
				for(int i=1; i<=size_y; i++) {
					for(int j=1; j<=size_x; j++) {
						if(j==size_x) {
							c = matrix[i][j]+'0';
							fputc(c, fp_sub);
							c= '\n';
							fputc(c, fp_sub);
						}
						else {
							c = matrix[i][j]+'0';
							fputc(c, fp_sub);
							c = ' ';
							fputc(c, fp_sub);
						}
					}
				}
				fclose(fp_sub);
			}
			gettimeofday(&endTime, NULL);
			//실행시간 출력
			execution_time = ((endTime.tv_sec - startTime.tv_sec)*1000000 + (endTime.tv_usec - startTime.tv_usec));
			printf("execution time : %dms\n", execution_time);
		}
		else if(action==3) {
			//이전 파일 삭제
			system("find . -name \"*gen*\" -delete");
			int pidc, status;
			pid_t **pid;
			int *work;
			int start;
			FILE *sub;
			int x,y;
			printf("How many child processes will you use?");
			scanf("%d", &process_num);
			work=(int *)malloc(sizeof(int)*process_num);
			printf("How many generations will you run?");
			scanf("%d", &generation);
			pid = (pid_t **)malloc(sizeof(pid_t*)*generation);
			for(int i=0; i<generation; i++) {
				pid[i] = (pid_t *)malloc(sizeof(pid_t)*thread_num);
			}			
			//실행 시간측정 시작
			gettimeofday(&startTime, NULL);
			file_open(argv[1]);
			for(int i=0; i<process_num; i++) {
				work[i]=size_y/process_num;
				if((size_y%process_num)-i>0)
					work[i]+=1;
			}
			for(int k=0; k<generation; k++) {
				for(int m=0; m<process_num; m++) {
					if((pidc = fork())==0) {
						if(m==0)
							start=1;
						else {
							start=0;
							for(int i=0; i<m; i++){
								start+=work[i];
							}
							start+=1;
						}
						for(int i=start; i<=start+work[m]-1; i++) {
							for(int j=1; j<=size_x; j++) {
								count =0;
								for(int l=0; l<8; l++) {
									if(matrix[i+dy[l]][j+dx[l]]==1) {
										count++;
									}
								}
								if(matrix[i][j]==1 && (count<=2 || count>=7))
									matrix_dup[i][j]=0;
								else if(matrix[i][j]==1 && (count>=3 && count <=6))
									matrix_dup[i][j]=1;
								else if(matrix[i][j]==0 && count==4)
									matrix_dup[i][j]=1;
								else
									matrix_dup[i][j]=0;
							}
						}
						memset(buf, 0, sizeof(buf));
						sprintf(buf, "sub_file_%d", m);
						sub = fopen(buf, "w");
						for(int i=start; i<=start+work[m]-1; i++) {
							for(int j=1; j<=size_x; j++) {
								if(j==size_x) {
									c = matrix_dup[i][j]+'0';
									fputc(c, sub);
									c= '\n';
									fputc(c, sub);
								}
								else {
									c = matrix_dup[i][j]+'0';
									fputc(c, sub);
									c = ' ';
									fputc(c, sub);
								}
							}	
						}
						fclose(sub);
						exit(1);
					}
					else {
						pid[k][m]=pidc;
					}
				}
				//wait할게 없을 때까지 작동
				for(int i=0; i<process_num; i++) {
					wait(&status);
				}
				//matrix배열로 옮겨주기
				x=1;
				y=1;
				for(int m=0; m<process_num; m++) {
					memset(buf, 0, sizeof(buf));
					sprintf(buf, "sub_file_%d", m);
					sub=fopen(buf, "r");
					fseek(sub, 0, SEEK_SET);
					while(!feof(sub)) {
						c=fgetc(sub);
						if(c=='\n') {
							y++;
							x=1;
							continue;
						}
						else if(c==' ') {
							continue;
						}
						else if(c=='0' || c=='1') {
							matrix[y][x++] = c-'0';
						}
					}					
				}
				//sub파일 삭제
				system("find . -name \"*sub*\" -delete");	
				//세대별 중간, 결과 파일
				if(k==(generation-1)) {
					fp_sub = fopen("output.matrix", "w");
				}
				else {
					memset(buf, 0, sizeof(buf));
					sprintf(buf, "gen_%d.matrix", k+1);
					fp_sub = fopen(buf, "w");
				}
				for(int i=1; i<=size_y; i++) {
					for(int j=1; j<=size_x; j++) {
						if(j==size_x) {
							c = matrix[i][j]+'0';
							fputc(c, fp_sub);
							c= '\n';
							fputc(c, fp_sub);
						}
						else {
							c = matrix[i][j]+'0';
							fputc(c, fp_sub);
							c = ' ';
							fputc(c, fp_sub);
						}
					}
				}

				fclose(fp_sub);	
			}
			gettimeofday(&endTime, NULL);
			//pid 출력
			for(int k=0; k<generation; k++) {
				for(int m=0; m<process_num; m++) {
					printf("%d ",pid[k][m]);
				}
				printf("\n");
			}
			//실행시간 출력
			execution_time = ((endTime.tv_sec - startTime.tv_sec)*1000000 + (endTime.tv_usec - startTime.tv_usec));
			printf("execution time : %dms\n", execution_time);
		}

		else if(action==4) {
			//이전 파일 삭제
			system("find . -name \"*gen*\" -delete");
			int status;
			int *work;
			int *start;
			pthread_t **tid;
			struct thread_data *thread_data_array;
			printf("How many thread will you use?");
			scanf("%d", &thread_num);
			printf("How many generations will you run?");
			scanf("%d", &generation);
			tid= (pthread_t **)malloc(sizeof(pthread_t*)*generation);
			for(int i=0; i<generation; i++) {
				tid[i] = (pthread_t*)malloc(sizeof(pthread_t)*thread_num);
			}
			work=(int *)malloc(sizeof(int)*thread_num);
			start=(int *)malloc(sizeof(int)*thread_num);
			thread_data_array=(struct thread_data *)malloc(sizeof(struct thread_data)*thread_num);

			//실행 시간측정 시작
			gettimeofday(&startTime, NULL);
			file_open(argv[1]);
			for(int i=0; i<thread_num; i++) {
				work[i]=size_y/thread_num;
				if((size_y%thread_num)-i>0)
					work[i]+=1;
			}

			for(int k=0; k<generation; k++) {
				for(int m=0; m<thread_num; m++) {
					start[m]=0;
					if(m==0)
						start[m]=1;
					else {
						for(int i=0; i<m; i++)
							start[m]+=work[i];
						start[m]+=1;
					}
					thread_data_array[m].t_start=start[m];
					thread_data_array[m].t_work=work[m];
					if(pthread_create(&tid[k][m], NULL, parrallel_thread, (void*)&thread_data_array[m])!=0) {
						fprintf(stderr, "pthread_create error\n");
						exit(1);
					}
				}
				//생성한 thread가 모두 종료될 때까지 기다린다
				for(int i=0; i<thread_num; i++) {
					pthread_join(tid[k][i], (void *)&status);
				}

				//matrix배열로 옮겨주기
				for(int i=1; i<=size_y; i++) {
					for(int j=1; j<=size_x; j++) {
						matrix[i][j]=matrix_dup[i][j];
					}
				}
				//세대별 중간, 결과 파일
				if(k==(generation-1)) {
					fp_sub = fopen("output.matrix", "w");
				}
				else {
					memset(buf, 0, sizeof(buf));
					sprintf(buf, "gen_%d.matrix", k+1);
					fp_sub = fopen(buf, "w");
				}
				for(int i=1; i<=size_y; i++) {
					for(int j=1; j<=size_x; j++) {
						if(j==size_x) {
							c = matrix[i][j]+'0';
							fputc(c, fp_sub);
							c= '\n';
							fputc(c, fp_sub);
						}
						else {
							c = matrix[i][j]+'0';
							fputc(c, fp_sub);
							c = ' ';
							fputc(c, fp_sub);
						}
					}
				}
				fclose(fp_sub);	
			}
			gettimeofday(&endTime, NULL);
			//tid 출력
			for(int k=0; k<generation; k++) {
				for(int m=0; m<thread_num; m++) {
					printf("%ld ",tid[k][m]);
				}
				printf("\n");
			}
			//실행시간 출력
			execution_time = ((endTime.tv_sec - startTime.tv_sec)*1000000 + (endTime.tv_usec - startTime.tv_usec));
			printf("execution time : %dms\n", execution_time);	
		}

		else {
			printf("wrong number\n");
			continue;
		}
	}
}

void *parrallel_thread(void *arg) {
	struct thread_data *data;
	int p_start;
	int p_work;
	int count;
	data = (struct thread_data *)arg;
	p_start = data->t_start;
	p_work = data->t_work;
	for(int i=p_start; i<=p_start+p_work-1; i++) {
		for(int j=1; j<=size_x; j++) {
			count =0;
			for(int l=0; l<8; l++) {
				if(matrix[i+dy[l]][j+dx[l]]==1) {
					count++;
				}
			}
			if(matrix[i][j]==1 && (count<=2 || count>=7))
				matrix_dup[i][j]=0;
			else if(matrix[i][j]==1 && (count>=3 && count <=6))
				matrix_dup[i][j]=1;
			else if(matrix[i][j]==0 && count==4)
				matrix_dup[i][j]=1;
			else	
				matrix_dup[i][j]=0;
		}
	}
	pthread_exit(NULL);
}

void file_open(char *input) {
	int size;
	char c;
	int x=1;
	int y=1;
	//argv[1]로 바꿔주기
	if((fp = fopen(input, "r"))==NULL) {
		fprintf(stderr, "open error");
		exit(1);
	}
	//열갯수 구하기
	while((c=fgetc(fp))!=EOF) {
		if(c=='\n') {
			size = ftell(fp);
			size_x = size/2;
			break;
		}
	}
	//행갯수 구하기
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	size_y = size/(size_x*2);

	//matrix 입력받기
	fseek(fp, 0, SEEK_SET);
	while(!feof(fp)) {
		c=fgetc(fp);
		if(c=='\n') {
			y++;
			x=1;
			continue;
		}
		else if(c==' ') {
			continue;
		}
		else if(c=='0' || c=='1') {
			matrix[y][x++] = c-'0';
		}
	}
	fclose(fp);
}

