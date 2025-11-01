## 1. 📜 **Quy định chung**

> ❗ Lưu ý quan trọng
> 
> - **Làm việc theo nhóm**: Tối đa **3 sinh viên** một nhóm.
> - **Chính sách về sao chép**: Nếu có sự sao chép bài làm, tất cả các bài tập liên quan sẽ bị **0 điểm** cho toàn bộ các thành viên trong nhóm.
> - **Hệ điều hành**: Môi trường làm việc bắt buộc là **Linux**.

---

## 2. 📬 **Nộp bài**

Việc nộp bài cần tuân thủ nghiêm ngặt các quy định sau để hệ thống có thể chấm điểm tự động.

- **Nền tảng**: Nộp bài trực tiếp trên website của môn học (**MOODLE**). Mọi hình thức nộp bài qua email hay các kênh khác đều không được chấp nhận.
- **Tên file**: `StudentID1_StudentID2_StudentID3.zip`.
    - Các mã số sinh viên (StudentID) phải được sắp xếp theo thứ tự tăng dần.
    - Ví dụ: Nhóm có 2 thành viên `2312001` và `2312003`, tên file sẽ là `2312001_2312003.zip`.
- **Nội dung file .zip**:
    1. 📄 **Báo cáo (Report)**: `StudentID1_StudentID2_StudentID3_Report.pdf`
        - Viết ngắn gọn, súc tích. Không cần chèn mã nguồn vào báo cáo.
        - Đừng tốn quá nhiều công sức hoặc đưa mã nguồn vào bài viết của bạn. Mục đích của báo cáo là giúp bạn làm rõ giải pháp, bất kỳ vấn đề nào trong bài làm của bạn và bổ sung thông tin có thể hữu ích cho việc chấm điểm. Nếu bạn gặp phải những vấn đề cụ thể, những phương pháp bạn đã thử nhưng không hiệu quả, hoặc những khái niệm chưa được triển khai đầy đủ, thì phần giải thích trong báo cáo có thể giúp chúng tôi chấm điểm một phần. Điều này sẽ giúp giảng viên cân nhắc cho điểm một phần (partial credit).
    2. 💾 **Release File vá (Patch file)**:  `StudentID1_StudentID2_StudentID3.patch`
        - Đây là file chứa sự khác biệt giữa mã nguồn của bạn và mã nguồn gốc.
        - Tạo bằng lệnh: `git diff > StudentID1_StudentID2_StudentID3.patch`
    3. 📂 **Mã nguồn (Source code)**: Zip toàn bộ thư mục mã nguồn `xv6` sau khi đã chạy lệnh `make clean`.

---

## 3. 🎤 **Phỏng vấn (Demo Interviews)**

Việc đánh giá project sẽ dựa trên các tiêu chí sau:

- **Tính đầy đủ (Completeness)**: Hoàn thành tất cả các yêu cầu.
- **Tính đúng đắn (Correctness)**: Chương trình chạy đúng, không có lỗi.
- **Phong cách lập trình (Programming style)**: Mã nguồn sạch sẽ, dễ đọc, có chú thích.
- **Kiểm thử (Thoroughness of testing)**: Các trường hợp kiểm thử đầy đủ và hợp lý.
- **Mức độ hiểu mã nguồn (Code understanding)**: Bạn phải hiểu rõ từng dòng code mình viết.

---

## 4. 🛠️ **Hướng dẫn: Thêm một System Call mới vào xv6**

Đây là phần hướng dẫn chi tiết từng bước để tạo một lời gọi hệ thống có tên là `hello`.

### **Giai đoạn 1: Chỉnh sửa trong Không gian Kernel (Kernel Space)**

1. **Bước 1: Định nghĩa số hiệu system call**
Trong file `kernel/syscall.h`, định nghĩa một số hiệu mới.
    
    ```c
    #define SYS_hello 22
    
    ```
    
