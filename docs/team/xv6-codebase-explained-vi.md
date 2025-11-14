# Hướng Dẫn Hiểu Cấu Trúc và Logic của xv6-labs-2025

## 🎯 Mục Đích Tài Liệu

Tài liệu này giải thích chi tiết **cấu trúc thư mục**, **vai trò của từng file**, và **logic hoạt động** của các thành phần chính trong xv6. Mục tiêu là giúp developer hiểu sâu codebase để tự tin implement và debug Lab 1 (System Calls).

---

## 📂 Tổng Quan Cấu Trúc Thư Mục

```
xv6-labs-2025/
├── kernel/          # Kernel source code (chạy ở kernel space)
├── user/            # User programs (chạy ở user space)
├── mkfs/            # File system image creator
├── conf/            # Build configuration
├── Makefile         # Build system
├── README           # Basic documentation
└── grade-lab-syscall # Auto-grading script
```

---

## 🔧 Phần 1: Thư Mục `kernel/` - Kernel Source Code

### 1.1. System Call Infrastructure

#### `kernel/syscall.h`
**Vai trò**: Định nghĩa **system call numbers**

```c
#define SYS_fork    1
#define SYS_exit    2
#define SYS_wait    3
#define SYS_pipe    4
#define SYS_read    5
...
#define SYS_close   21
```

**Logic**:
- Mỗi syscall có một số hiệu duy nhất (từ 1 đến N)
- User space load số này vào thanh ghi `a7` trước `ecall`
- Kernel dùng số này để dispatch đến handler tương ứng

**Khi thêm syscall mới**:
```c
#define SYS_trace   22  // ADD THIS
#define SYS_sysinfo 23  // ADD THIS
```

---

#### `kernel/syscall.c`
**Vai trò**: **Dispatcher** - Route syscall number → handler function

**Cấu trúc quan trọng**:

1. **Function prototypes**:
```c
extern uint64 sys_fork(void);
extern uint64 sys_exit(void);
extern uint64 sys_wait(void);
...
```

2. **Syscall table** (function pointer array):
```c
static uint64 (*syscalls[])(void) = {
[SYS_fork]    sys_fork,
[SYS_exit]    sys_exit,
[SYS_wait]    sys_wait,
[SYS_pipe]    sys_pipe,
[SYS_read]    sys_read,
...
[SYS_close]   sys_close,
};
```

3. **Main dispatcher function**:
```c
void syscall(void)
{
  int num;
  struct proc *p = myproc();  // Get current process

  num = p->trapframe->a7;  // Extract syscall number từ register a7
  
  if(num > 0 && num < NELEM(syscalls) && syscalls[num]) {
    // Call handler và lưu return value vào a0
    p->trapframe->a0 = syscalls[num]();
  } else {
    // Unknown syscall
    printf("%d %s: unknown sys call %d\n", p->pid, p->name, num);
    p->trapframe->a0 = -1;
  }
}
```

**Logic flow**:
```
usertrap() → syscall() → syscalls[num]() → return value → trapframe->a0
```

**Helper functions để extract arguments**:

```c
// Lấy argument thứ n (int)
void argint(int n, int *ip)
{
  *ip = argraw(n);
}

// argraw() đọc từ trapframe
static uint64 argraw(int n)
{
  struct proc *p = myproc();
  switch (n) {
  case 0: return p->trapframe->a0;
  case 1: return p->trapframe->a1;
  case 2: return p->trapframe->a2;
  ...
  }
}

// Lấy argument thứ n (pointer/address)
void argaddr(int n, uint64 *ip)
{
  *ip = argraw(n);
}

// Lấy argument thứ n (string)
int argstr(int n, char *buf, int max)
{
  uint64 addr;
  argaddr(n, &addr);
  return fetchstr(addr, buf, max);  // Copy string từ user space
}
```

