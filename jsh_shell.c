#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h>
#include <dirent.h> 
#include <time.h> 
#include <sys/sysmacros.h>

char argv[10][100]; //명령어 문자열 저장 

int cmd_pwd(int argc, char argv[][100]); //현재 작업중인 경로 출력 
int cmd_chmod(int argc, char argv[][100]); //파일 접근권한 변경 
int cmd_ls(int argc, char argv[][100]); //현재 경로 디렉토리 이름 출력 
int cmd_cd(int argc, char argv[][100]); //현재 경로 변경 
int cmd_stat(int argc, char argv[][100]); //파일 상태 출력 
int cmd_mkdir(int argc, char argv[][100]); //디렉토리 생성 
int cmd_ps(int argc, char argv[][100]); //현재 실행중인 프로세스 출력 
int cmd_cp(int argc, char argv[][100]); //파일 복사 
int cmd_rm(int argc, char argv[][100]); //파일 및 디렉토리 지우기 
int cmd_ln(int argc, char argv[][100]); //파일 링크 생성 
int cmd_cat(int argc, char argv[][100]); //파일 내용 한개 이상 출력 

void history_init(char history[100][100]){ //history 초기화 함수 
	for(int i = 0; i < 100; i++){
		for(int j = 0; j < 100; j++){
			history[i][j] = NULL; //NULL 초기화 
		}
	}
}
char command_token(char command[]){ 
	//argv 초기화 
	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 100; j++){
			argv[i][j] = NULL; //NULL 초기화 
		}
	}

	int str_pointer = 0;//command index value
	int cmd_str_pointer =0;//cmd 2차원 index value
	//cmd[][0],cmd[][1] ...

	//argc count
	char *token = strchr(command,' ');
	int argc = 0;

	while(token != NULL){
		argc++;  
		token = strchr(token+1,' ');
	}

	if(argc >= 11){
		printf("Error\n");
	}else{
		
		for(int i = 0; i < argc+1; i++){//들어온 수 만큼 
			cmd_str_pointer = 0;
			
			while(command[str_pointer] != '\n'){//command index value \n이 아닐때까지
				if(command[str_pointer] == ' '){//command index value ' ' 이면 
					argv[i][cmd_str_pointer] = '\0';
					//argc[0][3] = \0 
					//argc[1][3] = \0
					str_pointer++;
					break;
				}else{
					argv[i][cmd_str_pointer] = command[str_pointer];
					//argv[0][0] = command[0] c
					//argv[0][1] = command[1] a
					//argv[0][2] = command[2] t
					
					//argc[1][0] = command[4] a
					//argc[1][1] = command[5] .
					//argc[1][2] = command[6] t
					//argc[1][3] = command[7] x
					//argc[1][4] = command[8] t
				}
				cmd_str_pointer++;
				str_pointer++;
			}
		}
		return argc;
	}
}

int main(){
	char command[100];//cat a.txt
	int argc = 0;

	int roof_num = 1; 
	char history[100][100]; //history 저장 
	history_init(history); //history 초기화 

	while(1){
		printf("$ my_shell : ");
		fgets(command, 100, stdin);
		argc = command_token(command);
		
		strcpy(history[roof_num], command); //입력된 command history에 복사 
		roof_num++;

		if(command[0] == '!'){//입력된 command !이면 

			int history_num = atoi(command+1);//믄자열 정수로 변환 

			if(history_num > roof_num){//저장되지 않은 ![number] 입력 시 
				printf(" !%d history is not existed\n", history_num);
				roof_num = roof_num -1; 
			}else{
				strcpy(command, history[history_num]); //history 저장된 명령어 command에 복사 
				argc = command_token(command); //명령어실행 
			}
		}
		if(strcmp(argv[0], "history") == 0){
			if(roof_num > 101){
				printf("failed to save history");
			}
			for(int i = 1; i < roof_num && roof_num < 101; i++){ 
				printf("	%d : %s", i, history[i]); //저장된 명령어 출력 
			}
		}else if(strcmp(argv[0], "cat") == 0){ 
			cmd_cat(argc, argv);
		}else if(strcmp(argv[0], "pwd") == 0){
			cmd_pwd(argc, argv);
		}else if(strcmp(argv[0], "cd") == 0){
			cmd_cd(argc, argv);
		}else if(strcmp(argv[0], "mkdir") == 0){
			cmd_mkdir(argc, argv);
		}else if(strcmp(argv[0], "ls") == 0){
			cmd_ls(argc, argv);
		}else if(strcmp(argv[0], "ps") == 0){
			cmd_ps(argc, argv);
		}else if(strcmp(argv[0], "cp") == 0){
			cmd_cp(argc, argv);	
		}else if(strcmp(argv[0], "rm") == 0){
			cmd_rm(argc, argv);
		}else if(strcmp(argv[0], "ln") == 0){
			cmd_ln(argc, argv);
		}else if(strcmp(argv[0], "stat") == 0){
			cmd_stat(argc, argv);
		}else if(strcmp(argv[0], "chmod") == 0){
			cmd_chmod(argc, argv);
		}else if(strcmp(argv[0], "exit") == 0){
			exit(0); //종료 
		}else{
			printf("%s is not found\n", command);
		}
	}
}

