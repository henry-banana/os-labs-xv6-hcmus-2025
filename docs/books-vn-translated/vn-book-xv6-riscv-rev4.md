# 🖥️ **Chương 1: Giao diện Hệ điều hành**

### 🎯 **Nhiệm vụ chính của Hệ điều hành (HĐH)**

- **Chia sẻ tài nguyên:** Quản lý và phân chia tài nguyên phần cứng (CPU, bộ nhớ) cho nhiều chương trình cùng lúc.
- **Trừu tượng hóa:** Cung cấp một giao diện đơn giản, mạnh mẽ hơn so với việc tương tác trực tiếp với phần cứng phức tạp.
- **Bảo vệ và Cô lập:** Đảm bảo các chương trình không can thiệp lẫn nhau và không phá hỏng HĐH.

### 💡 **Các khái niệm cốt lõi**

1. 👑 **Kernel (Nhân):**
    - Là chương trình trung tâm của HĐH, có **toàn quyền kiểm soát** phần cứng.
    - Cung cấp các dịch vụ cho chương trình người dùng.
2. 🏃‍♂️ **Process (Tiến trình):**
    - Là một chương trình đang được thực thi.
    - Mỗi tiến trình có không gian bộ nhớ riêng, bao gồm:
        - **Mã lệnh (Instructions):** Các chỉ thị CPU.
        - **Dữ liệu (Data):** Các biến toàn cục.
        - **Ngăn xếp (Stack):** Quản lý các lời gọi hàm.
3. **Không gian Người dùng (User Space) vs. Không gian Nhân (Kernel Space):**
    - **User Space:** Nơi các tiến trình thông thường chạy. **Quyền hạn bị giới hạn**, không thể truy cập trực tiếp phần cứng.
    - **Kernel Space:** Nơi kernel chạy. **Có toàn quyền**, có thể thực hiện mọi thao tác.
4. 📞 **Lời gọi Hệ thống (System Call):**
    - Là **cầu nối duy nhất** giữa User Space và Kernel Space.
    - Khi một tiến trình cần dịch vụ từ HĐH (ví dụ: đọc tệp), nó thực hiện một system call, tạm thời chuyển quyền điều khiển cho kernel.

### 🐚 **Ví dụ điển hình: Shell**

- Shell (như `bash`) **chỉ là một chương trình người dùng bình thường**, không phải là một phần của kernel.
- Nó đọc lệnh từ bạn, sau đó sử dụng các system call (như `fork`, `exec`, `wait`) để thực thi các lệnh đó.
- 👉 Điều này chứng tỏ sức mạnh của giao diện system call: bất kỳ ai cũng có thể viết một chương trình shell mà không cần sửa đổi kernel.

---

### 🧠 **1.1 Tiến trình và Bộ nhớ**

Các system call cơ bản để quản lý tiến trình:

- **`fork()`** 👶
    - **Chức năng:** Tạo ra một tiến trình con mới, là một **bản sao y hệt** của tiến trình cha (sao chép bộ nhớ, dữ liệu, ngăn xếp).
    - **Giá trị trả về:**
        - Trong tiến trình cha: Trả về **PID (Process ID)** của tiến trình con.
        - Trong tiến trình con: Trả về **0**.
        - Lỗi: Trả về -1.
- **`exit(status)`** 🏁
    - **Chức năng:** Chấm dứt tiến trình hiện tại và giải phóng tài nguyên.
    - `status` là một số nguyên được trả về cho tiến trình cha đang chờ.
- **`wait(status)`** 👨‍👦
    - **Chức năng:** Tiến trình cha **chờ** một trong các tiến trình con của nó kết thúc.
    - Khi con kết thúc, `wait` trả về PID của con và ghi trạng thái thoát vào con trỏ `status`.
- **`exec(file, argv)`** 🎭
    - **Chức năng:** **Thay thế hoàn toàn** không gian bộ nhớ của tiến trình hiện tại bằng một chương trình mới được tải từ `file`.
    - Các lệnh, dữ liệu, ngăn xếp cũ đều bị xóa.
    - Nếu thành công, `exec` **không bao giờ quay trở lại**.

💡 **Tại sao `fork()` và `exec()` lại tách biệt?**
Đây là một thiết kế cực kỳ quan trọng và mạnh mẽ! Sự tách biệt này cho phép shell thực hiện các thao tác **ở giữa** hai lời gọi, mà quan trọng nhất là **chuyển hướng I/O**.

```c
// Ví dụ về fork() và wait()
int pid = fork();
if (pid > 0) {
    // Đây là mã của tiến trình cha
    printf("I am parent, my child's PID is %d\\n", pid);
    wait(0); // Chờ con kết thúc
    printf("My child has finished.\\n");
} else if (pid == 0) {
    // Đây là mã của tiến trình con
    printf("I am child, exiting now.\\n");
    exit(0);
}

```

---

### 📚 **Bảng các Lời gọi Hệ thống của xv6**

| Lời gọi Hệ thống | Mô tả |
| --- | --- |
| `int fork()` | Tạo một tiến trình, trả về PID của con. |
| `int exit(int status)` | Chấm dứt tiến trình hiện tại. |
| `int wait(int *status)` | Chờ một tiến trình con thoát. |
| `int kill(int pid)` | Chấm dứt một tiến trình khác. |
| `int exec(...)` | Tải và thực thi một chương trình mới. |
| `char *sbrk(int n)` | Tăng bộ nhớ của tiến trình. |
| `int open(...)` | Mở một tệp, trả về một mô tả tệp (fd). |
| `int read(...)` | Đọc dữ liệu từ một fd. |
| `int write(...)` | Ghi dữ liệu vào một fd. |
| `int close(int fd)` | Đóng một fd. |
| `int dup(int fd)` | Nhân bản một fd. |
| `int pipe(int p[])` | Tạo một đường ống (pipe). |
| ... | *Và các lời gọi khác để quản lý tệp/thư mục.* |

---

### 📂 **1.2 I/O và Mô tả Tệp (File Descriptors)**

- **Mô tả Tệp (File Descriptor - fd):** 🔢
    - Là một **số nguyên nhỏ** đại diện cho một đối tượng I/O mà kernel quản lý.
    - Đây là một sự trừu tượng hóa mạnh mẽ: **mọi thứ đều trông giống như một tệp** (tệp trên đĩa, console, đường ống, thiết bị).
- **Quy ước quan trọng:** Mỗi tiến trình bắt đầu với 3 fd mặc định:
    - `fd 0`: **Standard Input** (Đầu vào chuẩn)
    - `fd 1`: **Standard Output** (Đầu ra chuẩn)
    - `fd 2`: **Standard Error** (Lỗi chuẩn)
- **Các hàm chính:**
    - `read(fd, buf, n)`: Đọc dữ liệu từ `fd`.
    - `write(fd, buf, n)`: Ghi dữ liệu vào `fd`.
    - `close(fd)`: Đóng `fd`.
    - `dup(fd)`: Sao chép một `fd`. `fd` mới và `fd` cũ sẽ trỏ đến **cùng một đối tượng I/O** và **chia sẻ cùng một con trỏ vị trí**.

⚙️ **Cơ chế Chuyển hướng I/O (ví dụ `cat < input.txt`)**
Đây là lúc sức mạnh của việc tách `fork()` và `exec()` thể hiện rõ nhất:

1. Shell gọi `fork()`.
2. **Trong tiến trình con:**
    - `close(0)`; // Đóng đầu vào chuẩn (đang trỏ đến console).
    - `open("input.txt", O_RDONLY)`; // Mở tệp. Vì `fd 0` là số nhỏ nhất còn trống, kernel sẽ gán `fd 0` cho tệp này.
    - `exec("cat", ...)` // Chạy chương trình `cat`.
3. Kết quả: `cat` chạy và khi nó đọc từ `fd 0`, nó đang thực sự đọc từ `input.txt` mà **không hề hay biết**.

---

### 🔗 **1.3 Đường ống (Pipes)**

- **Pipe là gì?**
    - Một bộ đệm nhỏ trong kernel, hoạt động như một kênh giao tiếp một chiều.
    - Lời gọi `pipe(p)` tạo ra một pipe và trả về **hai mô tả tệp**: `p[0]` để **đọc** và `p[1]` để **ghi**.
- **Sử dụng:** Thường được dùng để kết nối đầu ra của một tiến trình với đầu vào của tiến trình khác.
    - Ví dụ: `grep "error" log.txt | wc -l`
- **Lợi ích so với tệp tạm thời:**
    1. **Tự động dọn dẹp:** Không cần xóa tệp tạm.
    2. **Xử lý luồng dữ liệu lớn:** Không giới hạn bởi dung lượng đĩa.
    3. **Cho phép thực thi song song:** Các tiến trình trong pipeline có thể chạy cùng lúc.

---

### 🗂️ **1.4 Hệ thống Tệp**

- **Cấu trúc:** Một cây thư mục bắt đầu từ thư mục gốc (`/`).
- **Inode:** Một cấu trúc dữ liệu trên đĩa chứa **siêu dữ liệu** (metadata) về một tệp (loại, kích thước, vị trí các khối dữ liệu, ...). Tên tệp không được lưu trong inode.
- **Liên kết (Link):**
    - Tên tệp được lưu trong một thư mục, cùng với một con trỏ đến inode của tệp đó.
    - Một inode có thể có nhiều tên (nhiều liên kết). Lệnh `link` tạo ra một liên kết mới.
    - Lệnh `unlink` xóa một liên kết (tên tệp). Dữ liệu của tệp chỉ thực sự bị xóa khi **số liên kết về 0** và **không còn tiến trình nào đang mở tệp đó**.

# 🖥️ **Chương 2: Tổ chức Hệ điều hành**

### 🎯 **Mục tiêu chính**

Một HĐH hiện đại phải đáp ứng được ba yêu cầu cơ bản:

1. **Đa hợp (Multiplexing):** Chia sẻ tài nguyên (như CPU) cho nhiều tiến trình. Ngay cả khi chỉ có một CPU, HĐH tạo ra ảo giác rằng mỗi tiến trình có CPU riêng của mình.
2. **Cô lập (Isolation):** Ngăn chặn một tiến trình lỗi làm ảnh hưởng đến các tiến trình khác hoặc toàn bộ hệ thống.
3. **Tương tác (Interaction):** Cung cấp các cơ chế an toàn để các tiến trình có thể giao tiếp và hợp tác với nhau (ví dụ: pipes).

### 🛡️ **Cơ chế Bảo vệ Phần cứng**

Để đạt được sự cô lập, HĐH phải dựa vào phần cứng. CPU RISC-V cung cấp các **chế độ thực thi** khác nhau:

- **Machine Mode (Chế độ Máy):** 👑
    - **Toàn quyền kiểm soát**.
    - CPU khởi động ở chế độ này.
    - xv6 chỉ sử dụng chế độ này trong giai đoạn đầu để thiết lập hệ thống, sau đó nhanh chóng chuyển sang Supervisor Mode.
- **Supervisor Mode (Chế độ Giám sát):** 👮
    - Nơi **kernel** hoạt động.
    - Có thể thực thi các **lệnh đặc quyền** (privileged instructions) như bật/tắt ngắt, truy cập các thanh ghi điều khiển.
    - Không gian bộ nhớ của kernel chạy ở chế độ này được gọi là **Kernel Space**.
- **User Mode (Chế độ Người dùng):** 🧑‍💻
    - Nơi các **tiến trình người dùng** (ứng dụng, shell) hoạt động.
    - **Quyền hạn bị giới hạn**.
    - Nếu một tiến trình cố gắng thực thi một lệnh đặc quyền, CPU sẽ gây ra một **ngoại lệ (exception)** và chuyển quyền điều khiển cho kernel để xử lý (thường là kết thúc tiến trình đó).
    - Không gian bộ nhớ của tiến trình chạy ở chế độ này được gọi là **User Space**.

💡 **Làm thế nào để chuyển từ User sang Supervisor?**
Một tiến trình người dùng không thể tự ý nhảy vào code của kernel. Cách duy nhất là thông qua một lệnh đặc biệt:

- **`ecall` (Environment Call):** Lệnh này gây ra một **bẫy (trap)**, khiến CPU:
    1. Chuyển sang **Supervisor Mode**.
    2. Lưu lại vị trí lệnh hiện tại.
    3. Nhảy đến một **điểm vào (entry point)** đã được kernel định sẵn.
    
    Điều này đảm bảo rằng các chương trình người dùng chỉ có thể vào kernel thông qua các cổng được kiểm soát chặt chẽ.
    

### 🏛️ **Kiến trúc Kernel**

Có hai cách tiếp cận chính để tổ chức mã nguồn của HĐH:

1. **Monolithic Kernel (Nhân nguyên khối):**
    - **Toàn bộ HĐH** (quản lý tệp, lập lịch, mạng, trình điều khiển) chạy trong một chương trình lớn duy nhất trong **Kernel Space**.
    - **Ưu điểm:** Hiệu suất cao do các thành phần có thể gọi hàm của nhau trực tiếp. Dễ dàng chia sẻ dữ liệu.
    - **Nhược điểm:** Một lỗi trong một trình điều khiển có thể làm sập toàn bộ hệ thống. Mã nguồn lớn và phức tạp.
    - **Ví dụ:** Linux, FreeBSD, và **xv6**.
