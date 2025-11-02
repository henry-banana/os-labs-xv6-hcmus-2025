<aside>
💡

Tham khảo (1.0, 1.1, 1.2, 1.4, 2.5, 2.6, 3.4, 3.5, 3.7, 4.3, 4.4, 8.13)

</aside>

# 📝**Hệ điều hành xv6**

### *Một hệ điều hành giảng dạy đơn giản, giống Unix*

## **Lời nói đầu và Lời cảm ơn**

Tài liệu này được biên soạn dựa trên cuốn sách về **xv6**, một hệ điều hành được thiết kế với mục đích giảng dạy. Xv6 được mô phỏng theo Unix Version 6 (v6) của Dennis Ritchie và Ken Thompson, nhưng được triển khai bằng ngôn ngữ ANSI C cho kiến trúc đa lõi RISC-V. Mục tiêu chính là cung cấp một hệ thống đủ đơn giản để sinh viên có thể nghiên cứu song song giữa lý thuyết và mã nguồn thực tế, một phương pháp đã được áp dụng thành công tại Viện Công nghệ Massachusetts (MIT).

Các tác giả của tài liệu gốc gửi lời cảm ơn đến rất nhiều người đã đóng góp phản hồi, báo cáo lỗi và đề xuất cải tiến, góp phần hoàn thiện cuốn sách và hệ điều hành xv6.

---

# **Chương 1: Giao diện Hệ điều hành**

Vai trò cơ bản của một hệ điều hành (HĐH) là quản lý và chia sẻ tài nguyên phần cứng của máy tính giữa nhiều chương trình khác nhau. HĐH trừu tượng hóa các dịch vụ cấp thấp của phần cứng, ví dụ như giúp một trình soạn thảo văn bản không cần phải quan tâm đến loại ổ đĩa đang được sử dụng. Quan trọng hơn, HĐH cung cấp một giao diện được kiểm soát để các chương trình người dùng có thể yêu cầu các dịch vụ của HĐH thông qua các **lời gọi hệ thống (system calls)**.

Dựa trên cấu trúc truyền thống của một HĐH, chúng ta có các khái niệm sau:

- **Kernel (Nhân):** Là một chương trình đặc biệt, cung cấp các dịch vụ cho các chương trình đang chạy. Kernel thực thi ở chế độ đặc quyền (**kernel space**) và có toàn quyền truy cập vào phần cứng.
- **Tiến trình (Process):** Là một chương trình đang chạy, bao gồm bộ nhớ chứa mã lệnh, dữ liệu và một ngăn xếp (stack). Mỗi tiến trình thực thi trong không gian riêng của nó, được gọi là **user space**, nơi các quyền hạn bị giới hạn để đảm bảo sự cô lập và an toàn.
- **Lời gọi hệ thống (System call):** Là cơ chế duy nhất để một tiến trình chuyển từ `user space` sang `kernel space` để yêu cầu một dịch vụ từ kernel. Kernel thực hiện dịch vụ đó và sau đó trả quyền điều khiển về cho tiến trình.

### **1.1. Tiến trình và Bộ nhớ (Processes and memory)**

Trong xv6, một tiến trình bao gồm bộ nhớ không gian người dùng và trạng thái riêng tư được kernel quản lý. Kernel sử dụng cơ chế chia sẻ thời gian (time-sharing) để chuyển đổi CPU giữa các tiến trình. Các lời gọi hệ thống sau đây là nền tảng cho việc quản lý tiến trình và bộ nhớ:

- `fork()`: Tạo ra một tiến trình mới (tiến trình con) bằng cách sao chép chính xác bộ nhớ của tiến trình đang gọi (tiến trình cha).
    - Trong tiến trình cha, `fork()` trả về **PID** (Process Identifier) của tiến trình con.
    - Trong tiến trình con, `fork()` trả về **0**.
    - Tiến trình cha và con có không gian bộ nhớ và thanh ghi riêng biệt; việc thay đổi một biến trong tiến trình này không ảnh hưởng đến tiến trình kia.
- `exit(status)`: Chấm dứt tiến trình đang thực thi và giải phóng các tài nguyên. Tham số `status` được trả về cho tiến trình cha đang chờ. Lời gọi này không bao giờ trở về.
- `wait(status)`: Cho phép tiến trình cha đợi một trong các tiến trình con của nó kết thúc. Lời gọi này trả về PID của tiến trình con đã kết thúc và sao chép trạng thái kết thúc của con vào địa chỉ được trỏ bởi `status`.
- `exec(file, argv)`: Thay thế hoàn toàn bộ nhớ của tiến trình hiện tại bằng một chương trình mới được tải từ một tệp. Nó không quay trở lại chương trình gọi mà bắt đầu thực thi từ điểm vào của chương trình mới.
    
    > 💡 Tại sao fork() và exec() lại tách biệt?
    Việc tách hai chức năng này là một thiết kế cực kỳ mạnh mẽ. Nó cho phép shell (tiến trình cha) có cơ hội thiết lập môi trường cho tiến trình con sau khi fork() nhưng trước khi exec(). Ví dụ kinh điển nhất là chuyển hướng I/O, nơi shell có thể đóng các file descriptor chuẩn và mở các tệp khác thay thế trước khi thực thi lệnh mới.
    > 
