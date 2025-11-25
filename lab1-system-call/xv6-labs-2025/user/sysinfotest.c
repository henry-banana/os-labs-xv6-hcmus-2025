#include "kernel/types.h"
#include "kernel/riscv.h"
#include "kernel/sysinfo.h"
#include "user/user.h"

// Helper: call sysinfo and check for errors
void
sinfo(struct sysinfo *info) {
  if (sysinfo(info) < 0) {
    printf("FAIL: sysinfo failed\n");
    exit(1);
  }
}

// Test 1: Basic functionality - can we call sysinfo?
void
testcall() {
  struct sysinfo info;
  
  if (sysinfo(&info) < 0) {
    printf("FAIL: sysinfo failed\n");
    exit(1);
  }

  // Test with invalid pointer - should fail
  if (sysinfo((struct sysinfo *) 0xeaeb0b5b00002f5e) != 0xffffffffffffffff) {
    printf("FAIL: sysinfo succeeded with bad argument\n");
    exit(1);
  }
  
  printf("testcall: OK\n");
}

// Test 2: Free memory counting
void
testmem() {
  struct sysinfo info;
  uint64 initial_free;
  
  sinfo(&info);
  initial_free = info.freemem;
  
  // Allocate one page
  if((uint64)sbrk(PGSIZE) == 0xffffffffffffffff){
    printf("sbrk failed\n");
    exit(1);
  }

  sinfo(&info);
  
  // Free memory should decrease by PGSIZE
  if (info.freemem >= initial_free) {
    printf("FAIL: free mem didn't decrease after sbrk\n");
    exit(1);
  }
  
  // Free the page
  if((uint64)sbrk(-PGSIZE) == 0xffffffffffffffff){
    printf("sbrk failed\n");
    exit(1);
  }

  sinfo(&info);
  
  // Should return to initial value (approximately)
  printf("testmem: Free memory = %ld bytes\n", info.freemem);
}

// Test 3: Process counting
void
testproc() {
  struct sysinfo info;
  uint64 nproc;
  int status;
  int pid;
  
  sinfo(&info);
  nproc = info.nproc;

  pid = fork();
  if(pid < 0){
    printf("fork failed\n");
    exit(1);
  }
  
  if(pid == 0){
    // Child: check process count increased
    sinfo(&info);
    if(info.nproc != nproc + 1) {
      printf("FAIL: nproc is %ld instead of %ld\n", info.nproc, nproc+1);
      exit(1);
    }
    exit(0);
  }
  
  // Parent: wait for child
  wait(&status);
  sinfo(&info);
  
  // After child exits, should return to original count
  if(info.nproc != nproc) {
    printf("FAIL: nproc is %ld instead of %ld\n", info.nproc, nproc);
    exit(1);
  }

  printf("testproc: nproc = %ld\n", info.nproc);
}

// Test 4: File descriptor counting
void
testfile() {
  struct sysinfo info;
  uint64 nfile;
  int fd;
  
  sinfo(&info);
  nfile = info.nopenfiles;

  // Open a file
  fd = open("README", 0);
  if(fd < 0){
    printf("FAIL: cannot open file\n");
    exit(1);
  }

  // Check file count increased
  sinfo(&info);
  if(info.nopenfiles != nfile + 1) {
    printf("FAIL: nfile is %ld instead of %ld\n", info.nopenfiles, nfile + 1);
    exit(1);
  }

  // Close file
  close(fd);
  sinfo(&info);
  
  // Should return to original count
  if(info.nopenfiles != nfile) {
    printf("FAIL: nfile is %ld instead of %ld\n", info.nopenfiles, nfile);
    exit(1);
  }

  printf("testfile: nfile = %ld\n", info.nopenfiles);
}

int
main(int argc, char *argv[])
{
  printf("sysinfotest: start\n");
  
  testcall();
  testmem();
  testproc();
  testfile();

  printf("sysinfotest: OK\n"); 
  exit(0);
}