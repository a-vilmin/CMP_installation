#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
  char* dissco_path = argv[1];
  char* dissco_proj = argv[2];

  pid_t dissco_pid = fork();

  if(dissco_pid > 0){
    //in child
    puts("Starting DISSCO....");
    execlp(argv[1], argv[1], argv[2], (char*)NULL);
  }

  else if(dissco_pid < 0){
    //fork failed
    perror("Fork failed!!");
    exit(errno);
  }

  else{
    //in parent
    int status;
    waitpid(dissco_pid, &status, 0);
    if(WIFEXITED(status))
      printf("DISSCO EXITED WITH STATUS %d\n", WEXITSTATUS(status));
    return 0;
  }
}