2. **Microkernel (Vi nhân):**
    - **Kernel chỉ chứa các chức năng tối thiểu** (quản lý tiến trình, bộ nhớ, giao tiếp liên tiến trình - IPC).
    - Các dịch vụ khác (hệ thống tệp, mạng) chạy dưới dạng các **tiến trình thông thường (server)** trong **User Space**.
    - **Ưu điểm:** An toàn và ổn định hơn (lỗi trong server hệ thống tệp không làm sập kernel). Dễ dàng mở rộng.
    - **Nhược điểm:** Hiệu suất thấp hơn do phải dựa vào IPC để giao tiếp giữa các thành phần.
    - **Ví dụ:** QNX, Minix.

### 📂 **Cấu trúc Mã nguồn xv6**

Mã nguồn của kernel xv6 nằm trong thư mục `kernel/`. Các tệp được tổ chức theo module.

| Tệp | Mô tả |
| --- | --- |
| `main.c`, `proc.c` | Khởi tạo, quản lý tiến trình và lập lịch. |
| `vm.c` | Quản lý bộ nhớ ảo và bảng trang. |
| `kalloc.c` | Cấp phát bộ nhớ vật lý. |
| `trap.c` | Xử lý các bẫy, ngắt, và lời gọi hệ thống. |
| `syscall.c`, `sysproc.c`, `sysfile.c` | Triển khai các lời gọi hệ thống. |
| `fs.c`, `file.c`, `bio.c`, `log.c` | Hệ thống tệp (inodes, tệp, bộ đệm, ghi nhật ký). |
| `spinlock.c`, `sleeplock.c` | Triển khai các cơ chế khóa. |
| `console.c`, `uart.c` | Trình điều khiển cho console và cổng serial. |
| `entry.S`, `kernelvec.S`, `swtch.S`, `trampoline.S` | Mã Assembly cấp thấp cho khởi động, xử lý bẫy, chuyển ngữ cảnh. |

---

### 🏃‍♂️ **2.5 Tổng quan về Tiến trình**

Một tiến trình trong xv6 là đơn vị của sự cô lập. Nó bao gồm:

- **Không gian địa chỉ (Address Space):** 🗺️
    - Mỗi tiến trình có một **không gian địa chỉ ảo** riêng, tạo ảo giác rằng nó có toàn bộ bộ nhớ cho riêng mình.
    - Kernel sử dụng **bảng trang (page tables)** do phần cứng cung cấp để ánh xạ các địa chỉ ảo này đến các địa chỉ vật lý trong RAM.
    - Bố cục bộ nhớ ảo của một tiến trình trong xv6:
        - **Vùng thấp:** Mã lệnh (chỉ đọc, thực thi), dữ liệu toàn cục (đọc/ghi).
        - **Heap:** Vùng nhớ có thể mở rộng động (dùng cho `malloc`).
        - **Stack:** Ngăn xếp người dùng (đọc/ghi), tự động phát triển xuống.
        - **Vùng cao:** **Trampoline page** và **trapframe**, dùng để chuyển đổi giữa User và Kernel Space.
- **Luồng thực thi (Thread of control):** 🧵
    - Mỗi tiến trình có một luồng thực thi duy nhất.
    - Trạng thái của luồng (các thanh ghi CPU) được lưu lại khi tiến trình không chạy và được khôi phục khi nó chạy trở lại.
    - Một tiến trình có **hai ngăn xếp**:
        1. **User Stack:** Được sử dụng khi chạy trong User Mode.
        2. **Kernel Stack:** Được sử dụng khi tiến trình thực hiện system call hoặc bị ngắt để vào Kernel Mode. Ngăn xếp này được bảo vệ khỏi mã người dùng.

### 🚀 **2.6 Khởi động xv6 và Tạo Tiến trình Đầu tiên**

Quá trình khởi động diễn ra như sau:

1. **Boot loader:** Tải kernel xv6 từ đĩa vào bộ nhớ tại địa chỉ vật lý `0x80000000`.
2. **`_entry` (Assembly):** CPU bắt đầu thực thi ở đây (Machine Mode). Nó thiết lập một ngăn xếp ban đầu và gọi hàm C `start`.
3. **`start` (C):**
    - Thực hiện các thiết lập cần thiết chỉ có thể làm trong Machine Mode (ví dụ: lập trình ngắt bộ định thời).
    - Sau đó, nó **chuyển sang Supervisor Mode** và nhảy đến hàm `main`.
4. **`main` (C):**
    - Khởi tạo các hệ thống con của kernel (console, cấp phát bộ nhớ, hệ thống tệp, ...).
    - Gọi `userinit()` để tạo ra **tiến trình người dùng đầu tiên**.
5. **`userinit()`:**
    - Tạo một tiến trình mới, cấp phát bộ nhớ cho nó.
    - Sao chép một đoạn mã Assembly nhỏ (`initcode.S`) vào bộ nhớ của tiến trình này.
    - `initcode.S` chỉ làm một việc: thực hiện **lời gọi hệ thống đầu tiên**: `exec("/init", ...)`
6. **`exec("/init")`:** Kernel tải chương trình `/init` từ hệ thống tệp, thay thế mã `initcode.S`.
7. **`/init` (User program):**
    - Mở console làm các file descriptor 0, 1, và 2.
    - Khởi động một **shell**.
    - Lúc này, hệ thống đã sẵn sàng nhận lệnh từ người dùng! 🎉
    
    ### 🔐 **2.7 Mô hình Bảo mật**
    
    Mô hình bảo mật trong thiết kế HĐH xoay quanh các giả định và mục tiêu sau:
    
    - **Giả định về Mã Người dùng (User Code):** 😈
        - Kernel phải luôn giả định rằng mã người dùng là **độc hại và có lỗi**.
        - Nó sẽ cố gắng:
            - Truy cập bộ nhớ ngoài phạm vi cho phép.
            - Thực thi các lệnh đặc quyền.
            - Truyền các đối số vô lý cho system call để lừa kernel.
    - **Mục tiêu của Kernel:** 🎯
        - **Thực thi sự cô lập một cách nghiêm ngặt.**
        - Giới hạn một tiến trình chỉ có thể truy cập vào bộ nhớ, thanh ghi của chính nó và tương tác với kernel thông qua các system call đã được định nghĩa.
        - **Phải xác thực tất cả đầu vào** từ không gian người dùng.
    - **Giả định về Mã Kernel (Kernel Code):** 😇
        - Mã kernel được giả định là **được viết cẩn thận và không có ý đồ xấu**.
        - Nó được tin tưởng sẽ tuân thủ đúng các quy tắc nội bộ.
        - **Thực tế:** Kernel cũng có lỗi (bugs), và các lỗi này có thể bị mã độc khai thác để leo thang đặc quyền. Do đó, các HĐH hiện đại có thêm nhiều lớp phòng thủ bên trong kernel (stack canaries, type checking, assertions).
    - **Giả định về Phần cứng:** ⚙️
        - Phần cứng (CPU, RAM, đĩa) được giả định là hoạt động **chính xác** theo đặc tả kỹ thuật.
    
    💡 **xv6 không có khái niệm người dùng (users).** Mọi tiến trình đều chạy với quyền hạn tương đương, giống như tất cả đều là `root` trên hệ thống Unix.
    
    ### 🌎 **2.8 Thế giới thực**
    
    - **Luồng (Threads):**
        - Hầu hết các HĐH hiện đại hỗ trợ **nhiều luồng thực thi trong cùng một tiến trình**.
        - Các luồng này **chia sẻ cùng một không gian địa chỉ** nhưng có các thanh ghi và ngăn xếp riêng.
        - Điều này cho phép một chương trình có thể tận dụng nhiều lõi CPU để thực hiện các công việc song song.
        - **xv6 không hỗ trợ đa luồng** trong một tiến trình. Mỗi tiến trình chỉ có một luồng.
    
    ### ✍️ **2.9 Bài tập**
    
    1. Thêm một lời gọi hệ thống vào xv6 để trả về lượng bộ nhớ trống hiện có.

# 📖 **Chương 3: Bảng Trang (Page Tables)**

### 🎯 **Mục tiêu chính**

Bảng trang là cơ chế phần cứng và phần mềm cho phép HĐH:

1. **Cung cấp không gian địa chỉ riêng tư** cho mỗi tiến trình.
2. **Tạo ảo giác về bộ nhớ liên tục**, mặc dù bộ nhớ vật lý có thể bị phân mảnh.
3. **Bảo vệ bộ nhớ:** Ngăn một tiến trình truy cập vào bộ nhớ của tiến trình khác hoặc của kernel.

### 💡 **Các khái niệm cốt lõi**

- **Địa chỉ ảo (Virtual Address):** 🗺️
    - Là địa chỉ mà các lệnh CPU (trong cả user và kernel mode) sử dụng.
    - Mỗi tiến trình có không gian địa chỉ ảo riêng, bắt đầu từ 0.
- **Địa chỉ vật lý (Physical Address):** 🏠
    - Là địa chỉ thực tế trên các chip RAM.
    - Kernel cần quản lý việc ánh xạ từ địa chỉ ảo sang địa chỉ vật lý.
- **Phân trang (Paging):** ⚙️
    - Phần cứng quản lý bộ nhớ (MMU - Memory Management Unit) chịu trách nhiệm **dịch địa chỉ ảo sang địa chỉ vật lý**.
    - Cả không gian địa chỉ ảo và vật lý đều được chia thành các khối có kích thước cố định gọi là **trang (page)** (thường là 4KB).
    - **Bảng trang (Page Table)** là cấu trúc dữ liệu mà MMU sử dụng để thực hiện việc dịch địa chỉ. Nó chứa các **mục bảng trang (Page Table Entries - PTEs)**.

### ⚙️ **3.1 Phần cứng Phân trang RISC-V (Sv39)**

xv6 chạy trên RISC-V với chế độ phân trang **Sv39**:

- **Địa chỉ ảo:** 64-bit, nhưng chỉ sử dụng **39 bit thấp** (tương đương 512 GB không gian ảo).
- **Địa chỉ vật lý:** 56-bit.
- **Dịch địa chỉ:** Một địa chỉ ảo 39-bit được chia thành:
    - **27 bit chỉ số (Index):** Dùng để tra cứu trong bảng trang.
    - **12 bit offset:** Dùng để chỉ định byte bên trong trang 4KB.
- **Cấu trúc bảng trang 3 cấp:**
    - Thay vì một bảng trang khổng lồ, RISC-V sử dụng một cây 3 cấp để tiết kiệm bộ nhớ.
    - Để dịch một địa chỉ ảo, phần cứng MMU thực hiện 3 lần truy cập bộ nhớ để "đi" qua cây này và tìm PTE cuối cùng.
    - **`satp` register:** Thanh ghi này chứa **địa chỉ vật lý** của bảng trang cấp cao nhất (gốc của cây). Kernel phải nạp địa chỉ này vào `satp` để kích hoạt bảng trang cho một tiến trình.
- **Translation Look-aside Buffer (TLB):** ⚡
    - Việc đi qua 3 cấp bảng trang cho mỗi lần truy cập bộ nhớ là rất chậm.
    - CPU có một **bộ đệm cache phần cứng** đặc biệt gọi là TLB để lưu các kết quả dịch địa chỉ gần đây.
    - Hầu hết các lần dịch địa chỉ sẽ trúng (hit) trong TLB, giúp tăng tốc đáng kể.

### 📄 **Mục Bảng Trang (Page Table Entry - PTE)**

Mỗi PTE chứa hai phần thông tin quan trọng:

1. **Số trang vật lý (Physical Page Number - PPN):** Địa chỉ của trang vật lý tương ứng.
2. **Các cờ (Flags):** Các bit điều khiển quyền truy cập.

| Cờ | Ý nghĩa |
| --- | --- |
| `PTE_V` (Valid) | Bit hợp lệ. Nếu bit này là 0, PTE không hợp lệ, truy cập sẽ gây ra **lỗi trang (page fault)**. |
| `PTE_R` (Read) | Cho phép đọc. |
| `PTE_W` (Write) | Cho phép ghi. |
| `PTE_X` (Execute) | Cho phép thực thi (coi nội dung trang là mã lệnh). |
| `PTE_U` (User) | Cho phép truy cập từ **User Mode**. Nếu bit này là 0, trang chỉ có thể được truy cập từ **Supervisor Mode**. |

💡 **Ví dụ về sử dụng cờ:**

- Vùng mã lệnh của một tiến trình sẽ có cờ `R` và `X` bật, nhưng `W` tắt để ngăn chương trình tự sửa đổi mã của mình.
- Tất cả các trang của kernel sẽ có cờ `U` tắt để ngăn các tiến trình người dùng truy cập vào bộ nhớ của kernel.
- **Trang bảo vệ (Guard Page):** Kernel đặt một trang không hợp lệ (`V=0`) ngay bên dưới ngăn xếp của người dùng. Nếu ngăn xếp bị tràn, việc truy cập vào trang này sẽ ngay lập tức gây ra lỗi trang, giúp phát hiện lỗi sớm.

