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
  char *path = "~/home";
  char input[1024];
  char *arg[10];
  int argc = 0;
  
  while (1){
    /* Wait for input */
    printf ("prompt> %s ", path);
    fgets (input, sizeof(input), stdin);
    
    
    /* Parse input */ 
    char * iptr = input; //input pointer
    while ((argv[argc] = strsep(&iptr, " ")) != NULL)
    {
      printf("%s", argv[argc]);
      argc++;
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
  }
  return 0;
}