- `sbrk(n)`: Tăng bộ nhớ dữ liệu của tiến trình thêm `n` byte trong thời gian chạy, thường được sử dụng bởi các hàm cấp phát bộ nhớ như `malloc`.

### **1.2. I/O và Mô tả tệp (I/O and File descriptors)**

- **Mô tả tệp (File descriptor)** là một số nguyên nhỏ đại diện cho một đối tượng do kernel quản lý mà tiến trình có thể đọc hoặc ghi. Nó là một sự trừu tượng hóa mạnh mẽ, che giấu sự khác biệt giữa tệp, đường ống (pipe) và thiết bị, biến chúng thành các **luồng byte (stream of bytes)**.
- **Theo quy ước:**
    - Mô tả tệp `0`: Đầu vào chuẩn (standard input)
    - Mô tả tệp `1`: Đầu ra chuẩn (standard output)
    - Mô tả tệp `2`: Đầu ra lỗi chuẩn (standard error)
- **Các lời gọi hệ thống chính cho I/O:**
    - `read(fd, buf, n)`: Đọc tối đa `n` byte từ mô tả tệp `fd` vào bộ đệm `buf`.
    - `write(fd, buf, n)`: Ghi `n` byte từ bộ đệm `buf` vào mô tả tệp `fd`.
    - `close(fd)`: Giải phóng một mô tả tệp, làm cho nó có thể được tái sử dụng.
- **Chuyển hướng I/O (I/O redirection):** `fork()` sao chép bảng mô tả tệp của cha cho con. `exec()` giữ nguyên bảng mô tả tệp. Sự kết hợp này cho phép shell dễ dàng thực hiện chuyển hướng. Ví dụ, để chạy lệnh `cat < input.txt`:
    
    ```c
    if(fork() == 0) {
      close(0); // Đóng standard input
      open("input.txt", O_RDONLY); // Mở input.txt, nó sẽ nhận fd thấp nhất còn trống là 0
      exec("cat", argv); // `cat` bây giờ sẽ đọc từ input.txt
    }
    
    ```
    
- `dup(fd)`: Sao chép một mô tả tệp hiện có, trả về một mô tả tệp mới cùng trỏ đến một đối tượng I/O. Cả hai mô tả tệp này chia sẻ cùng một con trỏ vị trí (offset) trong tệp.

### **1.4. Hệ thống tệp (File system)**

- Hệ thống tệp của xv6 có cấu trúc phân cấp gồm các tệp dữ liệu và thư mục, bắt đầu từ thư mục gốc (`/`).
- **Inode và Link:** Tên của một tệp là riêng biệt với chính tệp đó. Cùng một tệp cơ sở, được gọi là một **inode**, có thể có nhiều tên, được gọi là các **link (liên kết)**. Một inode chứa siêu dữ liệu (metadata) về một tệp, bao gồm loại (tệp, thư mục, thiết bị), kích thước, số lượng liên kết (`nlink`), và vị trí các khối dữ liệu trên đĩa.
- **Các lời gọi hệ thống liên quan:**
    - `chdir(dir)`: Thay đổi thư mục làm việc hiện tại của tiến trình.
    - `mkdir(dir)`: Tạo một thư mục mới.
    - `mknod(file, major, minor)`: Tạo một tệp thiết bị đặc biệt, được xác định bởi số hiệu major và minor.
    - `fstat(fd, struct stat *st)`: Lấy thông tin từ inode của một mô tả tệp đang mở và điền vào cấu trúc `stat`.
    - `link(old, new)`: Tạo một tên mới (`new`) cho cùng một inode của `old`. Tăng `nlink` của inode lên 1.
    - `unlink(path)`: Xóa một tên khỏi hệ thống tệp. Giảm `nlink` của inode. Inode và các khối dữ liệu của nó chỉ được giải phóng khi `nlink` về 0 và không còn mô tả tệp nào đang mở trỏ đến nó.

---

# **Chương 2: Tổ chức Hệ điều hành**

Chương này chuyển từ việc xem xét "cái gì" (giao diện) sang "như thế nào" (tổ chức bên trong). Một hệ điều hành phải đáp ứng ba yêu cầu cốt lõi: **ghép kênh (multiplexing)** tài nguyên, đảm bảo **sự cô lập (isolation)**, và cho phép **tương tác (interaction)** một cách có kiểm soát.