### 🗺️ **3.2 Không gian Địa chỉ của Kernel**

- Kernel cũng có bảng trang riêng của mình, mô tả cách nó nhìn thấy bộ nhớ.
- xv6 sử dụng một kỹ thuật đơn giản gọi là **ánh xạ trực tiếp (direct mapping)** cho hầu hết bộ nhớ của kernel.
    - Địa chỉ ảo `0x80000000` được ánh xạ tới địa chỉ vật lý `0x80000000`.
    - Địa chỉ ảo `0x80001000` được ánh xạ tới địa chỉ vật lý `0x80001000`, v.v.
    - Điều này giúp đơn giản hóa code của kernel khi nó cần truy cập bộ nhớ vật lý.
- **Ngoại lệ:**
    - **Trampoline page:** Được ánh xạ ở cả địa chỉ cao trong không gian ảo và địa chỉ vật lý thực của nó.
    - **Kernel stacks:** Mỗi tiến trình có một kernel stack riêng, được ánh xạ ở vùng địa chỉ ảo cao, với một trang bảo vệ không hợp lệ bên dưới nó.
        
        ### 🛠️ **3.3 Mã nguồn: Tạo một Không gian Địa chỉ**
        
        Các chức năng chính để quản lý bảng trang nằm trong `vm.c` (Virtual Memory).
        
        - `kvminit()`: 🚀
            - **Chức năng:** Khởi tạo **bảng trang của kernel** trong quá trình khởi động.
            - **Các bước:**
                1. Tạo bảng trang gốc (root page table).
                2. Gọi `kvmmap()` để thiết lập các ánh xạ cần thiết cho kernel.
                3. Các ánh xạ này bao gồm: mã lệnh và dữ liệu của kernel, toàn bộ RAM vật lý (ánh xạ trực tiếp), và các thiết bị I/O được ánh xạ bộ nhớ (memory-mapped I/O).
        - `walk(pagetable, va, alloc)`: 🚶‍♂️
            - **Chức năng:** Hàm quan trọng nhất! Mô phỏng lại cách phần cứng MMU duyệt qua cây bảng trang 3 cấp để tìm **địa chỉ của PTE** tương ứng với một **địa chỉ ảo (`va`)**.
            - **Tham số `alloc`:** Nếu trong quá trình duyệt, một bảng trang cấp trung gian chưa tồn tại, hàm `walk` sẽ cấp phát một trang mới cho nó nếu `alloc` là `true`.
            - Hàm này là trái tim của việc quản lý bộ nhớ ảo.
        - `mappages(pagetable, va, size, pa, perm)`: 🗺️
            - **Chức năng:** Tạo ánh xạ cho một vùng địa chỉ.
            - Nó ánh xạ một dải địa chỉ ảo (bắt đầu từ `va`, kích thước `size`) tới một dải địa chỉ vật lý (bắt đầu từ `pa`) với các quyền (`perm`) đã cho.
            - Nó thực hiện điều này bằng cách lặp qua các trang trong dải địa chỉ và gọi `walk()` để tìm PTE cho mỗi trang, sau đó điền thông tin địa chỉ vật lý và cờ quyền vào PTE đó.
        
        ### 🧱 **3.5 Mã nguồn: Bộ cấp phát Bộ nhớ Vật lý**
        
        Các chức năng này nằm trong `kalloc.c`.
        
        - **Cấu trúc dữ liệu:** Kernel quản lý các trang vật lý trống bằng một **danh sách liên kết (linked list)**.
            - Mỗi nút trong danh sách liên kết (một `struct run`) được lưu trữ ngay bên trong chính trang vật lý trống đó. Một cách sử dụng bộ nhớ rất thông minh!
            - Danh sách này được bảo vệ bởi một **spinlock** (`kmem.lock`) để tránh xung đột (race conditions) khi nhiều CPU cùng cấp phát bộ nhớ.
        - `kinit()`: 🏁
            - Được gọi khi khởi động để khởi tạo bộ cấp phát.
            - Nó xây dựng danh sách liên kết ban đầu, bao gồm tất cả các trang RAM vật lý nằm giữa cuối mã kernel và cuối bộ nhớ vật lý (`PHYSTOP`).
        - `kfree(pa)`: 🚮
            - **Chức năng:** Giải phóng một trang vật lý (`pa`).
            - Nó thêm trang đó vào đầu danh sách liên kết các trang trống.
            - **Thao tác an toàn:** Trước khi thêm vào danh sách, nó ghi rác vào toàn bộ trang để giúp phát hiện các lỗi "sử dụng sau khi giải phóng" (use-after-free).
        - `kalloc()`: ✨
            - **Chức năng:** Cấp phát một trang vật lý.
            - Nó lấy trang đầu tiên ra khỏi danh sách liên kết, và trả về địa chỉ vật lý của nó.
            - Nếu hết bộ nhớ, nó trả về 0.
        
        ### 🧑‍💻 **3.6 Không gian Địa chỉ của Tiến trình**
        
        - **Hình 3.4** minh họa chi tiết bố cục không gian địa chỉ của một tiến trình người dùng, bao gồm cả nội dung ban đầu của ngăn xếp do `exec` tạo ra.
        - Ngăn xếp ban đầu chứa:
            - Các chuỗi đối số dòng lệnh (`argv`).
            - Một mảng các con trỏ trỏ đến các chuỗi đó.
            - Các giá trị giả để làm cho chương trình bắt đầu tại `main(argc, argv)` một cách chính xác.
        
        ### 📞 **3.7 & 3.8 Mã nguồn: `sbrk` và `exec`**
        
        - **`sbrk(n)` (thông qua `growproc`)**:
            - **Chức năng:** Mở rộng hoặc thu hẹp vùng heap của tiến trình.
            - Nếu `n > 0` (mở rộng):
                - Gọi `uvmalloc()`, hàm này sẽ dùng `kalloc()` để lấy các trang vật lý mới.
                - Sử dụng `mappages()` để ánh xạ các trang vật lý mới này vào bảng trang của tiến trình.
            - Nếu `n < 0` (thu hẹp):
                - Gọi `uvmdealloc()`, hàm này sẽ dùng `kfree()` để trả lại các trang vật lý.
                - Cập nhật các PTE trong bảng trang để đánh dấu chúng là không hợp lệ.
        - **`exec()`**:
            - **Chức năng:** Thay thế chương trình hiện tại bằng một chương trình mới.
            - **Các bước chính:**
                1. Đọc tiêu đề của tệp thực thi (định dạng **ELF**).
                2. **Tạo một bảng trang mới, trống rỗng**.
                3. Lặp qua các **đoạn (segments)** được mô tả trong tiêu đề ELF (thường là một đoạn cho mã lệnh và một đoạn cho dữ liệu).
                4. Với mỗi đoạn:
                    - Cấp phát bộ nhớ vật lý bằng `uvmalloc()`.
                    - Tải nội dung của đoạn từ tệp vào bộ nhớ vật lý vừa cấp phát.
                5. Cấp phát một trang cho **ngăn xếp người dùng**.
                6. Sao chép các đối số dòng lệnh vào đỉnh của ngăn xếp.
                7. Thiết lập một **trang bảo vệ (guard page)** bên dưới ngăn xếp.
                8. **Giải phóng bảng trang cũ**.
                9. Cài đặt bảng trang mới và chuẩn bị các thanh ghi để quay trở lại không gian người dùng và bắt đầu thực thi chương trình mới tại điểm vào của nó.

# 💣 **Chương 4: Bẫy (Traps) và Lời gọi Hệ thống**

### 🎯 **Mục tiêu chính**

Chương này giải thích cách kernel xử lý các sự kiện làm gián đoạn luồng thực thi bình thường.

### 💡 **Ba loại "Bẫy"**

"Bẫy" (trap) là một thuật ngữ chung cho ba loại sự kiện sau:

1. 📞 **Lời gọi Hệ thống (System Call):**
    - Một sự kiện **chủ động** và **đồng bộ**.
    - Xảy ra khi một chương trình người dùng thực thi lệnh `ecall`.
    - Mục đích: Yêu cầu một dịch vụ từ kernel.
2. 💥 **Ngoại lệ (Exception):**
    - Một sự kiện **bất ngờ** và **đồng bộ**.
    - Xảy ra khi một lệnh (trong user hoặc kernel mode) thực hiện một hành động bất hợp pháp.
    - **Ví dụ:** Chia cho 0, truy cập một địa chỉ bộ nhớ không hợp lệ (gây ra **lỗi trang - page fault**), thực thi một lệnh không xác định.
3. ⚡ **Ngắt (Interrupt):**
    - Một sự kiện **bất ngờ** và **bất đồng bộ**.
    - Gây ra bởi một thiết bị phần cứng bên ngoài CPU.
    - **Ví dụ:** Một gói tin mạng vừa đến, đĩa đã đọc xong dữ liệu, bộ định thời (timer) hết hạn.
    - Mục đích: Báo cho CPU rằng thiết bị cần được chú ý.

**Điểm chung:** Tất cả đều buộc CPU phải tạm dừng công việc hiện tại, chuyển sang kernel mode, và nhảy đến một trình xử lý (handler) đặc biệt để giải quyết sự kiện. Sau khi xử lý xong, kernel thường sẽ quay trở lại luồng thực thi đã bị gián đoạn một cách **trong suốt**.

### ⚙️ **4.1 Cơ chế Bẫy của RISC-V**

Phần cứng RISC-V cung cấp một bộ các **thanh ghi điều khiển (control registers)** để kernel cấu hình và xử lý bẫy. Các thanh ghi quan trọng nhất (ở Supervisor mode):

| Thanh ghi | Chức năng |
| --- | --- |
| `stvec` | (Supervisor Trap Vector) Kernel ghi **địa chỉ của trình xử lý bẫy** vào đây. Khi có bẫy, PC sẽ tự động nhảy đến địa chỉ này. |
| `sepc` | (Supervisor Exception Program Counter) Khi có bẫy, CPU **tự động lưu giá trị của PC** (địa chỉ lệnh gây ra bẫy) vào đây. |
| `scause` | (Supervisor Cause) CPU ghi một **mã số** vào đây để cho biết **nguyên nhân** của bẫy (ví dụ: mã 8 cho system call, mã 13/15 cho lỗi trang đọc/ghi). |
| `sstatus` | (Supervisor Status) Chứa các bit trạng thái, quan trọng nhất là bit `SIE` (Supervisor Interrupt Enable) để **bật/tắt ngắt**. |
| `sscratch` | (Supervisor Scratch) Một thanh ghi "nháp" mà kernel có thể sử dụng tạm thời trong trình xử lý bẫy mà không làm hỏng thanh ghi của người dùng. |

**Quy trình phần cứng khi có bẫy:**

1. (Nếu là ngắt) Kiểm tra `sstatus.SIE`. Nếu ngắt bị tắt, bỏ qua.
2. Tự động **tắt ngắt** (xóa bit `sstatus.SIE`).
3. Sao chép PC vào `sepc`.
4. Lưu lại chế độ hiện tại (user/supervisor).
5. Ghi nguyên nhân vào `scause`.
6. Chuyển sang **Supervisor Mode**.
7. Sao chép `stvec` vào PC.
8. Bắt đầu thực thi tại trình xử lý bẫy.

**Lưu ý:** Phần cứng **không** tự động chuyển bảng trang hay chuyển ngăn xếp. Kernel phải tự làm những việc này.

### 🚀 **4.2 Bẫy từ Không gian Người dùng**

Đây là luồng xử lý khi một bẫy (system call, exception, interrupt) xảy ra lúc CPU đang chạy mã người dùng.

**Vấn đề nan giải:** 🤯
Khi bẫy xảy ra, CPU vẫn đang sử dụng **bảng trang của người dùng**. Trình xử lý bẫy (`stvec`) phải nằm ở một địa chỉ ảo mà **có thể truy cập được** từ bảng trang này. Nhưng đồng thời, trình xử lý cần chuyển sang **bảng trang của kernel** để làm việc. Làm thế nào để thực hiện chuyển đổi này mà không bị "mất dấu"?

**Giải pháp của xv6: Trang Trampoline** 🤸

- **Trang Trampoline (TRAMPOLINE):** Một trang vật lý duy nhất chứa mã xử lý bẫy cấp thấp (`uservec`).
- **Ánh xạ kép:** Trang này được ánh xạ vào **MỌI không gian địa chỉ**:
    1. Nó được ánh xạ vào một địa chỉ ảo **cố định và rất cao** trong **bảng trang của mỗi tiến trình người dùng**.
    2. Nó cũng được ánh xạ tại **cùng một địa chỉ ảo cao đó** trong **bảng trang của kernel**.
- **Lợi ích:**
    - Khi bẫy xảy ra, CPU nhảy đến `uservec` thông qua ánh xạ của người dùng.
    - Sau khi `uservec` chuyển `satp` sang bảng trang của kernel, nó vẫn có thể tiếp tục thực thi vì mã của nó cũng được ánh xạ ở đó. Nó hoạt động như một "cầu nối" an toàn.

**Luồng thực thi chi tiết:**

