#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);


char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};


int (*builtin_func[]) (char **) = {
  &shell_cd,
  &shell_help,
  &shell_exit
};

int shell_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}



int shell_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("Error for built in ");
    }
  }
  return 1;
}


int shell_help(char **args)
{
  int i;
  printf("The shell here \n");
  printf("Type program names and arguments, and enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < shell_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Use the  command for information on other programs.\n");
  return 1;
}



int shell_exit(char **args)
{
  return 0;
}


int shell_launch(char **args)
{
    pid_t pid,wpid;
    int status;

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
          perror("Error forking");
    }
    else
    {
        // parent process
        do {
            wpid = waitpid(pid,&status,WUNTRACED);
        }while (!WIFEXITED(status) && !WIFSIGNALED(status));
        
    }
    return 1;
    
}



int shell_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < shell_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return shell_launch(args);
}


char *shell_read_line(void)
{
    char *line= NULL;
    ssize_t bufsize= 0;
    getline(&line, &bufsize, stdin);
    return line;
}



#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
char **shell_split_line(char *line)
{
    int bufsize = LSH_TOK_BUFSIZE, position = 0, toklen;

    char **tokens = malloc(bufsize * sizeof(char));

    char *token, *token_copy;



    token= strtok(line,LSH_TOK_DELIM);
    while(token != NULL)
    {
        toklen = strlen(token);
        token_copy = malloc((toklen + 1) * sizeof(char));
        strcpy(token_copy,token);

        tokens[position]=token_copy;
        position++;

        if (position <= bufsize)
        {
            bufsize +=LSH_TOK_BUFSIZE;
            tokens =realloc (tokens,bufsize);

            if (! tokens)
            {
                fprintf(stderr,"shell:allocation error\n");
                exit(EXIT_FAILURE);
            }
            
        }
        token =strtok(NULL, LSH_TOK_DELIM);
    
    }
    tokens[position]=NULL;
    return tokens;
    

}


void shell_free_args(char **args)
{
    char **iter = args;
    while (*iter != NULL)
    {
        free(*iter);
        iter++;
    }
    free(args);
    
}



void shell (void)
{
    char *line;
    char **args;
    int status;
    do {
        printf("$");
        line = shell_read_line();
        args= shell_split_line(line);
        status= shell_execute(args);  
        free(line);
        shell_free_args(args);
    }while(status);     
}


int main(int argc, char **argv )
{
    shell();
    return EXIT_SUCCESS;
    return 0;
}