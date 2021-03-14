/*
Student Name: Zhi Zhang
Student Number: 18210054
Email: zhi.zhang@ucdconnect.ie
*/


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

extern char** environ;
void init_shell();
void printDir();
void prompt();
void cmdHandle(char** input);
void separateStr(char *buffer, char **output_str, char *by );
void my_cd(char **input);
int my_redirect(char **input);
void my_ls(char **input);
void my_cat(char **input);
void str_replace(char *str,char *oldstr,char *newstr);

void init_shell(){
    printf("***Welcome to use 智's Shell***\n");
    printf("            ***|***\n");
    printf("          *\\***|***/*\n");
    printf("         ***\\**|**/***\n");
    printf("        *****\\*|*/*****\n");
    printf("         *****\\|/*****\n");
    printf("           *********\n");
    printf("            *******\n");
    printf("           *********\n");
    char* userName = getenv("USER");
    printf("[User: @%s]\n",userName);
    printf("\n");
    sleep(1);
}


void printDir(){
    char cwd[1024];
    getcwd(cwd,sizeof(cwd));
    printf("%s\n",cwd);
}

void my_ls(char **input){
    char *arg[]={"ls",(char*)0};
    
    if(input[1]==NULL || strcmp(input[1],">")==0){
        execv("/bin/ls",arg);
    }else if(chdir(input[1])!= 0){  
   	printf("ls: %s: %s\n",input[1],strerror(errno));
    }else{
        char cwd[1024];
        getcwd(cwd,sizeof(cwd));
        chdir(input[1]);
        execv("/bin/ls",arg);
        chdir(cwd);
    }
    
}

void str_replace(char *str,char *oldstr,char *newstr){
    char bstr[strlen(str)];//转换缓冲区
    memset(bstr,0,sizeof(bstr));
 
    for(int i = 0;i < strlen(str);i++){
        if(!strncmp(str+i,oldstr,strlen(oldstr))){//查找目标字符串
            strcat(bstr,newstr);
            i += strlen(oldstr) - 1;
        }else{
        	strncat(bstr,str + i,1);//保存一字节进缓冲区
	    }
    }
 
    strcpy(str,bstr);
}

void my_cd(char **input){
    
    //handel "~" home char input.
    str_replace(input[1],"~",getenv("HOME"));
    
    if(input[1]==NULL){
        chdir(getenv("HOME"));
    }else if(chdir(input[1])!= 0){
   	printf("cd: %s: %s\n",input[1],strerror(errno));
    }else{
        chdir(input[1]);
    }
    
}

int my_redirect(char **input){
    char *filename;
    int count=0;
    int position=0;
    while(input[count]!=NULL){
        if(strcmp(">",input[count])==0) position = count;
        count++;
    }
    if(position!=0) filename = input[position+1];
    else return -1;
    //printf("%s",filename);
    int f = open ( filename , O_WRONLY|O_CREAT|O_TRUNC,0666 ) ;
    dup2 ( f , 1 ) ;
    close(f);
    return 0;
}

void my_cat(char **input){
    if(input[1]==NULL){
        printf("[Usage] cat FILE_NAME\n");
        exit(-1);
    }
    FILE *fp = fopen(input[1], "r");//打开文件
    int read;
    
    if(fp == NULL){
        perror(input[1]);
        printf("open src %s failed!\n", input[1]);
        exit(-1);
    }

    while(1){
           read = fgetc(fp);//读取文件
           if(feof(fp))  break;//判断是否读取完毕
           fputc(read,stdout);//将数据输出到屏幕
    }
    fclose(fp);
}

