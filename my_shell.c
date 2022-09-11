#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <signal.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

pid_t  arr[MAX_NUM_TOKENS];
int fc_pg=0;

/* Splits the string by space and returns the array of tokens */

char **tokenize(char *line){
	char **tokens=(char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
	char *token=(char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
	int i, tokenIndex=0,tokenNo=0;
	
	for(i=0; i<strlen(line); i++){
		char readChar = line[i];
		
		if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
		token[tokenIndex] = '\0';
			if (tokenIndex != 0){
				tokens[tokenNo]=(char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
				strcpy(tokens[tokenNo++], token);
				tokenIndex= 0;			
			}
		} else{
			token[tokenIndex++]=readChar;
		}
	}
	free(token);
	tokens[tokenNo]=NULL;
	return tokens;

}


 
/* Signal Handler for SIGINT */
void sigintHandler(int sig_num){
//	signal(SIGINT, sigintHandler);
	printf("\n Cannot be terminated using Ctrl+C. Type exit.\n$ ");
	fflush(stdout);
	int pid;
	if (fc_pg){
		kill(fc_pg, SIGKILL);
		waitpid(pid, NULL,0);
	}
	while ( (pid = waitpid(-1, NULL, WNOHANG)) > 0)
	printf("\n%d Shell: Background process finished ", pid);
}

int main(int argc, char * argv[]){
	char line[MAX_INPUT_SIZE];
	char **tokens;
	int i,counter=0;
	

	signal(SIGINT, sigintHandler);
	while(1){
		bzero(line, sizeof(line));
		fflush(stdout);
		while ( waitpid(-1, NULL, WNOHANG) > 0);
		printf("$ ");
		// Take characters excluding the \n character
		scanf("%[^\n]", line);
	// wait for user to enter newline character
		getchar();
		//printf("Command entered: %s (TODO: Remove this)\n", line);
		
		// Terminate with new line
		line[strlen(line)]='\n';
		tokens = tokenize(line);
		
		// Shell implementation
		
		if (tokens[0] == NULL){
			continue;
		}

		if ( !strcmp(tokens[0] ,"cd")){
			//printf("\n%s Go to cd\n", tokens[0]);
			char s[100];

    			// printing current working directory
    			//printf("%s\n", getcwd(s, 100));

					//	chdir(tokens[1]);
			 if(chdir(tokens[1]) != 0) {
            			fprintf(stderr, "Shell: Incorrect command\n");
       }
			continue;
		} else if (!strcmp(tokens[0], "exit") || !strcmp(tokens[0], "quit")){	
    			int pid;
                      //  while ( (pid = waitpid(-1, NULL, WNOHANG)) > 0)
                        //printf("\nchild %d terminated", pid);
			for (int i=0;i<counter; i++ ){
				int status=kill(arr[i], SIGKILL);
				pid = waitpid(arr[i], NULL, 0);
				//printf("\nchild %d terminated\n", pid);
				if (!status){
                        		printf("\nchild %d terminated", arr[i]);
				}
			}
			exit(0);

		}
		
		int size=0;
		for (i = 0; tokens[i]!=NULL; i++){
			//printf("found token %s (TODO: Remove this)\n", tokens[i]);
			size++;
		}
		int background=0;
		// printf("\nSIZE: %d", size);
		// fflush(stdout);
		// int size= sizeof(tokens) / sizeof(tokens[0]);
		//
		if (! strcmp(tokens[size-1], "&")){
			background = 1;
			//printf("\nbackground process");
			//fflush(stdout);
			tokens[size-1]=NULL;
		}

		pid_t wpid;
		int status;
		pid_t pid=fork();
		
		if (pid < 0){
			printf("Fork call failed!\n");
		} else if (pid== 0){ // child process
			// printf("\nchild pid: %d\n", getpid());
			// fflush(stdout);
			setpgid(0,0);
			execvp(tokens[0],tokens);
			printf("Command execution failed!\n");
			fflush(stdout);
			exit(1);
		} else { // parent process
			if(background){
				//int pid;
				arr[counter++]=pid;
				while ( (pid = waitpid(pid, NULL, WNOHANG)) > 0);
				//printf("\nchild %d terminated\n", pid);
			} else{
				//do{	
				fc_pg= pid;
				wpid= waitpid(pid, NULL, 0);
				
				//	wpid = waitpid(pid, &status, WUNTRACED);
				//	printf("wid: %d",wpid);
				   // } while (!WIFEXITED(status) && !WIFSIGNALED(status));
			//	int wc=wait(NULL);
			}
		}
		
		
		// Freeing the memory
		for(i=0; tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);
	}
	return 0;
}