2. **Bước 2: Khai báo và ánh xạ hàm xử lý**
Trong file `kernel/syscall.c`, khai báo prototype và ánh xạ số hiệu với hàm xử lý.
    
    ```c
    extern int sys_hello(void);
    
    static int (*syscalls[])(void) = {
    ...
    [SYS_hello]   sys_hello,
    };
    
    ```
    
3. **Bước 3: Hiện thực hàm xử lý system call**
Trong file `kernel/sysproc.c`, viết logic chính của system call.
    
    ```c
    int sys_hello(void) {
      printf("Hello, world!\\n");
      return 0;
    }
    
    ```
    

### **Giai đoạn 2: Chỉnh sửa trong Không gian Người dùng (User Space)**

1. **Bước 4: Định nghĩa hàm cho người dùng**
Trong file `user/user.h`, định nghĩa prototype cho các chương trình người dùng gọi.
    
    ```c
    int hello(void);
    
    ```
    
2. **Bước 5: Thêm giao diện gọi system call**
Trong script `user/usys.pl` (tự động tạo mã assembly), thêm tên hàm của bạn.
    
    ```perl
    entry("hello");
    
    ```
    
3. **Bước 6: Viết chương trình kiểm thử**
Tạo file `user/testhello.c` để gọi và kiểm tra system call.
    
    ```c
    #include "user.h"
    
    int main() {
      hello();
      exit(0);
    }
    
    ```
    

### **Giai đoạn 3: Biên dịch và Kiểm thử**

1. **Bước 7: Cập nhật `Makefile`**
Thêm tên chương trình kiểm thử vào biến `UPROGS` trong `Makefile`.
    
    ```makefile
    UPROGS=\\
        $U/_testhello\\
        ...
    
    ```
    
2. **Bước 8: Biên dịch và chạy thử**
    
    ```bash
    $ make qemu
    $ ./testhello
    Hello, world!
    $
    
    ```
    

---

## 5. 📦 **Xử lý tham số và giá trị trả về**

> Tư duy về Ranh giới User-Kernel
> 
> 
> Hãy tưởng tượng chương trình của bạn (User Space) là một khách hàng trong ngân hàng. Kernel là khu vực an toàn phía sau quầy giao dịch.
> 
> - **System Call** giống như bạn điền vào một biểu mẫu và đưa cho giao dịch viên.
> - **Lệnh `ecall`** là hành động bấm chuông gọi giao dịch viên.
> - **`copyin()` / `copyout()`** là các quy trình an ninh: giao dịch viên sao chép thông tin từ biểu mẫu của bạn vào hệ thống (`copyin`) và sao chép kết quả ra giấy đưa cho bạn (`copyout`), chứ không bao giờ cho bạn truy cập trực tiếp vào máy tính của họ.

Làm thế nào để kernel nhận tham số từ người dùng và trả dữ liệu về?

- **Nhận tham số từ User Space**: Kernel không thể truy cập trực tiếp vào bộ nhớ của user space.
    - Sử dụng các hàm `argint()`, `argaddr()`, `fetchaddr()` để lấy các tham số (số nguyên, con trỏ) một cách an toàn. Các hàm này bên trong sẽ gọi `copyin()` để sao chép dữ liệu từ user space vào kernel space.
- **Trả dữ liệu về User Space**:
    - Sử dụng hàm `copyout()` để sao chép dữ liệu từ kernel space ra vùng nhớ của user space mà một con trỏ đã trỏ tới.
- **Ví dụ tham khảo:** Nghiên cứu cách `sys_fstat()` và `sys_exec()` hoạt động.

---

## 6. 🎯 **Yêu cầu của Project**

Trước khi bắt đầu, hãy đọc kỹ Chương 2 và mục 4.3, 4.4 của Chương 4 trong sách xv6.

Đảm bảo bạn đã chuyển sang đúng nhánh `syscall` của project:

```bash
git fetch
git checkout syscall
make clean

```

### **Yêu cầu 1: System Call `trace` (5 điểm)**

**Mô tả:** Hiện thực một system call mới `int trace(int mask)` để theo dõi các lời gọi hệ thống khác.

