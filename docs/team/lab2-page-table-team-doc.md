# HỆ ĐIỀU HÀNH
# PROJECT 2 – Bảng Trang (Page Tables)

## 1. Quy định chung:

- Đồ án được thực hiện theo nhóm: mỗi nhóm tối đa **3 sinh viên**.
- **Các bài tập giống nhau sẽ bị chấm 0 điểm cho toàn bộ bài thực hành (mặc dù có điểm cho các bài tập và đồ án thực hành khác).**
- Môi trường lập trình: **Linux**

## 2. Nộp bài:

***Nộp bài tập trực tiếp trên trang web môn học (MOODLE), không chấp nhận nộp qua email hoặc các hình thức khác.***

Tên tệp: **StudentID1_StudentID2_StudentID3.zip** (với StudentID1 < StudentID2 < StudentID3)

Ví dụ: Nhóm của bạn có 3 sinh viên: 2312001, 2312002 và 2312003, tên tệp là: **2312001_2312002_2312003.zip**

**Bao gồm:**

*   **StudentID1_StudentID2_StudentID3_Report.pdf:** Các bài viết (báo cáo) nên ngắn gọn và súc tích. Đừng tốn quá nhiều công sức hoặc đưa mã nguồn vào bài viết của bạn. Mục đích của báo cáo là cho bạn cơ hội để làm rõ giải pháp của mình, bất kỳ vấn đề nào với công việc của bạn, và thêm thông tin có thể hữu ích cho việc chấm điểm. Nếu bạn gặp vấn đề hoặc sự cố cụ thể, các hướng tiếp cận bạn đã thử nhưng không hiệu quả, hoặc các khái niệm chưa được thực hiện đầy đủ, thì một lời giải thích trong báo cáo của bạn có thể giúp chúng tôi cho điểm thành phần.
*   **Release:** Tệp diff (bản vá diff, Ví dụ: `$ git diff > <StudentID1_StudentID2_StudentID3>.patch`)
*   **Source:** Tệp Zip của xv6 (phiên bản đã được làm sạch - make clean)

***Lưu ý: Cần thực hiện đúng các yêu cầu trên, nếu không, bài làm sẽ không được chấm.***

---

## 3. Phỏng vấn Demo

Việc thực hiện của bạn được chấm điểm dựa trên sự hoàn thiện, tính đúng đắn, phong cách lập trình, sự kỹ lưỡng trong kiểm thử, giải pháp của bạn và sự hiểu biết về mã nguồn.

Khi quản lý khóa học này, chúng tôi cố gắng hết sức để đưa ra đánh giá công bằng cho từng cá nhân dựa trên đóng góp của mỗi người vào dự án.

## 4. Các yêu cầu

Trong bài lab này, bạn sẽ khám phá các bảng trang (page tables) và sửa đổi chúng để tăng tốc một số system calls (lời gọi hệ thống) nhất định và để phát hiện những trang nào đã được truy cập.