**Khi thêm syscall mới**:
1. Add prototype: `extern uint64 sys_trace(void);`
2. Add to table: `[SYS_trace] sys_trace,`
3. (Optional) Add name for tracing: `[SYS_trace] "trace",`

---

#### `kernel/sysproc.c`
**Vai trò**: Implement **process-related syscall handlers**

**Examples**:

```c
// Get process ID
uint64 sys_getpid(void)
{
  return myproc()->pid;  // Simple: just return pid
}

// Exit process
uint64 sys_exit(void)
{
  int n;
  argint(0, &n);  // Extract exit status từ a0
  kexit(n);
  return 0;  // not reached
}

// Fork process
uint64 sys_fork(void)
{
  return kfork();  // Gọi kernel fork implementation
}

// Sleep (pause)
uint64 sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);  // Extract sleep duration
  if(n < 0)
    n = 0;
  
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){  // Sleep for n ticks
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}
```

**Pattern chung**:
1. Extract arguments bằng `argint()`, `argaddr()`, `argstr()`
2. Validate arguments (nếu cần)
3. Call kernel function để thực hiện logic
4. Return kết quả (hoặc error code)

**Khi implement syscall mới**:
```c
uint64 sys_trace(void)
{
  int mask;
  argint(0, &mask);  // Step 1: Extract argument
  myproc()->trace_mask = mask;  // Step 2: Perform action
  return 0;  // Step 3: Return success
}
```

---

#### `kernel/sysfile.c`
**Vai trò**: Implement **file-related syscall handlers**

**Examples**:

```c
// Open file
uint64 sys_open(void)
{
  char path[MAXPATH];
  int fd, omode;
  struct file *f;
  struct inode *ip;

  argint(1, &omode);  // Extract open mode
  if(argstr(0, path, MAXPATH) < 0)  // Extract path
    return -1;

  // ... logic to open file ...
  
  return fd;  // Return file descriptor
}

// Read from file
uint64 sys_read(void)
{
  struct file *f;
  int n;
  uint64 p;

  argaddr(1, &p);  // Buffer address
  argint(2, &n);   // Number of bytes
  if(argfd(0, 0, &f) < 0)  // Get file from fd
    return -1;
  
  return fileread(f, p, n);  // Perform read
}
```

---

### 1.2. Process Management

#### `kernel/proc.h`
**Vai trò**: Định nghĩa **data structures** cho process management

**Cấu trúc quan trọng**:

```c
// Process state
enum procstate { UNUSED, USED, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

// Per-process state
struct proc {
  struct spinlock lock;        // Lock để bảo vệ process state

  // p->lock must be held when using these:
  enum procstate state;        // Process state
  void *chan;                  // Sleep channel (nếu SLEEPING)
  int killed;                  // Flag: process bị kill?
  int xstate;                  // Exit status
  int pid;                     // Process ID

  // wait_lock must be held when using this:
  struct proc *parent;         // Parent process

  // Private to process (không cần lock):
  uint64 kstack;               // Kernel stack (virtual address)
  uint64 sz;                   // Size of process memory (bytes)
  pagetable_t pagetable;       // User page table
  struct trapframe *trapframe; // Saved user registers
  struct context context;      // Switch context (kernel registers)
  struct file *ofile[NOFILE];  // Open files
  struct inode *cwd;           // Current directory
  char name[16];               // Process name (for debugging)
  
  // ADD FOR LAB1:
  int trace_mask;              // Syscall trace mask
};
```

**Các trường quan trọng giải thích**:

- **`state`**: Trạng thái của process
  - `UNUSED`: Slot trống trong process table
  - `USED`: Process đã allocate nhưng chưa sẵn sàng run
  - `RUNNABLE`: Sẵn sàng chạy (trong run queue)
  - `RUNNING`: Đang chạy trên CPU
  - `SLEEPING`: Đang chờ event (I/O, lock, ...)
  - `ZOMBIE`: Đã exit nhưng chưa được parent thu hồi

