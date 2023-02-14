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

char argv[10][100]; //��ɾ� ���ڿ� ���� 

int cmd_pwd(int argc, char argv[][100]); //���� �۾����� ��� ��� 
int cmd_chmod(int argc, char argv[][100]); //���� ���ٱ��� ���� 
int cmd_ls(int argc, char argv[][100]); //���� ��� ���丮 �̸� ��� 
int cmd_cd(int argc, char argv[][100]); //���� ��� ���� 
int cmd_stat(int argc, char argv[][100]); //���� ���� ��� 
int cmd_mkdir(int argc, char argv[][100]); //���丮 ���� 
int cmd_ps(int argc, char argv[][100]); //���� �������� ���μ��� ��� 
int cmd_cp(int argc, char argv[][100]); //���� ���� 
int cmd_rm(int argc, char argv[][100]); //���� �� ���丮 ����� 
int cmd_ln(int argc, char argv[][100]); //���� ��ũ ���� 
int cmd_cat(int argc, char argv[][100]); //���� ���� �Ѱ� �̻� ��� 

void history_init(char history[100][100]){ //history �ʱ�ȭ �Լ� 
	for(int i = 0; i < 100; i++){
		for(int j = 0; j < 100; j++){
			history[i][j] = NULL; //NULL �ʱ�ȭ 
		}
	}
}
char command_token(char command[]){ 
	//argv �ʱ�ȭ 
	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 100; j++){
			argv[i][j] = NULL; //NULL �ʱ�ȭ 
		}
	}

	int str_pointer = 0;//command index value
	int cmd_str_pointer =0;//cmd 2���� index value
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
		
		for(int i = 0; i < argc+1; i++){//���� �� ��ŭ 
			cmd_str_pointer = 0;
			
			while(command[str_pointer] != '\n'){//command index value \n�� �ƴҶ�����
				if(command[str_pointer] == ' '){//command index value ' ' �̸� 
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
	char history[100][100]; //history ���� 
	history_init(history); //history �ʱ�ȭ 

	while(1){
		printf("$ my_shell : ");
		fgets(command, 100, stdin);
		argc = command_token(command);
		
		strcpy(history[roof_num], command); //�Էµ� command history�� ���� 
		roof_num++;

		if(command[0] == '!'){//�Էµ� command !�̸� 

			int history_num = atoi(command+1);//���ڿ� ������ ��ȯ 

			if(history_num > roof_num){//������� ���� ![number] �Է� �� 
				printf(" !%d history is not existed\n", history_num);
				roof_num = roof_num -1; 
			}else{
				strcpy(command, history[history_num]); //history ����� ��ɾ� command�� ���� 
				argc = command_token(command); //��ɾ���� 
			}
		}
		if(strcmp(argv[0], "history") == 0){
			if(roof_num > 101){
				printf("failed to save history");
			}
			for(int i = 1; i < roof_num && roof_num < 101; i++){ 
				printf("	%d : %s", i, history[i]); //����� ��ɾ� ��� 
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
			exit(0); //���� 
		}else{
			printf("%s is not found\n", command);
		}
	}
}

int cmd_cat(int argc, char argv[][100]){

	FILE *fp; //���� ����ü ������ ���� 
	char buf[255];
	int line_number = 1;
	
	if(strcmp(argv[1], "-n") == 0){ //�ɼ� -n 

		for(int i = 2; i < argc+1; i++){ 

			char *filename = argv[i];
			fp = fopen(filename, "r"); //���� �б� �������� ����

			if(fp != NULL){
				while(fgets(buf, 255, fp) != NULL){//���Ͽ��� ���ڿ� �������� 
					printf("%d %s",line_number++, buf);
				}
				fclose(fp);//���� �ݱ�  
			}else{
				printf("%s : No such file or directory\n", filename);
			}
			
		}		
	}else{
		for(int i = 1; i < argc+1; i++){//���� ���� �� ��ŭ 

			char *filename = argv[i];
			fp = fopen(filename, "r"); //���� �б� �������� ���� 

			if(fp != NULL){
				while(fgets(buf, 255, fp) != NULL){//���Ͽ��� ���ڿ� �������� 
					printf("%s", buf);
				}
				fclose(fp); //���� �ݱ� 
			}else{
				printf("%s : No such file or directory\n", filename);
			}
		}
	}
	
	return 0;
} 

int cmd_pwd(int argc, char argv[][100]){

	char buf[100];
	char *result = getcwd(buf, 100); //���� �۾����� ��� buf�� ��� 

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
		int result = chdir(path);//���丮 ��� �̵� 

		if(result != 0){
			printf("change directory fail\n");
			return -1;
		}
	return 0;
}

int cmd_mkdir(int argc, char argv[][100]){

	for(int i = 1; i < argc+1; i++){//���� ���� �� ��ŭ 
			
		char *pathname = argv[i];
		int result = mkdir(pathname, 0755); //���丮 0755 ���� ���� ���� 

		if(result != 0){
			printf("make directory error\n");
			return -1;
		}
	}
	return 0;
}

int cmd_ls(int argc, char argv[][100]){

	DIR *dp; //���丮 ���� ������ ���� 
	struct dirent *entry; //���丮 ����ü ������ ���� 

	char buf[255];
	getcwd(buf, 255); //���� �۾����� ���丮 ��� 

	if((dp = opendir(buf)) != NULL){//���丮 ���� 
		while((entry = readdir(dp)) != NULL){//���丮 ����ü ���� �б� 
			if(entry->d_name[0] == '.'){ //���丮 ���� �̸� �� ���� ���� ��� X 
				continue;
			}
			printf("%s\n", entry->d_name); 
		}
	}else{
		printf("current directory error\n");
	}

	closedir(dp); //���丮 �ݱ�
	
   return 0;
}

int cmd_cp(int argc, char argv[][100]){

	FILE *src, *dst; //���� ����ü ���� ���� 
	int c; 
	
	if(argc != 2){
		printf("Usage: %s <src_file> <dst_file>\n", argv[0]);
		return -1;
	}

	char *in_file = argv[1];
	char *out_file = argv[2];

	if((src = fopen(in_file, "r")) ==  NULL){//���� �б� �������� ���� 
		printf( "Can't open %s for reading.\n", in_file);
		return -1;
	}

	if((dst = fopen(out_file, "w")) == NULL){//���� ���� �������� ���� 
		printf("Can't open %s for writing.\n",out_file);
		return -1;
	}

	while((c = fgetc(src)) != EOF) //���� ���� �ƴҶ����� ���� �б� 
		fputc(c, dst); //���Ͽ� ���� ���� 

	fclose(src); //src ���� �ݱ� 
	fclose(dst); //dst ���� �ݱ� 
	
	return 0;
}
int cmd_rm(int argc, char argv[][100]){

	if(strcmp(argv[1], "-r") == 0){//�ɼ� [-r]

		for(int i = 2; i < argc+1; i++){//���� ���� �� ��ŭ 
			char *pathname = argv[i];
			int result = rmdir(pathname); //���丮 ���� 

			if(result != 0){
				printf("remove directory error\n");
				return -1;
			}
		}
	}else{
		for(int i = 1; i < argc+1; i++){//���� ���� �� ��ŭ 
			char *pathname = argv[i];
			int result = unlink(pathname);//���� ���� 

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

	if(link(oldpath, newpath) != 0){//���� ���Ͽ� ���� ��ũ ���� 
		printf("fail make link\n");
		return -1;
	}else{
		printf("success make link\n");
	}
	return 0;
}

int cmd_stat(int argc, char argv[][100]){

	char *statpath = argv[1];
	struct stat sb; //���� ����ü ���� ���� 

	if(stat(statpath, &sb) !=0){//�Է� ���� ���� stat
		printf("%s : No such file or directory\n", statpath);
		return -1;
	}else{ //stat ����ü���� ���� �������� 
		printf("File : %s\n", statpath);
		printf("Size : %lld\t", (long long)sb.st_size); 
		printf("Blocks : %lld\t", (long long)sb.st_blocks);
		printf("Io Blocks : %ld\t\t", (long)sb.st_blksize);
		
		switch(sb.st_mode & S_IFMT){ //���� ���� Ȯ�� 
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

	if(access(argv[2], F_OK) != 0){//���� ���� ���� Ȯ�� 
	}

	int mode = 0;

	sscanf(argv[1], "%o", &mode);//8������ ��ȯ 

	if(chmod(file, mode) != 0){//���ٱ��� ���� 
		printf("fail change mode\n");
		return -1;
	}else{
		printf("success change mode\n");
	}
	return 0;
}

int cmd_ps(int argc, char argv[][100]){
	
	DIR *dp; //���丮 ���� ���� 
	FILE *fp; //���� ����ü ���� ����
	struct dirent *entry;//���丮 ����ü  ���� ���� 
	struct stat sb; //stat ����ü ���� ���� 

	int pid;
	char cmd[128], path[128];

	dp = opendir("/proc"); //proc ���丮 ���� 
	///proc�� Ŀ�� ���μ����� �����ϴ� �� ���� ���� ���μ������� ���� ���丮�� ����
	printf("PID\tCMD\n");

	while((entry = readdir(dp)) != NULL){ //���丮 ����ü ���� ��� 
		lstat(entry->d_name, &sb);
		
		if(S_ISDIR(sb.st_mode)){//���丮 ���� �˻� 

			pid = atoi(entry->d_name);// ���ڿ� ������ ��ȯ 
			
			if(pid > 0){
				strcpy(path, "/proc/"); // proc path �� ���� 
				strcat(path, entry->d_name); //path�� entry->d_name ���� 
				strcat(path, "/cmdline"); //path�� cmdline ���� 
				//cmdline �ش� ���μ����� �����ϱ� ���� ����� ��ɾ ��� �ִ�.
				//���� �� Ŀ�ο� �Ѱ��� ��� ���ڵ��� ��� �ִ�.

				fp = fopen(path, "r"); //���� �б� �������� ���� 
				
				if(fp == NULL){
					printf("error for %s\n", path);
				}
				fgets(cmd, 128, fp);//���ڿ� �������� 
				
				if(strlen(cmd)>1){
					printf("%d\t%s\n", pid, cmd);
				fclose(fp);//���� �ݱ� 
				}
			}
		}
	}
	return 0;
}