### **2.1. Trừu tượng hóa tài nguyên vật lý**

Tại sao chúng ta cần HĐH thay vì chỉ liên kết các ứng dụng với một thư viện cung cấp các hàm system call?

- **Cách tiếp cận thư viện:** Mỗi ứng dụng có thể tương tác trực tiếp với phần cứng. Điều này có thể mang lại hiệu năng cao nhưng đòi hỏi các ứng dụng phải "hợp tác" tốt với nhau (ví dụ: tự nguyện nhường CPU).
- **Nhược điểm:** Nếu có nhiều hơn một ứng dụng, cách tiếp cận này rất mong manh. Một ứng dụng lỗi có thể làm treo toàn bộ hệ thống. Các ứng dụng cũng không tin tưởng lẫn nhau.
- **Cách tiếp cận HĐH (trừu tượng hóa):**
    1. **Cô lập mạnh mẽ:** HĐH ngăn các ứng dụng truy cập trực tiếp vào phần cứng nhạy cảm. Thay vào đó, nó cung cấp các dịch vụ trừu tượng. Ví dụ, thay vì để ứng dụng đọc/ghi trực tiếp vào sector của ổ đĩa, HĐH cung cấp hệ thống tệp với các lời gọi `open`, `read`, `write`.
    2. **Tiện lợi:** Các trừu tượng hóa như hệ thống tệp, bộ nhớ ảo, và file descriptor giúp lập trình viên dễ dàng hơn.
    3. **Quản lý tài nguyên:** HĐH có thể quản lý việc chia sẻ tài nguyên một cách công bằng và hiệu quả (ví dụ: chia sẻ thời gian CPU, quản lý bộ nhớ khi bộ nhớ vật lý bị hạn hẹp).

### **2.2. Chế độ người dùng, Chế độ giám sát và System Calls**

Sự cô lập mạnh mẽ đòi hỏi một ranh giới cứng giữa ứng dụng và HĐH, được hỗ trợ bởi phần cứng CPU.

- **Các chế độ thực thi (RISC-V):**
    - **Machine mode:** Đặc quyền cao nhất, dùng để khởi động.
    - **Supervisor mode (kernel space):** Cho phép thực thi các lệnh đặc quyền (ví dụ: bật/tắt ngắt). Kernel chạy ở chế độ này.
    - **User mode (user space):** Quyền hạn bị giới hạn. Ứng dụng chạy ở chế độ này.
- **Cơ chế Trap:** Nếu một ứng dụng ở chế độ người dùng cố gắng thực hiện một lệnh đặc quyền, CPU sẽ không thực thi lệnh đó mà thay vào đó sẽ tạo ra một **trap**, chuyển quyền điều khiển sang kernel ở chế độ giám sát để xử lý.
- **Lệnh `ecall`:** Là một lệnh đặc biệt mà ứng dụng sử dụng để yêu cầu một dịch vụ từ kernel một cách có kiểm soát. Nó tạo ra một trap, chuyển sang chế độ giám sát tại một điểm vào duy nhất do kernel định nghĩa, đảm bảo kernel luôn kiểm soát được quá trình chuyển đổi.

### **2.3. Tổ chức Kernel**

- **Kernel nguyên khối (Monolithic kernel):** Toàn bộ HĐH nằm trong kernel và chạy ở chế độ giám sát. Ưu điểm là hiệu năng cao do tích hợp chặt chẽ. Nhược điểm là một lỗi nhỏ có thể làm sập toàn bộ hệ thống. (Ví dụ: Linux, xv6).
- **Microkernel:** Chỉ một phần nhỏ mã HĐH chạy ở chế độ giám sát. Phần lớn các dịch vụ (hệ thống tệp, mạng) chạy dưới dạng các tiến trình người dùng (`server`). Ưu điểm là độ tin cậy cao hơn. Nhược điểm là hiệu năng có thể thấp hơn do chi phí giao tiếp giữa các tiến trình (IPC).

### **2.5. Tổng quan về tiến trình (Process overview)**

Trong xv6, tiến trình là đơn vị của sự cô lập. Nó cung cấp cho chương trình ảo giác rằng nó có một cỗ máy riêng.

- **Không gian địa chỉ (Address space):** Mỗi tiến trình có một hệ thống bộ nhớ riêng, ngăn các tiến trình khác đọc hoặc ghi vào bộ nhớ của nó. Kernel sử dụng **bảng trang (page tables)** do phần cứng cung cấp để ánh xạ địa chỉ ảo (virtual address) mà lệnh sử dụng tới địa chỉ vật lý (physical address) trong RAM.
    - Không gian địa chỉ của một tiến trình trong xv6 bắt đầu từ địa chỉ ảo 0 và có thể phát triển lên đến `MAXVA` (`0x3fffffffff`). Nó bao gồm mã lệnh, dữ liệu toàn cục, ngăn xếp, và vùng heap.
