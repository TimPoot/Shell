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

char * getParentDir(char path[]){
  int i = 2;
  if(strcmp(path, "/") == 0){
    printf("There is no parent directory\n");
  }else{
    path[strlen(path) - 1] = '\0';
    printf("%i", atoi(strrchr(path, '/')));
    path[atoi(strrchr(path, '/'))] = '\0';
  }
  return path;
}

//checks if the name of the executeble exists in standard locations,
//returns NULL if not found.
char * findDefaultPath (struct stat statbuf, char *filename){
  char fpath[1024];
  int i;
  for (i = 0; mypath[i] != NULL; i++){
    strcpy(fpath, mypath[i]);
    strcat(fpath, filename);
    if (stat(fpath, &statbuf) == 0){
      return fpath;
    }
  }
  return NULL;
}


int main (){
  DIR *directory;
  struct stat statbuf; //statbuffer used for stat()
  char path[1024];
  char input[1024];
  char *argv[11];
  int argc = 0;
  int exit = 1;
  int status;
  int i;
  
  chdir(getenv("HOME"));

  while (exit){
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
      exit = 0;
    }

    /* If necessary locate executable using mypath array 
    Launch executable */
    if (fork() == 0){
      if(strstr(argv[0], "/") != NULL){ //check if first arg contains a "/" 
        if(stat(argv[0], &statbuf) == 0){
          execv(argv[0], argv);
        }
      }else{
        if(findDefaultPath(statbuf, argv[0]) != NULL){
          execv(findDefaultPath(statbuf, argv[0]), argv);
        }
      }
    }else{
      wait(&status);
    }

    //clear arg
    for (i = 0; i < argc; i++){
      argv[i][0] = '\0';
    }
    argc = 0;
  }
  return 0;
}
