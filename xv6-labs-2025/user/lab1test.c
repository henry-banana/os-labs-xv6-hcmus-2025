#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/riscv.h"    // For PGSIZE
#include "kernel/sysinfo.h"  // For struct sysinfo
#include "kernel/syscall.h"  // For SYS_ defines (SYS_getpid, ...)
#include "kernel/fcntl.h"     // For O_RDONLY

int failed_tests = 0;
int total_tests = 0;

int check_u64(char* description, uint64 actual, uint64 expected) {
    printf("  -> Check: %s\n", description);
    printf("     - Expected: %ld\n", (unsigned long) expected);
    printf("     - Actual:   %ld\n", (unsigned long)  actual);
    if (actual == expected) {
        printf("     - Result:  [OK]\n");
        return 1;
    } else {
        printf("     - Result:  [FAIL]\n");
        return 0;
    }
}

// Function to check two int values
int check_int(char* description, int actual, int expected) {
    printf("  -> Check: %s\n", description);
    printf("     - Expected: %ld\n", (unsigned long) expected);
    printf("     - Actual:   %ld\n", (unsigned long) actual);
    if (actual == expected) {
        printf("     - Result:  [OK]\n");
        return 1;
    } else {
        printf("     - Result:  [FAIL]\n");
        return 0;
    }
}

// Function to check a condition is true
int check_true(char* description, int condition) {
    printf("  -> Check: %s\n", description);
    printf("     - Expected: true (non-zero)\n");
    printf("     - Actual:   %s\n", condition ? "true" : "false");
    if (condition) {
        printf("     - Result:  [OK]\n");
        return 1;
    } else {
        printf("     - Result:  [FAIL]\n");
        return 0;
    }
}


// Helper function to call sysinfo and handle basic errors
void sinfo(struct sysinfo *info) {
  if (sysinfo(info) < 0) {
    printf("   CRITICAL ERROR: sysinfo() call failed. Stopping test.\n");
    exit(1); 
  }
}

// Function to check A < B for uint64
int check_less_than_u64(char* description, uint64 val_a, uint64 val_b) {
    printf("  -> Check: %s\n", description);
    printf("     - Expected: Actual value < Threshold value \n");
    printf("     - Actual:   %ld < %ld\n", (unsigned long) val_a, (unsigned long) val_b);
    if (val_a < val_b) {
        printf("     - Result:  [OK]\n");
        return 1;
    } else {
        printf("     - Result:  [FAIL]\n");
        return 0;
    }
}

// Function to check A approximately equals B (within tolerance) for uint64
int check_approx_eq_u64(char* description, uint64 actual, uint64 expected, uint64 tolerance) {
    printf("  -> Check: %s\n", description);
    uint64 lower_bound = expected - tolerance;
    uint64 upper_bound = expected + tolerance;
    printf("     - Expected:  Within range [%ld, %ld]\n", (unsigned long) lower_bound, (unsigned long) upper_bound);
    printf("     - Actual:   %ld\n", (unsigned long) actual);
    if (actual >= lower_bound && actual <= upper_bound) {
        printf("     - Result:  [OK]\n");
        return 1;
    } else {
        printf("     - Result:  [FAIL]\n");
        return 0;
    }
}

// ==================================================
// ======= sysinfo Tests ============================
// ==================================================

void test_sysinfo_bad_pointer() {
  total_tests++;
  printf("\n\nTest: sysinfo() with invalid pointer...\n");
  int pass = 1;

  if(!check_int("sysinfo(NULL) should return -1", sysinfo(0x0), -1)) 
    pass = 0;
  
  if(!check_int("sysinfo(kernel_addr) should return -1", sysinfo((struct sysinfo *)0x80000000), -1))
  pass = 0;

  if(!pass) failed_tests++;

  printf("... End of invalid pointer test: %s\n", pass ? "[SUCCESS]" : "[FAILED]");
}

void test_sysinfo_freemem() {
  total_tests++;
  printf("\n\nTest: sysinfo.freemem with sbrk()...\n");
  int pass = 1;

  struct sysinfo info;
  sinfo(&info);
  uint64 initial_free = info.freemem;
  printf("  - Initial free memory: %ld\n", (unsigned long)initial_free);

  char *mem = sbrk(PGSIZE);
  if (!check_true("sbrk(PGSIZE) must succeed", mem != (char*)-1)) {
    failed_tests++;
    printf("... End of freemem test: [FAILED]\n");
    return;
  }
  
  sinfo(&info);
  uint64 free_after_alloc = info.freemem;
  printf("  - Free after alloc: %ld (delta: -%ld)\n",
         (unsigned long)free_after_alloc,
         (unsigned long)(initial_free - free_after_alloc));
  if (!check_less_than_u64("Free memory must decrease after allocation", free_after_alloc, initial_free)) {
    pass = 0;
  }

  sbrk(-PGSIZE);
  sinfo(&info);
  uint64 final_free = info.freemem;
  printf("  - Final free memory: %ld (delta from initial: %ld)\n",
         (unsigned long)final_free,
         (unsigned long)(final_free - initial_free));
  
  if (!check_approx_eq_u64("Free memory must return close to initial value", final_free, initial_free, PGSIZE)) {
    pass = 0;
  }

  if(!pass) failed_tests++;
  printf("... End of freemem test: %s\n", pass ? "[SUCCESS]" : "[FAILED]");
}

