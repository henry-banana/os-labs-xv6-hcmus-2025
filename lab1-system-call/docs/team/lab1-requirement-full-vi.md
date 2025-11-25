# Lab 1 – System Calls: Yêu Cầu Chi Tiết (Vietnamese)

## 🎯 Mục Tiêu Tổng Quan

Lab 1 tập trung vào việc **hiểu rõ cơ chế system call** trong hệ điều hành xv6, từ đó triển khai 2 system call mới:
1. **`trace(int mask)`** - Theo dõi các lời gọi hệ thống
2. **`sysinfo(struct sysinfo *info)`** - Thu thập thông tin hệ thống

Qua bài lab này, bạn sẽ:
- Hiểu luồng xử lý từ user space → kernel space → trả về user space
- Nắm vững kiến trúc RISC-V calling convention
- Thực hành debug kernel với GDB
- Làm việc với process structure, memory management, và file table

---

## 📋 Yêu Cầu Chi Tiết

### **Phần 1: System Call `trace`** ⭐⭐

#### Mô tả chức năng
Tạo một system call cho phép **theo dõi (trace)** các system call mà một process gọi. Khi một syscall được trace, kernel sẽ in ra:
```
<pid>: syscall <name> -> <return_value>
```

#### Cách sử dụng
```bash
$ trace 32 grep hello README
3: syscall read -> 1023
3: syscall read -> 966
3: syscall read -> 70
3: syscall read -> 0
```

Trong đó:
- `32` là mask (bit 5 = `1 << SYS_read`)
- Chỉ syscall `read` được trace (vì bit 5 được bật)
- PID = 3
- Mỗi dòng in return value của syscall

#### Yêu cầu kỹ thuật

1. **User-level program: `user/trace.c`**
   - Nhận tham số từ command line: `trace MASK command [args...]`
   - Gọi syscall `trace(mask)` để thiết lập mask cho process hiện tại
   - Sau đó `exec` command được chỉ định
   - Xử lý lỗi: nếu thiếu tham số hoặc `exec` thất bại

2. **Kernel-level implementation:**
   
   **a) Thêm số hiệu syscall**
   - File: `kernel/syscall.h`
   - Định nghĩa: `#define SYS_trace 22`
   
   **b) Lưu trace mask trong process struct**
   - File: `kernel/proc.h`
   - Thêm field: `int trace_mask;` vào `struct proc`
   
   **c) Triển khai syscall handler**
   - File: `kernel/sysproc.c`
   - Hàm: `uint64 sys_trace(void)`
   - Logic:
     ```c
     int mask;
     argint(0, &mask);  // Lấy tham số mask
     myproc()->trace_mask = mask;  // Lưu vào process
     return 0;
     ```
   
   **d) Kế thừa trace_mask qua fork**
   - File: `kernel/proc.c`
   - Hàm: `kfork()`
   - Sao chép `trace_mask` từ parent sang child
   
   **e) In thông tin syscall khi được trace**
   - File: `kernel/syscall.c`
   - Hàm: `syscall()`
   - Logic:
     - Kiểm tra bit tương ứng trong `trace_mask`
     - Nếu bit được bật, in `<pid>: syscall <name> -> <retval>`
     - Cần tạo mảng tên syscall để map số hiệu → tên

3. **User-space stub**
   - File: `user/usys.pl`
   - Thêm: `entry("trace");`
   - File: `user/user.h`
   - Thêm prototype: `int trace(int);`

