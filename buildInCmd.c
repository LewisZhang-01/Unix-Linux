/*
Student Name: Zhi Zhang
Student Number: 18210054
Email: zhi.zhang@ucdconnect.ie
*/

#include "ZShell.h"

//Print the current directory.
void printDir(){
    char cwd[1024];
    getcwd(cwd,sizeof(cwd));//Get current work directory.
    printf("%s\n",cwd);
}

//Customized version of perror().
void newPerror(char **input,char *cmd){
    char *temp = malloc(sizeof(char)*(strlen(input[1])+10));
    strcpy(temp,cmd);//Copy cmd to temp.
    perror(strcat(temp,input[1]));//Print format:[cmd: input: ..error..]
}

//List files
void ls(char **input){
    char *arg[]={"ls",(char*)0};
    //List all files in current directory.
    if(input[1]==NULL || strcmp(input[1],">")==0){
        execv("/bin/ls",arg);
    }else if(chdir(input[1])!= 0){ //Cannot change directory. 
        newPerror(input,"ls: ");
   	//printf("ls: %s: %s\n",input[1],strerror(errno));
    }else{//Lists the files in the specified directory.
        char cwd[1024];
        getcwd(cwd,sizeof(cwd));//Stores the current directory for later use when returning.
        chdir(input[1]);//Change the dir to the dir entered by the user.
        execv("/bin/ls",arg);//List files.
        chdir(cwd);//Back to previous dir.
    }
}

//Replace a value in a string.
void str_replace(char *str,char *oldstr,char *newstr){
    //Define a buffer string.
    char buffer_str[strlen(str)];
    memset(buffer_str,0,sizeof(buffer_str));//Initialize to 0.
    //Check oldstr one by one in str.
    for(int i = 0;i < strlen(str);i++){
        if(!strncmp(str+i,oldstr,strlen(oldstr))){//Find the target string(oldstr).
            strcat(buffer_str,newstr);//Appends newstr to buffer instead of oldstr.
            i += strlen(oldstr) - 1;//Move on to the next.
        }else{
            strncat(buffer_str,str + i,1);//Append 1 char into buffer.
	}
    }
    strcpy(str,buffer_str);//Copy from buffer to str.
}

//Change directory.
void cd(char **input){
    //handel "~" home char input.
    str_replace(input[1],"~",getenv("HOME"));
    //If user only enter cd, change to HOME dir.
    if(input[1]==NULL){
        chdir(getenv("HOME"));
    }else if(chdir(input[1])!= 0){//Cannot change directory. 
        newPerror(input,"cd: ");
   	//printf("cd: %s: %s\n",input[1],strerror(errno));
    }else{
        chdir(input[1]);//Change to specified directory.
    }
}

//Catenate.
void cat(char **input){
    //If user only enter cat.
    if(input[1]==NULL){
        printf("[Usage] cat FILE_NAME\n");//Print cat usage.
        exit(-1);//Exit.
    }
    FILE *fp = fopen(input[1], "r");//Open file.
    int read;
    //Invalid case handle.
    if(fp == NULL){
        newPerror(input,"cat: ");
        exit(-1);
    }
    while(1){
        read = fgetc(fp);//Read file.
        if(feof(fp))  break;//Check whether the read is complete.
        fputc(read,stdout);//Print out.
    }
    fclose(fp);//Close file.
}

//Command handler.
void cmdHandle(char **input){
           
    char *command = input[0];//Assign command with 1st of input.
    pid_t child_pid;
    int child_status;
    int cmd = -1;
    //Numbers corresponding to different command.
    if(strcmp(input[0],"exit")==0 || strcmp(input[0],"quit")==0)  exit(0);
    if(strcmp(input[0],"help")==0) cmd=0;
    if(strcmp(input[0],"ls")==0) cmd=1;
    if(strcmp(input[0],"cd")==0) cmd=2;
    if(strcmp(input[0],"pwd")==0) cmd=3;
    if(strcmp(input[0],"echo")==0) cmd=4;
    if(strcmp(input[0],"sleep")==0) cmd=5;
    if(strcmp(input[0],"mkdir")==0) cmd=6;
    if(strcmp(input[0],"rmdir")==0) cmd=7;
    if(strcmp(input[0],"whoami")==0) cmd=8;
    if(strcmp(input[0],"cat")==0) cmd=9;
    if(strcmp(input[0],"rm")==0) cmd=10;

    switch(cmd){
        case 0://help
            printf("\t<Author Info>\tAuthor Name:\tZhi Zhang\n\t\t\tStuent number:\t18210054\n\t\t\tEmail:\t\tzhi.zhang@ucdconnect.ie\n\n");
            printf("\t<Build-in CMD>\tls,cd,pwd,echo,sleep,mkdir,rmdir,whoami,cat,rm\n\n");
            break;
        case 1://ls
            child_pid = fork();
            if(child_pid == 0){
        	ls(input);
        	exit(0);
   	    }else{
        	wait(&child_status);
    	    }
            break;
        case 2://cd
            cd(input);
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
	    break;
	case 6://mkdir
	    child_pid = fork();
            if(child_pid == 0){
        	if(mkdir(input[1],0777)!=0){
                    newPerror(input,"mkdir: ");
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
                    newPerror(input,"rmdir: ");
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
        	cat(input); 
                exit(0);
   	    }else{
        	wait(&child_status);
    	    }
    	    break;
    	case 10://rm
    	     child_pid = fork();
             if(child_pid == 0){
                 if(unlink(input[1])!=0){
                 perror("rm");
                 exit(-1);
             }   
             exit(0);
   	     }else{
        	 wait(&child_status);
    	     }
             break;
        default:
            printf("[ERROR] command not found!\n");
            printf("<Help?>\thelp\n");
    }
}