- **Luồng (Thread):** Mỗi tiến trình có một luồng thực thi (thread of control), tạo ra ảo giác rằng nó có CPU của riêng mình.
- **Trạng thái Kernel (`struct proc`):** Trạng thái của một tiến trình được lưu trong cấu trúc `struct proc`, bao gồm các thành phần quan trọng:
    - `p->pagetable`: Con trỏ tới bảng trang của tiến trình, định nghĩa không gian địa chỉ của nó.
    - `p->kstack`: Con trỏ tới đỉnh của **ngăn xếp kernel**. Khi một tiến trình thực hiện lời gọi hệ thống hoặc bị ngắt, kernel sẽ thực thi trên ngăn xếp này. Việc tách biệt ngăn xếp kernel và ngăn xếp người dùng là cực kỳ quan trọng: kernel có thể hoạt động an toàn ngay cả khi tiến trình đã làm hỏng ngăn xếp người dùng của chính nó.
    - `p->state`: Trạng thái của tiến trình (ví dụ: `UNUSED`, `SLEEPING`, `RUNNABLE`, `RUNNING`, `ZOMBIE`).

### **2.6. Mã nguồn: Khởi động xv6 và tiến trình đầu tiên**

Quá trình khởi động của xv6 diễn ra theo các bước sau:

1. Khi máy RISC-V được bật nguồn, nó chạy một **boot loader** từ ROM. Boot loader này tải kernel xv6 vào bộ nhớ tại địa chỉ vật lý `0x80000000`.
2. CPU bắt đầu thực thi kernel tại `_entry` (`kernel/entry.S`) ở **chế độ máy (machine mode)**, với phân trang bị vô hiệu hóa.
3. `_entry` thiết lập một ngăn xếp ban đầu và gọi hàm C `start` (`kernel/start.c`).
4. Hàm `start` thực hiện một số cấu hình chỉ cho phép ở chế độ máy, sau đó chuyển sang **chế độ giám sát (supervisor mode)** và chuyển quyền điều khiển đến hàm `main` (`kernel/main.c`).
5. Hàm `main` khởi tạo các thiết bị và hệ thống con, sau đó gọi `userinit` (`kernel/proc.c`) để tạo tiến trình người dùng đầu tiên.
6. `userinit` tạo một tiến trình, thiết lập không gian địa chỉ của nó để thực thi một đoạn mã hợp ngữ nhỏ là `initcode.S`.
7. `initcode.S` chứa mã để thực hiện lời gọi hệ thống đầu tiên: `exec("/init")`. Nó đặt số hiệu `SYS_EXEC` vào thanh ghi `a7` và gọi lệnh `ecall`.
8. Kernel xử lý lời gọi `exec`, thay thế bộ nhớ của tiến trình bằng chương trình `/init`.
9. Khi `exec` hoàn tất, nó trở về không gian người dùng và bắt đầu chạy `/init`. Tiến trình `init` (`user/init.c`) tạo một thiết bị console và khởi động shell. Tại thời điểm này, hệ thống đã sẵn sàng hoạt động.

---

# **Chương 3: Bảng trang (Page tables)**

Bảng trang là cơ chế phần cứng mà hệ điều hành sử dụng để triển khai sự cô lập và ghép kênh không gian địa chỉ.

### **3.1. Phần cứng phân trang (Paging hardware)**

- **Địa chỉ ảo vs. Địa chỉ vật lý:** Các lệnh của CPU thao tác trên **địa chỉ ảo**. Bộ nhớ RAM vật lý được truy cập bằng **địa chỉ vật lý**. Phần cứng quản lý bộ nhớ (MMU) dịch từ địa chỉ ảo sang địa chỉ vật lý.
- **Kiến trúc Sv39 RISC-V:**
    - Địa chỉ ảo dài 64-bit, nhưng chỉ 39 bit thấp nhất được sử dụng.
    - Bảng trang được tổ chức dưới dạng **cây ba cấp**.
    - Một địa chỉ ảo 39-bit được chia thành:
        - 27 bit cao nhất để làm chỉ số.
        - 12 bit thấp nhất là **offset** bên trong một trang 4096-byte.
    - 27 bit chỉ số được chia thành ba phần 9-bit, tương ứng để tra cứu trong 3 cấp của cây bảng trang (L2, L1, L0).