Trước khi bắt đầu viết code, hãy đọc Chương 3 của [sách xv6](https://pdos.csail.mit.edu/6.828/2023/xv6/book-riscv-rev3.pdf), và các tệp liên quan:

*   `kernel/memlayout.h`, tệp này nắm giữ bố cục của bộ nhớ.
*   `kernel/vm.c`, tệp này chứa hầu hết code về bộ nhớ ảo (VM).
*   `kernel/kalloc.c`, tệp này chứa code để cấp phát và giải phóng bộ nhớ vật lý.

Cũng có thể sẽ hữu ích khi tham khảo [tài liệu hướng dẫn kiến trúc đặc quyền RISC-V](https://github.com/riscv/riscv-isa-manual/releases/download/Draft-20201127-26a57d0/riscv-privileged-20211203.pdf).

Để bắt đầu bài lab, hãy chuyển sang nhánh `pgtbl`:

```bash
$ git fetch
$ git checkout pgtbl
$ make clean
```

### In một bảng trang (Print a page table)

Để giúp bạn hình dung các bảng trang RISC-V, và có lẽ để hỗ trợ gỡ lỗi sau này, nhiệm vụ này là viết một hàm in ra nội dung của một bảng trang.

Định nghĩa một hàm có tên là `vmprint()`. Nó sẽ nhận một đối số `pagetable_t`, và in bảng trang đó theo định dạng được mô tả dưới đây.

```text
page table 0x0000000087f6b000
..0: pte 0x0000000021fd9c01 pa 0x0000000087f67000
.. ..0: pte 0x0000000021fd9801 pa 0x0000000087f66000
.. .. ..0: pte 0x0000000021fda01b pa 0x0000000087f68000
.. .. ..1: pte 0x0000000021fd9417 pa 0x0000000087f65000
.. .. ..2: pte 0x0000000021fd9007 pa 0x0000000087f64000
.. .. ..3: pte 0x0000000021fd8c17 pa 0x0000000087f63000
..255: pte 0x0000000021fda801 pa 0x0000000087f6a000
.. ..511: pte 0x0000000021fda401 pa 0x0000000087f69000
.. .. ..509: pte 0x0000000021fdcc13 pa 0x0000000087f73000
.. .. ..510: pte 0x0000000021fdd007 pa 0x0000000087f74000
.. .. ..511: pte 0x0000000020001c0b pa 0x0000000080007000
init: starting sh
```

Dòng đầu tiên hiển thị đối số truyền vào `vmprint`. Sau đó là một dòng cho mỗi PTE (Page Table Entry), bao gồm các PTE tham chiếu đến các trang bảng trang (page-table pages) nằm sâu hơn trong cây. Mỗi dòng PTE được thụt lề bằng một số lượng chuỗi " .." biểu thị độ sâu của nó trong cây. Mỗi dòng PTE hiển thị chỉ mục PTE trong trang bảng trang của nó, các bit pte, và địa chỉ vật lý được trích xuất từ PTE. Đừng in các PTE không hợp lệ (not valid). Trong ví dụ trên, trang bảng trang cấp cao nhất có các ánh xạ cho mục 0 và 255. Cấp tiếp theo xuống cho mục 0 chỉ có chỉ mục 0 được ánh xạ, và cấp dưới cùng cho chỉ mục 0 đó có các mục 0, 1, và 2 được ánh xạ.

Bạn phải cung cấp một cách để in ra bảng trang của tiến trình khi nó được thực thi mà không làm ảnh hưởng đến system call `exec` hiện có. Điều này có nghĩa là system call `exec` hiện tại vẫn được sử dụng để thực thi một chương trình bình thường và bảng trang của tiến trình chỉ được in ra khi người dùng muốn.

Một số gợi ý:

*   Bạn nên kiểm tra mã nguồn của system call `exec` trong `kernel/sysfile.c` và hàm `exec()` trong `kernel/exec.c`.
*   Hàm `walk()` và `freewalk()` trong `kernel/vm.c` có thể truyền cảm hứng.
*   Bạn có thể đặt `vmprint()` trong `kernel/vm.c`.
*   `vmprint(p->pagetable)` nên được gọi trong `exec.c` ngay trước khi trả về `argc`.
*   Sử dụng các macro ở cuối tệp `kernel/riscv.h`.
*   Định nghĩa nguyên mẫu (prototype) cho `vmprint` trong `kernel/defs.h` để bạn có thể gọi nó từ `exec.c`.
*   Sử dụng `%p` trong các lệnh gọi `printf` của bạn để in ra đầy đủ các PTE hex 64-bit và các địa chỉ như trong ví dụ.

Đối với mỗi trang lá (leaf page) trong đầu ra của `vmprint`, hãy giải thích những gì nó chứa về mặt logic và các bit cấp quyền của nó là gì. Hình 3.4 trong sách xv6 có thể hữu ích, mặc dù lưu ý rằng hình đó có thể có tập hợp các trang hơi khác so với tiến trình `init` đang được kiểm tra ở đây.

### Phát hiện những trang nào đã được truy cập (Detect which pages have been accessed)

Một số trình thu gom rác (một dạng quản lý bộ nhớ tự động) có thể hưởng lợi từ thông tin về việc những trang nào đã được truy cập (đọc hoặc ghi). Trong phần này của bài lab, bạn sẽ thêm một tính năng mới vào xv6 để phát hiện và báo cáo thông tin này đến không gian người dùng (userspace) bằng cách kiểm tra các bit truy cập trong bảng trang RISC-V. Phần cứng RISC-V page walker đánh dấu các bit này trong PTE bất cứ khi nào nó giải quyết một lần trượt TLB (TLB miss).

Công việc của bạn là hiện thực `pgaccess()`, một system call báo cáo những trang nào đã được truy cập. System call này nhận ba đối số. Thứ nhất, nó nhận địa chỉ ảo bắt đầu của trang người dùng đầu tiên cần kiểm tra. Thứ hai, nó nhận số lượng trang cần kiểm tra. Cuối cùng, nó nhận một địa chỉ người dùng đến một bộ đệm (buffer) để lưu kết quả vào một bitmask (một cấu trúc dữ liệu sử dụng một bit cho mỗi trang và trang đầu tiên tương ứng với bit có trọng số thấp nhất - least significant bit). Bạn sẽ nhận được trọn điểm cho phần này của bài lab nếu test case `pgaccess` vượt qua khi chạy `pgtbltest`.

Một số gợi ý:

*   Đọc `pgaccess_test()` trong `user/pgtbltest.c` để xem `pgaccess` được sử dụng như thế nào.
*   Bắt đầu bằng cách hiện thực `sys_pgaccess()` trong `kernel/sysproc.c`.
*   Bạn sẽ cần phân tích các đối số bằng cách sử dụng `argaddr()` và `argint()`.
*   Đối với bitmask đầu ra, dễ dàng hơn nếu lưu trữ một bộ đệm tạm thời trong kernel và sao chép nó sang người dùng (thông qua `copyout()`) sau khi điền vào đó các bit đúng.
*   Bạn có thể đặt giới hạn trên cho số lượng trang có thể được quét.
*   `walk()` trong `kernel/vm.c` rất hữu ích để tìm đúng các PTE.
*   Bạn sẽ cần định nghĩa `PTE_A`, bit truy cập, trong `kernel/riscv.h`. Tham khảo [tài liệu kiến trúc đặc quyền RISC-V](https://github.com/riscv/riscv-isa-manual/releases/download/Draft-20201127-26a57d0/riscv-privileged-20211203.pdf) để xác định giá trị của nó.
*   Hãy chắc chắn xóa `PTE_A` sau khi kiểm tra xem nó có được thiết lập hay không. Nếu không, sẽ không thể xác định liệu trang có được truy cập kể từ lần cuối cùng `pgaccess()` được gọi hay không (tức là, bit sẽ được thiết lập mãi mãi).
*   `vmprint()` có thể hữu ích để gỡ lỗi các bảng trang.

## 5. Điểm (Grade)

| Số | Bài tập | Điểm |
| :--- | :--- | :--- |
| 1 | Print a page table (In một bảng trang) | 5 |
| 2 | Detect which pages have been accessed (Phát hiện trang đã truy cập) | 5 |

## 6. Tài liệu tham khảo (Reference)

*   [https://pdos.csail.mit.edu/6.1810/2023/labs/pgtbl.html](https://pdos.csail.mit.edu/6.1810/2023/labs/pgtbl.html)