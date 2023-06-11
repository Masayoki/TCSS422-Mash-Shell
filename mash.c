#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#define FILENAME_SIZE 256
#define CMD_SIZE 256
#define NUM_ARGS 5
#define NUM_CMDS 3

void childProcess(char *f_name, char *cmd, int cmd_num) {

  //Variable declaration
  char *args[NUM_ARGS + 2]; //Command + filename + args + NULL
  char *tok;
  int i;
  clock_t start, end;
  
  //Tokenize command
  char *full_cmd = strdup(cmd);
  i = 0;
  tok = strtok(cmd, " ");
  args[i] = tok;
  
  while ((tok = strtok(NULL, " ")) != NULL) {
    ++i;
    args[i] = tok;
    
  }
  
  //Appending filename to args
  ++i;
  args[i] = f_name;

  //End args with NULL
  ++i;
  args[i] = NULL;
  
  //Executing the command
  printf("-----CMD %d: %s---------------------------------------------------------\n", cmd_num, full_cmd);
  start = clock();
  pid_t pid = fork();
  if (pid == 0) {
  
    //Inside the child process
    execvp(args[0], args);
    
    //If execvp fails, the child process will exit here
    exit(EXIT_FAILURE);
    
  } else if (pid < 0) {
    fprintf(stderr, "Failed to fork child process.\n");
    printf("CMD%d:[SHELL %d] STATUS CODE=-1\n", cmd_num, cmd_num);
    exit(EXIT_FAILURE);
  } else {
    int status;
    waitpid(pid, &status, 0);
    end = clock();
    double elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
    //printf("Result took:%.0fms\n", elapsed * 1000);
    if (WIFEXITED(status)) {
      int exit_status = WEXITSTATUS(status);
      if (exit_status == 0) {
        printf("Result took:%.0fms\n", elapsed * 1000);
      } else {
        printf("CMD%d:[SHELL %d] STATUS CODE=%d\n", cmd_num, cmd_num, exit_status);
        printf("Result took:%.0fms\n", elapsed * 1000);
      }
      
    }
    
  }
  free(full_cmd);
}

int main() {

  //Variable declaration
  char f_name[FILENAME_SIZE];
  char cmds[NUM_CMDS][CMD_SIZE];
  pid_t pids[NUM_CMDS];
  int i;
  clock_t startTime, endTime;
  double elapsedTime;
  
  //Reads in the commands
  for (i = 0; i < NUM_CMDS; ++i) {
    printf("mash-%d>", i + 1);
    fgets(cmds[i], sizeof(cmds[i]), stdin);
    cmds[i][strlen(cmds[i]) - 1] = '\0';
  
  }
  
  //Reads in the filename
  printf("file>");
  fgets(f_name, sizeof(f_name), stdin);
  f_name[strlen(f_name) - 1] = '\0';
  
  
  //Measures start time
  startTime = clock();
  
  //Wait for all child processes to complete
  for (i = 0; i < NUM_CMDS; ++i) {
    childProcess(f_name, cmds[i], i + 1);
    
  }
  
  //Measures end time
  endTime = clock();

  //Measures elapsed time
  elapsedTime = ((double) (endTime - startTime)) / CLOCKS_PER_SEC;

  //Prints elapsed time
  printf("--------------------------------------------------------------------------------\n");
  printf("Children process IDs: %d %d %d.\n", getpid() + 1, getpid() + 2, getpid() + 3);
  printf("Total elapsed time:%.0fms\n", elapsedTime * 1000);

  //End
  return 0;
  
}
  
