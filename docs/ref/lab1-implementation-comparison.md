# Lab 1 System Call - So Sánh Implementation của 2 Teams

> **Tài liệu phân tích**: So sánh cách implement trace() và sysinfo() của Team A (23127091_23127306_23127315) và Team B (23127215_23127326_23127407)

---

## 📊 Tổng Quan

| Khía Cạnh | Team A (091/306/315) | Team B (215/326/407) |
|-----------|----------------------|----------------------|
| **Chất lượng code** | ⭐⭐⭐⭐⭐ Clean, chuẩn | ⭐⭐⭐ Có code thừa, comment nhiều |
| **Cách đặt tên** | Chuẩn convention (`nfree`, `nproc`) | Không nhất quán (`kfreemem` vs `nfree`) |
| **Organization** | Logic tách biệt rõ ràng | Lẫn lộn code/comment |
| **Syscall thêm** | Chỉ có trace + sysinfo | Có thêm `sys_hello()` (không cần thiết) |

---

## 🔍 Chi Tiết So Sánh

### 1. Trace System Call

#### **Team A - Approach Chuẩn**
```c
// kernel/syscall.c
void syscall(void) {
  int num;
  struct proc *p = myproc();

  num = p->trapframe->a7;
  if(num > 0 && num < NELEM(syscalls) && syscalls[num]) {
    p->trapframe->a0 = syscalls[num]();  // ← Gọi syscall trước
    
    // Kiểm tra trace mask SAU khi gọi syscall
    if ((p->trace_mask & (1 << num)) != 0) { 
      printf("%d: syscall %s -> %ld\n", p->pid, syscall_names[num], p->trapframe->a0);
    }
  } else {
    printf("%d %s: unknown sys call %d\n", p->pid, p->name, num);
    p->trapframe->a0 = -1;
  }
}
```

**✅ Ưu điểm:**
- **Logic rõ ràng**: Gọi syscall → Lưu return value → Kiểm tra trace → In log
- **Inline execution**: Không cần biến tạm `retval`
- **Bit masking chuẩn**: `(1 << num)` để check từng syscall riêng lẻ

---

#### **Team B - Approach Dài Dòng**
```c
// kernel/syscall.c
void syscall(void) {
  int num;
  struct proc *p = myproc();

  num = p->trapframe->a7;
  if(num > 0 && num < NELEM(syscalls) && syscalls[num]) {
    uint64 retval = syscalls[num]();  // ← Lưu vào biến tạm
        
    // Nếu tiến trình có bật trace và system call nằm trong mask
    if (p->trace_mask & (1 << num)) {
      printf("%d: syscall %s -> %ld\n", p->pid, syscall_names[num], retval);
    }
    
    p->trapframe->a0 = retval; // ← Gán return value sau

  //   uint64 retval = syscalls[num]();
  //   // Use num to lookup...
  //   // [NHIỀU COMMENT CODE CŨ BỊ COMMENT OUT]
  //   p->trapframe->a0 = syscalls[num]();
  // } else {
  //   printf("%d %s: unknown sys call %d\n", p->pid, p->name, num);
  //   p->trapframe->a0 = -1;
  }
}
```

**❌ Vấn đề:**
- **Biến tạm không cần thiết**: `uint64 retval` làm code dài ra
- **Code bị comment out**: Nhiều dòng code cũ không xóa sạch
- **Thiếu else branch**: Không handle trường hợp syscall không tồn tại (bị comment)

---

### 2. Sysinfo System Call

#### **Team A - Approach Modular**
```c
// kernel/sysproc.c
uint64 sys_sysinfo(void) {
  uint64 addr;
  struct sysinfo info;
  struct proc *p = myproc();
  
  argaddr(0, &addr);

  info.freemem = nfree();    // ← Helper function riêng biệt
  info.nproc = nproc();      // ← Helper function riêng biệt
  info.nfile = nfile();      // ← Helper function riêng biệt

  if (copyout(p->pagetable, addr, (char *)&info, sizeof(info)) < 0) {
    return -1;
  }
  return 0;
}
```

**Helper functions trong các file tương ứng:**

```c
// kernel/kalloc.c
uint64 nfree(void) {
  int byteCount = 0;
  struct run *r;
  
  acquire(&kmem.lock);
  r = kmem.freelist; 
  while (r != 0) { 
    byteCount += PGSIZE;
    r = r->next; 
  }
  release(&kmem.lock);
  return byteCount; 
}

// kernel/proc.c
uint64 nproc(void) {
  int procCount = 0; 
  struct proc *p; 

  for (p = proc; p < &proc[NPROC]; p++) { 
    acquire(&p->lock);
    if (p->state != UNUSED) { 
      procCount++; 
    }
    release(&p->lock);
  }
  return procCount;
}

// kernel/file.c
uint64 nfile(void) {
  int fileCount = 0;
  int i;

  acquire(&ftable.lock);
  for(i = 0; i < NFILE; i++){
    if(ftable.file[i].ref > 0)
      fileCount++;
  }
  release(&ftable.lock);
  return fileCount;
}
```

