/*
Student Name: Zhi Zhang
Student Number: 18210054
Email: zhi.zhang@ucdconnect.ie
*/

#include "ZShell.h"

//Initialization of shell.
void init_shell(){
    printf("**************************Welcome to use Zhi's Shell***************************\n");
    printf("           ***|***                                                        \n");
    printf("         *\\***|***/*          *****   *     *  *******  *        *      \n");
    printf("        ***\\**|**/***       *         *     *  *        *        *      \n");
    printf("       *****\\*|*/*****      *         *     *  *        *        *      \n");
    printf("        *****\\|/*****        ******   *******  *******  *        *      \n");
    printf("          *********                *  *     *  *        *        *      \n");
    printf("           *******                 *  *     *  *        *        *      \n");
    printf("          *********         *******   *     *  *******  *******  *******\n");
    printf("\n");
    sleep(1);
}

//Split the string by space/char/etc..
void separateStr(char *buffer, char **output_str, char *by ){
    int count = 0;
    char *temp = strtok(buffer,by);//String tokenize.
    while(temp != NULL){
        output_str[count] = temp;//Store current one into output string.
        count++;
        output_str = realloc(output_str,(count+1)*sizeof(char*));//Re-allocate size for output string.
        temp = strtok(NULL,by);//Move on to the next one.
    }  
    //Finally add NULL to the last position of output string.
    output_str[count] = NULL;
    output_str = realloc(output_str,(count+1)*sizeof(char*));
}

//Print prompt.
void prompt(){
    time_t rawtime;
    struct tm *info;
    char buffer[80];
    time( &rawtime );//Gets the number of seconds, since 1/January/1970, stored in rawtime.
    info = localtime( &rawtime );//Get local time.
    strftime(buffer, 80, "%d/%m %H:%M", info);//Format the time in a custom way.
    printf("[%s]# ", buffer );
}

//Signal handler.
void sig_handler(int signo){
    if(signo == SIGINT){//Catched SIGINT.
        printf("\n");//New line.
        prompt();//Print prompt
        fflush(stdout);//Flush buffer.
    }
}

//Main function.
int main ()
{
    char *buffer;   
    size_t bufferSize = 1024;
    size_t temp = 0;
    
    init_shell();//Initialize the shell.
    
    while(1){
        //Allocates buffers for user input.
        buffer = malloc(sizeof(char) * bufferSize);
    	if(buffer == NULL){
            perror("Unable to allocate buffer.\n");
            exit(1);
        }
        
        prompt();// display prompt on the screen.
 
    	//Catching SIGINT when enter Ctrl+C.
      	fflush(stdout);
      	if(signal(SIGINT, sig_handler)==SIG_ERR)  printf("\ncannot catch SIGINT\n");//Handle errors.
      	
      	//Read user input into buffer, exit when receive EOF (Ctrl+D).
    	if((temp = getline(&buffer,&bufferSize,stdin))==EOF){ 
    	    printf("EOF\n"); 
    	    exit(EXIT_SUCCESS); 
    	}
	
        //Remove the newline character from the user input.
    	buffer[strcspn (buffer, "\n")] = 0;
    	char **input = malloc(sizeof(char*));//Store user input.
    	
    	//Split the buffer string by space, and store into input string.
    	separateStr(buffer,input," ");
    	
    	//Check if need redirect.
    	if(redirect(input)==false) cmdHandle(input);
    	
        //Free buff.  
        free(input);
        free(buffer); 
    }
   
   return 0;
}