void cmdHandle(char **input){
           
    char *command = input[0];
    //printf("command:=%s=\n",command);
    pid_t child_pid;
    int child_status;
    int cmd = -1;
    
    if(strcmp(input[0],"exit")==0 || strcmp(input[0],"quit")==0)  exit(0);
    if(strcmp(input[0],"ls")==0) cmd=1;
    if(strcmp(input[0],"cd")==0) cmd=2;
    if(strcmp(input[0],"pwd")==0) cmd=3;
    if(strcmp(input[0],"echo")==0) cmd=4;
    if(strcmp(input[0],"sleep")==0) cmd=5;
    if(strcmp(input[0],"mkdir")==0) cmd=6;
    if(strcmp(input[0],"rmdir")==0) cmd=7;
    if(strcmp(input[0],"whoami")==0) cmd=8;
    if(strcmp(input[0],"cat")==0) cmd=9;

    switch(cmd){
        case 1://ls
            child_pid = fork();
            if(child_pid == 0){
        	my_ls(input);
        	exit(0);
   	    }else{
        	wait(&child_status);
    	    }
            break;
        case 2://cd
            my_cd(input);
            break;
        case 3://pwd
            printDir();
            break;
        case 4://echo
            child_pid = fork();
            if(child_pid == 0){
        	execvp(command,input);
        	exit(0);	
   	    }else{
        	wait(&child_status);
    	    }
	    break;
        case 5://sleep
            child_pid = fork();
            if(child_pid == 0){
        	sleep(atoi(input[1]));
        	exit(0);
   	    }else{
        	wait(&child_status);
    	    }
    	    printf("sleep finished\n");
	    break;
	case 6://mkdir
	    child_pid = fork();
            if(child_pid == 0){
        	if(mkdir(input[1],0770)!=0){
                    perror("mkdir");
                    exit(-1);
                }
                exit(0);
   	    }else{
        	wait(&child_status);
    	    }
            break;
	case 7://rmdir
	    child_pid = fork();
            if(child_pid == 0){
        	if(rmdir(input[1]) != 0){
                    perror("rmdir");
                    exit(-1);
                }
                exit(0);
   	    }else{
        	wait(&child_status);
    	    }
            break;
        case 8://whoami
    	    printf("You are: %s\n",getenv("USER")); 
    	    break;
    	case 9://cat
    	    child_pid = fork();
            if(child_pid == 0){
        	my_cat(input); 
                exit(0);
   	    }else{
        	wait(&child_status);
    	    }
    	    break;
        default:
            printf("[ERROR] command not found!\n");
    }
}

void separateStr(char *buffer, char **output_str, char *by ){
    int count = 0;
    char *temp = strtok(buffer,by);
    while(temp != NULL){
        output_str[count] = temp;
        count++;
        output_str = realloc(output_str,(count+1)*sizeof(char*));
        //printf("%s\n",temp);
        temp = strtok(NULL,by);
    }  
    output_str[count] = NULL;
    output_str = realloc(output_str,(count+1)*sizeof(char*));
    
}

void prompt(){
 
    //date [dd/mm hh :mm]#
    time_t rawtime;
    struct tm *info;
    char buffer[80];
 
    time( &rawtime );
 
    info = localtime( &rawtime );
 
    strftime(buffer, 80, "%d/%m %H:%M", info);
    printf("[%s]# ", buffer );

}

void sig_handler(int signo){
    if(signo == SIGINT){
        printf("\n");
        prompt();
        fflush(stdout);
    }
}

//Main function.
int main ()
{
    char *buffer;   
    size_t bufferSize = 1024;
    size_t temp = 0;
    int child_pid,child_status;
    
    init_shell(); 
    
    while(1){
        buffer = malloc(sizeof(char) * bufferSize);
    	if(buffer == NULL)
    	{
            perror("Unable to allocate buffer.\n");
            exit(1);
        }
        
        prompt();// display prompt on the screen.
 
    	// read lines  
    	//printf("Catching SIGINT, try Ctrl+C");
      	fflush(stdout);
      	if(signal(SIGINT, sig_handler)==SIG_ERR)  {
      	  printf("\ncannot catch SIGINT\n");
      	}
      	  
      	  
    	if((temp = getline(&buffer,&bufferSize,stdin))==EOF){ 
    		printf("EOF\n"); 
    		exit(EXIT_SUCCESS); 
    	}
    	
    	
    	
    	buffer[strcspn (buffer, "\n")] = 0;
    	char **input = malloc(sizeof(char*));
    	separateStr(buffer,input," ");
    	
    	//my_redirect(input);
    	
    	char *filename;
    int count=0;
    int position=0;
    while(input[count]!=NULL){
        if(strcmp(">",input[count])==0) position = count;
        count++;
    }
    if(position!=0) {
    
    	child_pid = fork();
        if(child_pid == 0){
        	filename = input[position+1];
    
    		int f = open ( filename , O_WRONLY|O_CREAT|O_TRUNC,0666 ) ;
    		int fd = dup2 ( f , 1 ) ;
    
    		if(input[0]!=NULL) cmdHandle(input);
    		if(fd!=0)  exit(1);
    	close(f);	
   	
   	}else{
        	wait(&child_status);
    	}
    	    
    	    
    	
    }else{
        if(input[0]!=NULL) cmdHandle(input);
    }
    	// free buff  
    	free(input);
    	free(buffer); 
    }
   
   return 0;
   
}