- **`trapframe`**: Lưu **user registers** khi trap vào kernel
  ```c
  struct trapframe {
    uint64 kernel_satp;   // Kernel page table
    uint64 kernel_sp;     // Kernel stack pointer
    uint64 kernel_trap;   // Address of usertrap()
    uint64 epc;           // Saved user PC
    uint64 ra, sp, gp, tp, t0-t6, s0-s11, a0-a7;  // All registers
  };
  ```

- **`context`**: Lưu **kernel registers** khi context switch
  ```c
  struct context {
    uint64 ra;  // Return address
    uint64 sp;  // Stack pointer
    uint64 s0-s11;  // Callee-saved registers
  };
  ```

- **`ofile[NOFILE]`**: Array of open file pointers (file descriptor table)
  - `ofile[0]` = stdin
  - `ofile[1]` = stdout
  - `ofile[2]` = stderr

---

#### `kernel/proc.c`
**Vai trò**: Implement **process management functions**

**Quan trọng functions**:

1. **`myproc()`** - Lấy process hiện tại:
```c
struct proc* myproc(void)
{
  push_off();  // Disable interrupts
  struct cpu *c = mycpu();
  struct proc *p = c->proc;  // Process đang chạy trên CPU này
  pop_off();   // Enable interrupts
  return p;
}
```

2. **`kfork()`** - Tạo child process:
```c
int kfork(void)
{
  int i, pid;
  struct proc *np;  // New process
  struct proc *p = myproc();  // Parent process

  // Allocate process
  if((np = allocproc()) == 0){
    return -1;
  }

  // Copy user memory from parent to child
  if(uvmcopy(p->pagetable, np->pagetable, p->sz) < 0){
    freeproc(np);
    release(&np->lock);
    return -1;
  }
  np->sz = p->sz;

  // Copy trace_mask (FOR LAB1)
  np->trace_mask = p->trace_mask;

  // Copy open files
  for(i = 0; i < NOFILE; i++)
    if(p->ofile[i])
      np->ofile[i] = filedup(p->ofile[i]);
  np->cwd = idup(p->cwd);

  safestrcpy(np->name, p->name, sizeof(p->name));

  pid = np->pid;

  release(&np->lock);

  acquire(&wait_lock);
  np->parent = p;
  release(&wait_lock);

  acquire(&np->lock);
  np->state = RUNNABLE;  // Make child runnable
  release(&np->lock);

  return pid;  // Return child PID to parent
}
```

**Logic**: Child là bản sao chính xác của parent (memory, files, state)

3. **`kexit()`** - Terminate process:
```c
void kexit(int status)
{
  struct proc *p = myproc();

  // Close all open files
  for(int fd = 0; fd < NOFILE; fd++){
    if(p->ofile[fd]){
      struct file *f = p->ofile[fd];
      fileclose(f);
      p->ofile[fd] = 0;
    }
  }

  // ... cleanup ...

  p->xstate = status;
  p->state = ZOMBIE;  // Change to zombie state

  // Wake up parent (waiting in wait())
  wakeup(p->parent);

  // Scheduler will switch to another process
  sched();
  panic("zombie exit");
}
```

4. **`kwait()`** - Wait for child to exit:
```c
int kwait(uint64 addr)
{
  struct proc *pp;
  int havekids, pid;
  struct proc *p = myproc();

  acquire(&wait_lock);

  for(;;){
    // Scan through table looking for exited children
    havekids = 0;
    for(pp = proc; pp < &proc[NPROC]; pp++){
      if(pp->parent == p){
        havekids = 1;
        if(pp->state == ZOMBIE){
          // Found one
          pid = pp->pid;
          // ... copy exit status to user ...
          freeproc(pp);
          release(&wait_lock);
          return pid;
        }
      }
    }

    // No exited child found, sleep
    if(!havekids || killed(p)){
      release(&wait_lock);
      return -1;
    }
    
    sleep(p, &wait_lock);  // Wait for child to exit
  }
}
```