1. **Bẫy xảy ra!** ➡️ CPU nhảy đến `uservec` (trong `trampoline.S`).
2. **`uservec` (Assembly):**
    - Dùng thanh ghi `sscratch` để tạm thời lưu một thanh ghi (ví dụ `a0`).
    - **Lưu tất cả 32 thanh ghi người dùng** vào một cấu trúc dữ liệu đặc biệt gọi là **trapframe**, được cấp phát cho mỗi tiến trình.
    - Tải địa chỉ của bảng trang kernel, ngăn xếp kernel, và hàm `usertrap` từ trapframe.
    - **Chuyển sang bảng trang của kernel** (bằng cách ghi vào `satp`).
    - Nhảy đến hàm C `usertrap`.
3. **`usertrap` (C):**
    - Đây là trình xử lý bẫy cấp cao. Nó thực hiện logic chính.
    - Dựa vào thanh ghi `scause` để xác định nguyên nhân bẫy:
        - Nếu là **system call**: Gọi hàm `syscall()`. Tăng `sepc` lên 4 để quay trở lại lệnh *tiếp theo* sau `ecall`.
        - Nếu là **ngắt thiết bị**: Gọi `devintr()` để xử lý.
        - Nếu là **ngoại lệ** (ví dụ: lỗi trang): Kết thúc tiến trình.
    - Trước khi quay trở lại, kiểm tra xem tiến trình có cần bị kết thúc hoặc nhường CPU (yield) không.
4. **`usertrapret` (C) & `userret` (Assembly):**
    - Chuẩn bị để quay trở lại không gian người dùng.
    - `userret` thực hiện các thao tác ngược lại với `uservec`:
        - **Chuyển về bảng trang của người dùng**.
        - **Khôi phục tất cả 32 thanh ghi người dùng** từ trapframe.
        - Thực thi lệnh `sret` (Supervisor Return) để quay trở lại không gian người dùng, khôi phục PC từ `sepc` và chuyển về User Mode.

### 📞 **4.3 & 4.4 Mã nguồn: Gọi System Call và Truyền Đối số**

- **`syscall()`:**
    - Lấy số hiệu system call từ thanh ghi `a7` đã được lưu trong trapframe.
    - Dùng số hiệu này làm chỉ số vào một mảng các con trỏ hàm (`syscalls[]`) để gọi hàm triển khai tương ứng (ví dụ: `sys_fork`, `sys_read`).
    - Giá trị trả về của hàm `sys_*` được ghi vào thanh ghi `a0` trong trapframe, để chương trình người dùng có thể nhận được kết quả.
- **Truyền đối số:**
    - Các đối số của system call (đặc biệt là các con trỏ) được truyền từ không gian người dùng.
    - **Rủi ro bảo mật:** Kernel **không bao giờ được tin tưởng** các con trỏ này. Một con trỏ xấu có thể trỏ vào bộ nhớ của kernel.
    - **Giải pháp:** Kernel cung cấp các hàm an toàn như `copyin` và `copyout` để sao chép dữ liệu giữa không gian kernel và không gian người dùng. Các hàm này sẽ cẩn thận kiểm tra và dịch địa chỉ ảo của người dùng bằng bảng trang của tiến trình đó, đảm bảo không có truy cập bất hợp pháp.

### 🔥 **4.6 Ngoại lệ Lỗi trang (Page-Fault Exceptions)**

- **Page Fault** xảy ra khi CPU cố gắng truy cập một địa chỉ ảo mà:
    - Không có ánh xạ (PTE không hợp lệ, `V=0`).
    - Vi phạm quyền truy cập (ví dụ: ghi vào một trang chỉ đọc, `W=0`).
- **Phản ứng của xv6:** Rất đơn giản. Nếu lỗi trang xảy ra trong không gian người dùng, **kernel sẽ kết thúc tiến trình đó**.

💡 **Các kỹ thuật nâng cao sử dụng Page Fault (không có trong xv6):**

- **Copy-on-Write (COW) Fork:** 🐄
    - Khi `fork()`, thay vì sao chép toàn bộ bộ nhớ, kernel cho phép tiến trình cha và con **chia sẻ chung** tất cả các trang vật lý, nhưng đánh dấu chúng là **chỉ đọc (read-only)** trong cả hai bảng trang.
    - Nếu một trong hai tiến trình cố gắng **ghi** vào một trang được chia sẻ, một page fault sẽ xảy ra.
    - Kernel sẽ bắt lỗi này, tạo một bản sao của trang đó, ánh xạ bản sao mới vào bảng trang của tiến trình gây lỗi với quyền ghi, và sau đó cho phép tiến trình tiếp tục.
    - **Lợi ích:** `fork()` trở nên cực kỳ nhanh, và bộ nhớ chỉ được sao chép khi thực sự cần thiết.
- **Cấp phát lười (Lazy Allocation):** 😴
    - Khi một tiến trình yêu cầu thêm bộ nhớ (ví dụ qua `sbrk`), kernel chỉ cập nhật kích thước bộ nhớ của nó mà **không thực sự cấp phát trang vật lý nào**.
    - Lần đầu tiên tiến trình truy cập vào một trang trong vùng nhớ mới, một page fault sẽ xảy ra.
    - Kernel sẽ bắt lỗi này, cấp phát một trang vật lý, ánh xạ nó vào bảng trang, và cho tiến trình tiếp tục.
- **Phân trang ra đĩa (Paging to Disk):** 💾
    - Khi thiếu RAM, kernel có thể di chuyển một số trang ít được sử dụng từ RAM ra một vùng đặc biệt trên đĩa (paging area).
    - PTE tương ứng sẽ được đánh dấu là không hợp lệ.
    - Nếu tiến trình truy cập lại trang đó, một page fault sẽ xảy ra. Kernel sẽ đọc lại trang từ đĩa vào RAM (có thể phải đẩy một trang khác ra trước) và cho tiến trình tiếp tục.

# 🔌 **Chương 5: Ngắt và Trình điều khiển Thiết bị**

### 🎯 **Mục tiêu chính**

- **Trình điều khiển (Driver):** Là phần mã trong kernel chịu trách nhiệm quản lý một thiết bị phần cứng cụ thể.
- Nhiệm vụ của driver:
    - Cấu hình thiết bị.
    - Yêu cầu thiết bị thực hiện một thao tác (ví dụ: đọc một khối từ đĩa).
    - Xử lý ngắt khi thiết bị hoàn thành.
    - Cung cấp giao diện cho các tiến trình (thường thông qua các system call như `read`, `write`).

### 💡 **Cấu trúc Driver: Top Half vs. Bottom Half**

Hầu hết các driver được chia thành hai phần:

1. **Top Half (Nửa trên):**
    - Chạy trong ngữ cảnh của một tiến trình đang thực hiện system call (ví dụ: `read`).
    - Nó khởi tạo một yêu cầu tới phần cứng (ví dụ: yêu cầu UART gửi một ký tự).
    - Sau đó, nó thường sẽ **ngủ (sleep)**, chờ đợi thiết bị hoàn thành.
2. **Bottom Half (Nửa dưới):**
    - Chạy trong ngữ-cảnh-ngắt, **không liên quan** đến bất kỳ tiến trình cụ thể nào.
    - Được kích hoạt khi thiết bị phần cứng phát ra một **ngắt (interrupt)**.
    - Nó xác định thao tác nào đã hoàn tất.
    - Thực hiện các công việc cần thiết (ví dụ: sao chép dữ liệu nhận được vào bộ đệm).
    - **Đánh thức (wakeup)** tiến trình đang ngủ ở top half.

### ⌨️ **5.1 & 5.2 Mã nguồn: Trình điều khiển Console**

Ví dụ về driver `console.c` (giao tiếp với phần cứng UART mô phỏng).

- **Đầu vào (Khi bạn gõ phím):**
    1. Phần cứng UART nhận một ký tự và **phát ra một ngắt**.
    2. Trình xử lý bẫy của kernel gọi `uartintr()` (đây là **bottom half**).
    3. `uartintr()` đọc ký tự từ thanh ghi của UART và đưa nó vào một **bộ đệm đầu vào (input buffer)** trong kernel.
    4. Nó cũng xử lý các ký tự đặc biệt như backspace.
    5. Nếu một dòng hoàn chỉnh (kết thúc bằng newline) được nhập, nó sẽ gọi `wakeup()` để đánh thức bất kỳ tiến trình nào đang chờ đọc.
    6. Trong khi đó, một tiến trình (như shell) có thể đã gọi `read(0, ...)` và bị đưa vào trạng thái ngủ (sleep) bởi `consoleread()` (đây là **top half**).
    7. Khi được `wakeup`, `consoleread()` sẽ sao chép dòng từ bộ đệm của kernel vào bộ đệm của người dùng và trả về.
- **Đầu ra (Khi chương trình `printf`):**
    1. Một tiến trình gọi `write(1, ...)` (đây là **top half**).
    2. `uartputc()` sao chép từng ký tự vào một **bộ đệm đầu ra (output buffer)** trong kernel.
    3. Nó bắt đầu quá trình truyền ký tự đầu tiên.
    4. Khi UART gửi xong một ký tự, nó **phát ra một ngắt**.
    5. `uartintr()` (đây là **bottom half**) sẽ lấy ký tự tiếp theo từ bộ đệm đầu ra và yêu cầu UART gửi nó đi. Quá trình này lặp lại cho đến khi bộ đệm trống.

💡 **Lợi ích của việc đệm (Buffering):**
Cơ chế top/bottom half cùng với bộ đệm cho phép **I/O đồng thời (I/O Concurrency)**. Tiến trình có thể tiếp tục công việc khác ngay sau khi `write` sao chép dữ liệu vào bộ đệm, mà không cần phải chờ thiết bị phần cứng chậm chạp gửi xong từng byte.

### 🤝 **5.3 Đồng thời trong Trình điều khiển**

Viết mã cho trình điều khiển rất phức tạp vì các vấn đề về đồng thời:

1. **Nhiều tiến trình cùng lúc:** Hai tiến trình trên hai CPU khác nhau có thể cùng gọi `read` trên console.
2. **Ngắt xen ngang:** CPU đang thực thi mã *top half* (ví dụ `consoleread`) có thể bị một ngắt từ chính thiết bị đó làm gián đoạn, và bắt đầu chạy mã *bottom half* (`consoleintr`).
3. **Ngắt trên CPU khác:** Một CPU đang chạy `consoleread`, trong khi một CPU khác nhận ngắt và chạy `consoleintr`.

Nếu không được bảo vệ, những tình huống này sẽ dẫn đến **xung đột (race conditions)** trên các cấu trúc dữ liệu của driver (như bộ đệm).

- **Giải pháp:** Sử dụng **khóa (locks)**. Bạn sẽ thấy các lệnh gọi `acquire()` và `release()` trong mã nguồn của driver. Chúng đảm bảo rằng tại một thời điểm, chỉ có một luồng (tiến trình hoặc trình xử lý ngắt) có thể truy cập vào các dữ liệu được chia sẻ. Chương 6 sẽ đi sâu vào chi tiết về khóa.

### ⏱️ **5.4 Ngắt Bộ định thời (Timer Interrupts)**

Đây là một loại ngắt cực kỳ quan trọng, là nền tảng cho việc **đa nhiệm ưu tiên (preemptive multitasking)**.

- **Nguồn gốc:** Mỗi lõi CPU có một thiết bị định thời phần cứng riêng. Kernel lập trình cho thiết bị này để nó phát ra một ngắt một cách **định kỳ** (ví dụ, 100 lần mỗi giây).
- **Mục đích trong xv6:**
    1. **Duy trì thời gian hệ thống:** Trình xử lý ngắt bộ định thời (`clockintr`) sẽ tăng một biến toàn cục gọi là `ticks`.
    2. **Lập lịch lại (Rescheduling):** Ngắt này cho phép kernel **giành lại quyền kiểm soát** từ tiến trình đang chạy, ngay cả khi tiến trình đó đang trong một vòng lặp vô hạn và không tự nguyện nhường CPU.
    3. Sau khi xử lý ngắt, kernel sẽ gọi `yield()`, buộc tiến trình hiện tại phải nhường CPU và cho bộ lập lịch cơ hội để chạy một tiến trình khác.

💡 **Hệ quả quan trọng:** Bất kỳ đoạn mã nào trong kernel cũng có thể bị ngắt bộ định thời làm gián đoạn và bị chuyển sang một CPU khác. Điều này đòi hỏi các lập trình viên kernel phải viết mã rất cẩn thận để đối phó với sự đồng thời.

### 🌎 **5.5 Thế giới thực**

- **Programmed I/O (PIO) vs. Direct Memory Access (DMA):**
    - Trình điều khiển UART của xv6 sử dụng **PIO**: CPU phải trực tiếp tham gia vào việc di chuyển từng byte dữ liệu giữa thiết bị và bộ nhớ. Đơn giản nhưng **chậm**.
    - Các thiết bị tốc độ cao (như đĩa cứng, card mạng) sử dụng **DMA**: Kernel chỉ cần cung cấp cho thiết bị một con trỏ đến một vùng đệm trong RAM. Sau đó, thiết bị sẽ **tự động** đọc/ghi dữ liệu vào RAM mà **không cần sự can thiệp của CPU**. CPU có thể làm việc khác trong thời gian đó. Khi hoàn tất, thiết bị sẽ phát một ngắt.
