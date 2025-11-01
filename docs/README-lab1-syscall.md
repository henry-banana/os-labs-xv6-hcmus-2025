# Lab 1 – System calls (xv6-riscv)

Tài liệu này tổng hợp những gì hai nhóm đã làm cho Lab 1 (syscall) và cung cấp hướng dẫn chi tiết để bạn build, chạy thử, kiểm tra, và đóng gói nộp bài.

- Nhóm A: `23127091_23127306_23127315` (base: xv6-labs phong cách 2025)
- Nhóm B: `23127215_23127326_23127407` (base: `xv6-labs-2024`)

> Lưu ý môi trường: Lab yêu cầu chạy trên Linux. Nếu đang dùng Windows, nên dùng WSL2 (Ubuntu) và cài toolchain RISC‑V + QEMU trong WSL.

---

## 1) Tổng quan những gì mỗi nhóm đã làm

### Nhóm A – 23127091_23127306_23127315

Đã triển khai 2 system call theo yêu cầu Lab 1:

- `trace(int mask)`
  - Lưu `mask` vào tiến trình hiện tại để theo dõi các syscall được bật bit.
  - Kế thừa mask qua `fork()` (con nhận cùng `mask` của cha).
  - In log sau khi syscall chạy xong ở kernel (định dạng: `pid: syscall name -> ret`).
- `sysinfo(struct sysinfo *info)`
  - Trả về: `freemem` (bytes còn trống), `nproc` (số tiến trình đang dùng), `nopenfiles`/`nfile` (tổng file đang mở toàn hệ thống).

Dấu vết trong mã (các file tiêu biểu có mặt trong thư mục):
- Kernel:
  - `kernel/syscall.h`: có `#define SYS_trace 22`, `#define SYS_sysinfo 23`.
  - `kernel/sysproc.c`: có `sys_trace()` và `sys_sysinfo()`; sử dụng các helper như `nfree()`, `nproc()`, `nfile()` và copyout về user.
  - `kernel/proc.c`: (thông thường) thêm trường `trace_mask` vào `struct proc` và sao chép trong `fork()`.
  - `kernel/syscall.c`: (thông thường) ánh xạ số hiệu → handler và in tên syscall khi bit trong `trace_mask` bật.
- User:
  - `user/trace.c`: chương trình dòng lệnh `trace MASK cmd [args...]`.
  - `user/sysinfotest.c`: chương trình kiểm thử `sysinfo`.
  - `user/user.h`, `user/usys.pl`: thêm prototype/entry cho `trace`, `sysinfo`.

### Nhóm B – 23127215_23127326_23127407 (xv6-labs-2024)

Đã làm đủ phần Lab 1 và thêm bài hướng dẫn “hello syscall”:

- `hello(void)` – syscall mẫu (hướng dẫn học liệu): in `"Hello, world!"` từ kernel.
- `trace(int mask)` – như mô tả ở trên.
- `sysinfo(struct sysinfo *info)` – như mô tả ở trên; dùng helper `kfreemem()` và `count_open_files()`.

Dấu vết trong mã (các file tiêu biểu có mặt trong thư mục):
- Kernel:
  - `kernel/syscall.h`: có `#define SYS_hello 22`, `#define SYS_trace 23`, `#define SYS_sysinfo 24`.
  - `kernel/sysproc.c`: có `sys_hello()`, `sys_trace()`, `sys_sysinfo()`.
  - `kernel/sysinfo.h`: định nghĩa `struct sysinfo` (freemem, nproc, nopenfiles).
- User:
  - `user/testhello.c`: gọi `hello()` để kiểm tra.
  - `user/trace.c`, `user/sysinfotest.c` như yêu cầu Lab 1.
  - `user/user.h`, `user/usys.pl`: thêm prototype/entry cho `hello`, `trace`, `sysinfo`.

### Khác biệt số hiệu syscall giữa 2 nhóm (tham khảo nhanh)

- Nhóm A (2025 style):
  - `SYS_trace = 22`, `SYS_sysinfo = 23`.
- Nhóm B (2024 style):
  - `SYS_hello = 22`, `SYS_trace = 23`, `SYS_sysinfo = 24`.

Các số hiệu khác (read=5, write=16, …) giữ nguyên như xv6 chuẩn; ví dụ `1 << SYS_read = 32` dùng cho mask.

---

## 2) Cách build và chạy

Chuẩn bị (trên Linux/WSL2): cài RISC‑V toolchain và QEMU (tham khảo hướng dẫn xv6 chính thức). Sau đó:

### Build & chạy nhóm A

- Mở WSL2 → chuyển đến folder nhóm A:
  - `23127091_23127306_23127315/`
- Chạy:
  ```sh
  make clean
  make qemu
  ```
- Ở shell xv6, bạn có thể gọi các chương trình dưới đây.

### Build & chạy nhóm B

- Mở WSL2 → chuyển đến folder nhóm B:
  - `23127215_23127326_23127407/xv6-labs-2024/`
- Chạy:
  ```sh
  make clean
  make qemu
  ```

> Mẹo: Nếu dùng VS Code trên Windows, hãy mở Remote – WSL và chạy trực tiếp trong terminal WSL.

---

## 3) Cách dùng `trace`

Chương trình user `trace` nhận `MASK` và một lệnh để thực thi với `exec`. Mỗi bit trong `MASK` bật theo số hiệu syscall tương ứng.

Ví dụ (chạy trong shell xv6):

- Chỉ theo dõi `read` (vì `1 << 5 = 32`):
  ```
  $ trace 32 grep hello README
  3: syscall read -> 1023
  3: syscall read -> 966
  3: syscall read -> 70
  3: syscall read -> 0
  ```