**Helper function để count processes (FOR LAB1)**:
```c
uint64 count_process(void)
{
  struct proc *p;
  uint64 n = 0;
  
  for(p = proc; p < &proc[NPROC]; p++) {
    acquire(&p->lock);
    if(p->state != UNUSED) {
      n++;
    }
    release(&p->lock);
  }
  
  return n;
}
```

---

### 1.3. Memory Management

#### `kernel/kalloc.c`
**Vai trò**: **Physical memory allocator**

**Data structure**:
```c
struct run {
  struct run *next;  // Linked list of free pages
};

struct {
  struct spinlock lock;
  struct run *freelist;  // Head of free page list
} kmem;
```

**Visualization**:
```
kmem.freelist → [Page 1] → [Page 2] → [Page 3] → NULL
                  4096B      4096B      4096B
```

**Functions**:

1. **`kalloc()`** - Allocate one page:
```c
void * kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;  // Remove from free list
  release(&kmem.lock);

  if(r)
    memset((char*)r, 5, PGSIZE); // Fill with junk (for debugging)
  
  return (void*)r;  // Return physical address of page
}
```

2. **`kfree()`** - Free one page:
```c
void kfree(void *pa)
{
  struct run *r;

  // Validate address
  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs
  memset(pa, 1, PGSIZE);
  
  r = (struct run*)pa;

  acquire(&kmem.lock);
  r->next = kmem.freelist;  // Add to front of free list
  kmem.freelist = r;
  release(&kmem.lock);
}
```

**Helper function để count free memory (FOR LAB1)**:
```c
uint64 kfreemem(void)
{
  struct run *r;
  uint64 n = 0;
  
  acquire(&kmem.lock);
  for(r = kmem.freelist; r; r = r->next) {
    n += PGSIZE;  // Count each free page
  }
  release(&kmem.lock);
  
  return n;  // Total free memory in bytes
}
```

---

#### `kernel/vm.c`
**Vai trò**: **Virtual memory management**

**Key concepts**:
- **Page table**: Map virtual address → physical address
- **3-level page table** trên RISC-V (SV39 mode)
- **Kernel page table**: Shared giữa tất cả processes
- **User page table**: Mỗi process có page table riêng

**Important functions**:

1. **`walk()`** - Traverse page table:
```c
pte_t * walk(pagetable_t pagetable, uint64 va, int alloc)
{
  // Walk 3-level page table to find PTE for va
  // If alloc=1, create intermediate page tables if needed
  ...
}
```

2. **`mappages()`** - Map virtual → physical:
```c
int mappages(pagetable_t pagetable, uint64 va, uint64 size, uint64 pa, int perm)
{
  // Create mapping from va to pa with permissions perm
  ...
}
```

3. **`copyout()`** - Copy kernel → user space:
```c
int copyout(pagetable_t pagetable, uint64 dstva, char *src, uint64 len)
{
  uint64 n, va0, pa0;

  while(len > 0){
    va0 = PGROUNDDOWN(dstva);
    pa0 = walkaddr(pagetable, va0);  // Translate VA to PA
    if(pa0 == 0)
      return -1;  // Invalid address
    
    n = PGSIZE - (dstva - va0);
    if(n > len)
      n = len;
    
    memmove((void *)(pa0 + (dstva - va0)), src, n);  // Copy data

    len -= n;
    src += n;
    dstva = va0 + PGSIZE;
  }
  return 0;
}
```

**Logic**: Translate từng page của user address, sau đó copy data vào physical address tương ứng.

---

### 1.4. File System

#### `kernel/file.h` & `kernel/file.c`
**Vai trò**: **File descriptor layer**

