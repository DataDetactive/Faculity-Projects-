#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int execute_onther_command(char **args)
{
    pid_t pid;

    pid =fork();
    if(pid == 0)
    {
        if (execvp(args[0],args)== -1)
        {
            perror("child procces");
        }
        exit(EXIT_FAILURE);
    } 
    else if (pid < 0)
    {
        perror("Error: Faild to create child process");
    }
    else
    {
      wait(NULL); // Enter to parent process 
    }
    return 1;
}

char *read_command(void)
{
    char *line= NULL;
    ssize_t bufsize;
    getline(&line, &bufsize, stdin);
    return line;
}


int execute_command(char **args)
{
  // Handle Empty command was entered.
  if (args[0] == NULL) {
     return 1; 
  }
  
  //Handle "exit" command
  if(strcmp(args[0],"exit") == 0){
    return 0;
  }

  //Handle Cd Command
  if(strcmp(args[0], "cd") == 0){

    if (args[1] == NULL)
      {
        fprintf(stderr, "Error: expected argument to \"cd\"\n");
      }
    else
      {
        // execute cd command
        if (chdir(args[1]) != 0){ perror("Error"); }
      }
        return 1;
  }

  //Handle Other Command
  return execute_onther_command(args);
}


char **split_line(char *line)
{
    int bufsize = 64, position = 0, toklen;

    char **tokens = malloc(bufsize * sizeof(char));

    char *token, *token_copy;

    token= strtok(line," \t\r\n\a");
    while(token != NULL)
    {
        toklen = strlen(token);
        token_copy = malloc((toklen + 1) * sizeof(char));
        strcpy(token_copy,token);

        tokens[position]=token_copy;
        position++;

        if (position <= bufsize)
        {
            bufsize +=64;
            tokens =realloc (tokens,bufsize);

            if (! tokens)
            {
                fprintf(stderr,"shell:allocation error\n");
                exit(EXIT_FAILURE);
            }
            
        }
        token =strtok(NULL, " \t\r\n\a");
    
    }
    tokens[position]=NULL;
    return tokens;
}


int main(int argc, char **argv )
{
    char *line;
    char **args;
    int status = 1;
    while(status){
        printf("os-project-$ ");
        line = read_command();
        args= split_line(line);
        status = execute_command(args);  
    }
    return EXIT_SUCCESS;
    return 0;
}