- **Ngắt (Interrupt) vs. Thăm dò (Polling):**
    - **Ngắt** rất tốt cho các sự kiện không thường xuyên, nhưng chi phí (overhead) để xử lý một ngắt khá cao.
    - **Polling** là việc CPU liên tục kiểm tra trạng thái của thiết bị trong một vòng lặp (`while (device_is_busy) {}`). Tốn tài nguyên CPU nhưng có độ trễ thấp hơn.
    - Các driver hiện đại thường **kết hợp cả hai**: chúng bắt đầu bằng ngắt, nhưng nếu thấy thiết bị đang hoạt động ở tốc độ cao, chúng sẽ tạm thời tắt ngắt và chuyển sang polling để giảm overhead.

# 🔒 **Chương 6: Khóa (Locking)**

### 🎯 **Mục tiêu chính**

Chương này giới thiệu kỹ thuật cơ bản nhất để quản lý **sự đồng thời (concurrency)**: **khóa (locks)**.

- **Vấn đề:** Khi nhiều luồng thực thi (trên nhiều CPU hoặc do chuyển ngữ cảnh) cùng truy cập và sửa đổi một dữ liệu được chia sẻ, kết quả có thể bị sai lệch hoặc cấu trúc dữ liệu có thể bị hỏng. Tình huống này được gọi là **xung đột (race condition)**.
- **Giải pháp:** Khóa cung cấp **loại trừ lẫn nhau (mutual exclusion)**.
    - Một khóa được liên kết với một (hoặc một nhóm) dữ liệu được chia sẻ.
    - Trước khi truy cập dữ liệu, một luồng phải **giành được (acquire)** khóa.
    - Sau khi truy cập xong, luồng phải **nhả (release)** khóa.
    - Cơ chế khóa đảm bảo rằng **tại một thời điểm, chỉ có một luồng duy nhất có thể giữ khóa**, và do đó, chỉ có một luồng duy nhất có thể truy cập vào dữ liệu được bảo vệ.

### race_car: **6.1 Xung đột (Races)**

Hãy xem xét một ví dụ đơn giản: hai CPU cùng lúc gọi hàm `kfree()` để trả một trang nhớ về danh sách các trang trống.

```c
// Mã giả của kfree (không có khóa)
void kfree(page *p) {
    p->next = freelist; // (1) Đặt con trỏ next của trang mới
    freelist = p;       // (2) Cập nhật đầu danh sách
}

```

**Kịch bản xung đột:**

1. CPU 1 thực thi dòng (1), `p1->next` trỏ đến đầu danh sách cũ (`old_head`).
2. **Trước khi** CPU 1 thực thi dòng (2), CPU 2 cũng thực thi dòng (1). `p2->next` cũng trỏ đến `old_head`.
3. CPU 1 thực thi dòng (2), `freelist` bây giờ trỏ đến `p1`.
4. CPU 2 thực thi dòng (2), `freelist` bây giờ trỏ đến `p2`.

**Kết quả:** 😱 Trang `p1` đã **bị mất**! Đầu danh sách trỏ đến `p2`, `p2->next` trỏ đến `old_head`, không có gì trỏ đến `p1` cả. Đây là một lỗi nghiêm trọng.

Đoạn mã giữa lúc một luồng bắt đầu sửa đổi dữ liệu được chia sẻ và lúc nó hoàn thành được gọi là **vùng tranh chấp (critical section)**. Khóa được dùng để bảo vệ các vùng tranh chấp này.

### 🛠️ **6.2 Mã nguồn: Khóa**

xv6 cung cấp hai loại khóa chính:

1. **Spinlocks (Khóa xoay):**
    - **Triển khai:** Được biểu diễn bằng `struct spinlock`. Một trường `locked` trong struct này cho biết khóa đang được giữ hay không.
    - **`acquire(lock)`:**
        - Sử dụng một **lệnh nguyên tử (atomic instruction)** của CPU (như `amoswap` trên RISC-V). Lệnh này đọc giá trị cũ của `lock->locked` và ghi giá trị mới (1) vào đó trong **một bước duy nhất, không thể bị xen ngang**.
        - `acquire` thực hiện lệnh này trong một vòng lặp. Nếu giá trị cũ là 0 (khóa tự do), nó đã thành công giành được khóa và thoát khỏi vòng lặp.
        - Nếu giá trị cũ là 1 (khóa đang bị giữ), nó sẽ tiếp tục **"xoay" (spin)** trong vòng lặp, lặp đi lặp lại việc thử giành khóa.
    - **`release(lock)`:**
        - Sử dụng một lệnh nguyên tử để đặt `lock->locked` về 0.
    - **Đặc điểm:**
        - **Hiệu quả cho các vùng tranh chấp ngắn.**
        - **Lãng phí tài nguyên CPU** nếu phải chờ lâu, vì CPU chỉ bận rộn xoay vòng mà không làm gì hữu ích.
        - **Quan trọng:** Trong khi giữ một spinlock, **ngắt phải bị tắt** trên CPU đó để tránh deadlock. Nếu một ngắt xảy ra và trình xử lý ngắt cũng cố gắng `acquire` cùng một khóa, hệ thống sẽ bị treo.
2. **Sleeplocks (Khóa ngủ):** (Sẽ được thảo luận chi tiết hơn trong Chương 7)
    - **Triển khai:**
        - Nếu một tiến trình cố gắng giành một sleeplock đang bị giữ, thay vì xoay vòng, nó sẽ **nhường CPU** và đi vào trạng thái **ngủ (SLEEPING)**.
        - Khi khóa được nhả, tiến trình đang giữ khóa sẽ **đánh thức (wakeup)** một trong các tiến trình đang ngủ.
    - **Đặc điểm:**
        - **Hiệu quả cho các vùng tranh chấp dài** (ví dụ: chờ thao tác I/O trên đĩa).
        - Không lãng phí CPU.
        - Không thể sử dụng trong các trình xử lý ngắt (vì trình xử lý ngắt không thể ngủ).

### 🤝 **6.3 Mã nguồn: Sử dụng Khóa**

Việc sử dụng khóa đúng cách là một nghệ thuật. Dưới đây là một số nguyên tắc và ví dụ:

- **Nguyên tắc:**
    1. Bất kỳ biến nào có thể được **ghi** bởi một luồng trong khi một luồng khác có thể **đọc hoặc ghi** nó, đều cần được bảo vệ bởi một khóa.
    2. Nếu một **bất biến (invariant)** liên quan đến nhiều biến, tất cả các biến đó phải được bảo vệ bởi **cùng một khóa**. (Ví dụ: trong một danh sách liên kết kép, cả con trỏ `next` của nút trước và con trỏ `prev` của nút sau phải được cập nhật bên trong cùng một vùng tranh chấp).
- **Độ mịn của khóa (Lock Granularity):**
    - **Khóa thô (Coarse-grained):** Dùng một khóa duy nhất để bảo vệ một cấu trúc dữ liệu lớn hoặc toàn bộ một hệ thống con.
        - **Ví dụ:** `kalloc.c` của xv6 sử dụng một khóa duy nhất (`kmem.lock`) cho toàn bộ danh sách các trang trống.
        - **Ưu điểm:** Đơn giản, dễ lập luận về tính đúng đắn.
        - **Nhược điểm:** **Hạn chế tính song song**. Nếu hai CPU cùng gọi `kalloc`, một CPU phải chờ, làm mất đi lợi ích của hệ thống đa lõi.
    - **Khóa mịn (Fine-grained):** Dùng nhiều khóa nhỏ hơn, mỗi khóa bảo vệ một phần nhỏ của dữ liệu.
        - **Ví dụ:** Hệ thống tệp của xv6 có một khóa riêng cho mỗi tệp (`inode->lock`).
        - **Ưu điểm:** **Tăng tính song song**. Các tiến trình thao tác trên các tệp khác nhau có thể chạy đồng thời mà không xung đột.
        - **Nhược điểm:** Phức tạp hơn, có nguy cơ **deadlock** cao hơn.

### 🔗 **6.4 Khóa chết (Deadlock) và Thứ tự Khóa**

- **Deadlock là gì?** 💀
    - Xảy ra khi hai (hoặc nhiều) luồng đang chờ nhau một cách vô tận.
    - **Kịch bản kinh điển:**
        1. Luồng A giành được Khóa 1.
        2. Luồng B giành được Khóa 2.
        3. Luồng A cố gắng giành Khóa 2 (và bị chặn vì Luồng B đang giữ nó).
        4. Luồng B cố gắng giành Khóa 1 (và bị chặn vì Luồng A đang giữ nó).
        - => Cả hai luồng sẽ chờ nhau mãi mãi.
- **Giải pháp:** 📜
    - **Thiết lập một thứ tự giành khóa toàn cục (global lock acquisition order).**
    - Tất cả các đoạn mã trong toàn bộ kernel phải tuân thủ nghiêm ngặt quy tắc: nếu cần giành nhiều khóa, phải luôn giành chúng theo **cùng một thứ tự đã định sẵn**.
    - **Ví dụ:** Nếu quy tắc là "luôn giành Khóa 1 trước Khóa 2", thì kịch bản deadlock ở trên sẽ không thể xảy ra. Luồng B sẽ bị chặn ngay khi cố gắng giành Khóa 1, trước khi nó có cơ hội giành Khóa 2 và tạo ra vòng lặp chờ đợi.

💡 **Thứ tự khóa trong xv6:**

- xv6 có nhiều chuỗi thứ tự khóa phức tạp. Ví dụ, khi tạo một tệp, kernel phải giành khóa theo thứ tự: khóa thư mục -> khóa inode của tệp mới -> khóa bộ đệm đĩa... Việc vi phạm thứ tự này sẽ dẫn đến deadlock.

### 🔁 **6.5 Khóa tái nhập (Re-entrant Locks)**

- **Khái niệm:** Một loại khóa cho phép cùng một luồng có thể `acquire` khóa đó nhiều lần mà không bị deadlock với chính nó. Khóa chỉ được nhả ra khi số lần `release` bằng số lần `acquire`.
- **xv6 có dùng không?** **KHÔNG.**
- **Lý do:** Khóa tái nhập làm cho việc lập luận về tính đúng đắn của mã trở nên khó khăn hơn. Chúng phá vỡ trực giác rằng một vùng tranh chấp là **nguyên tử (atomic)**. Nếu một hàm `f()` giữ khóa và gọi một hàm `h()`, và `h()` lại gọi một hàm `g()` cũng cố gắng giành khóa đó, thì với khóa tái nhập, `g()` sẽ thành công. Điều này có thể dẫn đến việc một bất biến bị vi phạm mà `f()` không ngờ tới.
- Với khóa không-tái-nhập (như trong xv6), kịch bản trên sẽ gây ra **deadlock** ngay lập tức. Mặc dù không lý tưởng, nhưng một deadlock rõ ràng thường dễ gỡ lỗi hơn là một lỗi logic tinh vi do phá vỡ tính nguyên tử.

# 🏃 **Chương 7: Lập lịch (Scheduling)**

### 🎯 **Mục tiêu chính**

Lập lịch là quá trình HĐH quyết định **tiến trình nào sẽ được chạy trên CPU** và **trong bao lâu**. Mục tiêu là tạo ra ảo giác rằng tất cả các tiến trình đang chạy đồng thời.

### 💡 **Khi nào xv6 chuyển đổi tiến trình?**

1. **Chuyển đổi tự nguyện (Voluntary):**
    - Khi một tiến trình thực hiện một system call mà nó phải chờ đợi một sự kiện nào đó.
    - **Ví dụ:**
        - `read()` trên một pipe rỗng.
        - `wait()` chờ một tiến trình con kết thúc.
        - `sleep()` để tạm dừng trong một khoảng thời gian.
    - Trong những trường hợp này, tiến trình sẽ gọi `sleep()`, tự nguyện nhường CPU.
2. **Chuyển đổi bắt buộc (Involuntary / Preemptive):**
    - Khi một **ngắt bộ định thời (timer interrupt)** xảy ra.
    - Điều này cho phép kernel giành lại quyền kiểm soát từ một tiến trình đang tính toán miệt mài (ví dụ: trong một vòng lặp vô hạn) và không tự nguyện nhường CPU.
    - Kernel sẽ buộc tiến trình đó phải `yield()` CPU.

### ⚙️ **7.2 Mã nguồn: Chuyển đổi Ngữ cảnh (Context Switching)**

Đây là cơ chế cấp thấp để chuyển CPU từ luồng này sang luồng khác.

- **Ngữ cảnh (Context):** Là tập hợp các thanh ghi của CPU (`ra`, `sp`, `s0-s11`, ...). Nó đại diện cho trạng thái thực thi của một luồng tại một thời điểm.
- **Hàm `swtch(old_context, new_context)` (Assembly):**
    - Đây là một hàm "ma thuật" được viết bằng mã máy.
    - **Chức năng:**
        1. **Lưu** tất cả các thanh ghi hiện tại vào cấu trúc `old_context`.
        2. **Tải** tất cả các thanh ghi từ cấu trúc `new_context`.
        3. `return`.
    - **Điều kỳ diệu:** Lệnh `return` không quay trở lại nơi `swtch` được gọi, mà nó "quay trở lại" nơi mà luồng *mới* đã từng gọi `swtch` trong quá khứ! Bởi vì thanh ghi `ra` (return address) đã được thay đổi. Cùng lúc đó, thanh ghi `sp` (stack pointer) cũng được thay đổi, nên việc chuyển đổi này cũng là **chuyển đổi ngăn xếp**.