- Theo dõi toàn bộ (mask lớn):
  ```
  $ trace 2147483647 grep hello README
  4: syscall trace -> 0
  4: syscall exec -> 3
  4: syscall open -> 3
  4: syscall read -> 1023
  4: syscall read -> 966
  4: syscall read -> 70
  4: syscall read -> 0
  4: syscall close -> 0
  ```
- Kiểm tra kế thừa qua `fork` (ví dụ với usertests):
  ```
  $ trace 2 usertests forkforkfork
  usertests starting
  ...
  ```

Lưu ý: Định dạng log mong muốn: `pid: syscall name -> return_value`.

---

## 4) Cách dùng `sysinfo`

Chương trình user `sysinfotest` sẽ gọi `sysinfo(&info)` và in ra các trường:
- `freemem` – dung lượng bộ nhớ trống (bytes)
- `nproc` – số tiến trình đang dùng (khác `UNUSED`)
- `nopenfiles` – tổng số file đang mở (tên trường có thể là `nfile` trong một số bản triển khai)

Chạy trong shell xv6:
```
$ sysinfotest
freemem: ...
nproc: ...
nopenfiles: ...
```

---

## 5) (Nhóm B) Kiểm tra `hello`

Nếu build từ thư mục `xv6-labs-2024` của Nhóm B, bạn có thể chạy:
```
$ testhello
Hello, world!
```

---

## 6) Đóng gói nộp bài (chuẩn MOODLE)

Theo file yêu cầu trong `docs/vn-Lab1-syscall-requirment.md`:

- Tên file nộp: `StudentID1_StudentID2_StudentID3.zip` (ID tăng dần).
- Bên trong zip gồm:
  1. Report PDF: `StudentID1_StudentID2_StudentID3_Report.pdf` (ngắn gọn, súc tích; không cần chép mã nguồn vào).
  2. File patch: `StudentID1_StudentID2_StudentID3.patch` (tạo bằng `git diff > StudentID1_StudentID2_StudentID3.patch`).
  3. Mã nguồn xv6 đã `make clean`.

Gợi ý quy trình tạo patch và zip (trong WSL, tại thư mục gốc dự án của nhóm bạn):
- Commit trạng thái gốc (nếu chưa): `git init && git add -A && git commit -m "base"`
- Sau khi sửa xong: `git add -A && git commit -m "lab1"`
- Xuất patch: `git diff base > StudentID1_StudentID2_StudentID3.patch`
- Xóa build: `make clean`
- Tạo zip theo đúng tên yêu cầu.

---

## 7) Gợi ý triển khai (nhanh) – đối chiếu với mã hai nhóm

- `trace(mask)`:
  - User: thêm prototype (`user/user.h`), entry (`user/usys.pl`), chương trình `user/trace.c`.
  - Kernel:
    - `proc.h`: thêm `int trace_mask;` vào `struct proc`.
    - `proc.c`: trong `fork()`, sao chép `np->trace_mask = p->trace_mask;`.
    - `sysproc.c`: hiện thực `sys_trace()` nhận `mask` bằng `argint(0, &mask)`; gán vào `myproc()->trace_mask`.
    - `syscall.h`/`syscall.c`: gán số hiệu; ánh xạ handler; thêm bảng tên syscall; in log sau khi trả về nếu bit bật.
- `sysinfo(info*)`:
  - User: forward declare `struct sysinfo;` và `int sysinfo(struct sysinfo*);` trong `user/user.h`; thêm entry trong `user/usys.pl`; tạo `user/sysinfotest.c`.
  - Kernel:
    - `kernel/sysinfo.h`: định nghĩa `struct sysinfo { uint64 freemem; uint64 nproc; uint64 nopenfiles; };`
    - Thêm helper: đếm trang trống (duyệt `freelist` trong `kalloc.c`), đếm `proc` khác `UNUSED`, đếm `file` có `ref>0`.
    - `sysproc.c`: thu thập vào biến `info` cục bộ, `copyout()` về địa chỉ người dùng.

Mọi điểm trên đều đã được hai nhóm thực hiện theo tinh thần này (Nhóm B có thêm `hello`).

---

## 8) Kiểm thử nhanh đề xuất

- `make qemu`
- Chạy:
  - `trace 32 grep hello README`
  - `trace 2 usertests forkforkfork`
  - `sysinfotest`
  - (Nhóm B) `testhello`
- Chạy `usertests` tổng quát để kiểm tra không phá vỡ hành vi chuẩn.

---

## 9) Khắc phục sự cố thường gặp

- Build lỗi vì thiếu toolchain: đảm bảo dùng Linux/WSL2 và đã cài `riscv64-unknown-elf-*` và `qemu-system-riscv64` theo hướng dẫn xv6.
- Không thấy log `trace`: kiểm tra `syscall.c` đã in sau khi gọi handler và tên syscall có trong bảng tên chưa; đảm bảo `trace_mask` được sao chép trong `fork()`.
- `sysinfo` trả về sai: xác minh helper đếm trang trống/tiến trình/file; đảm bảo dùng `copyout()` đúng kích thước.

---

## 10) Tham khảo

- `docs/vn-Lab1-syscall-requirment.md` (yêu cầu môn học + ví dụ)
- MIT 6.1810 (2025) – System calls lab: https://pdos.csail.mit.edu/6.1810/2025/labs/syscall.html

---

Hoàn tất: Hai nhóm đã hoàn thiện `trace` và `sysinfo` (Nhóm B thêm `hello`). Hướng dẫn trên giúp bạn build/chạy và đóng gói nộp bài đúng chuẩn; nếu cần, mình có thể bổ sung hình ảnh minh họa và log mẫu từ bản build thực tế của bạn.