4. **Makefile**
   - Thêm `$U/_trace\` vào biến `UPROGS`

#### Test cases
```bash
$ trace 32 grep hello README
$ trace 2147483647 grep hello README  # trace tất cả syscall
$ trace 2 usertests forkforkfork     # trace syscall fork
```

---

### **Phần 2: System Call `sysinfo`** ⭐⭐⭐

#### Mô tả chức năng
Thu thập thông tin về trạng thái hệ thống hiện tại:
- `freemem`: Số byte bộ nhớ còn trống
- `nproc`: Số process đang ở trạng thái `NOT UNUSED`
- `nopenfiles`: Tổng số file descriptor đang mở trong toàn hệ thống

#### Cách sử dụng
```c
struct sysinfo info;
sysinfo(&info);
printf("free memory: %d bytes\n", info.freemem);
printf("processes: %d\n", info.nproc);
printf("open files: %d\n", info.nopenfiles);
```

#### Yêu cầu kỹ thuật

1. **Định nghĩa struct**
   - File: `kernel/sysinfo.h` (tạo mới)
   ```c
   struct sysinfo {
     uint64 freemem;   // Số byte bộ nhớ trống
     uint64 nproc;     // Số process đang dùng
     uint64 nopenfiles; // Số file descriptor đang mở
   };
   ```

2. **User-level program: `user/sysinfotest.c`**
   - Đã có sẵn để test
   - Không cần sửa

3. **Kernel implementation:**
   
   **a) Thêm số hiệu syscall**
   - File: `kernel/syscall.h`
   - `#define SYS_sysinfo 23`
   
   **b) Triển khai syscall handler**
   - File: `kernel/sysproc.c`
   - Hàm: `uint64 sys_sysinfo(void)`
   - Logic:
     ```c
     uint64 addr;
     argaddr(0, &addr);  // Lấy địa chỉ struct từ user space
     
     struct sysinfo info;
     info.freemem = kfreemem();      // Đếm bộ nhớ trống
     info.nproc = count_process();   // Đếm số process
     info.nopenfiles = count_files(); // Đếm file đang mở
     
     if(copyout(myproc()->pagetable, addr, (char *)&info, sizeof(info)) < 0)
       return -1;
     return 0;
     ```
   
   **c) Helper functions:**
   
   - **`kfreemem()`** - File: `kernel/kalloc.c`
     ```c
     uint64 kfreemem(void) {
       struct run *r;
       uint64 n = 0;
       
       acquire(&kmem.lock);
       for(r = kmem.freelist; r; r = r->next)
         n += PGSIZE;
       release(&kmem.lock);
       
       return n;
     }
     ```
   
   - **`count_process()`** - File: `kernel/proc.c`
     ```c
     uint64 count_process(void) {
       struct proc *p;
       uint64 n = 0;
       
       for(p = proc; p < &proc[NPROC]; p++) {
         acquire(&p->lock);
         if(p->state != UNUSED)
           n++;
         release(&p->lock);
       }
       
       return n;
     }
     ```
   
   - **`count_files()`** - File: `kernel/file.c`
     ```c
     uint64 count_files(void) {
       struct file *f;
       uint64 n = 0;
       
       acquire(&ftable.lock);
       for(f = ftable.file; f < ftable.file + NFILE; f++) {
         if(f->ref > 0)
           n++;
       }
       release(&ftable.lock);
       
       return n;
     }
     ```

4. **Khai báo prototype**
   - File: `kernel/defs.h`
   - Thêm:
     ```c
     // kalloc.c
     uint64          kfreemem(void);
     
     // proc.c
     uint64          count_process(void);
     
     // file.c
     uint64          count_files(void);
     ```

5. **User-space stub**
   - File: `user/usys.pl`: `entry("sysinfo");`
   - File: `user/user.h`:
     ```c
     struct sysinfo;  // Forward declaration
     int sysinfo(struct sysinfo *);
     ```

---

## 🧪 Kiểm Tra Và Đánh Giá

### Chạy test
```bash
$ make qemu
...
init: starting sh
$ trace 32 grep hello README
$ sysinfotest
$ usertests
```

### Chấm điểm tự động
```bash
$ make grade
```

Kết quả mong đợi:
```
== Test trace 32 grep == trace 32 grep: OK (2.0s)
== Test trace all grep == trace all grep: OK (1.0s)
== Test trace nothing == trace nothing: OK (1.0s)
== Test trace children == trace children: OK (14.0s)
== Test sysinfotest == sysinfotest: OK (2.5s)
== Test usertests == usertests: OK (66.4s)
Score: 100/100
```

---

## 🔍 Debug với GDB

### Khởi động GDB
```bash
# Terminal 1
$ make qemu-gdb

# Terminal 2
$ gdb-multiarch
(gdb) file kernel/kernel
(gdb) target remote localhost:26000
(gdb) break syscall
(gdb) continue
```

### Các lệnh GDB hữu ích
```gdb
(gdb) backtrace          # Xem call stack
(gdb) info registers     # Xem giá trị thanh ghi
(gdb) p myproc()         # In địa chỉ process hiện tại
(gdb) p /x *myproc()     # In nội dung struct proc (hex)
(gdb) p /x $sstatus      # In thanh ghi sstatus
(gdb) x/10i $pc          # Disassemble 10 lệnh từ PC
(gdb) layout src         # Hiển thị source code
(gdb) step               # Step vào function
(gdb) next               # Step over
```

---

## 📝 Báo Cáo Yêu Cầu

File: `StudentID1_StudentID2_StudentID3_Report.pdf`

### Nội dung báo cáo

1. **Giới thiệu** (1 trang)
   - Mục tiêu lab
   - Tổng quan về system call mechanism
   - Công cụ và môi trường

2. **Phần 1: System Call `trace`** (3-4 trang)
   - Thiết kế và luồng thực thi
   - Giải thích từng file đã sửa
   - Snippet code quan trọng (có chú thích)
   - Screenshot kết quả test
   - Khó khăn và cách giải quyết

3. **Phần 2: System Call `sysinfo`** (3-4 trang)
   - Thiết kế và luồng thực thi
   - Giải thích các helper function
   - Cách đếm freemem, nproc, nopenfiles
   - Sử dụng locks để tránh race condition
   - Screenshot kết quả test

4. **Debugging Experience** (1-2 trang)
   - Các lỗi gặp phải và cách debug
   - GDB commands đã dùng
   - Lessons learned

5. **Kết luận** (1 trang)
   - Tổng kết kiến thức đạt được
   - Hiểu biết sâu hơn về xv6
   - Hướng cải tiến (nếu có)