### 👨‍⚖️ **7.3 Mã nguồn: Lập lịch**

Luồng chuyển đổi từ tiến trình A sang tiến trình B:

1. Tiến trình A (trong kernel mode, ví dụ sau một ngắt) quyết định nhường CPU (ví dụ, gọi `yield()`).
2. `yield()` gọi `sched()`.
3. **`sched()`:**
    - Đảm bảo các điều kiện an toàn (ví dụ: đang giữ khóa tiến trình).
    - Gọi `swtch(&A->context, &cpu->scheduler_context)`.
    - Ngữ cảnh của A được lưu lại, và CPU bắt đầu thực thi trong ngữ cảnh của **luồng lập lịch riêng của CPU**.
4. **`scheduler()`:**
    - Đây là một vòng lặp vô tận, chạy trên một ngăn xếp riêng (không phải ngăn xếp của tiến trình nào).
    - Nó **quét qua bảng tiến trình** để tìm một tiến trình có trạng thái là `RUNNABLE`.
    - Giả sử nó tìm thấy tiến trình B.
    - Nó gọi `swtch(&cpu->scheduler_context, &B->context)`.
    - Ngữ cảnh của bộ lập lịch được lưu lại, và CPU bắt đầu thực thi trong ngữ cảnh của B, chính xác tại nơi B đã từng gọi `sched()` để đi ngủ.

💡 **Coroutines:** `sched()` và `scheduler()` có thể được coi là các **coroutines**. Chúng chuyển quyền điều khiển qua lại cho nhau thông qua `swtch`.

### 📌 **7.4 Mã nguồn: `mycpu` và `myproc`**

- Làm thế nào kernel biết nó đang chạy trên CPU nào và cho tiến trình nào?
- Trên một hệ thống đa lõi, không thể dùng biến toàn cục.
- **Giải pháp của xv6:**
    - Khi khởi động, mỗi CPU được gán một **ID phần cứng (hartid)** duy nhất.
    - Kernel đảm bảo rằng thanh ghi `tp` của RISC-V **luôn chứa hartid** của CPU hiện tại.
    - **`mycpu()`:** Đọc `tp` để lấy hartid, sau đó dùng nó làm chỉ số vào một mảng các cấu trúc `cpu` để tìm ra cấu trúc dữ liệu cho CPU hiện tại.
    - **`myproc()`:** Gọi `mycpu()` để lấy struct `cpu`, sau đó trả về con trỏ `proc` được lưu trong đó.
    - **Quan trọng:** Cần phải **tắt ngắt** khi gọi các hàm này để tránh trường hợp tiến trình bị di chuyển sang CPU khác ngay giữa chừng, làm cho kết quả trả về bị sai.

### 😴 **7.5 & 7.6 Ngủ và Đánh thức (`Sleep` và `Wakeup`)**

`sleep` và `wakeup` là một cặp cơ chế **đồng bộ hóa có điều kiện (conditional synchronization)**. Chúng cho phép một luồng tạm dừng việc thực thi (ngủ) để chờ một điều kiện nào đó trở thành đúng, và một luồng khác có thể thông báo (đánh thức) rằng điều kiện đó đã xảy ra.

**Vấn đề: "Thức giấc bị lỡ" (Lost Wakeup)** 😱
Hãy tưởng tượng một kịch bản đơn giản giữa nhà sản xuất (Producer) và người tiêu dùng (Consumer).

```c
// Phiên bản lỗi của Consumer
while (count == 0) {
    // ??? Nếu CPU bị ngắt ở đây
    sleep(channel);
}
// ... tiêu thụ dữ liệu ...

```

**Kịch bản lỗi:**

1. Consumer kiểm tra `count` và thấy nó bằng 0.
2. **Đúng lúc này**, trước khi Consumer gọi `sleep()`, một ngắt xảy ra và CPU chuyển sang chạy Producer.
3. Producer tăng `count` lên 1 và gọi `wakeup(channel)`.
4. Vì Consumer chưa ngủ, lời gọi `wakeup` **không có tác dụng gì**. "Lời đánh thức" này đã bị lãng phí.
5. CPU chuyển lại cho Consumer. Nó tiếp tục thực thi và gọi `sleep()`, đi vào giấc ngủ.
6. **Kết quả:** Consumer ngủ mãi mãi, chờ đợi một lời đánh thức đã xảy ra trong quá khứ. Đây là lỗi **lost wakeup**.

**Giải pháp: Nguyên tử hóa việc kiểm tra và ngủ** ⚛️
Để giải quyết vấn đề này, việc kiểm tra điều kiện và hành động đi ngủ phải được thực hiện một cách **nguyên tử**. `sleep` trong xv6 được thiết kế để làm điều này.

**Giao diện đúng của `sleep` trong xv6:**`sleep(channel, lock)`

- `channel`: Một giá trị tùy ý (thường là địa chỉ của một cấu trúc dữ liệu) để xác định sự kiện đang chờ.
- `lock`: Một con trỏ đến một **spinlock** đang được luồng gọi giữ.

**`sleep(channel, lock)` thực hiện các bước sau một cách nguyên tử:**

1. Đánh dấu tiến trình hiện tại là `SLEEPING` và ghi lại `channel`.
2. Gọi `sched()` để bắt đầu quá trình chuyển ngữ cảnh.
3. **Quan trọng:** Hàm `sched()` sẽ **nhả (release)** `lock` *sau khi* tiến trình đã an toàn trên đường đi ngủ.

**`wakeup(channel)`:**

1. Quét qua toàn bộ bảng tiến trình.
2. Tìm tất cả các tiến trình có trạng thái là `SLEEPING` và đang chờ trên `channel` đã cho.
3. Chuyển trạng thái của chúng thành `RUNNABLE`.

**Luồng hoạt động đúng:**

```c
acquire(&lock);
while (condition_is_false) {
    sleep(channel, &lock);
}
// ... thực hiện công việc ...
release(&lock);

```

1. Consumer giành được khóa `lock`.
2. Nó kiểm tra điều kiện (ví dụ: `count == 0`) và thấy điều kiện sai.
3. Nó gọi `sleep(channel, &lock)`.
4. Bên trong `sleep`, tiến trình được đánh dấu `SLEEPING`. Sau đó, `lock` được nhả ra.
5. Bây giờ, Producer có thể giành được `lock`, thay đổi điều kiện (tăng `count`), và gọi `wakeup(channel)`. Vì Consumer đã được đánh dấu là `SLEEPING`, lời gọi `wakeup` sẽ tìm thấy nó và đánh thức nó dậy.
6. Khi Consumer chạy lại (sau khi `sleep` trả về), nó **vẫn đang giữ khóa `lock`** (khóa được `sleep` tự động giành lại trước khi trả về). Nó sẽ quay lại đầu vòng lặp `while`, kiểm tra lại điều kiện, và lần này sẽ thấy điều kiện đúng và thoát khỏi vòng lặp.

💡 **Tại sao phải dùng vòng lặp `while`?**
Vì có thể có nhiều tiến trình cùng ngủ trên một channel. Khi `wakeup` được gọi, tất cả chúng đều được đánh thức. Nhưng chỉ có một tiến trình đầu tiên chạy sẽ giành được khóa và "tiêu thụ" sự kiện. Các tiến trình còn lại, khi chạy, sẽ thấy điều kiện lại trở về sai, và chúng phải quay lại ngủ. Đây được gọi là **spurious wakeup**.

---

### 🔗 **7.7 Mã nguồn: Đường ống (Pipes)**

Việc triển khai pipe trong `pipe.c` là một ví dụ kinh điển về Producer-Consumer sử dụng `sleep` và `wakeup`.

- **`pipewrite()` (Producer):**
    - Giành khóa của pipe.
    - Lặp qua các byte để ghi.
    - Nếu bộ đệm **đầy** (`nwrite == nread + PIPESIZE`):
        - Gọi `wakeup()` để đánh thức bất kỳ reader nào có thể đang ngủ.
        - Gọi `sleep()` để chờ cho đến khi có không gian trống, truyền vào khóa của pipe.
    - Ghi một byte vào bộ đệm.
    - Sau khi ghi xong, gọi `wakeup()` lần nữa để đánh thức reader.
    - Nhả khóa.
- **`piperead()` (Consumer):**
    - Giành khóa của pipe.
    - Nếu bộ đệm **rỗng** (`nread == nwrite`):
        - Gọi `sleep()` để chờ cho đến khi có dữ liệu, truyền vào khóa của pipe.
    - Đọc một byte từ bộ đệm.
    - Sau khi đọc, gọi `wakeup()` để đánh thức bất kỳ writer nào có thể đang ngủ vì bộ đệm đầy.
    - Nhả khóa.

### 🏁 **7.8 Mã nguồn: `wait`, `exit`, và `kill`**

Đây là một ví dụ phức tạp hơn về sự phối hợp giữa các tiến trình.

- **`exit()`:**
    1. Đóng tất cả các tệp đang mở.
    2. "Trao lại" tất cả các tiến trình con (nếu có) cho tiến trình `init`.
    3. **Đánh thức tiến trình cha** của nó (bằng cách `wakeup(parent)`), phòng trường hợp cha đang ngủ trong `wait()`.
    4. Thay đổi trạng thái của chính mình thành **`ZOMBIE`**.
    5. Gọi `sched()` để nhường CPU **vĩnh viễn**. Tiến trình `ZOMBIE` không bao giờ chạy lại, nó chỉ tồn tại như một "bóng ma" chứa PID và trạng thái thoát.
- **`wait()`:**
    1. Quét qua bảng tiến trình để tìm một tiến trình con có trạng thái là `ZOMBIE`.
    2. Nếu tìm thấy:
        - Thu thập PID và trạng thái thoát của con.
        - **Dọn dẹp** hoàn toàn tài nguyên của tiến trình con (giải phóng bộ nhớ, kernel stack, và cấu trúc `proc`). Đây là lúc tiến trình con thực sự biến mất.
        - Trả về PID của con.
    3. Nếu không tìm thấy con nào là `ZOMBIE` (nhưng vẫn còn con đang chạy):
        - Gọi `sleep()` để chờ một trong các con gọi `exit()`.
        - Khi được đánh thức, lặp lại từ bước 1.
- **`kill(pid)`:**
    - Không thực sự "giết" tiến trình ngay lập tức. Điều này quá nguy hiểm vì tiến trình nạn nhân có thể đang ở giữa một thao tác quan trọng trong kernel.
    - `kill` chỉ làm một việc đơn giản: tìm tiến trình có `pid` tương ứng và đặt cờ **`p->killed = 1`**.
    - Nếu tiến trình nạn nhân đang ở trạng thái `SLEEPING`, `kill` cũng sẽ đánh thức nó.
    - **Khi nào tiến trình thực sự thoát?**
        - Lần tiếp theo tiến trình nạn nhân quay trở lại từ kernel mode về user mode (sau một system call hoặc ngắt), mã xử lý bẫy (`usertrap`) sẽ kiểm tra cờ `p->killed`. Nếu cờ được đặt, nó sẽ buộc tiến trình gọi `exit()`.
        - Một số vòng lặp `sleep` cũng kiểm tra cờ `killed` để có thể thoát sớm hơn.

### 🌎 **7.10 Thế giới thực**

- **Chính sách Lập lịch:**
    - xv6 sử dụng chính sách đơn giản nhất: **Round Robin** (xoay vòng). Mọi tiến trình đều có cơ hội chạy như nhau.
    - Các HĐH thực tế có các chính sách phức tạp hơn nhiều:
        - **Lập lịch theo độ ưu tiên (Priority-based):** Các tiến trình quan trọng hơn được chạy trước.
        - **Lập lịch đa cấp phản hồi (Multi-level Feedback Queue):** Kết hợp nhiều hàng đợi với các độ ưu tiên và quantum thời gian khác nhau để cân bằng giữa tính tương tác và thông lượng.
- **Các cơ chế đồng bộ hóa khác:**
    - **Semaphore:** Một bộ đếm được bảo vệ, khái quát hóa vấn đề Producer-Consumer.
    - **Condition Variables:** Thường được sử dụng cùng với mutex. Cung cấp các hàm `wait()`, `signal()` (đánh thức một luồng), và `broadcast()` (đánh thức tất cả các luồng). Đây là cơ chế phổ biến trong các thư viện luồng như Pthreads.

# 🗂️ **Chương 8: Hệ thống Tệp**

### 🎯 **Mục tiêu chính**

Hệ thống tệp (File System - FS) chịu trách nhiệm **tổ chức và lưu trữ dữ liệu một cách bền bỉ (persistently)** trên các thiết bị lưu trữ như đĩa cứng.

**Các thách thức chính:**