- **`mask`**: Một số nguyên, mỗi bit trong `mask` tương ứng với một system call. Nếu bit `i` được bật, system call `SYS_... = i` sẽ được theo dõi.
- **Output:** Khi một system call được theo dõi trả về, kernel phải in ra một dòng theo định dạng: `process id: syscall name -> return value`.
- **Kế thừa:** Trạng thái theo dõi (mask) phải được kế thừa bởi các tiến trình con được tạo ra bởi `fork()`.

**Ví dụ kết quả:** (process ID của bạn có thể khác)

1. **Chỉ theo dõi `read`:**
    
    ```bash
    $ trace 32 grep hello README
    3: syscall read -> 1023
    3: syscall read -> 966
    3: syscall read -> 70
    3: syscall read -> 0
    $
    $ trace 2147483647 grep hello README
    4: syscall trace -> 0
    4: syscall exec -> 3
    4: syscall open -> 3
    4: syscall read -> 1023
    4: syscall read -> 966
    4: syscall read -> 70
    4: syscall read -> 0
    4: syscall close -> 0
    $
    $ grep hello README
    $ 
    ```
    
    *(Giải thích: `32` là `1 << SYS_read`)*
    
    Ở ví dụ trên, `32` là `1 << SYS_read`, nên chỉ có lời gọi hệ thống `read` được theo dõi.
    
2. **Theo dõi và kế thừa qua `fork`:**
    
    ```bash
    $ trace 2 usertests forkforkfork
    usertests starting 
    test forkforkfork: 407: syscall fork -> 408 
    408: syscall fork -> 409 
    409: syscall fork -> 410 
    410: syscall fork -> 411 
    409: syscall fork -> 412 
    410: syscall fork -> 413 
    409: syscall fork -> 414 
    411: syscall fork -> 415 
    ... 
    $
    ```
    

💡 **Gợi ý:**

1. **Viết `user/trace.c`**: Chương trình này sẽ nhận `mask` từ `argv[1]`, sau đó gọi `trace(mask)`. Cuối cùng, nó dùng `exec` để thực thi lệnh được chỉ định trong các tham số còn lại (`argv[2]`, `argv[3]`, ...).
2. **Cập nhật Makefile**: Thêm `$U/_trace` vào `UPROGS`.
3. **Thêm các file cần thiết**: Bổ sung prototype vào `user/user.h`, entry vào `user/usys.pl`, và định nghĩa `SYS_trace` trong `kernel/syscall.h`. Khi biên dịch lại, bạn sẽ thấy lỗi vì chưa cài đặt `sys_trace` trong kernel.
4. **Cài đặt `sys_trace`**: Trong `kernel/sysproc.c`, viết hàm `sys_trace()`. Hàm này sẽ lấy tham số `mask` bằng `argint()` và lưu nó vào một trường mới (ví dụ: `tracemask`) trong `struct proc` (định nghĩa trong `kernel/proc.h`).
5. **Kế thừa mask**: Sửa hàm `fork()` trong `kernel/proc.c` để sao chép giá trị `tracemask` từ tiến trình cha sang tiến trình con.
6. **In thông tin trace**: Sửa hàm `syscall()` trong `kernel/syscall.c`. Sau khi một system call thực thi xong (sau `p->trapframe->a0 = ...`), kiểm tra xem bit tương ứng trong `p->tracemask` có được bật hay không. Nếu có, in ra thông tin theo định dạng yêu cầu. Bạn sẽ cần một mảng chứa tên của các system call để in ra.

### **Yêu cầu 2: System Call `sysinfo` (5 điểm)**

**Mô tả:** Bạn sẽ thêm một system call tên là `int sysinfo(struct sysinfo *info)` để thu thập thông tin hệ thống.

**Cấu trúc Dữ liệu:**

- **Prototype**: `int sysinfo(struct sysinfo *info)`
- **Chức năng**:
    - Lời gọi hệ thống này nhận một con trỏ đến `struct sysinfo`.
    - Kernel sẽ điền thông tin vào struct này, bao gồm:
        - `freemem`: Lượng bộ nhớ trống (tính bằng byte).
        - `nproc`: Số lượng tiến trình đang được sử dụng (trạng thái khác `UNUSED`).
        - `nopenfiles`: Tổng số file đang được mở trên toàn hệ thống.