6. **Phân công công việc** (1 trang)
   - Dev A: ...
   - Dev B: ...
   - Dev C: ...
   - Mức độ đóng góp: A: 33%, B: 34%, C: 33%

---

## 🎤 Chuẩn Bị Phỏng Vấn (Demo)

### Câu hỏi khái niệm

1. **System call là gì? Tại sao cần system call?**
   - VN: "System call là cơ chế để user program yêu cầu dịch vụ từ kernel. Cần vì user space không có quyền truy cập trực tiếp phần cứng."
   - EN: "A system call is the mechanism for user programs to request services from the kernel, necessary because user space lacks direct hardware access."

2. **Giải thích luồng xử lý của một system call trong xv6.**
   - User program gọi wrapper (ví dụ `trace()` trong `user.h`)
   - Wrapper thực thi `ecall` instruction (trong `usys.S`)
   - CPU chuyển sang kernel mode, nhảy đến `uservec` (trampoline)
   - `usertrap()` → `syscall()` → dispatch đến handler cụ thể
   - Handler thực thi, return value lưu vào `a0`
   - `usertrapret()` → quay về user mode

3. **Thanh ghi a7 dùng để làm gì trong system call?**
   - Chứa system call number để kernel biết syscall nào được gọi
   - Ví dụ: `SYS_trace = 22`, stub sẽ `li a7, 22` trước `ecall`

4. **Tại sao trace_mask phải được copy trong fork()?**
   - Child process kế thừa tất cả state của parent
   - Nếu không copy, child sẽ không trace được syscall như parent

5. **Giải thích cơ chế copyout() trong sysinfo.**
   - `copyout()` copy data từ kernel space sang user space
   - Cần vì kernel không thể trực tiếp ghi vào địa chỉ user
   - Sử dụng page table của process để translate địa chỉ

### Câu hỏi triển khai

6. **Code của bạn xử lý race condition như thế nào?**
   - Sử dụng locks: `kmem.lock`, `p->lock`, `ftable.lock`
   - Acquire lock trước khi đọc shared data
   - Release ngay sau khi xong

7. **Nếu mask = 0, trace có in gì không?**
   - Không, vì không có bit nào được set
   - Kiểm tra: `if(mask & (1 << num))`

8. **Giải thích cách bạn đếm freemem.**
   - Duyệt linked list `kmem.freelist`
   - Mỗi node = 1 page (4096 bytes)
   - Tổng số = `count * PGSIZE`

### Câu hỏi debug

9. **Làm thế nào để debug kernel panic?**
   - Xem `sepc` (địa chỉ lệnh gây lỗi)
   - Tra trong `kernel.asm` để tìm hàm
   - Dùng GDB: `break syscall`, step qua từng lệnh
   - In backtrace để thấy call chain

10. **Nếu sysinfo trả về giá trị sai, bạn debug như thế nào?**
    - In giá trị trước khi copyout: `printf("freemem=%d\n", info.freemem);`
    - Kiểm tra helper function có đúng không
    - Dùng GDB: `p info` để xem struct
    - Verify logic đếm: chạy từng loop riêng

---

## ⚠️ Lưu Ý Quan Trọng

### Về coding style
- **Comment code rõ ràng** (tiếng Anh hoặc tiếng Việt)
- **Tên biến có ý nghĩa**: `trace_mask` thay vì `tm`
- **Indentation nhất quán**: dùng 2 spaces như xv6 gốc
- **Không để code thừa**: xóa debug printf trước khi nộp

### Về testing
- **Test cả edge cases**: mask = 0, mask = -1, ...
- **Test fork inheritance**: child có trace được không?
- **Test concurrent**: chạy nhiều process cùng lúc
- **Chạy `usertests`**: đảm bảo không làm hỏng code cũ

### Về nộp bài
- **Filename**: `StudentID1_StudentID2_StudentID3.zip`
- **Nội dung zip**:
  - Report PDF
  - Thư mục source (đã `make clean`)
- **Deadline**: Xem trên Moodle
- **Không sao chép**: zero tolerance policy

---

## 📚 Tài Liệu Tham Khảo

1. **xv6 book**: Chapter 2 (Operating system organization), Chapter 4 (Traps and system calls)
2. **RISC-V Calling Convention**: Hiểu cách truyền tham số qua thanh ghi
3. **xv6 source code**: Đọc các syscall có sẵn như `sys_uptime()`, `sys_getpid()`
4. **GDB manual**: Học debug kernel-level code
5. **Lab instructions**: https://pdos.csail.mit.edu/6.1810/2025/labs/syscall.html

---

## ✅ Checklist Trước Khi Nộp

- [ ] `trace` compile và chạy đúng
- [ ] `sysinfotest` pass
- [ ] `usertests` pass (không làm hỏng code cũ)
- [ ] `make grade` cho 100/100
- [ ] Code có comment đầy đủ
- [ ] Report hoàn chỉnh (PDF)
- [ ] Đã test trên môi trường clean (clone lại repo mới)
- [ ] Filename đúng format
- [ ] Hiểu rõ từng dòng code mình viết (chuẩn bị demo)

---

**Chúc các bạn thành công! 🎉**