int cmd_cat(int argc, char argv[][100]){

	FILE *fp; //파일 구조체 포인터 변수 
	char buf[255];
	int line_number = 1;
	
	if(strcmp(argv[1], "-n") == 0){ //옵션 -n 

		for(int i = 2; i < argc+1; i++){ 

			char *filename = argv[i];
			fp = fopen(filename, "r"); //파일 읽기 전용으로 열기

			if(fp != NULL){
				while(fgets(buf, 255, fp) != NULL){//파일에서 문자열 가져오기 
					printf("%d %s",line_number++, buf);
				}
				fclose(fp);//파일 닫기  
			}else{
				printf("%s : No such file or directory\n", filename);
			}
			
		}		
	}else{
		for(int i = 1; i < argc+1; i++){//들어온 파일 수 만큼 

			char *filename = argv[i];
			fp = fopen(filename, "r"); //파일 읽기 전용으로 열기 

			if(fp != NULL){
				while(fgets(buf, 255, fp) != NULL){//파일에서 문자열 가져오기 
					printf("%s", buf);
				}
				fclose(fp); //파일 닫기 
			}else{
				printf("%s : No such file or directory\n", filename);
			}
		}
	}
	
	return 0;
} 

int cmd_pwd(int argc, char argv[][100]){

	char buf[100];
	char *result = getcwd(buf, 100); //현재 작업중인 경로 buf에 담기 

	if(result != NULL){
		printf("%s\n", buf);
	}else{
		printf("Error : pwd\n");
		return -1;
	}
	return 0;
	
}

int cmd_cd(int argc, char argv[][100]){

		char *path = argv[1];
		int result = chdir(path);//디렉토리 경로 이동 

		if(result != 0){
			printf("change directory fail\n");
			return -1;
		}
	return 0;
}

int cmd_mkdir(int argc, char argv[][100]){

	for(int i = 1; i < argc+1; i++){//들어온 파일 수 만큼 
			
		char *pathname = argv[i];
		int result = mkdir(pathname, 0755); //디렉토리 0755 파일 모드로 생성 

		if(result != 0){
			printf("make directory error\n");
			return -1;
		}
	}
	return 0;
}

int cmd_ls(int argc, char argv[][100]){

	DIR *dp; //디렉토리 접근 포인터 변수 
	struct dirent *entry; //디렉토리 구조체 포인터 변수 

	char buf[255];
	getcwd(buf, 255); //현재 작업중인 디렉토리 경로 

	if((dp = opendir(buf)) != NULL){//디렉토리 열기 
		while((entry = readdir(dp)) != NULL){//디렉토리 구조체 정보 읽기 
			if(entry->d_name[0] == '.'){ //디렉토리 파일 이름 중 숨김 파일 출력 X 
				continue;
			}
			printf("%s\n", entry->d_name); 
		}
	}else{
		printf("current directory error\n");
	}

	closedir(dp); //디렉토리 닫기
	
   return 0;
}

int cmd_cp(int argc, char argv[][100]){

	FILE *src, *dst; //파일 구조체 접근 변수 
	int c; 
	
	if(argc != 2){
		printf("Usage: %s <src_file> <dst_file>\n", argv[0]);
		return -1;
	}

	char *in_file = argv[1];
	char *out_file = argv[2];

	if((src = fopen(in_file, "r")) ==  NULL){//파일 읽기 전용으로 열기 
		printf( "Can't open %s for reading.\n", in_file);
		return -1;
	}

	if((dst = fopen(out_file, "w")) == NULL){//파일 쓰기 전용으로 열기 
		printf("Can't open %s for writing.\n",out_file);
		return -1;
	}

	while((c = fgetc(src)) != EOF) //파일 끝이 아닐때까지 문자 읽기 
		fputc(c, dst); //파일에 문자 쓰기 

	fclose(src); //src 파일 닫기 
	fclose(dst); //dst 파일 닫기 
	
	return 0;
}
int cmd_rm(int argc, char argv[][100]){

	if(strcmp(argv[1], "-r") == 0){//옵션 [-r]

		for(int i = 2; i < argc+1; i++){//들어온 파일 수 만큼 
			char *pathname = argv[i];
			int result = rmdir(pathname); //디렉토리 삭제 

			if(result != 0){
				printf("remove directory error\n");
				return -1;
			}
		}
	}else{
		for(int i = 1; i < argc+1; i++){//들어온 파일 수 만큼 
			char *pathname = argv[i];
			int result = unlink(pathname);//파일 삭제 

			if(result !=0){
				printf("remove file error\n");
				return -1;
			}
		}
	}
	return 0;
}