- **Mục Bảng Trang (Page Table Entry - PTE):**
    - Là một phần tử trong bảng trang.
    - Chứa **Số trang vật lý (Physical Page Number - PPN)** (44 bit) và các **cờ (flags)** (10 bit).
    - **Quá trình dịch địa chỉ:** Phần cứng kết hợp PPN từ PTE với offset 12-bit từ địa chỉ ảo để tạo ra địa chỉ vật lý 56-bit cuối cùng.
- **Các cờ PTE quan trọng:**
    - `PTE_V` (Valid): Cho biết PTE có hợp lệ hay không. Nếu không, truy cập sẽ gây ra **lỗi trang (page fault)**.
    - `PTE_R` (Readable): Cho phép đọc.
    - `PTE_W` (Writable): Cho phép ghi.
    - `PTE_X` (Executable): Cho phép thực thi mã lệnh.
    - `PTE_U` (User): Cho phép truy cập từ chế độ người dùng. Nếu cờ này không được đặt, trang chỉ có thể được truy cập ở chế độ giám sát.
- **Thanh ghi `satp`:** Kernel phải ghi địa chỉ vật lý của trang bảng trang gốc (cấp cao nhất) vào thanh ghi `satp`. Mỗi lõi CPU có `satp` riêng, cho phép mỗi lõi chạy một tiến trình khác nhau với không gian địa chỉ riêng.
- **TLB (Translation Look-aside Buffer):** Để tăng tốc quá trình dịch địa chỉ, CPU cache lại các kết quả dịch gần đây trong một bộ đệm gọi là TLB.

### **3.2. Không gian địa chỉ Kernel**

Kernel xv6 sử dụng **ánh xạ trực tiếp (direct mapping)**: địa chỉ ảo của kernel bằng với địa chỉ vật lý. Điều này đơn giản hóa mã kernel.

- **Ngoại lệ:**
    1. **Trang trampoline:** Được ánh xạ ở đỉnh không gian địa chỉ ảo.
    2. **Các trang ngăn xếp kernel:** Được ánh xạ ở vùng địa chỉ cao, với một **trang bảo vệ (guard page)** không hợp lệ ngay bên dưới để phát hiện tràn ngăn xếp.

### **3.4. Cấp phát bộ nhớ vật lý (Physical memory allocation)**

Kernel cần cấp phát và giải phóng bộ nhớ vật lý một cách linh hoạt cho các bảng trang, bộ nhớ người dùng, ngăn xếp kernel, và các bộ đệm khác.

- **Cơ chế:** Xv6 quản lý bộ nhớ vật lý trống (giữa cuối kernel và `PHYSTOP`) dưới dạng một **danh sách liên kết (linked list)** của các trang 4096-byte.
- **Cấu trúc dữ liệu:** Mỗi trang trống được coi như một cấu trúc `struct run`, chứa một con trỏ `next` đến trang trống tiếp theo.
    
    > 💡 Một kỹ thuật thông minh: Kernel không cần phải cấp phát bộ nhớ riêng để lưu trữ cấu trúc của danh sách liên kết. Thay vào đó, nó lưu trữ chính cấu trúc struct run bên trong trang vật lý trống đó. Vì trang đó không chứa dữ liệu hữu ích nào khác, việc này hoàn toàn an toàn và hiệu quả.
    > 
- **Hoạt động:**
    - Việc **cấp phát** một trang (`kalloc`) tương đương với việc lấy phần tử đầu tiên ra khỏi danh sách liên kết.
    - Việc **giải phóng** một trang (`kfree`) là việc thêm trang đó vào đầu danh sách liên kết.
- **Đồng bộ hóa:** Toàn bộ danh sách được bảo vệ bởi một spinlock duy nhất (`kmem.lock`) để ngăn chặn race condition khi nhiều lõi CPU cố gắng cấp phát hoặc giải phóng bộ nhớ cùng lúc.

### **3.7. Mã nguồn: `sbrk`**

`sbrk` là lời gọi hệ thống cho phép một tiến trình tăng hoặc giảm kích thước bộ nhớ của nó.

- **Chức năng:** `sbrk(n)` được hiện thực bởi hàm `growproc` trong kernel.
- **`growproc` thực hiện:**
    - Nếu `n > 0` (tăng bộ nhớ): Nó gọi `uvmalloc` để cấp phát `n` byte bộ nhớ mới.
        - `uvmalloc` lặp lại `n` lần, mỗi lần gọi `kalloc` để lấy một trang vật lý trống.
        - Sau đó, nó gọi `mappages` để thêm các **Mục Bảng Trang (PTEs)** mới vào bảng trang của tiến trình, ánh xạ các địa chỉ ảo mới tới các trang vật lý vừa được cấp phát. Các PTE này được đặt cờ `PTE_W`, `PTE_R`, `PTE_U`, và `PTE_V`.
    - Nếu `n < 0` (giảm bộ nhớ): Nó gọi `uvmdealloc` để giải phóng bộ nhớ.
        - `uvmdealloc` gọi `uvmunmap`, hàm này sử dụng `walk` để tìm các PTE tương ứng, sau đó gọi `kfree` để trả các trang vật lý về danh sách trống.