1. **Cấu trúc dữ liệu trên đĩa:** Cần một cách để biểu diễn cây thư mục, tệp, và theo dõi các khối đĩa nào đã được sử dụng, khối nào còn trống.
2. **Khôi phục sau sự cố (Crash Recovery):** Nếu hệ thống bị mất điện đột ngột giữa một thao tác ghi (ví dụ: tạo tệp), FS phải có khả năng tự sửa chữa về một trạng thái nhất quán sau khi khởi động lại.
3. **Đồng thời (Concurrency):** Nhiều tiến trình có thể truy cập FS cùng lúc. Cần có cơ chế khóa để bảo vệ tính toàn vẹn của dữ liệu.
4. **Hiệu suất (Performance):** Truy cập đĩa rất chậm so với RAM. FS cần một **bộ đệm cache** trong bộ nhớ để giữ lại các khối đĩa được truy cập thường xuyên.

### 🏛️ **Kiến trúc phân lớp của Hệ thống tệp xv6**

Hệ thống tệp của xv6 được tổ chức thành nhiều lớp, mỗi lớp xây dựng dựa trên sự trừu tượng hóa của lớp bên dưới nó.

| Lớp | Chức năng |
| --- | --- |
| **File Descriptor** | Trừu tượng hóa tệp, pipe, thiết bị thành một giao diện `fd` thống nhất. |
| **Pathname** | Xử lý các đường dẫn dạng cây (`/a/b/c`), thực hiện tra cứu. |
| **Directory** | Triển khai thư mục dưới dạng một loại tệp đặc biệt chứa các cặp (tên, số inode). |
| **Inode** | Biểu diễn các tệp riêng lẻ. Quản lý siêu dữ liệu (metadata) và các khối dữ liệu của tệp. |
| **Logging** | Đảm bảo các thao tác đa bước (ví dụ: tạo tệp) là **nguyên tử (atomic)** đối với sự cố. |
| **Buffer Cache** | Cache các khối đĩa trong RAM để tăng tốc và đồng bộ hóa truy cập. |
| **Disk** | Trình điều khiển cấp thấp để đọc/ghi các khối trên thiết bị đĩa vật lý. |

### 💾 **Bố cục trên đĩa (On-Disk Layout)**

Đĩa được chia thành các phân vùng có chức năng khác nhau:

| Khối | Nội dung | Mô tả |
| --- | --- | --- |
| 0 | Boot block | Chứa boot loader. |
| 1 | **Superblock** | 👑 Siêu dữ liệu về toàn bộ FS: kích thước, số inode, số khối dữ liệu, vị trí của nhật ký... |
| 2.. | **Log** | ✍️ Vùng dành cho ghi nhật ký giao dịch, dùng để khôi phục sau sự cố. |
| ... | **Inodes** | 📄 Mảng các cấu trúc `dinode` (inode trên đĩa), mỗi inode mô tả một tệp. |
| ... | **Bitmap** | 🗺️ Một chuỗi các bit, mỗi bit tương ứng với một khối dữ liệu, cho biết khối đó trống hay đã được sử dụng. |
| ... | **Data blocks** | 📦 Các khối chứa nội dung thực tế của tệp và thư mục. |

### caching **8.2 & 8.3 Lớp Bộ đệm Cache (Buffer Cache)**

Lớp này (`bio.c`) là trung gian giữa các lớp FS cao hơn và trình điều khiển đĩa.

- **Chức năng:**
    1. **Cache:** Giữ các bản sao của các khối đĩa trong RAM. Lần truy cập tiếp theo vào cùng một khối sẽ rất nhanh vì không cần đọc lại từ đĩa.
    2. **Đồng bộ hóa:** Đảm bảo rằng tại một thời điểm, chỉ có một tiến trình có thể sửa đổi một khối đĩa cụ thể.
- **Các hàm chính:**
    - **`bread(dev, blockno)`:** "Block Read"
        1. Tìm kiếm trong cache xem khối (`blockno`) đã có trong bộ nhớ chưa.
        2. Nếu có, trả về một con trỏ đến bộ đệm.
        3. Nếu không:
            - Tìm một bộ đệm ít được sử dụng nhất (Least Recently Used - LRU).
            - Đọc nội dung khối từ đĩa vào bộ đệm đó.
            - Trả về con trỏ.
        4. **Quan trọng:** Bộ đệm trả về đã được **khóa (locked)**, đảm bảo quyền truy cập độc quyền.
    - **`bwrite(buf)`:** Ghi nội dung của một bộ đệm đã bị sửa đổi ra đĩa.
    - **`brelse(buf)`:** "Block Release". **Nhả khóa** trên bộ đệm, cho phép các tiến trình khác sử dụng nó.

---

### ✍️ **8.4 & 8.5 Lớp Ghi nhật ký (Logging)**

Đây là giải pháp của xv6 cho vấn đề **khôi phục sau sự cố**.

- **Vấn đề:** Một thao tác FS, ví dụ như ghi vào một tệp, có thể yêu cầu nhiều lần ghi đĩa riêng lẻ:
    1. Ghi vào bitmap để đánh dấu một khối dữ liệu là đã sử dụng.
    2. Ghi vào inode để cập nhật kích thước tệp và thêm con trỏ đến khối dữ liệu mới.
    3. Ghi dữ liệu thực tế vào khối dữ liệu.
    - Nếu mất điện giữa bước 1 và 2, hệ thống sẽ ở trạng thái không nhất quán (một khối bị đánh dấu là đã sử dụng nhưng không thuộc về tệp nào cả -> rò rỉ không gian đĩa).
- **Giải pháp: Ghi nhật ký giao dịch (Transactional Logging)**
    1. **Thay vì ghi trực tiếp** vào các cấu trúc FS, HĐH đầu tiên ghi một bản "cam kết" vào một vùng đặc biệt trên đĩa gọi là **nhật ký (log)**. Cam kết này mô tả tất cả các lần ghi đĩa sẽ được thực hiện.
        - **`begin_op()`:** Báo cho hệ thống ghi nhật ký bắt đầu một giao dịch mới.
        - **`log_write()`:** Thay vì `bwrite()`, các lớp cao hơn gọi hàm này. Nó ghi lại thay đổi vào bộ nhớ nhưng không ghi ra đĩa ngay.
    2. Sau khi tất cả các thay đổi đã được ghi vào nhật ký, HĐH ghi một **bản ghi commit** đặc biệt vào nhật ký, xác nhận rằng giao dịch đã hoàn tất.
        - **`end_op()`:** Nếu đây là thao tác cuối cùng trong giao dịch, nó sẽ thực hiện commit.
    3. Chỉ **sau khi commit thành công**, HĐH mới bắt đầu sao chép các thay đổi từ nhật ký vào các vị trí thực sự của chúng trên đĩa (inode, bitmap, ...).
    4. Cuối cùng, xóa nhật ký.
- **Quá trình khôi phục:**
    - Khi khởi động lại sau sự cố, kernel kiểm tra nhật ký.
    - Nếu nhật ký có một bản ghi commit hoàn chỉnh, nó sẽ **thực hiện lại (replay)** tất cả các lần ghi trong nhật ký để đảm bảo hệ thống nhất quán.
    - Nếu nhật ký không có bản ghi commit (sự cố xảy ra trước khi commit), kernel chỉ cần **bỏ qua** nhật ký đó. Trạng thái của FS sẽ giống như thể giao dịch chưa bao giờ bắt đầu.
    - ➡️ **Kết quả:** Các thao tác FS trở thành **nguyên tử**: hoặc tất cả các lần ghi đều thành công, hoặc không có lần ghi nào cả.

---

### 📄 **8.8 & 8.10 Lớp Inode và Nội dung Inode**

- **Inode:** Như đã đề cập, là siêu dữ liệu của một tệp. `struct dinode` là cấu trúc trên đĩa. `struct inode` là cấu trúc trong bộ nhớ (có thêm thông tin như số tham chiếu, khóa).
- **Quản lý Inode:**
    - **`ialloc()`:** Tìm và cấp phát một inode trống trên đĩa.
    - **`iget()`:** Lấy một con trỏ đến một inode trong bộ nhớ (tăng số tham chiếu).
    - **`iput()`:** Giải phóng một con trỏ đến inode (giảm số tham chiếu). Nếu số tham chiếu và số liên kết (`nlink`) đều về 0, inode và các khối dữ liệu của nó sẽ được giải phóng.
    - **`ilock()` / `iunlock()`:** Khóa/mở khóa một inode để đọc/ghi siêu dữ liệu và nội dung của nó.
- **Cấu trúc nội dung tệp:**
    - Làm thế nào một inode trỏ đến tất cả các khối dữ liệu của một tệp?
    - Mảng `addrs[]` trong inode chứa các số hiệu khối đĩa.
    - **Các khối trực tiếp (Direct blocks):** `NDIRECT` (12 trong xv6) mục đầu tiên trong `addrs[]` trỏ trực tiếp đến 12 khối dữ liệu đầu tiên của tệp. (Cho các tệp nhỏ).
    - **Khối gián tiếp (Indirect block):** Mục cuối cùng trong `addrs[]` **không** trỏ đến dữ liệu, mà trỏ đến một **khối khác**, gọi là khối gián tiếp. Khối gián tiếp này chứa một mảng các số hiệu khối, trỏ đến các khối dữ liệu tiếp theo của tệp. (Cho các tệp lớn hơn).
    - **`bmap()`:** Một hàm tiện ích quan trọng giúp trừu tượng hóa cấu trúc này. Với một inode và một chỉ số khối logic (`bn`), nó trả về số hiệu khối đĩa vật lý tương ứng, tự động xử lý việc truy cập khối gián tiếp nếu cần.

---

### 🚪 **8.11 & 8.12 Lớp Thư mục và Tên đường dẫn**

- **Thư mục:** Chỉ là một tệp đặc biệt (`type = T_DIR`) mà nội dung của nó là một danh sách các cấu trúc `struct dirent`.
    - Mỗi `struct dirent` chứa một **tên tệp** và **số inode** tương ứng.
- **`dirlookup()`:** Tìm kiếm một tên trong một thư mục và trả về inode tương ứng.
- **Tra cứu đường dẫn (ví dụ `/a/b/c`):**
    - Hàm `namei()` (name to inode) thực hiện việc này.
    - Nó lặp qua từng thành phần của đường dẫn:
        1. Bắt đầu từ thư mục gốc (`/`).
        2. Gọi `dirlookup()` để tìm `a` trong thư mục gốc, lấy inode của `a`.
        3. Gọi `dirlookup()` để tìm `b` trong thư mục `a`, lấy inode của `b`.
        4. Gọi `dirlookup()` để tìm `c` trong thư mục `b`, lấy inode của `c` và trả về.
    - Việc này đòi hỏi xử lý khóa cẩn thận để tránh deadlock khi tra cứu các đường dẫn như `.` (thư mục hiện tại) và `..` (thư mục cha).

### 🎁 **8.13 Lớp Mô tả Tệp (File Descriptor)**

Lớp này (`file.c`) là lớp cao nhất trong kiến trúc hệ thống tệp, cung cấp sự trừu tượng hóa mà các chương trình người dùng nhìn thấy.

- **Cấu trúc dữ liệu:**
    - **Bảng tệp trên mỗi tiến trình:** Mỗi tiến trình có một mảng các con trỏ (`p->ofile[]`), nơi chỉ số của mảng chính là **mô tả tệp (fd)**.
    - **Bảng tệp toàn cục (`ftable`):** Một bảng duy nhất cho toàn bộ hệ thống chứa các cấu trúc `struct file`.
    - **`struct file`:**
        - Đây là đại diện cho một "tệp đang mở". Nó không phải là tệp trên đĩa, mà là một phiên làm việc với tệp đó.
        - Chứa các thông tin quan trọng:
            - Loại (tệp, pipe, thiết bị).
            - Một con trỏ đến inode (nếu là tệp) hoặc cấu trúc pipe (nếu là pipe).
            - Quyền truy cập (đọc/ghi).
            - **Số tham chiếu (`ref`):** Đếm có bao nhiêu fd đang trỏ đến `struct file` này.
            - **Con trỏ vị trí (`offset`):** Vị trí đọc/ghi hiện tại trong tệp. Đây là đặc điểm quan trọng của một tệp đang mở.
- **Mối quan hệ:**
    - `open()` tạo ra một `struct file` mới trong `ftable` và một mục mới trong bảng fd của tiến trình trỏ đến nó.
    - `fork()` sao chép bảng fd của cha cho con. Cả hai tiến trình sẽ trỏ đến **cùng một `struct file`**. Điều này có nghĩa là chúng **chia sẻ cùng một con trỏ vị trí (`offset`)**.
    - `dup()` tạo một mục fd mới trong cùng một tiến trình, cũng trỏ đến cùng một `struct file` và chia sẻ con trỏ vị trí.
    - Nếu hai tiến trình `open()` cùng một tệp một cách độc lập, chúng sẽ có hai `struct file` khác nhau, và do đó có hai con trỏ vị trí **riêng biệt**.
