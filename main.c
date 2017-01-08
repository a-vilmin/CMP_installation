#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/wait.h>

void puts_usage(){
  puts("You forgot something! Make sure to follow example");
  puts("./inst_handler <cmod file path> <cmod project path> <seed>");
}

int main(int argc, char* argv[]){
  if(argc != 4){
    puts_usage();
    exit(-1);
  }

  //parse arguments
  char* dissco_path = argv[1];
  char* dissco_proj = argv[2];
  char* init_seed = argv[3];
  
  //fork for running DISSCO
  pid_t dissco_pid = fork();

  int dissco_pipe[2];
  int pipe_stat = pipe2(dissco_pipe, O_CLOEXEC);

  if(pipe_stat == -1){
    perror("Pipe failed");
    exit(errno);
  }
  
  if(dissco_pid == 0){
    //in child to exec DISSCO

    //pipes for interacting with DISSCO (seed info/iterations)
    dup2(dissco_pipe[0], 0);
    close(dissco_pipe[1]);
    
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
    //send neccessary info to DISSCO to begin
    close(dissco_pipe[0]);
    dprintf(dissco_pipe[1], "1\n");
    dprintf(dissco_pipe[1], "%s\n", argv[4]);
    close(dissco_pipe[1]);

    int status;
    waitpid(dissco_pid, &status, 0);
    if(WIFEXITED(status))
      printf("DISSCO EXITED WITH STATUS %d\n", WEXITSTATUS(status));
    return 0;
  }
}
