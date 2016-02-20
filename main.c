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
  char *path = "/home";
  char input[1024];
  char *argv[10];
  int argc = 0;
  //int i; still needed ???
  
  while (1){
    /* Wait for input */
    printf ("prompt>%s ", path);
    fgets (input, sizeof(input), stdin);
    
    
    /* Parse input */ 
    char * iptr = input; //input pointer needed for strsep argument
    while ((argv[argc] = strsep(&iptr, " ")) != NULL){
      argc++;
    }  

    if(strcmp(argv[0], "cd") == 0){
      printf("test");
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
