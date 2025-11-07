#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  printf("Running testhelpers syscall...\n");
  testhelpers();
  exit(0);
}
