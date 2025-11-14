#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/riscv.h"    // For PGSIZE
#include "kernel/sysinfo.h"  // For struct sysinfo
#include "kernel/syscall.h"  // For SYS_ defines (SYS_getpid, ...)

static int total_tests = 0;
static int failed_tests = 0;

void fail(char *msg) {
  printf(" [FAIL]\n");
  printf("   Error: %s\n", msg);
  failed_tests++;
}

void pass() {
  printf(" [OK]\n");
}

void sinfo(struct sysinfo *info) {
  if (sysinfo(info) < 0) {
    fail("sysinfo() call failed (returned -1)");
  }
}

// === sysinfo Tests ===

// Test 1: Checks if sysinfo() correctly handles bad pointers
void test_sysinfo_bad_pointer() {
  total_tests++;
  printf("Test: sysinfo(bad_ptr)...");
  
  if (sysinfo(0x0) != -1) {
    fail("sysinfo(NULL) should have returned -1");
    return;
  }
  if (sysinfo((struct sysinfo *)0x80000000) != -1) {
    fail("sysinfo(kernel_addr) should have returned -1");
    return;
  }
  
  pass();
}

// Test 2: Checks if sysinfo.freemem correctly reflects sbrk() allocations
void test_sysinfo_freemem() {
  total_tests++;
  printf("Test: sysinfo.freemem (with sbrk)...");
  
  struct sysinfo info;
  uint64 initial_free;
  
  sinfo(&info);
  initial_free = info.freemem;
  
  char *mem = sbrk(PGSIZE);
  if (mem == (char*)-1) {
    fail("sbrk(PGSIZE) failed");
    return;
  }
  
  sinfo(&info);
  if (info.freemem >= initial_free) {
    fail("freemem did not decrease after sbrk()");
    return;
  }
  
  if (sbrk(-PGSIZE) == (char*)-1) {
    fail("sbrk(-PGSIZE) failed");
    return;
  }
  
  sinfo(&info);
  if (info.freemem < initial_free - 100 || info.freemem > initial_free + 100) {
    fail("freemem did not return to initial value after sbrk(-PGSIZE)");
    return;
  }
  
  pass();
}

// Test 3: Checks if sysinfo.nproc correctly counts processes during fork/exit
void test_sysinfo_nproc() {
  total_tests++;
  printf("Test: sysinfo.nproc (with fork)...");
  
  struct sysinfo info;
  uint64 nproc;
  
  sinfo(&info);
  nproc = info.nproc;

  int pid = fork();
  if (pid < 0) {
    fail("fork() failed");
    return;
  }
  
  if (pid == 0) {
    // Child process
    sinfo(&info);
    if (info.nproc != nproc + 1) {
      printf("FAIL (from child): nproc is %ld instead of %ld\n", info.nproc, nproc + 1);
    }
    exit(0);
  }
  
  // Parent process
  wait(0);
  sinfo(&info);
  
  if (info.nproc != nproc) {
    fail("nproc did not return to initial value after child exit");
    return;
  }
  
  pass();
}

// Test 4: Checks if sysinfo.nopenfiles correctly counts open files
void test_sysinfo_nfile() {
  total_tests++;
  printf("Test: sysinfo.nopenfiles (with open/close)...");
  
  struct sysinfo info;
  uint64 nfile;
  
  sinfo(&info);
  nfile = info.nopenfiles;

  int fd = open("README", 0);
  if (fd < 0) {
    fail("Could not open README");
    return;
  }
  
  sinfo(&info);
  if (info.nopenfiles != nfile + 1) {
    fail("nfile did not increment after open()");
    return;
  }

  close(fd);
  sinfo(&info);
  
  if (info.nopenfiles != nfile) {
    fail("nopenfiles did not return to initial value after close()");
    return;
  }
  
  pass();
}


// === trace Tests (Visual Inspection) ===

void test_trace_visual() {
  printf("\n== Starting TRACE Visual Tests ==\n");
  printf("Note: The following tests require visual inspection of the output.\n\n");

  // 1. Test simple syscall trace
  printf("Test 1: Tracing 'getpid' (syscall %d).\n", SYS_getpid);
  printf("  -> YOU SHOULD SEE 1 trace line for 'getpid':\n");
  trace(1 << SYS_getpid);
  getpid();
  trace(0);

  // 2. Test trace mask
  printf("\nTest 2: Tracing 'read' (%d) & 'write' (%d). NOT tracing 'open' (%d).\n", SYS_read, SYS_write, SYS_open);
  printf("  -> YOU SHOULD SEE 'write' and 'read', BUT NOT 'open':\n");
  
  trace((1 << SYS_read) | (1 << SYS_write));
  int fd = open("README", 0); // <-- This should NOT be traced
  if (fd < 0) {
    printf("Error: open README failed for trace test.\n");
  } else {
    char buf[10];
    read(fd, buf, 10);      // <-- This MUST be traced
    close(fd);
  }
  write(1, "  (Test write syscall)\n", 23); // <-- This MUST be traced
  trace(0);

  // 3. Test mask inheritance on fork() and 'exit' tracing
  printf("\nTest 3: Tracing 'fork' (%d), 'exit' (%d), 'wait' (%d).\n", SYS_fork, SYS_exit, SYS_wait);
  printf("  -> YOU SHOULD SEE 'fork', 'exit' (from child), and 'wait' (from parent):\n");

  trace((1 << SYS_fork) | (1 << SYS_exit) | (1 << SYS_wait) | (1 << SYS_getpid));

  int pid = fork(); // <-- This MUST be traced
  if (pid == 0) {
    printf("  [Child PID %d] Running and exiting...\n", getpid());
    exit(0); // <-- This MUST be traced
  }
  
  wait(0); // <-- This MUST be traced
  printf("  [Parent] Child has exited.\n");
  trace(0);

  printf("\n== Finished TRACE Visual Tests ==\n");
}

// === Main test runner ===

int
main(int argc, char *argv[])
{
  printf("=== STARTING COMPREHENSIVE LAB 1 TEST ===\n\n");
  
  printf("--- Part 1: Automated sysinfo Tests ---\n");
  test_sysinfo_bad_pointer();
  test_sysinfo_freemem();
  test_sysinfo_nproc();
  test_sysinfo_nfile();
  
  printf("\n--- Part 2: Visual trace Tests ---\n");
  test_trace_visual();
  
  printf("\n--- SUMMARY ---\n");
  printf("Automated tests run: %d\n", total_tests);
  printf("Automated tests failed: %d\n", failed_tests);
  
  if (failed_tests > 0) {
    printf("\nRESULT: AUTOMATED TESTS FAILED.\n");
  } else {
    printf("\nRESULT: ALL AUTOMATED TESTS PASSED.\n");
    printf("(Please check carefully the output above!\n");
  }
  
  exit(0);
}