> ❗ Lưu ý: Bảng trang của một tiến trình không chỉ là một cơ chế ánh xạ; nó còn là bản ghi duy nhất về những trang vật lý nào đã được cấp phát cho tiến trình đó. Đây là lý do tại sao việc giải phóng bộ nhớ người dùng (uvmunmap) đòi hỏi phải duyệt qua bảng trang.
> 

---

# **Chương 4: Bẫy và Lời gọi Hệ thống**

### **4.3. Mã nguồn: Gọi một Lời gọi Hệ thống**

Hãy theo dõi cách lời gọi hệ thống `exec` từ `initcode.S` được thực thi trong kernel.

1. **User Space (`initcode.S`):**
    - Đặt các đối số cho `exec` vào các thanh ghi `a0` và `a1`.
    - Đặt số hiệu của lời gọi hệ thống, `SYS_exec`, vào thanh ghi `a7`.
    - Thực thi lệnh `ecall`.
2. **Chuyển giao (Trap):**
    - Lệnh `ecall` bẫy vào kernel. CPU chuyển sang chế độ giám sát và nhảy đến địa chỉ trong thanh ghi `stvec`, tức là `uservec`.
3. **Kernel Space (Assembly - `uservec`):**
    - `uservec` lưu tất cả 32 thanh ghi của người dùng vào `trapframe` của tiến trình.
    - Nó tải các thông tin cần thiết từ `trapframe` (như con trỏ đến ngăn xếp kernel, địa chỉ hàm `usertrap`) và chuyển sang bảng trang của kernel.
    - Cuối cùng, nó nhảy đến hàm C `usertrap`.
4. **Kernel Space (C - `usertrap` và `syscall`):**
    - `usertrap` xác định nguyên nhân của bẫy. Trong trường hợp này, đó là một lời gọi hệ thống. Nó gọi hàm `syscall`.
    - `syscall` (`kernel/syscall.c`) đọc số hiệu lời gọi hệ thống từ thanh ghi `a7` đã được lưu trong `trapframe`.
    - Nó sử dụng số hiệu này làm chỉ số để tra cứu trong mảng con trỏ hàm `syscalls[]` và gọi hàm tương ứng, tức là `sys_exec`.
5. **Trở về:**
    - Khi `sys_exec` trả về, `syscall` ghi giá trị trả về vào thanh ghi `a0` trong `trapframe`.
    - Luồng điều khiển quay trở lại qua `usertrap`, `usertrapret`, và cuối cùng là `userret`, nơi các thanh ghi của người dùng được khôi phục từ `trapframe` và lệnh `sret` được thực thi để trở về không gian người dùng.

### **4.4. Mã nguồn: Đối số của Lời gọi Hệ thống**

Kernel cần một cách an toàn để truy cập các đối số do chương trình người dùng cung cấp, đặc biệt là các con trỏ.

- **Thách thức:**
    1. Một con trỏ do người dùng cung cấp có thể không hợp lệ hoặc trỏ đến bộ nhớ của kernel, cố gắng đánh lừa kernel.
    2. Kernel không thể sử dụng trực tiếp con trỏ này vì nó đang chạy với bảng trang của kernel, không phải của người dùng.
- **Giải pháp:** Kernel cung cấp các hàm để truyền dữ liệu một cách an toàn.
    - `argint()`, `argaddr()`, `argfd()`: Lấy đối số thứ n từ `trapframe` dưới dạng số nguyên, địa chỉ, hoặc mô tả tệp.
    - `fetchstr()` và `copyinstr()`: Sao chép một chuỗi từ không gian địa chỉ người dùng vào kernel.
        - `copyinstr` nhận một địa chỉ ảo (`srcva`) và một bảng trang (`pagetable`).
        - Nó gọi `walkaddr` (hàm này gọi `walk`) để dịch địa chỉ ảo `srcva` thành địa chỉ vật lý `pa0`.
        - Vì kernel ánh xạ trực tiếp toàn bộ RAM vật lý, nó có thể sao chép trực tiếp từ địa chỉ vật lý `pa0` vào bộ đệm của mình.
        - `walkaddr` cũng kiểm tra để đảm bảo địa chỉ ảo nằm trong không gian hợp lệ của người dùng, ngăn chặn các cuộc tấn công.
    - `copyout()`: Thực hiện chức năng ngược lại, sao chép dữ liệu từ kernel ra không gian người dùng một cách an toàn.

