#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <errno.h>

const char *mypath[] = {
  "./",
  "/usr/bin/",
  "/bin/",
  NULL
};

int parsePipe (char ** argv) {
  int i;
  for (i = 0; argv[i] != NULL; i++) {
    if (strstr(argv[i], "|") != NULL){
      argv[i] = NULL;
      i++;
      return i;
    }
  }
  return -1;
}


void startPipe(char ** argv, int argc){
  int pipefd[2];
  int status;
  pid_t pid1;
  pid_t pid2;
  int position;
  
  position = parsePipe(argv);
  for (int i = 0; argv[i] != NULL; i++){
    printf("%s ", argv[i]);
    printf("%s\n", argv[position + i]);
  }
  
  
  pipe(pipefd);
  fflush(stdout);
  pid1 = fork();
  if(pid1 == -1){
    printf("Forking error\n");
  }
  if(pid1 == 0){
    pid2 = fork();
    
    if(pid2 == 0) {
      close(pipefd[1]); //close write end of pipe
      dup2(pipefd[0],0);
      close(pipefd[0]);


      execv(argv[position], &argv[position]);
      exit(0);  
    }else {

      close(pipefd[0]);
      dup2(pipefd[1], STDOUT_FILENO);
      close(pipefd[1]);
      execv(argv[0], argv);
      exit(0);       
    }
  }else{
    wait(&status);
  }
  
    
 }



//checks if the name of the executeble exists in standard locations,
//returns NULL if not found.
void findDefaultPath (struct stat statbuf, char* fpath, char *filename){
  int i;
  for (i = 0; mypath[i] != NULL; i++){
    strcpy(fpath, mypath[i]);
    strcat(fpath, filename);
    if (stat(fpath, &statbuf) == 0){
      return;
    }
  }
  return;
}


//returns the amount of pipe characters in the array of strings
int containsPipe(char ** argv) {
  int i;
  int c = 0;
  for (i = 0; argv[i] != NULL; i++){
    if (strcmp(argv[i], "|") == 0)
      c++;
  }
  return c;
}


int main (){
  struct stat statbuf; //statbuffer used for stat()
  char fpath[1024];
  char path[1024];
  char input[1024];
  char *argv[11];
  int argc = 0;
  int runLoop = 1;
  int status;
  pid_t pid;
  
  chdir(getenv("HOME"));

  while (runLoop){
    /* Wait for input */
    printf ("prompt >%s ", getcwd(path, sizeof(path)));
    fgets (input, sizeof(input), stdin);
    
    
    /* Parse input */ 
    char * iptr = input; //input pointer needed for strsep argument
    while ((argv[argc] = strsep(&iptr, " ")) != NULL){
      argc++;
    }
    argv[argc-1][strlen(argv[argc-1])-1] = 0; //remove last \n from string 
    argv[argc] = NULL; //add terminating character

    if(strcmp(argv[0], "cd") == 0){ //cd 
      if(argc == 2){
        if(chdir(argv[1]) != 0){
          printf("%s\n", strerror(errno));
        }
      }else{
        printf("Please only enter 2 arguments\n");
      }
    }else if(strcmp(argv[0], "exit") == 0){ //exit 
      runLoop = 0;
    }else if (containsPipe(argv) > 1){
      printf("Error, no more than one pipe allowed\n");
    }else if (containsPipe(argv) == 1){
      startPipe(argv, argc);
    }else{
      pid = fork();
      if (pid == 0){
        if(strstr(argv[0], "/") != NULL){ //check if first arg contains a "/" 
          if(stat(argv[0], &statbuf) == 0){
            execv(argv[0], argv);
          }
        }else{
          findDefaultPath(statbuf, fpath, argv[0]);
          if(fpath != NULL){
            execv(fpath, argv);
          }
        }
        exit(0);
      }else{
        wait(&status);
      }
    }
    argc = 0;
  }
  return 0;
}