void test_sysinfo_nproc() {
  total_tests++;
  printf("\n\nTest: sysinfo.nproc with fork()...\n");
  int pass = 1;

  struct sysinfo info;
  sinfo(&info);
  uint64 nproc_before = info.nproc;
  printf("  - Initial process count: %ld\n", (unsigned long)nproc_before);

  int pid = fork();
  if (pid < 0) {
      printf("  -> Check: fork() must succeed\n");
      printf("     - Result: [FAIL] - fork() returned negative value.\n");
      failed_tests++;
      printf("... End of nproc test: [FAILED]\n");
      return;
  }
  
  if (pid == 0) {
    // === CHILD PROCESS ===
    // Child only checks and exits, doesn't print much to avoid race condition
    sinfo(&info);
    if(info.nproc == nproc_before + 1) {
      exit(0);
    } else {
      exit(1);
    }
  }
  
  // === PARENT PROCESS ===
  // Parent waits for child to finish before analyzing results
  int status;
  int waited = wait(&status); 
  if (!check_int("wait() returned child pid", waited, pid)) {
    pass = 0;
  }
  
  printf("  -> Check: fork() successfully created new process.\n");
  printf("     - Result: [OK]\n");

  // Check child exit code
  if (!check_int("Child process must exit with code 0 (indicating correct nproc)", status, 0)) {
    pass = 0;
  }

  sinfo(&info);
  if(!check_u64("nproc returns to initial value after child exits", info.nproc, nproc_before)) {
    pass = 0;
  }
  
  if(!pass) {
      failed_tests++;
  }
  printf("... End of nproc test: %s\n", pass ? "[SUCCESS]" : "[FAILED]");
}

void test_sysinfo_nfile() {
  total_tests++;
  printf("\n\nTest: sysinfo.nopenfiles with open()/close()...\n");
  int pass = 1;
  
  struct sysinfo info;
  sinfo(&info);
  uint64 nfile_before = info.nopenfiles;
  printf("  - Initial open files count: %ld\n", (unsigned long)nfile_before);

  int fd = open("README", O_RDONLY);
  if (!check_true("Opening README file must succeed", fd >= 0)) {
    failed_tests++;
    printf("... End of nfile test: [FAILED]\n");
    return;
  }
  
  sinfo(&info);
  if(!check_u64("nopenfiles must increase by 1 after open()", info.nopenfiles, nfile_before + 1))
    pass = 0;

  close(fd);
  sinfo(&info);
  
  if(!check_u64("nopenfiles must return to initial value after close()", info.nopenfiles, nfile_before))
    pass = 0;
  
  if(!pass) failed_tests++;
  printf("... End of nfile test: %s\n", pass ? "[SUCCESS]" : "[FAILED]");
}

// === trace Tests ===

void test_trace_visual() {
  printf("\n== Start TRACE Test (Visual Inspection) ==\n");
  printf("Note: The following tests require you to manually check the output.\n\n");

  // 1. Simple syscall trace test
  printf("Test 1: Trace syscall 'getpid' (syscall %ld).\n", (unsigned long)SYS_getpid);
  printf("  -> YOU SHOULD SEE 1 trace line for 'getpid':\n");
  trace(1 << SYS_getpid);
  getpid();
  trace(0);

  // 2. Test trace mask
  printf("\nTest 2: Trace 'read' (%ld) & 'write' (%ld). DO NOT trace 'open' (%ld).\n", (unsigned long)SYS_read, (unsigned long)SYS_write, (unsigned long)SYS_open);
  printf("  -> YOU SHOULD SEE 'write' and 'read', BUT NOT 'open':\n");
  
  trace((1 << SYS_read) | (1 << SYS_write));
  int fd = open("README", 0); 
  if (fd < 0) {
    printf("Error: cannot open README for trace test.\n");
  } else {
    char buf[10];
    read(fd, buf, 10);    
    close(fd);
  }
  write(1, "  (Test syscall write)\n", 23); 
  trace(0);

  // 3. Test mask inheritance via fork() and trace 'exit'
  printf("\nTest 3: Trace 'fork' (%ld), 'exit' (%ld), 'wait' (%ld).\n", (unsigned long)SYS_fork, (unsigned long)SYS_exit, (unsigned long)SYS_wait);
  printf("  -> YOU SHOULD SEE 'fork', 'exit' (from child), and 'wait' (from parent):\n");

  trace((1 << SYS_fork) | (1 << SYS_exit) | (1 << SYS_wait) | (1 << SYS_getpid));

  int pid = fork(); 
  if (pid == 0) {
    printf("  [Child PID %ld] Running and will exit...\n", (unsigned long)getpid());
    exit(0); 
  }
  
  wait(0); 
  printf("  [Parent] Child process has exited.\n");
  trace(0);

  printf("\n== End TRACE Test ==\n");
}

// === Main test runner ===

int
main(int argc, char *argv[])
{
  total_tests = 0;
  failed_tests = 0;

  printf("=== START COMPREHENSIVE TEST SUITE FOR LAB 1 ===\n\n");
  
  printf("--- Part 1: Automated sysinfo tests ---\n");
  test_sysinfo_bad_pointer();
  test_sysinfo_freemem();
  test_sysinfo_nproc();
  test_sysinfo_nfile();
  
  printf("\n--- Part 2: Visual trace tests ---\n");
  test_trace_visual();
  
  printf("\n--- SUMMARY ---\n");
  printf("Total automated tests run: %ld\n", (unsigned long)total_tests);
  printf("Total automated tests failed: %ld\n", (unsigned long)failed_tests);
  
  if (failed_tests > 0) {
    printf("\nRESULT: SOME AUTOMATED TESTS FAILED.\n");
  } else {
    printf("\nRESULT: ALL AUTOMATED TESTS PASSED.\n");
    printf("(Please carefully check the trace test output above!)\n");
  }
  
  exit(0);
}