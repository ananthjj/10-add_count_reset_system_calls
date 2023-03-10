#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <string.h>

/*
 * Put your syscall number here.
 */
#define SYS_count_SC 335
#define SYS_retrieveCount_SC 336
#define SYS_resetCount_SC 337

struct count_SC_struct{
  int forkCount;
  int vforkCount;
  int execveCount;
  int cloneCount;
  int clone3Count;
};

int main(int argc, char **argv)
{
  if (argc <= 1) {
    printf("Must provide a string to choose reset or retrieve.\n");
    return -1;
  }
  char* arg = argv[1];
  long res;
  if (strcmp(arg,"-reset")) {
	printf("Making reset system call.\n");
  	res = syscall(SYS_resetCount_SC);
  }
else {
  if (strcmp(arg,"-retrieve")){
    struct count_SC_struct scC;
    printf("Making retrieve system call\n");
    res = syscall(SYS_retrieveCount_SC, &scC);
    printf("System call count values are as follow:\n");
    printf("fork: %d, vfork: %d, clone: %d, execve: %d, clone3: %d\n", scC.forkCount, scC.vforkCount, scC.cloneCount, scC.execveCount, scC.clone3Count);
    printf("System call returned %ld.\n", res);
  }
  else{
    printf("Making system call with \"%s\".\n", arg);
    res = syscall(SYS_count_SC, arg);
  }
  return res;
 }
}