**✅ Ưu điểm:**
- **Separation of Concerns**: Mỗi module (kalloc, proc, file) tự quản lý logic của mình
- **Naming convention chuẩn**: `nfree`, `nproc`, `nfile` (n = number/count)
- **Locking đúng**: Mỗi function acquire/release lock của module mình
- **Reusability**: Các helper function có thể dùng lại cho syscall khác

---

#### **Team B - Approach Inline**
```c
// kernel/sysproc.c
extern struct proc proc[NPROC];  // ← Extern declaration trực tiếp
extern uint64 kfreemem();        // ← Tên function không chuẩn
extern int count_open_files();   // ← Tên function dài dòng

uint64 sys_sysinfo(void) {
    struct sysinfo info;

    info.freemem = kfreemem();  // ← Tên không match với Team A

    // Đếm số tiến trình đang chạy
    info.nproc = 0;
    struct proc *p;
    for (p = proc; p < proc + NPROC; p++) {  // ← Logic trực tiếp trong syscall
        if (p->state != UNUSED)
            info.nproc++;
    }

    info.nopenfiles = count_open_files();  // ← Field name khác: nopenfiles vs nfile

    uint64 dst;
    argaddr(0, &dst);
    
    if (copyout(myproc()->pagetable, dst, (char *)&info, sizeof(info)) < 0)
        return -1;
    
    return 0;
}
```

**Helper functions:**

```c
// kernel/kalloc.c
uint64 kfreemem() {  // ← Tên khác: kfreemem vs nfree
  struct run *r;
  uint64 free_bytes = 0;

  acquire(&kmem.lock);
  for (r = kmem.freelist; r; r = r->next) {
      free_bytes += PGSIZE;
  }
  release(&kmem.lock);
  return free_bytes;
}

// kernel/file.c
int count_open_files() {  // ← Tên dài: count_open_files vs nfile
  struct file *f;
  int count = 0;

  acquire(&ftable.lock);
  for (f = ftable.file; f < &ftable.file[NFILE]; f++) {
      if (f->ref > 0)
          count++;
  }
  release(&ftable.lock);
  return count;
}
```

**❌ Vấn đề:**
- **Không nhất quán về naming**: `kfreemem` vs `count_open_files` vs inline loop
- **Logic lẫn lộn**: Đếm process trực tiếp trong `sys_sysinfo()` thay vì tách ra function
- **Extern declarations**: Phải khai báo extern trong sysproc.c (không clean)
- **Struct field khác**: `nopenfiles` vs `nfile` (không theo xv6 convention)
- **Không có lock cho nproc**: Loop đếm process không acquire lock

---

### 3. Fork - Trace Mask Inheritance

#### **Team A**
```c
// kernel/proc.c - fork()
np->trapframe->a0 = 0;
np->trace_mask = p->trace_mask; // ← Inherit trace mask from parent

// increment reference counts on open file descriptors.
for(i = 0; i < NOFILE; i++)
```

**✅ Comment ngắn gọn, đúng trọng tâm**

---

#### **Team B**
```c
// kernel/proc.c - fork()
np->trapframe->a0 = 0;

// Copy trace mask từ tiến trình cha sang tiến trình con/////////////////////////
np->trace_mask = p->trace_mask;

// increment reference counts on open file descriptors.
for(i = 0; i < NOFILE; i++)
```

**❌ Comment dài dòng + dòng gạch không cần thiết**

---

### 4. Syscall Arrays

#### **Team A - Clean**
```c
static char *syscall_names[] = {
  "", "fork", "exit", "wait", "pipe", "read", "kill", "exec",
  "fstat", "chdir", "dup", "getpid", "sbrk", "sleep", "uptime",
  "open", "write", "mknod", "unlink", "link", "mkdir", "close", "trace"
};
```

**✅ Định nghĩa compact, đủ dùng**

---