**Data structures**:
```c
struct file {
  enum { FD_NONE, FD_PIPE, FD_INODE, FD_DEVICE } type;
  int ref;  // Reference count
  char readable;
  char writable;
  struct pipe *pipe;  // FD_PIPE
  struct inode *ip;   // FD_INODE and FD_DEVICE
  uint off;           // FD_INODE
  short major;        // FD_DEVICE
};

// Global file table
struct {
  struct spinlock lock;
  struct file file[NFILE];
} ftable;
```

**Functions**:

1. **`filealloc()`** - Allocate a file structure:
```c
struct file* filealloc(void)
{
  struct file *f;

  acquire(&ftable.lock);
  for(f = ftable.file; f < ftable.file + NFILE; f++){
    if(f->ref == 0){  // Find free slot
      f->ref = 1;
      release(&ftable.lock);
      return f;
    }
  }
  release(&ftable.lock);
  return 0;  // No free slot
}
```

2. **`fileclose()`** - Close a file:
```c
void fileclose(struct file *f)
{
  struct file ff;

  acquire(&ftable.lock);
  if(f->ref < 1)
    panic("fileclose");
  if(--f->ref > 0){  // Decrement ref count
    release(&ftable.lock);
    return;  // Still has references
  }
  ff = *f;
  f->ref = 0;
  f->type = FD_NONE;
  release(&ftable.lock);

  // Actually close the file (release inode, etc.)
  if(ff.type == FD_PIPE){
    pipeclose(ff.pipe, ff.writable);
  } else if(ff.type == FD_INODE || ff.type == FD_DEVICE){
    begin_op();
    iput(ff.ip);
    end_op();
  }
}
```

**Helper function để count open files (FOR LAB1)**:
```c
uint64 count_files(void)
{
  struct file *f;
  uint64 n = 0;
  
  acquire(&ftable.lock);
  for(f = ftable.file; f < ftable.file + NFILE; f++) {
    if(f->ref > 0) {  // File is open
      n++;
    }
  }
  release(&ftable.lock);
  
  return n;
}
```

---

### 1.5. Trap Handling

#### `kernel/trap.c`
**Vai trò**: **Handle traps** (exceptions, interrupts, syscalls)

**Key function**:

```c
void usertrap(void)
{
  int which_dev = 0;

  // Change to kernel page table
  w_stvec((uint64)kernelvec);

  struct proc *p = myproc();
  
  // Save user PC
  p->trapframe->epc = r_sepc();

  if(r_scause() == 8){  // System call
    if(killed(p))
      exit(-1);

    // Increment PC (để skip qua ecall instruction)
    p->trapframe->epc += 4;

    // Enable interrupts
    intr_on();

    // Dispatch syscall
    syscall();
  } else if((which_dev = devintr()) != 0){
    // Interrupt from device
  } else {
    // Exception (page fault, illegal instruction, etc.)
    printf("usertrap(): unexpected scause %p pid=%d\n", r_scause(), p->pid);
    printf("            sepc=%p stval=%p\n", r_sepc(), r_stval());
    setkilled(p);
  }

  if(killed(p))
    exit(-1);

  // Give up CPU if this is a timer interrupt
  if(which_dev == 2)
    yield();

  usertrapret();
}
```

**Logic flow**:
```
User program executes ecall
  → CPU traps to kernel
  → uservec (trampoline.S) saves registers
  → usertrap() determines trap cause
  → If syscall: syscall()
  → usertrapret() returns to user
  → userret (trampoline.S) restores registers
  → sret returns to user mode
```

---

## 👥 Phần 2: Thư Mục `user/` - User Programs

### 2.1. User-Space System Call Interface

#### `user/user.h`
**Vai trò**: **User-space API declarations**

```c
// System call prototypes
int fork(void);
int exit(int) __attribute__((noreturn));
int wait(int*);
int pipe(int*);
int write(int, const void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(const char*, char**);
int open(const char*, int);
int mknod(const char*, short, short);
int unlink(const char*);
...

// ADD FOR LAB1:
int trace(int);
struct sysinfo;
int sysinfo(struct sysinfo *);

// Library functions
int printf(const char*, ...);
void* malloc(uint);
void free(void*);
...
```