---

# **Chương 5: Ngắt và Trình điều khiển thiết bị**

Trình điều khiển (driver) là mã trong HĐH quản lý một thiết bị cụ thể.

- **Cấu trúc Top-half / Bottom-half:**
    - **Top half:** Chạy trong ngữ cảnh của tiến trình gọi (ví dụ, khi tiến trình gọi `read` hoặc `write`). Nó khởi tạo một yêu cầu tới phần cứng và sau đó có thể đi vào trạng thái ngủ (sleep).
    - **Bottom half:** Là **trình xử lý ngắt (interrupt handler)**. Nó được kích hoạt khi thiết bị hoàn thành một thao tác. Nó thực hiện công việc cần thiết (ví dụ: lấy dữ liệu từ thiết bị) và đánh thức (wakeup) tiến trình đang chờ ở top half.
- **Ví dụ: Trình điều khiển Console (UART)**
    - **Input:** Khi bạn gõ một ký tự, phần cứng UART tạo ra một ngắt. Trình xử lý ngắt (`uartintr`) đọc ký tự đó và đưa vào một bộ đệm. Khi một dòng hoàn chỉnh, nó đánh thức tiến trình đang gọi `read` trên console.
    - **Output:** Khi một tiến trình gọi `write`, dữ liệu được đưa vào một bộ đệm đầu ra. Byte đầu tiên được gửi ngay lập tức. Khi UART gửi xong một byte, nó tạo ngắt "transmit complete". Trình xử lý ngắt sẽ gửi byte tiếp theo từ bộ đệm.
- **Ngắt Timer:** Được tạo ra định kỳ bởi phần cứng đồng hồ của CPU. Xv6 sử dụng ngắt này để theo dõi thời gian và để thực hiện chuyển đổi ngữ cảnh bắt buộc (involuntary context switch), đảm bảo rằng không có tiến trình nào chiếm giữ CPU quá lâu.

# **Chương 6: Khóa (Locking)**

Trong một hệ thống đa lõi, nhiều CPU có thể truy cập và sửa đổi cùng một cấu trúc dữ liệu trong bộ nhớ, dẫn đến **race condition**. Khóa là một kỹ thuật kiểm soát đồng thời để đảm bảo **loại trừ tương hỗ (mutual exclusion)**.

- **Race Condition:** Xảy ra khi kết quả của một phép tính phụ thuộc vào thứ tự thời gian không thể đoán trước của các sự kiện. Ví dụ: hai CPU cùng lúc cố gắng thêm một trang vào danh sách bộ nhớ trống (`kfree`).
- **Khóa (Lock):** Một cơ chế đảm bảo rằng chỉ có một CPU tại một thời điểm có thể thực thi một đoạn mã quan trọng (critical section).
- **Spinlocks:**
    - Là loại khóa cơ bản trong xv6.
    - Được hiện thực bằng lệnh `amoswap` của RISC-V, một lệnh nguyên tử (atomic).
    - Một CPU cố gắng lấy một spinlock đang bị giữ sẽ "quay" (spin) trong một vòng lặp vô hạn, liên tục kiểm tra cho đến khi khóa được giải phóng. Điều này lãng phí chu kỳ CPU.
    - **Quan trọng:** Để tránh deadlock với trình xử lý ngắt, xv6 luôn **tắt ngắt** trên một CPU khi CPU đó đang giữ bất kỳ spinlock nào.
- **Sleep-locks:**
    - Được sử dụng cho các hoạt động kéo dài (ví dụ: thao tác đĩa).
    - Thay vì quay, một tiến trình cố gắng lấy một sleep-lock đang bị giữ sẽ **nhường CPU** (`sleep`) và chỉ được đánh thức khi khóa được giải phóng.
- **Deadlock:** Xảy ra khi hai hoặc nhiều tiến trình đang chờ đợi lẫn nhau để giải phóng tài nguyên. Cách phòng tránh phổ biến nhất là thiết lập một **thứ tự khóa toàn cục (global lock ordering)**. Tất cả các đoạn mã phải lấy các khóa theo cùng một thứ tự đã định trước.

# **Chương 7: Lập lịch (Scheduling)**

Lập lịch là quá trình HĐH ghép kênh nhiều tiến trình lên một số lượng CPU ít hơn.