#### **Team B - Verbose**
```c
static char *syscall_names[] = {
  [SYS_fork]   = "fork",
  [SYS_exit]   = "exit",
  [SYS_wait]   = "wait",
  [SYS_pipe]   = "pipe",
  [SYS_read]   = "read",
  [SYS_kill]   = "kill",
  [SYS_exec]   = "exec",
  [SYS_fstat]  = "fstat",
  [SYS_chdir]  = "chdir",
  [SYS_dup]    = "dup",
  [SYS_getpid] = "getpid",
  [SYS_sbrk]   = "sbrk",
  [SYS_sleep]  = "sleep",
  [SYS_uptime] = "uptime",
  [SYS_open]   = "open",
  [SYS_write]  = "write",
  [SYS_mknod]  = "mknod",
  [SYS_unlink] = "unlink",
  [SYS_link]   = "link",
  [SYS_mkdir]  = "mkdir",
  [SYS_close]  = "close",
  [SYS_hello]  = "hello",   // ← Syscall không cần thiết
  [SYS_trace]  = "trace",   // Thêm tên cho sys_trace
};
```

**❌ Designated initializers dài dòng + có `sys_hello` thừa**

---

## 🏆 Kết Luận

### Team A (23127091_23127306_23127315): ⭐⭐⭐⭐⭐

**Điểm mạnh:**
- ✅ **Code clean**: Không có comment code cũ, không có syscall thừa
- ✅ **Naming convention chuẩn**: `nfree`, `nproc`, `nfile` theo xv6 style
- ✅ **Modular design**: Helper functions tách biệt rõ ràng
- ✅ **Locking đúng**: Mỗi function acquire/release lock đúng cách
- ✅ **Maintainable**: Dễ đọc, dễ mở rộng

**Điểm yếu:**
- Không có lỗi đáng kể

---

### Team B (23127215_23127326_23127407): ⭐⭐⭐

**Điểm mạnh:**
- ✅ **Designated initializers**: Dùng `[SYS_trace] = sys_trace` (rõ ràng hơn)
- ✅ **Logic đúng**: Về mặt chức năng hoạt động ok

**Điểm yếu:**
- ❌ **Code bẩn**: Nhiều comment code cũ không xóa
- ❌ **Naming không nhất quán**: `kfreemem` vs `count_open_files` vs inline loop
- ❌ **Syscall thừa**: `sys_hello()` không liên quan đến Lab 1
- ❌ **Inline logic**: Đếm process trực tiếp trong `sys_sysinfo()` thay vì tách ra
- ❌ **Thiếu error handling**: Không có else branch cho syscall không hợp lệ
- ❌ **Extern trong .c file**: Khai báo `extern` trong sysproc.c thay vì dùng header

---

## 📝 Bài Học Cho Team của Bạn

### 1. Naming Convention
```c
// ✅ GOOD - Team A style
uint64 nfree(void)   // n = number, free = free memory
uint64 nproc(void)   // n = number, proc = process
uint64 nfile(void)   // n = number, file = file

// ❌ BAD - Team B style
uint64 kfreemem()           // k = kernel? Không rõ
int count_open_files()      // Dài dòng, không nhất quán
```

### 2. Code Organization
```c
// ✅ GOOD - Team A style
info.freemem = nfree();    // Function tách biệt
info.nproc = nproc();      // Function tách biệt
info.nfile = nfile();      // Function tách biệt

// ❌ BAD - Team B style
info.freemem = kfreemem(); // Function tách biệt
info.nproc = 0;            // Logic inline
for (p = proc; ...) {      // Loop trực tiếp trong syscall
    if (p->state != UNUSED)
        info.nproc++;
}
info.nopenfiles = count_open_files(); // Function tách biệt
```

### 3. Clean Code
```c
// ✅ GOOD - Team A style
p->trapframe->a0 = syscalls[num]();
if ((p->trace_mask & (1 << num)) != 0) { 
  printf(...);
}

// ❌ BAD - Team B style
uint64 retval = syscalls[num]();
if (p->trace_mask & (1 << num)) {
  printf(...);
}
p->trapframe->a0 = retval;
//   uint64 retval = syscalls[num]();  // ← Code cũ không xóa
//   p->trapframe->a0 = syscalls[num]();
```

---

## 🎯 Recommendations

**Cho Team của Bạn:**
1. ✅ **Follow Team A's approach**: Clean, modular, chuẩn convention
2. ✅ **Tách helper functions riêng**: `nfree()`, `nproc()`, `nfile()`
3. ✅ **Đặt tên nhất quán**: Theo xv6 style (ngắn gọn, rõ nghĩa)
4. ✅ **Xóa code comment**: Không để lại code cũ đã comment
5. ✅ **Implement đầy đủ error handling**: Có else branch cho mọi trường hợp

**Tránh:**
- ❌ Thêm syscall không liên quan (như `sys_hello`)
- ❌ Inline logic phức tạp trong syscall handler
- ❌ Dùng biến tạm không cần thiết
- ❌ Comment dài dòng khi logic đã rõ ràng
- ❌ Khai báo extern trong .c file thay vì header