---

#### `user/usys.pl`
**Vai trò**: **Generate assembly stubs** cho syscalls

**Perl script**:
```perl
sub entry {
    my $name = shift;
    print ".global $name\n";
    print "$name:\n";
    print " li a7, SYS_${name}\n";  # Load syscall number vào a7
    print " ecall\n";                # Trap vào kernel
    print " ret\n";                  # Return
}

entry("fork");
entry("exit");
...
entry("trace");      # ADD FOR LAB1
entry("sysinfo");    # ADD FOR LAB1
```

**Generated `usys.S`**:
```asm
.global fork
fork:
 li a7, SYS_fork
 ecall
 ret

.global trace
trace:
 li a7, SYS_trace
 ecall
 ret
```

---

### 2.2. User Programs

#### `user/trace.c` (FOR LAB1)
**Vai trò**: User program to trace syscalls

```c
#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  int i;
  char *nargv[MAXARG];

  if(argc < 3 || (argv[1][0] < '0' || argv[1][0] > '9')){
    fprintf(2, "Usage: trace mask command [args...]\n");
    exit(1);
  }

  // Call trace syscall
  if (trace(atoi(argv[1])) < 0) {
    fprintf(2, "%s: trace failed\n", argv[0]);
    exit(1);
  }

  // Prepare arguments for exec
  for(i = 2; i < argc && i < MAXARG; i++){
    nargv[i-2] = argv[i];
  }
  nargv[i-2] = 0;

  // Execute command (will inherit trace_mask)
  exec(nargv[0], nargv);
  
  fprintf(2, "%s: exec %s failed\n", argv[0], nargv[0]);
  exit(1);
}
```

**Example usage**:
```bash
$ trace 32 grep hello README
# 32 = (1 << 5) = trace SYS_read
```

---

#### `user/sysinfotest.c` (FOR LAB1)
**Vai trò**: Test program for sysinfo syscall

```c
#include "kernel/types.h"
#include "kernel/riscv.h"
#include "kernel/sysinfo.h"
#include "user/user.h"

void testcall() {
  struct sysinfo info;
  
  if (sysinfo(&info) < 0) {
    printf("FAIL: sysinfo failed\n");
    exit(1);
  }

  printf("freemem: %d\n", info.freemem);
  printf("nproc: %d\n", info.nproc);
  printf("nopenfiles: %d\n", info.nopenfiles);
}

int main(int argc, char *argv[])
{
  printf("sysinfotest: start\n");
  testcall();
  printf("sysinfotest: OK\n");
  exit(0);
}
```

---

## 🛠️ Phần 3: Build System

### `Makefile`
**Vai trò**: Build kernel và user programs

**Important targets**:

```makefile
# Build everything
all: kernel fs.img

# Build kernel
kernel: $(OBJS) kernel/kernel.ld
	$(LD) $(LDFLAGS) -T kernel/kernel.ld -o kernel/kernel $(OBJS)

# User programs
UPROGS=\
	$U/_cat\
	$U/_echo\
	...
	$U/_trace\        # ADD FOR LAB1
	$U/_sysinfotest\  # ADD FOR LAB1

# Run in QEMU
qemu: all
	$(QEMU) $(QEMUOPTS)

# Run with GDB server
qemu-gdb: all
	$(QEMU) $(QEMUOPTS) -gdb tcp::26000 -S

# Auto-grading
grade:
	./grade-lab-syscall
```

---

## 🎓 Phần 4: Key Concepts Summary

### 4.1. System Call Flow (Chi Tiết)

