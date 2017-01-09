#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>

void puts_usage(){
  puts("You forgot something! Make sure to follow example");
  puts("./inst_handler <cmod file path> <cmod project path>");
}

char* path_strip(char* path){
  int len = strlen(path);
  char* ret_path = malloc(len);

  while(path[len] != '/'){
    len--;
  }
  int i = 0;
  for(; i <= len; i++){
    ret_path[i] = path[i];
  }
  ret_path[i] = 0;
  return ret_path;
}
int main(int argc, char* argv[]){
  if(argc != 3){
    puts_usage();
    exit(-1);
  }

  //parse arguments
  char* dissco_path = argv[1];
  char* dissco_proj = argv[2];
  
  //fork for running DISSCO
  pid_t dissco_pid = fork();
  
  if(dissco_pid == 0){
    //in child to exec DISSCO
    puts("Starting DISSCO....");
    //execlp(argv[1], dissco_path, dissco_proj, (char*)NULL);
  }

  else if(dissco_pid < 0){
    //fork failed
    perror("Fork failed!!");
    exit(errno);
  }

  else{
    //in parent
    //send neccessary info to DISSCO to begin
    int status;
    char* sound_path;
    sound_path = path_strip(dissco_proj);
    strcat(sound_path, "SoundFiles");
    puts(sound_path);

    DIR* sound_dir = opendir(sound_path); 
    struct dirent *entry;
    struct stat statbuffer;
    //add in whatever param for stopping the installation
    while(1 && sound_dir){
      printf("%s\n", sound_path);
      break;
    }

    waitpid(dissco_pid, &status, 0);
    if(WIFEXITED(status))
      printf("DISSCO EXITED WITH STATUS %d\n", WEXITSTATUS(status));
    return 0;
  }
}