int cmd_ln(int argc, char argv[][100]){

	const char *oldpath, *newpath;
	oldpath = argv[1];
	newpath = argv[2];

	if(link(oldpath, newpath) != 0){//원본 파일에 대한 링크 생성 
		printf("fail make link\n");
		return -1;
	}else{
		printf("success make link\n");
	}
	return 0;
}

int cmd_stat(int argc, char argv[][100]){

	char *statpath = argv[1];
	struct stat sb; //상태 구조체 접근 변수 

	if(stat(statpath, &sb) !=0){//입력 받은 파일 stat
		printf("%s : No such file or directory\n", statpath);
		return -1;
	}else{ //stat 구조체에서 정보 가져오기 
		printf("File : %s\n", statpath);
		printf("Size : %lld\t", (long long)sb.st_size); 
		printf("Blocks : %lld\t", (long long)sb.st_blocks);
		printf("Io Blocks : %ld\t\t", (long)sb.st_blksize);
		
		switch(sb.st_mode & S_IFMT){ //파일 종류 확인 
			case S_IFBLK : printf("block device\n"); break;
			case S_IFCHR : printf("character device\n"); break;
			case S_IFDIR : printf("directory device\n"); break;
			case S_IFIFO : printf("FIFO/pipe\n"); break;
			case S_IFLNK : printf("symlink\n"); break;
			case S_IFREG : printf("reguler file\n"); break;
			case S_IFSOCK : printf("sockete\n"); break;	
		       	default : printf("unknown?\n"); break;
		}			  
		printf("Device : %ld, %ld\t", (long)major(sb.st_dev),(long)minor(sb.st_dev));
		printf("Inode : %ld\t", (long)sb.st_ino);
		printf("Link : %ld\n", (long)sb.st_nlink); 
		printf("Access : %lo\t", (unsigned long)sb.st_mode);
		printf("UID : %ld\t  Gid : %ld\n", (long)sb.st_uid, (long)sb.st_gid); 
		printf("Access : %s\n", ctime(&sb.st_atime));
		printf("Modify : %s\n", ctime(&sb.st_mtime));
		printf("Change : %s\n", ctime(&sb.st_ctime)); 
	}
	return 0;
}

int cmd_chmod(int argc, char argv[][100]){

	const char *file = argv[2];

	if(access(argv[2], F_OK) != 0){//파일 존재 여부 확인 
	}

	int mode = 0;

	sscanf(argv[1], "%o", &mode);//8진수로 변환 

	if(chmod(file, mode) != 0){//접근권한 변경 
		printf("fail change mode\n");
		return -1;
	}else{
		printf("success change mode\n");
	}
	return 0;
}

int cmd_ps(int argc, char argv[][100]){
	
	DIR *dp; //디렉토리 접근 변수 
	FILE *fp; //파일 구조체 접근 변수
	struct dirent *entry;//디렉토리 구조체  접근 변수 
	struct stat sb; //stat 구조체 접근 변수 

	int pid;
	char cmd[128], path[128];

	dp = opendir("/proc"); //proc 디렉토리 열기 
	///proc은 커널 프로세스를 포함하는 각 실행 중인 프로세스들을 위한 디렉토리를 포함
	printf("PID\tCMD\n");

	while((entry = readdir(dp)) != NULL){ //디렉토리 구조체 정보 얻기 
		lstat(entry->d_name, &sb);
		
		if(S_ISDIR(sb.st_mode)){//디렉토리 파일 검사 

			pid = atoi(entry->d_name);// 문자열 정수형 변환 
			
			if(pid > 0){
				strcpy(path, "/proc/"); // proc path 에 복사 
				strcat(path, entry->d_name); //path에 entry->d_name 연결 
				strcat(path, "/cmdline"); //path에 cmdline 연결 
				//cmdline 해당 프로세스를 실행하기 위해 사용한 명령어를 담고 있다.
				//부팅 시 커널에 넘겨진 모든 인자들을 담고 있다.

				fp = fopen(path, "r"); //파일 읽기 전용으로 열기 
				
				if(fp == NULL){
					printf("error for %s\n", path);
				}
				fgets(cmd, 128, fp);//문자열 가져오기 
				
				if(strlen(cmd)>1){
					printf("%d\t%s\n", pid, cmd);
				fclose(fp);//파일 닫기 
				}
			}
		}
	}
	return 0;
}