```
User Space (user/trace.c):
  trace(32);
    ↓
  Compiler generates:
    li a0, 32         # Load argument
    call trace        # Call stub
    ↓
User Space (usys.S):
  trace:
    li a7, SYS_trace  # a7 = 22
    ecall             # Trap to kernel
    ↓
Kernel (trampoline.S):
  uservec:
    save all registers to trapframe
    load kernel page table
    jump to usertrap()
    ↓
Kernel (trap.c):
  usertrap():
    identify trap cause = syscall
    call syscall()
    ↓
Kernel (syscall.c):
  syscall():
    num = p->trapframe->a7  # Read a7 = 22
    syscalls[22]()          # Call sys_trace()
    ↓
Kernel (sysproc.c):
  sys_trace():
    argint(0, &mask)        # Read a0 = 32
    myproc()->trace_mask = 32
    return 0
    ↓
Kernel (syscall.c):
  syscall():
    p->trapframe->a0 = 0    # Store return value
    ↓
Kernel (trap.c):
  usertrapret():
    prepare to return to user
    call userret()
    ↓
Kernel (trampoline.S):
  userret:
    restore all registers from trapframe
    sret                    # Return to user mode
    ↓
User Space:
  # Return value in a0
  if(trace(...) < 0) { ... }
```

---

### 4.2. Lock Strategy

**Spinlocks** (for short critical sections):
- `kmem.lock`: Protects free page list
- `p->lock`: Protects process state
- `ftable.lock`: Protects file table

**Pattern**:
```c
acquire(&lock);
// Critical section (fast, no sleep)
release(&lock);
```

**Sleeplocks** (for long critical sections):
- Inode locks
- Buffer cache locks

**Pattern**:
```c
acquiresleep(&lock);
// Critical section (can sleep, do I/O)
releasesleep(&lock);
```

---

### 4.3. Error Handling

**Trong kernel**:
- Return `-1` để indicate error
- User space check return value

**Example**:
```c
// Kernel
uint64 sys_sysinfo(void) {
  ...
  if(copyout(...) < 0)
    return -1;  // Error
  return 0;     // Success
}

// User
int ret = sysinfo(&info);
if(ret < 0) {
  printf("sysinfo failed\n");
}
```

---

## 📊 Phần 5: Data Flow Diagrams

### 5.1. Trace Syscall Flow

```
trace(32)  →  sys_trace()  →  myproc()->trace_mask = 32
                                      ↓
                              (saved in process)
                                      ↓
                              child = fork()
                                      ↓
                    child->trace_mask = parent->trace_mask
                                      ↓
                              read(fd, buf, n)
                                      ↓
                              syscall()
                                      ↓
                    if(trace_mask & (1 << SYS_read))
                      print "3: syscall read -> 512"
```

---

### 5.2. Sysinfo Data Collection

```
sysinfo(&info)
      ↓
sys_sysinfo()
      ↓
   ┌──┴──┐
   ↓     ↓     ↓
kfreemem()  count_process()  count_files()
   ↓          ↓                 ↓
Traverse   Loop proc[]      Loop ftable
freelist   count state!=UNUSED  count ref>0
   ↓          ↓                 ↓
40960      3                 5
   └──┬───────┘
      ↓
info.freemem = 40960
info.nproc = 3
info.nopenfiles = 5
      ↓
copyout(user_addr, &info, sizeof(info))
      ↓
User space receives filled struct
```

---

## ✅ Checklist Hiểu Codebase

Đảm bảo bạn hiểu:

- [ ] Cách syscall number được define (`syscall.h`)
- [ ] Cách dispatcher route đến handler (`syscall.c`)
- [ ] Cách extract arguments (`argint`, `argaddr`, `argstr`)
- [ ] Cách user stub generate (`usys.pl`)
- [ ] Process structure (`struct proc`)
- [ ] Cách fork copy process state
- [ ] Memory allocator (free list)
- [ ] File table (global shared structure)
- [ ] Cách copyout() hoạt động
- [ ] Lock strategy (when to acquire/release)
- [ ] Trap handling flow (`usertrap → syscall → handler`)

---

**Tài liệu này là roadmap để hiểu xv6. Hãy đọc source code theo từng phần và trace qua từng function!** 🚀