- **Các hàm chính:**
    - `filealloc()`: Tìm một `struct file` trống trong `ftable`.
    - `filedup()`: Tăng số tham chiếu của một `struct file`.
    - `fileclose()`: Giảm số tham chiếu. Nếu về 0, nó sẽ giải phóng inode hoặc pipe cơ bản (bằng cách gọi `iput()` hoặc `pipeclose()`).
    - `fileread()` / `filewrite()`: Kiểm tra quyền, sau đó gọi các hàm cấp thấp hơn (`readi`/`writei` cho inode, `piperead`/`pipewrite` cho pipe) và cập nhật con trỏ vị trí.

### 📞 **8.14 Mã nguồn: Lời gọi Hệ thống**

Với tất cả các lớp trừu tượng đã xây dựng, việc triển khai các system call liên quan đến tệp trở nên khá đơn giản (`sysfile.c`).

- **`sys_open()`:**
    1. Gọi `create()` (nếu có cờ `O_CREATE`) hoặc `namei()` để lấy inode của tệp.
    2. Gọi `filealloc()` để lấy một `struct file`.
    3. Cấp phát một fd trống trong bảng fd của tiến trình.
    4. Thiết lập `struct file` và mục fd để trỏ đến nhau.
- **`sys_read()` / `sys_write()` / `sys_close()`:**
    - Lấy đối số `fd`.
    - Tìm `struct file` tương ứng trong bảng fd của tiến trình.
    - Gọi các hàm tương ứng: `fileread()`, `filewrite()`, `fileclose()`.
- **`sys_link()` / `sys_unlink()`:**
    - Sử dụng các giao dịch (`begin_op`/`end_op`) để đảm bảo các thay đổi trên nhiều cấu trúc dữ liệu (inode, dữ liệu thư mục) là nguyên tử. Đây là một ví dụ tuyệt vời về sức mạnh của hệ thống ghi nhật ký.
- **`sys_pipe()`:**
    - Kết nối việc triển khai pipe (Chương 7) với hệ thống tệp.
    - Nó gọi `pipealloc()` để tạo pipe, sau đó cấp phát hai cặp `fd` và `struct file` cho hai đầu đọc và ghi của pipe.

# 🔄 **Chương 9: Nhìn lại về tính Đồng thời**

Chương này không giới thiệu khái niệm mới mà phân tích sâu hơn các mẫu khóa phức tạp và các kỹ thuật giống-khóa đã được sử dụng trong xv6.

### 💡 **9.1 Các Mẫu Khóa (Locking Patterns)**

- **Khóa cho Tập hợp + Khóa cho Phần tử:**
    - **Vấn đề:** Làm thế nào để khóa một đối tượng có thể chưa tồn tại? (Ví dụ: tìm một khối đĩa trong buffer cache).
    - **Giải pháp:** Sử dụng hai cấp độ khóa.
        1. Một khóa chung cho toàn bộ **tập hợp** (ví dụ: `bcache.lock` cho buffer cache).
        2. Một khóa riêng cho mỗi **phần tử** (ví dụ: `buf->lock` cho mỗi bộ đệm).
    - **Luồng hoạt động:** Giữ khóa tập hợp -> Tìm/tạo phần tử -> Giành khóa phần tử -> Nhả khóa tập hợp -> Làm việc với phần tử.
- **Chuyển giao quyền sở hữu khóa:**
    - Thông thường, một luồng giành và nhả cùng một khóa.
    - Nhưng đôi khi, quyền sở hữu khóa được "chuyển giao".
    - **Ví dụ:** Trong `yield()`, tiến trình hiện tại giành khóa `p->lock` của chính nó, nhưng sau `swtch`, chính **luồng lập lịch** là người nhả khóa đó ra. Khóa không thuộc về một luồng, mà thuộc về một **chuỗi thao tác nguyên tử** cần được bảo vệ.

### ✨ **9.2 Các Mẫu Giống-Khóa (Lock-like Patterns)**

xv6 sử dụng các kỹ thuật khác ngoài khóa để quản lý đồng thời.

- **Số tham chiếu (Reference Counts):**
    - Các trường `ref` trong `struct inode` và `struct file` hoạt động như một loại **khóa chia sẻ (shared lock)**.
    - Miễn là `ref > 0`, đối tượng sẽ **không bị giải phóng**. Điều này ngăn chặn lỗi "sử dụng sau khi giải phóng".
    - Nhiều luồng có thể cùng lúc giữ một tham chiếu (`iget`), nhưng việc sửa đổi đối tượng vẫn cần một khóa loại trừ (như `ilock`).
- **Cờ (Flags):**
    - Trường `p->state` của một tiến trình hoạt động như một cơ chế phối hợp. Việc chuyển đổi giữa các trạng thái (`RUNNING`, `SLEEPING`, `ZOMBIE`) phải được bảo vệ bởi khóa `p->lock`, nhưng chính trạng thái này điều khiển hành vi của bộ lập lịch.
- **Tắt ngắt:**
    - Xung quanh các lệnh gọi như `mycpu()`, kernel phải tắt ngắt.
    - Hành động này làm cho đoạn mã trở nên **nguyên tử đối với các ngắt trên cùng một CPU**, ngăn chặn việc chuyển ngữ cảnh bất ngờ có thể làm hỏng logic.

### 🤷‍♂️ **9.3 Không dùng khóa**

Trong một số trường hợp rất hiếm, xv6 truy cập dữ liệu được chia sẻ mà không cần khóa.

- **Lệnh nguyên tử phần cứng:** Việc triển khai spinlock sử dụng `amoswap`, có thể coi là một "khóa" ở cấp độ phần cứng.
- **Thứ tự bộ nhớ (Memory Ordering):**
    - **Vấn đề:** Compiler và CPU có thể **sắp xếp lại (reorder)** các lệnh ghi và đọc bộ nhớ để tối ưu hóa hiệu suất. Trên hệ thống đa lõi, điều này có thể khiến một CPU nhìn thấy các thay đổi của CPU khác theo một thứ tự sai lệch.
    - **Giải pháp:** Sử dụng **rào cản bộ nhớ (memory barriers)**. Các hàm `acquire` và `release` trong xv6 chứa các rào cản này. Chúng ra lệnh cho compiler và CPU không được di chuyển các thao tác bộ nhớ qua rào cản, đảm bảo rằng tất cả các thay đổi được thực hiện bên trong một vùng tranh chấp sẽ được các CPU khác "nhìn thấy" một cách nhất quán sau khi khóa được nhả.
    - Việc `fork()` ghi vào bộ nhớ của con và con sau đó đọc dữ liệu đó là một ví dụ. Không có khóa trực tiếp, nhưng các rào cản bộ nhớ bên trong các khóa được sử dụng trong quá trình này đảm bảo tính đúng đắn.

### 🚀 **9.4 Song song hóa (Parallelism)**

Mục tiêu của việc sử dụng khóa mịn là để tối đa hóa khả năng thực thi song song.

- **Tốt:** ✅
    - **Pipes:** Mỗi pipe có khóa riêng. Các tiến trình làm việc trên các pipe khác nhau có thể chạy song song hoàn toàn.
    - **Hệ thống tệp:** Mỗi tệp/thư mục có khóa riêng. Các thao tác trên các tệp khác nhau có thể chạy song song.
- **Chưa tốt:** 🚧
    - **Bộ cấp phát bộ nhớ (`kalloc`):** Chỉ có một khóa duy nhất. Đây là một **điểm nghẽn (bottleneck)** tiềm năng nếu nhiều CPU cùng cần cấp phát bộ nhớ.
    - **Bộ lập lịch (`scheduler`):** Mặc dù mỗi CPU có luồng lập lịch riêng, nhưng tất cả chúng đều quét qua cùng một bảng tiến trình và phải giành khóa `p->lock` cho mỗi tiến trình. Điều này tạo ra sự tranh chấp.

**Kết luận:** Thiết kế hệ thống khóa là một sự đánh đổi liên tục giữa **độ đơn giản**, **tính đúng đắn**, và **hiệu suất song song**. Đo lường trên khối lượng công việc thực tế thường là cách duy nhất để xác định xem một thiết kế khóa có gây ra điểm nghẽn hay không.

### ✍️ **9.5 Bài tập**

1. **Tăng tính song song cho Pipe:** Sửa đổi việc triển khai pipe của xv6 để cho phép một luồng `read` và một luồng `write` trên **cùng một pipe** có thể diễn ra song song trên các CPU khác nhau. (Gợi ý: có thể cần nhiều hơn một khóa).
2. **Giảm tranh chấp trong Bộ lập lịch:** Sửa đổi hàm `scheduler()` của xv6 để giảm sự tranh chấp khóa khi nhiều CPU cùng tìm kiếm các tiến trình `RUNNABLE`. (Gợi ý: có thể chia bảng tiến trình thành các danh sách riêng cho mỗi CPU không?).
3. **Tăng tính song song cho `fork()`:** Phân tích các điểm nghẽn do khóa trong `fork()` (ví dụ: `pid_lock`, `kmem.lock`) và cố gắng loại bỏ một số điểm tuần tự hóa này.

# 🏁 **Chương 10: Tóm tắt**

Cuốn sách này đã giới thiệu các ý tưởng cốt lõi của hệ điều hành bằng cách nghiên cứu chi tiết từng dòng mã của xv6.

### 💡 **Các ý tưởng lớn được thể hiện trong xv6**

1. **Trừu tượng hóa và Giao diện (Abstraction & Interfaces):**
    - HĐH che giấu sự phức tạp của phần cứng và cung cấp các giao diện đơn giản, mạnh mẽ cho lập trình viên.
    - **Ví dụ:** Giao diện **mô tả tệp (file descriptor)** thống nhất hóa cách truy cập tệp, thiết bị và pipe, cho phép các công cụ như shell và I/O redirection hoạt động một cách linh hoạt.
2. **Đa hợp và Ảo hóa (Multiplexing & Virtualization):**
    - HĐH chia sẻ một số ít tài nguyên vật lý (CPU, bộ nhớ) cho nhiều tiến trình, tạo ra ảo giác rằng mỗi tiến trình có máy tính riêng của mình.
    - **CPU ảo:** Được tạo ra bởi cơ chế **lập lịch** và **chuyển đổi ngữ cảnh**.
    - **Bộ nhớ ảo:** Được tạo ra bởi **bảng trang** phần cứng và sự quản lý của kernel.
3. **Đồng thời và Đồng bộ hóa (Concurrency & Synchronization):**
    - Hệ thống đa lõi và các ngắt tạo ra các vấn đề về đồng thời.
    - **Khóa (locks)** là công cụ cơ bản để đảm bảo **loại trừ lẫn nhau (mutual exclusion)** và bảo vệ tính toàn vẹn của dữ liệu được chia sẻ.
    - Các cơ chế phức tạp hơn như **`sleep` và `wakeup`** được xây dựng trên nền tảng khóa để cho phép các tiến trình phối hợp hành động một cách hiệu quả mà không lãng phí CPU.
4. **Bền bỉ và Khả năng phục hồi (Persistence & Reliability):**
    - Dữ liệu phải tồn tại sau khi tắt máy.
    - Hệ thống phải có khả năng phục hồi từ các sự cố đột ngột (mất điện).
    - **Ghi nhật ký giao dịch (logging)** là một kỹ thuật mạnh mẽ để đảm bảo các thao tác trên hệ thống tệp là **nguyên tử (atomic)**, giúp hệ thống luôn ở trạng thái nhất quán.
5. **Sự phân tách giữa Cơ chế và Chính sách (Separation of Mechanism and Policy):**
    - HĐH nên cung cấp các **cơ chế** (ví dụ: khả năng chuyển đổi giữa các tiến trình) nhưng để lại việc quyết định **chính sách** (ví dụ: quyết định tiến trình nào sẽ chạy tiếp theo) một cách linh hoạt.
    - xv6 không thể hiện rõ điều này (chính sách round-robin được tích hợp sẵn), nhưng các HĐH hiện đại thường cho phép cấu hình các chính sách lập lịch, I/O, v.v.

### 🚀 **xv6: Một công cụ học tập**

- xv6 không phải là một HĐH sản xuất. Nó được thiết kế để **đơn giản và dễ hiểu**. Nhiều thuật toán và cấu trúc dữ liệu của nó (ví dụ: quét tuyến tính trong bộ lập lịch, một khóa duy nhất cho bộ cấp phát) được chọn vì tính rõ ràng chứ không phải vì hiệu suất.
- Tuy nhiên, các ý tưởng và cấu trúc cơ bản mà nó thể hiện—tiến trình, không gian địa chỉ, bảng trang, bẫy, tệp, inode, các lớp trừu tượng, khóa—là nền tảng cho hầu hết các hệ điều hành hiện đại như Linux, Windows và macOS.
- Bằng cách hiểu sâu về cách xv6 hoạt động, bạn đã có được nền tảng vững chắc để tiếp cận và hiểu các hệ thống phức tạp hơn nhiều trong thế giới thực.

**Hành trình đã qua:** Từ việc một chương trình `ecall` đơn giản, chúng ta đã đi sâu vào các lớp phức tạp của kernel: xử lý bẫy, chuyển đổi ngữ cảnh, quản lý bộ nhớ ảo, các lớp của hệ thống tệp, và các thách thức của sự đồng thời. Mỗi dòng mã đều góp phần tạo nên một hệ thống hoạt động hoàn chỉnh.