- **Chuyển đổi ngữ cảnh (Context Switch):** Là quá trình lưu trạng thái (thanh ghi) của tiến trình hiện tại và phục hồi trạng thái của một tiến trình khác. Trong xv6, nó được thực hiện bởi hàm hợp ngữ `swtch`.
- **Scheduler:** Mỗi CPU có một luồng lập lịch riêng. Scheduler chạy một vòng lặp vô tận: tìm một tiến trình ở trạng thái `RUNNABLE`, chuyển ngữ cảnh sang nó, và chạy nó cho đến khi tiến trình đó tự nguyện nhường CPU (ví dụ, bằng cách gọi `sleep` hoặc `yield`).
- **Cơ chế `sleep` và `wakeup`:**
    - Là cơ chế đồng bộ hóa điều kiện cơ bản.
    - `sleep(chan, lock)`:
        1. Tiến trình gọi `sleep` phải đang giữ một spinlock (`lock`).
        2. `sleep` đặt trạng thái của tiến trình thành `SLEEPING` và chỉ định một **kênh chờ (wait channel)** `chan`.
        3. Nó giải phóng `lock`.
        4. Nó gọi scheduler để nhường CPU.
    - `wakeup(chan)`: Đánh thức tất cả các tiến trình đang ngủ trên kênh `chan`, chuyển trạng thái của chúng thành `RUNNABLE`.
    - **Vấn đề Lost Wakeup:** Là một race condition kinh điển, xảy ra nếu một `wakeup` diễn ra ngay sau khi một tiến trình kiểm tra điều kiện ngủ nhưng trước khi nó thực sự gọi `sleep`. Xv6 giải quyết vấn đề này bằng cách yêu cầu tiến trình đi ngủ phải giữ một khóa, và hàm `sleep` chỉ giải phóng khóa đó sau khi tiến trình đã được đánh dấu là `SLEEPING`, tạo ra một thao tác nguyên tử.
- **Sử dụng trong `wait`, `exit`, và `kill`:** `exit` đánh dấu tiến trình là `ZOMBIE` và gọi `wakeup` trên tiến trình cha. `wait` gọi `sleep` để chờ đợi một đứa con trở thành `ZOMBIE`. `kill` đặt một cờ `killed` trong `struct proc` của nạn nhân và đánh thức nó nếu nó đang ngủ.

# **Chương 8: Hệ thống tệp**

### **8.13. Lớp Mô tả tệp (File descriptor layer)**

Đây là lớp trừu tượng hóa giúp đạt được triết lý "mọi thứ là một tệp" của Unix.

- **Cấu trúc dữ liệu:**
    1. **Bảng mô tả tệp của tiến trình:** Mỗi tiến trình có một mảng riêng (`p->ofile[]`) chứa các con trỏ tới `struct file`. Chỉ số của mảng này chính là file descriptor.
    2. **Bảng tệp toàn cục (`ftable`):** Là một bảng duy nhất cho toàn bộ hệ thống, chứa tất cả các cấu trúc `struct file` đang được mở.
- **`struct file`:**
    - Đây là đại diện trong bộ nhớ cho một tệp đang mở. Nó là một lớp bao (wrapper).
    - Nó chứa các trường như `type` (cho biết nó là inode hay pipe), con trỏ đến inode hoặc pipe, `readable`, `writable`, và quan trọng nhất là `ref` (số lượng tham chiếu) và `off` (con trỏ vị trí đọc/ghi).
- **Sự khác biệt giữa `struct file` và `struct inode`:**
    - Một **inode** đại diện cho một tệp trên đĩa. Nó là duy nhất.
    - Một **file** đại diện cho một *phiên bản đang mở* của một tệp. Nhiều tiến trình có thể mở cùng một tệp, tạo ra nhiều `struct file` khác nhau, mỗi cái có con trỏ `off` riêng.
- **Chia sẻ `struct file`:**
    - Khi một tiến trình gọi `dup()` hoặc `fork()`, nhiều mục trong các bảng mô tả tệp (có thể của cùng một hoặc các tiến trình khác nhau) sẽ cùng trỏ đến **cùng một `struct file`**.
    - Trong trường hợp này, chúng chia sẻ **cùng một con trỏ vị trí `off`**. Điều này giải thích tại sao sau khi `fork`, việc ghi của tiến trình con sẽ tiếp tục từ vị trí mà tiến trình cha đã dừng lại.
- **Các hàm chính của lớp này:**
    - `filealloc()`: Tìm một `struct file` chưa được sử dụng trong `ftable` và cấp phát nó.
    - `filedup()`: Tăng số lượng tham chiếu (`ref`) của một `struct file`.
    - `fileclose()`: Giảm số lượng tham chiếu. Nếu `ref` về 0, nó sẽ giải phóng inode hoặc pipe bên dưới bằng cách gọi `iput()`.
    - `fileread()` / `filewrite()`: Kiểm tra quyền đọc/ghi, sau đó gọi các hàm tương ứng của lớp inode (`readi`/`writei`) hoặc lớp pipe. Nếu là inode, chúng sẽ sử dụng và cập nhật `f->off`. Pipe không có khái niệm về offset.

---