- **Định nghĩa `struct sysinfo`**: Bạn cần tạo một file header mới, ví dụ `kernel/sysinfo.h`, và định nghĩa struct này trong đó.
- **Chương trình người dùng**: Viết chương trình `user/sysinfotest.c` để kiểm tra.

**Hướng dẫn thực hiện (Gợi ý):**

1. **Chuẩn bị:** Viết chương trình test `user/sysinfotest.c`, thêm vào `Makefile`, và định nghĩa giao diện cho `sysinfo` trên toàn hệ thống.
2. **Khai báo prototype trong `user/user.h`:** Bạn cần khai báo "forward declaration" trước khi dùng struct trong prototype:
    
    ```c
    struct sysinfo;
    int sysinfo(struct sysinfo *);
    
    ```
    
3. **Hiện thực `sys_sysinfo()`:**
    - **`freemem`:** Viết một hàm trợ giúp mới trong `kernel/kalloc.c` để duyệt qua danh sách liên kết `freelist` và đếm số trang nhớ trống.
    - **`nproc`:** Viết một hàm trợ giúp trong `kernel/proc.c` để duyệt qua mảng `proc` và đếm các tiến trình có trạng thái khác `UNUSED`.
    - **`nopenfiles`:** Cách tiếp cận hiệu quả là duyệt qua bảng file toàn cục `ftable` trong `kernel/file.c`. Mỗi `struct file` có một trường đếm tham chiếu (`ref`). Đếm số lượng `struct file` có `ref > 0`.
4. **Trả dữ liệu:** Sau khi thu thập thông tin vào một biến `struct sysinfo` cục bộ trong kernel, sử dụng `copyout()` để sao chép dữ liệu này về con trỏ mà người dùng đã cung cấp.

---

## 7. 💯 **Thang điểm**

| **No.** | **Bài tập** | **Điểm** |
| --- | --- | --- |
| 1 | `trace` | 5 |
| 2 | `sysinfo` | 5 |

---

## 8. 📚 **Tài liệu tham khảo**

- [MIT 6.1810: System calls lab](https://pdos.csail.mit.edu/6.1810/2025/labs/syscall.html)
- [Lab1-syscall](https://drive.google.com/file/d/1_Re2VC0AwY5OdIyIaGZbNhS1NNuit7nx/view?usp=sharing)

## **9. Lời khuyên và Chiến lược Gỡ lỗi**

Lập trình kernel có thể đầy thách thức. Dưới đây là một số chiến lược để giúp bạn:

- **Làm từng bước nhỏ:** Đừng cố gắng hoàn thành toàn bộ một system call trong một lần. Sau mỗi thay đổi nhỏ, hãy biên dịch lại để đảm bảo không có lỗi cú pháp.
- **Sử dụng `printf` một cách thông minh:** `printf` là công cụ gỡ lỗi mạnh mẽ nhất trong xv6. Hãy đặt các lệnh in ở những vị trí quan trọng để theo dõi luồng thực thi và giá trị của các biến.
- **Hiểu các lỗi Kernel Panic:** Khi xv6 bị sập (panic), nó sẽ in ra một thông báo lỗi. Hãy đọc kỹ thông báo đó, nó thường cho bạn biết lỗi xảy ra ở file nào và vì sao.
- **Sử dụng Git thường xuyên:** Sau mỗi khi hoàn thành một chức năng nhỏ và chạy đúng, hãy `git commit` lại. Nếu bạn làm hỏng hệ thống, bạn luôn có thể quay lại phiên bản ổn định trước đó.
- **Đọc mã nguồn có sẵn:** Cách tốt nhất để biết cách làm một việc gì đó trong xv6 là tìm một system call có sẵn có chức năng tương tự và nghiên cứu cách nó được hiện thực.