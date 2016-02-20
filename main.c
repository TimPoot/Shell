#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char *mypath[] = {
  "./",
  "/usr/bin/",
  "/bin/",
  NULL
};

int main (){
  char path[1024];
  char input[1024];
  char *argv[10];
  int argc = 0;
  //int i; still needed ???

  strcpy(path, mypath[1]);
  while (1){
    /* Wait for input */
    printf ("prompt >%s ", path);
    fgets (input, sizeof(input), stdin);
    
    
    /* Parse input */ 
    char * iptr = input; //input pointer needed for strsep argument
    while ((argv[argc] = strsep(&iptr, " ")) != NULL){
      argc++;
    }
    argv[argc-1][strlen(argv[argc-1])-1] = 0; //remove last \n from string 

    if(strcmp(argv[0], "cd") == 0){
      if(argc == 2){
        strcat(path, argv[1]);
      }else{
        printf("Please only enter 2 arguments\n");
      }
    }

    /* If necessary locate executable using mypath array 
    Launch executable 
    if (fork () == 0){
      //TODO...
      execv (...);
      //TODO...
    }else{
      wait (...);
    }*/

    argc = 0;
  }
  return 0;
}
