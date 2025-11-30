# Lab 2: Bảng Trang (Page Tables) — Yêu Cầu Chính Thức

> **Nguồn**: https://pdos.csail.mit.edu/6.1810/2025/labs/pgtbl.html
> 
> **Mục đích file này**: Dịch và tổng hợp yêu cầu bài lab từ tài liệu chính thức (tiếng Việt)

---

## Tổng Quan

Trong bài lab này, bạn sẽ khám phá bảng trang (page tables) và sửa đổi chúng để triển khai các tính năng phổ biến của hệ điều hành.

### Tài Liệu Cần Đọc Trước Khi Code

- Chương 3 của [xv6 book](https://pdos.csail.mit.edu/6.1810/2025/xv6/book-riscv-rev5.pdf)
- `kernel/memlayout.h` - cấu trúc bố cục bộ nhớ
- `kernel/vm.c` - mã nguồn virtual memory chính
- `kernel/kalloc.c` - mã cấp phát và giải phóng bộ nhớ vật lý
- [RISC-V privileged architecture manual](https://drive.google.com/file/d/17GeetSnT5wW3xNuAHI95-SI1gPGd5sJ_/view)

### Chuẩn Bị

```bash
$ git fetch
$ git checkout pgtbl
$ make clean
```

---

## Phần 1: Kiểm Tra Bảng Trang Tiến Trình Người Dùng (Easy)

**Mục tiêu**: Giúp bạn hiểu bảng trang RISC-V bằng cách giải thích bảng trang của một tiến trình người dùng.

### Yêu cầu

1. Chạy `make qemu` và chạy chương trình `pgtbltest`
2. Hàm `print_pgtbl` in ra các mục PTE cho 10 trang đầu và 10 trang cuối của tiến trình `pgtbltest` sử dụng system call `pgpte` (đã được thêm sẵn cho lab này)
3. **Nhiệm vụ**: Với mỗi mục PTE trong output của `print_pgtbl`, **giải thích** nó chứa gì về mặt logic và các bit quyền (permission bits) là gì

### Output Mẫu

```
va 0 pte 0x21FCF45B pa 0x87F3D000 perm 0x5B
va 1000 pte 0x21FCE85B pa 0x87F3A000 perm 0x5B
...
va 0xFFFFD000 pte 0x0 pa 0x0 perm 0x0
va 0xFFFFE000 pte 0x21FD80C7 pa 0x87F60000 perm 0xC7
va 0xFFFFF000 pte 0x20001C4B pa 0x80007000 perm 0x4B
```

### Gợi ý

Tham khảo Hình 3.4 trong sách xv6 (lưu ý: hình có thể có tập hợp trang hơi khác so với tiến trình được kiểm tra).

---

## Phần 2: Tăng Tốc System Calls - `ugetpid()` (Easy)

**Mục tiêu**: Một số hệ điều hành (như Linux) tăng tốc các system call nhất định bằng cách chia sẻ dữ liệu trong vùng chỉ đọc giữa userspace và kernel. Điều này loại bỏ nhu cầu chuyển đổi kernel khi thực hiện các system call này.

### Yêu cầu

1. Khi mỗi tiến trình được tạo, ánh xạ **một trang chỉ đọc** tại địa chỉ `USYSCALL` (được định nghĩa trong `memlayout.h`)
2. Tại đầu trang này, lưu trữ một `struct usyscall` (cũng được định nghĩa trong `memlayout.h`)
3. Khởi tạo struct này để lưu **PID của tiến trình hiện tại**
4. Hàm `ugetpid()` đã được cung cấp sẵn ở phía userspace và sẽ tự động sử dụng ánh xạ USYSCALL

### Tiêu Chí Hoàn Thành

Test case `ugetpid` pass khi chạy `pgtbltest`

### Gợi ý

- Chọn các bit quyền (permission bits) cho phép userspace **chỉ đọc** trang
- Có một số việc cần làm trong vòng đời của một trang mới. Tham khảo cách xử lý trapframe trong `kernel/proc.c`

### Câu Hỏi Cần Trả Lời

> Những system call nào khác của xv6 có thể được tăng tốc bằng trang chia sẻ này? Giải thích cách thực hiện.

---

## Phần 3: In Bảng Trang - `vmprint()` (Easy)

**Mục tiêu**: Giúp bạn trực quan hóa bảng trang RISC-V và hỗ trợ debug trong tương lai.

### Yêu cầu

1. Viết hàm `vmprint()` trong `vm.c` để in nội dung của bảng trang
2. System call `kpgtbl()` đã được thêm sẵn, nó gọi `vmprint()` trong `vm.c`
3. Hàm nhận tham số `pagetable_t` và in bảng trang theo định dạng mô tả bên dưới

### Output Mẫu (khi chạy test `print_kpgtbl()`)

```
page table 0x0000000087f22000
 ..0x0000000000000000: pte 0x0000000021fc7801 pa 0x0000000087f1e000
 .. ..0x0000000000000000: pte 0x0000000021fc7401 pa 0x0000000087f1d000
 .. .. ..0x0000000000000000: pte 0x0000000021fc7c5b pa 0x0000000087f1f000
 .. .. ..0x0000000000001000: pte 0x0000000021fc705b pa 0x0000000087f1c000
 .. .. ..0x0000000000002000: pte 0x0000000021fc6cd7 pa 0x0000000087f1b000
 .. .. ..0x0000000000003000: pte 0x0000000021fc6807 pa 0x0000000087f1a000
 .. .. ..0x0000000000004000: pte 0x0000000021fc64d7 pa 0x0000000087f19000
 ..0x0000003fc0000000: pte 0x0000000021fc8401 pa 0x0000000087f21000
 .. ..0x0000003fffe00000: pte 0x0000000021fc8001 pa 0x0000000087f20000
 .. .. ..0x0000003fffffd000: pte 0x0000000021fd4813 pa 0x0000000087f52000
 .. .. ..0x0000003fffffe000: pte 0x0000000021fd00c7 pa 0x0000000087f40000
 .. .. ..0x0000003ffffff000: pte 0x0000000020001c4b pa 0x0000000080007000
```

### Định Dạng Output

- Dòng đầu tiên hiển thị tham số truyền vào `vmprint`
- Mỗi dòng PTE được thụt lề bằng `" .."` theo độ sâu trong cây
- Mỗi dòng PTE hiển thị: địa chỉ ảo (VA), các bit PTE, và địa chỉ vật lý (PA) được trích xuất từ PTE
- **KHÔNG in các PTE không hợp lệ (invalid)**

### Gợi ý

- Sử dụng các macro ở cuối file `kernel/riscv.h`
- Hàm `freewalk` có thể là nguồn cảm hứng
- Sử dụng `%p` trong `printf` để in đầy đủ 64-bit hex PTEs và địa chỉ

### Câu Hỏi Cần Trả Lời

> Với mỗi trang lá (leaf page) trong output của `vmprint`, giải thích nó chứa gì về mặt logic, các bit quyền là gì, và nó liên quan thế nào với output của bài tập `print_pgtbl()` trước đó.

---

## Phần 4: Sử Dụng Superpages (Moderate/Hard)

**Mục tiêu**: Phần cứng phân trang RISC-V hỗ trợ các trang **2 megabyte** cũng như các trang 4096-byte thông thường. Ý tưởng chung của các trang lớn hơn được gọi là **superpages**, và trang 2MB được gọi là **megapages**.

### Cách Hoạt Động

- Hệ điều hành tạo superpage bằng cách đặt bit `PTE_V` và `PTE_R` trong PTE cấp 1 (level-1)
- Đặt số trang vật lý (physical page number) trỏ đến đầu vùng 2MB của bộ nhớ vật lý
- Địa chỉ vật lý này **phải được căn chỉnh 2MB** (tức là bội số của 2MB)
- Tham khảo: RISC-V privileged manual, trang 112 (tìm "megapage" và "superpage")

### Lợi Ích

- Giảm lượng bộ nhớ vật lý sử dụng cho bảng trang
- Giảm cache miss trong TLB
- Với một số chương trình, điều này dẫn đến tăng hiệu suất đáng kể

### Yêu Cầu Cụ Thể

1. Sửa đổi kernel xv6 để sử dụng superpages
2. Nếu một chương trình người dùng gọi `sbrk()` với kích thước **>= 2 megabytes**, VÀ vùng địa chỉ mới tạo bao gồm một hoặc nhiều vùng được **căn chỉnh 2MB** và có kích thước **>= 2MB**, kernel nên sử dụng **một superpage duy nhất** (thay vì hàng trăm trang thông thường)

### Tiêu Chí Hoàn Thành

Các test case `superpg_fork` và `superpg_free` pass khi chạy `pgtbltest`

### Gợi Ý Chi Tiết

1. **Đọc test trước**: `superpg_fork` và `superpg_free` trong `user/pgtbltest.c`

2. **Điểm khởi đầu**: `sys_sbrk` trong `kernel/sysproc.c`, được gọi bởi system call `sbrk`. Theo dõi đường dẫn mã đến hàm `growproc` - nơi cấp phát bộ nhớ eager cho sbrk

3. **Cấp phát vùng 2MB**:
   - Sửa `kalloc.c` để dành riêng một vài vùng 2MB của bộ nhớ vật lý
   - Tạo các hàm `superalloc()` và `superfree()`
   - Bạn chỉ cần một số ít (handful) các chunk 2MB bộ nhớ

4. **Fork và Exit**:
   - Superpages phải được **cấp phát** khi tiến trình có superpages fork
   - Superpages phải được **giải phóng** khi tiến trình exit
   - Cần sửa `uvmcopy()` và `uvmunmap()`

5. **Demote superpage**:
   - Khi `sbrk` giải phóng **một phần** của superpage (ví dụ: giải phóng 4096 bytes cuối của superpage)
   - Bạn cần **"demote"** (hạ cấp) superpage thành các trang thông thường

### Tài Liệu Tham Khảo Nâng Cao

- [Juan Navarro et al. "Practical, transparent operating system support for superpages." SIGOPS 2002](https://www.usenix.org/conference/osdi-02/practical-transparent-operating-system-support-superpages)
- [A comprehensive analysis of superpage management mechanism and policies (ATC'20)](https://www.usenix.org/conference/atc20/presentation/zhu-weixi)

---

## Nộp Bài

### Thời Gian Làm Bài

Tạo file `time.txt`, ghi một số nguyên duy nhất là số giờ bạn đã dành cho lab này.

### Câu Trả Lời

Viết câu trả lời trong file `answers-pgtbl.txt`. Commit cả hai file.

### Nộp Bài

```bash
$ make grade      # Đảm bảo code pass tất cả tests
$ make zipball    # Tạo file lab.zip để nộp
```

---

## Bài Tập Thử Thách (Optional)

- Triển khai một số ý tưởng từ paper được tham chiếu ở trên để thiết kế superpage thực tế hơn
- Bỏ ánh xạ (unmap) trang đầu tiên của tiến trình người dùng để việc dereference null pointer sẽ gây ra fault. Bạn cần sửa `user.ld` để đoạn text bắt đầu từ, ví dụ, 4096 thay vì 0
- Thêm một system call báo cáo các dirty pages (trang đã sửa đổi) sử dụng `PTE_D`

---

## Tóm Tắt Các Phần

| Phần | Độ Khó | Mô Tả | Files Chính |
|------|--------|-------|-------------|
| Inspect page table | Easy | Giải thích output của `print_pgtbl` | `answers-pgtbl.txt` |
| Speed up syscalls (`ugetpid`) | Easy | Ánh xạ trang USYSCALL với PID | `kernel/proc.c`, `kernel/vm.c` |
| Print page table (`vmprint`) | Easy | In bảng trang theo định dạng | `kernel/vm.c` |
| Superpages | Moderate/Hard | Sử dụng trang 2MB cho sbrk lớn | `kernel/kalloc.c`, `kernel/vm.c`, `kernel/sysproc.c` |

---

## Phụ Lục: PTE Flags Reference

> Trích từ sách xv6 và `kernel/riscv.h`

| Cờ | Bit | Ý Nghĩa |
|----|-----|---------|
| `PTE_V` | 0 | Valid - Trang có hợp lệ/được ánh xạ không? |
| `PTE_R` | 1 | Read - Có được phép đọc trang không? |
| `PTE_W` | 2 | Write - Có được phép ghi trang không? |
| `PTE_X` | 3 | Execute - Có được phép thực thi mã từ trang không? |
| `PTE_U` | 4 | User - Cho phép truy cập từ user mode? |
| `PTE_G` | 5 | Global - Trang global (không flush khi ASID thay đổi) |
| `PTE_A` | 6 | Accessed - Trang đã được truy cập (hardware đặt) |
| `PTE_D` | 7 | Dirty - Trang đã được ghi (hardware đặt) |

### Macros Hữu Ích (kernel/riscv.h)

```c
#define PGSIZE 4096           // Kích thước trang thường
#define PGSHIFT 12            // Số bit offset

#define PA2PTE(pa) ((((uint64)pa) >> 12) << 10)  // PA → PTE
#define PTE2PA(pte) (((pte) >> 10) << 12)        // PTE → PA
#define PTE_FLAGS(pte) ((pte) & 0x3FF)           // Lấy flags từ PTE
```

---

*Tài liệu dịch từ https://pdos.csail.mit.edu/6.1810/2025/labs/pgtbl.html*
