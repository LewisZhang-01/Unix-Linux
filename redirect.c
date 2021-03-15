/*
Student Name: Zhi Zhang
Student Number: 18210054
Email: zhi.zhang@ucdconnect.ie
*/

#include "ZShell.h"

//Redirect when user enter ">".
bool redirect(char **input){
    char *filename;
    int count=0;
    int position=0;
    int child_pid,child_status;
    while(input[count]!=NULL){//Traversal input.
        //If the input contains '>', assign index to the position variable.
        if(strcmp(">",input[count])==0) position = count;
        count++;
    }
    if(position!=0) {//'>' detected in user input.
        child_pid = fork();
        if(child_pid == 0){
            filename = input[position+1];//Gets the target file name.
    	    int f = open (filename,O_WRONLY|O_CREAT|O_TRUNC,0666);
    	    int fd = dup2 (f,1);
    	    input[position]=NULL;
    	    if(input[0]!=NULL)  cmdHandle(input);//Execute the command.
            if(fd!=0)  exit(1);//Exit when dup2 finished.
            close(f);//Close file.
        }else{
            wait(&child_status);//Waiting for child finish.
            return true;
    	}
    }else if(input[0]==NULL){//No '>' in user input.
         return true;//If user enter nothing, return true to skip.
    }
    return false;//Otherwise, return false.
}
