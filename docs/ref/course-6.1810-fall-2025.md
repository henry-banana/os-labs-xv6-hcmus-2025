Tài liệu này là một kim chỉ nam, được thiết kế để dẫn dắt người đọc qua các lớp trừu tượng phức tạp của một hệ điều hành hiện đại. Bằng cách mổ xẻ mã nguồn của xv6, chúng ta sẽ không chỉ học lý thuyết suông mà còn chứng kiến cách các khái niệm như tiến trình, bộ nhớ ảo, cơ chế bẫy, tương tranh và hệ thống tệp được hiện thực hóa trong thực tế. Mục tiêu là trang bị cho bạn một nền tảng vững chắc để có thể tự tin phân tích và hiểu các hệ điều hành phức tạp hơn như Linux hay macOS.

# 1. Tổng quan Khóa học 6.1810 và Hệ điều hành xv6

Trước khi đi sâu vào các chi tiết kỹ thuật, việc hiểu rõ bối cảnh của khóa học 6.1810 và công cụ học tập chính của chúng ta — hệ điều hành xv6 — là vô cùng quan trọng. Phần này sẽ cung cấp những thông tin nền tảng cần thiết, đặt nền móng cho hành trình khám phá sắp tới.

### 1.1. Thông tin Khóa học 6.1810 (Học kỳ Thu 2025)

Dưới đây là các thông tin hành chính quan trọng về khóa học 6.1810 tại MIT, được tổng hợp từ thông báo chính thức:

- **Tên môn học:** 6.1810: Operating System Engineering.
- **Đối tượng:** Môn học được thiết kế cho sinh viên đại học (AUS subject), cung cấp kiến thức nhập môn về hệ điều hành.
- **Thời gian và Địa điểm:** Các buổi học sẽ diễn ra vào Thứ Hai và Thứ Tư, từ 1:00pm đến 2:30pm tại phòng 45-230, bắt đầu từ Thứ Tư, ngày 3 tháng 9.
- **Yêu cầu thi cử:** Sẽ có một bài thi giữa kỳ và một bài thi cuối kỳ. Khóa học yêu cầu sinh viên tham dự các kỳ thi trực tiếp.
- **Thông tin liên hệ:** Mọi thắc mắc hoặc bình luận về khóa học có thể được gửi đến đội ngũ giảng viên qua địa chỉ email: `61810-staff@lists.csail.mit.edu`.

### 1.2. Giới thiệu về xv6

xv6 là linh hồn của khóa học và là công cụ chính mà chúng ta sẽ sử dụng. Dựa trên lời nói đầu và chương đầu tiên của sách giáo khoa xv6, có thể tóm tắt các đặc điểm chính như sau:

- **Định nghĩa:** xv6 là một hệ điều hành giảng dạy đơn giản, mang phong cách của Unix.
- **Nguồn gốc:** Nó được mô phỏng theo Unix Version 6 (v6), một tác phẩm kinh điển của Dennis Ritchie và Ken Thompson.
- **Ngôn ngữ và Kiến trúc:** xv6 được viết bằng ngôn ngữ ANSI C và được thiết kế cho kiến trúc đa lõi RISC-V.
- **Vai trò của Kernel:** Trái tim của xv6 là kernel, một chương trình đặc biệt có nhiệm vụ cung cấp các dịch vụ thiết yếu cho các chương trình đang chạy (được gọi là các tiến trình).

Mục tiêu cốt lõi của một hệ điều hành, như được nêu trong sách giáo khoa, có thể được tóm gọn như sau:

The job of an operating system is to share a computer among multiple programs and to provide a more useful set of services than the hardware alone supports.

### 1.3. Cấu trúc Tài liệu và Các Bài thực hành (Labs)

Để biến lý thuyết thành kỹ năng thực tiễn, khóa học 6.1810 bao gồm một loạt các bài thực hành (labs) đầy thử thách. Cấu trúc của tài liệu này sẽ bám sát theo tiến trình của sách giáo khoa xv6, và mỗi phần lý thuyết sẽ được liên kết với một hoặc nhiều bài thực hành tương ứng để củng cố kiến thức.

Danh sách các bài thực hành chính bao gồm:

- `Lab System calls`
- `Lab Page tables`
- `Lab Traps`
- `Lab Copy on-write`
- `Lab network driver`
- `Lab Lock`
- `Lab File system`
- `Lab mmap`

Để bắt đầu, chúng ta cần tìm hiểu về các giao diện mà hệ điều hành cung cấp cho chương trình người dùng—cầu nối đầu tiên giữa thế giới ứng dụng và sức mạnh của kernel.

# 2. Giao diện Hệ Điều Hành (Operating System Interfaces)

Giao diện hệ điều hành, được thể hiện thông qua các **lời gọi hệ thống (system calls)**, là cầu nối cơ bản và duy nhất giữa chương trình ứng dụng và kernel. Tầm quan trọng chiến lược của việc thiết kế một giao diện hẹp nhưng mạnh mẽ, như triết lý của Unix, không thể bị xem nhẹ. Nó cho phép kết hợp các cơ chế đơn giản để tạo ra các chức năng phức tạp, mang lại tính tổng quát và sự đơn giản cho toàn bộ hệ thống.

### 2.1. Tiến trình và Bộ nhớ (Processes and Memory)

- **Tiến trình (Process):** Một tiến trình được định nghĩa là một chương trình đang trong quá trình thực thi. Nó sở hữu một không gian bộ nhớ riêng, bao gồm mã lệnh, dữ liệu và một ngăn xếp (stack).
- **Các Lời gọi Hệ thống Cơ bản:**
    - `fork()`: Tạo ra một tiến trình mới (tiến trình con) bằng cách sao chép chính xác không gian bộ nhớ của tiến trình đang gọi (tiến trình cha).
    - `exec()`: Thay thế không gian bộ nhớ của tiến trình hiện tại bằng một chương trình mới được tải từ một tệp.
    - `exit()`: Chấm dứt tiến trình hiện tại và giải phóng tài nguyên của nó.
    - `wait()`: Cho phép tiến trình cha chờ đợi một tiến trình con của nó kết thúc.
- **Sự Tách biệt giữa `fork()` và `exec()`:** Việc tách hai chức năng này mang lại sự linh hoạt đáng kinh ngạc. Nó cho phép shell (trình thông dịch dòng lệnh) thực hiện các thao tác phức tạp như chuyển hướng I/O (I/O redirection) bằng cách sửa đổi các mô tả tệp của tiến trình con sau khi `fork()` nhưng trước khi gọi `exec()`.
- **Cấp phát Bộ nhớ Động:** Lời gọi hệ thống `sbrk()` cho phép một tiến trình yêu cầu kernel cấp phát thêm bộ nhớ cho vùng dữ liệu (heap) của nó trong lúc chạy.

### 2.2. I/O và Mô tả tệp (File Descriptors)

Mô tả tệp (file descriptor) là một cơ chế trừu tượng hóa mạnh mẽ, biến mọi thứ—từ tệp, thiết bị, đến ống dẫn—trông giống như một luồng byte đơn giản.

- **Định nghĩa:** Mô tả tệp là một số nguyên nhỏ đại diện cho một đối tượng do kernel quản lý mà một tiến trình có thể đọc hoặc ghi.
- **Quy ước Chuẩn:**
    - `0`: Standard Input (Đầu vào chuẩn)
    - `1`: Standard Output (Đầu ra chuẩn)
    - `2`: Standard Error (Đầu ra lỗi chuẩn)
- **Các Lời gọi Hệ thống I/O:** `read()`, `write()`, và `close()` hoạt động trên các mô tả tệp này, cho phép chương trình đọc, ghi và giải phóng các đối tượng I/O.
- **Chuyển hướng I/O:** Shell thực hiện chuyển hướng I/O một cách tài tình. Ví dụ, để chạy lệnh `cat < input.txt`, shell sẽ:
    1. Gọi `fork()` để tạo tiến trình con.
    2. Trong tiến trình con, gọi `close(0)` để đóng đầu vào chuẩn.
    3. Gọi `open("input.txt", O_RDONLY)`, kernel sẽ cấp phát mô tả tệp nhỏ nhất còn trống, chính là `0`.
    4. Gọi `exec("cat", ...)` để chạy chương trình `cat`. Giờ đây, đầu vào chuẩn của `cat` chính là tệp `input.txt`.
- **`dup()`:** Lời gọi này sao chép một mô tả tệp, cho phép hai mô tả tệp cùng trỏ đến một đối tượng I/O và chia sẻ cùng một con trỏ vị trí (offset).

### 2.3. Ống dẫn (Pipes)

Ống dẫn (pipe) là một phương tiện giao tiếp giữa các tiến trình, cho phép đầu ra của một tiến trình trở thành đầu vào của tiến trình khác.

- **Mô tả:** Pipe là một vùng đệm nhỏ trong kernel, được truy cập thông qua một cặp mô tả tệp: một để đọc và một để ghi.
- **Ví dụ:** Để tạo một pipeline như `grep | wc`, shell sẽ tạo một pipe, `fork()` hai tiến trình con, và kết nối đầu ra chuẩn của `grep` với đầu ghi của pipe, và đầu vào chuẩn của `wc` với đầu đọc của pipe.
- **Ưu điểm so với Tệp Tạm thời:**
    1. **Tự động dọn dẹp:** Pipe sẽ tự biến mất khi các tiến trình liên quan kết thúc.
    2. **Xử lý luồng dữ liệu lớn:** Pipe có thể xử lý luồng dữ liệu dài vô hạn mà không cần lưu trữ toàn bộ trên đĩa.
    3. **Thực thi song song:** Các giai đoạn của pipeline có thể chạy song song, tăng hiệu suất.

### 2.4. Hệ thống Tệp (File System)

Hệ thống tệp của xv6 tổ chức dữ liệu dưới dạng một cấu trúc cây gồm các tệp và thư mục.

- **Tệp và Thư mục:** Tệp chứa dữ liệu, trong khi thư mục chứa các tham chiếu đến các tệp và thư mục khác.
- **Đường dẫn (Path) và Thư mục hiện hành:** Đường dẫn (`/a/b/c`) là cách để xác định vị trí của một tệp. Lời gọi `chdir()` thay đổi thư mục làm việc hiện tại của tiến trình.
- **Inode và Link:** Tên tệp chỉ là một tham chiếu (link) đến một cấu trúc dữ liệu cơ bản gọi là **inode**. Inode chứa siêu dữ liệu về tệp (loại, kích thước, vị trí các khối dữ liệu). Một inode có thể có nhiều tên (liên kết cứng - hard link).
- **Các Lời gọi Hệ thống liên quan:** `mkdir()` (tạo thư mục), `mknod()` (tạo tệp thiết bị), `fstat()` (lấy thông tin inode), `link()` (tạo liên kết cứng), và `unlink()` (xóa một tên khỏi hệ thống tệp).

### 2.5. Bài thực hành liên quan

**`Lab: System calls`**

Kiến thức trong chương này là nền tảng cho bài thực hành đầu tiên. Bài thực hành này sẽ yêu cầu bạn triển khai một lời gọi hệ thống mới từ đầu, giúp bạn hiểu sâu sắc cơ chế chuyển đổi giữa không gian người dùng và không gian kernel, cũng như cách kernel truy cập các đối số do người dùng cung cấp một cách an toàn.

Sau khi đã hiểu các giao diện mà hệ điều hành cung cấp, bước tiếp theo là khám phá cách hệ điều hành được tổ chức bên trong để hiện thực hóa các giao diện này.

# 3. Tổ chức Hệ Điều Hành (Operating System Organization)

Câu hỏi trung tâm của chương này là: làm thế nào một hệ điều hành có thể quản lý và phân chia tài nguyên máy tính một cách an toàn và hiệu quả cho nhiều chương trình? Để làm được điều này, hệ điều hành phải đáp ứng ba yêu cầu cốt lõi: **multiplexing** (ghép kênh nhiều chương trình lên một tài nguyên), **isolation** (cô lập các chương trình khỏi nhau), và **interaction** (cung cấp cơ chế tương tác có kiểm soát).

### 3.1. Trừu tượng hóa Tài nguyên Vật lý

Lý do chính để có một hệ điều hành thay vì chỉ một thư viện liên kết với ứng dụng là để đạt được sự cô lập mạnh mẽ.

- **Thư viện vs. Hệ điều hành:** Cách tiếp cận thư viện yêu cầu các ứng dụng phải hợp tác và tin tưởng lẫn nhau. Ngược lại, hệ điều hành áp đặt các quy tắc bằng cách trừu tượng hóa tài nguyên.
- **Lợi ích của Trừu tượng hóa:** Thay vì cho phép chương trình truy cập trực tiếp đĩa, hệ điều hành cung cấp hệ thống tệp. Thay vì quản lý bộ nhớ vật lý, nó cung cấp không gian địa chỉ ảo thông qua `exec`. Những trừu tượng hóa này không chỉ mang lại sự tiện lợi mà còn là nền tảng cho sự cô lập.

### 3.2. Chế độ Người dùng, Chế độ Giám sát và Lời gọi Hệ thống

Phần cứng CPU cung cấp các cơ chế thiết yếu để hỗ trợ sự cô lập.

- **Các chế độ thực thi của RISC-V:** CPU RISC-V có ba chế độ:
    1. **Machine mode:** Chế độ đặc quyền cao nhất, chủ yếu dùng để khởi động.
    2. **Supervisor mode (Chế độ Giám sát):** Kernel hoạt động ở chế độ này, được phép thực thi các lệnh đặc quyền (ví dụ: tắt/bật ngắt).
    3. **User mode (Chế độ Người dùng):** Các chương trình ứng dụng chạy ở chế độ này với các đặc quyền bị hạn chế.
- **`ecall`:** Khi một chương trình người dùng cần dịch vụ từ kernel, nó thực thi lệnh `ecall`. Lệnh này gây ra một bẫy, chuyển CPU từ user mode sang supervisor mode và bắt đầu thực thi mã tại một điểm vào đã được kernel định sẵn.
- **Kiến trúc Kernel:**
    - **Monolithic Kernel (Kernel Đơn khối):** Toàn bộ hệ điều hành chạy trong một không gian địa chỉ duy nhất ở chế độ giám sát. xv6 và Linux là các ví dụ điển hình.
    - **Microkernel (Vi nhân):** Chỉ các chức năng cốt lõi nhất (quản lý tiến trình, IPC) chạy ở chế độ giám sát. Các dịch vụ khác (hệ thống tệp, trình điều khiển) chạy như các tiến trình ở chế độ người dùng.

| Tính năng | Monolithic Kernel | Microkernel |
| --- | --- | --- |
| **Ưu điểm** | Hiệu suất cao do giao tiếp nội bộ nhanh. | Độ tin cậy cao hơn (lỗi ở một server không làm sập cả hệ thống), dễ mở rộng. |
| **Nhược điểm** | Phức tạp, một lỗi nhỏ có thể làm sập toàn bộ hệ thống. | Hiệu suất thấp hơn do chi phí giao tiếp giữa các tiến trình (IPC). |

### 3.3. Tổ chức Mã nguồn xv6 và Tổng quan về Tiến trình

**Mã nguồn Kernel xv6 (`kernel/`)**

| Tệp | Chức năng |
| --- | --- |
| `bio.c` | Bộ đệm khối đĩa (Block cache) cho hệ thống tệp. |
| `console.c` | Kết nối với bàn phím và màn hình người dùng. |
| `entry.S` | Các lệnh khởi động đầu tiên. |
| `exec.c` | Lời gọi hệ thống `exec()`. |
| `file.c` | Hỗ trợ mô tả tệp. |
| `fs.c` | Hệ thống tệp. |
| `kalloc.c` | Bộ cấp phát trang vật lý. |
| `kernelvec.S` | Xử lý các bẫy từ kernel. |
| `log.c` | Ghi nhật ký hệ thống tệp và phục hồi sau sự cố. |
| `main.c` | Điều khiển khởi tạo các module khác khi khởi động. |
| `pipe.c` | Ống dẫn (Pipes). |
| `plic.c` | Bộ điều khiển ngắt của RISC-V. |
| `printf.c` | Xuất định dạng ra console. |
| `proc.c` | Tiến trình và lập lịch. |
| `sleeplock.c` | Cơ chế khóa cho phép nhường CPU (sleep-locks). |
| `spinlock.c` | Cơ chế khóa không nhường CPU (spinlocks). |
| `start.c` | Mã khởi động sớm ở machine mode. |
| `string.c` | Thư viện chuỗi C và mảng byte. |
| `swtch.S` | Mã assembly để chuyển đổi ngữ cảnh. |
| `syscall.c` | Điều phối các lời gọi hệ thống đến hàm xử lý. |
| `sysfile.c` | Các lời gọi hệ thống liên quan đến tệp. |
| `sysproc.c` | Các lời gọi hệ thống liên quan đến tiến trình. |
| `trampoline.S` | Mã assembly để chuyển đổi giữa không gian người dùng và kernel. |
| `trap.c` | Mã C để xử lý và quay về từ các bẫy và ngắt. |
| `uart.c` | Trình điều khiển thiết bị console cổng serial. |
| `virtio_disk.c` | Trình điều khiển thiết bị đĩa. |
| `vm.c` | Quản lý bảng trang và không gian địa chỉ. |

**Cấu trúc của một Tiến trình trong xv6**

Một tiến trình trong xv6 bao gồm:

- **Không gian địa chỉ (Address Space):** xv6 sử dụng bảng trang (page tables) để tạo ra ảo giác rằng mỗi tiến trình có một không gian bộ nhớ riêng, bắt đầu từ địa chỉ 0. Bố cục bộ nhớ bao gồm mã lệnh, dữ liệu, heap, ngăn xếp, và một trang **trampoline** ở địa chỉ ảo cao nhất. Ngay bên dưới trang trampoline là một trang **trapframe**, nơi kernel lưu các thanh ghi của tiến trình trong một bẫy (như trong Hình 2.3 của sách).
- **Luồng thực thi (Thread of Control):** Mỗi tiến trình có một luồng thực thi. Khi tiến trình chạy ở user space, nó sử dụng ngăn xếp người dùng (user stack). Khi nó vào kernel (qua system call hoặc ngắt), nó chuyển sang sử dụng ngăn xếp kernel (kernel stack).
- **Trạng thái tiến trình:** Kernel lưu giữ trạng thái của mỗi tiến trình trong một cấu trúc `struct proc`, bao gồm các trường quan trọng như `pagetable` (con trỏ đến bảng trang), `kstack` (địa chỉ ngăn xếp kernel), và `state` (ví dụ: `RUNNING`, `SLEEPING`).

### 3.4. Quá trình Khởi động xv6

Quá trình khởi động hệ điều hành xv6 diễn ra theo một chuỗi các bước được xác định rõ ràng:

1. Boot loader (bộ nạp khởi động) tải kernel xv6 vào bộ nhớ tại địa chỉ vật lý `0x80000000`.
2. CPU bắt đầu thực thi ở chế độ `machine mode` tại điểm vào `_entry`, nơi nó thiết lập một ngăn xếp ban đầu.
3. Hàm `start()` được gọi, thực hiện một số cấu hình cấp thấp và sau đó chuyển sang `supervisor mode` bằng cách thực thi lệnh `mret`.
4. Việc thực thi tiếp tục tại hàm `main()`, nơi kernel khởi tạo các thiết bị và các hệ thống con khác nhau như bộ cấp phát bộ nhớ, bảng tiến trình, và hệ thống tệp.
5. `main()` gọi `userinit()` để tạo tiến trình người dùng đầu tiên.
6. Tiến trình đầu tiên này thực thi một đoạn mã nhỏ (`initcode.S`), mà công việc duy nhất của nó là thực hiện lời gọi hệ thống `exec("/init")`.
7. Tiến trình `init` khởi động, mở console và bắt đầu một shell. Tại thời điểm này, hệ thống đã sẵn sàng nhận lệnh từ người dùng.

Một trong những cơ chế quan trọng nhất để thực hiện trừu tượng hóa bộ nhớ là bảng trang, và đó sẽ là chủ đề chính của phần tiếp theo.

# 4. Bảng Trang (Page Tables)

Bảng trang đóng vai trò trung tâm trong kiến trúc của hầu hết các hệ điều hành hiện đại. Chúng không chỉ là cơ chế cung cấp không gian địa chỉ riêng cho mỗi tiến trình, mà còn là nền tảng cho nhiều kỹ thuật quản lý bộ nhớ tinh vi như copy-on-write, cấp phát lười (lazy allocation) và phân trang theo yêu cầu (demand paging).

### 4.1. Phần cứng Phân trang (Paging Hardware)

Kiến trúc RISC-V (cấu hình Sv39 mà xv6 sử dụng) cung cấp cơ chế phân trang mạnh mẽ.

- **Địa chỉ Ảo và Địa chỉ Vật lý:** Các lệnh CPU thao tác trên địa chỉ ảo (virtual address). Phần cứng phân trang, với sự trợ giúp của kernel, dịch các địa chỉ ảo này thành địa chỉ vật lý (physical address) để truy cập vào RAM.
- **Cấu trúc Bảng trang 3 cấp:** Trong Sv39, một địa chỉ ảo 64-bit chỉ sử dụng 39 bit thấp. Việc dịch địa chỉ được thực hiện thông qua một cây 3 cấp:
    - 27 bit cao của địa chỉ ảo được chia thành ba phần 9 bit.
    - Mỗi phần 9 bit được sử dụng làm chỉ số để tra cứu trong một bảng 512 mục ở mỗi cấp của cây.
    - 12 bit thấp còn lại là độ lệch (offset) trong trang 4KB.
- **Page Table Entry (PTE):** Mỗi mục trong bảng trang chứa:
    - Số trang vật lý (Physical Page Number - PPN).
    - Các cờ (flags) điều khiển truy cập:
        - `PTE_V` (Valid): Mục này có hợp lệ không.
        - `PTE_R` (Read): Cho phép đọc.
        - `PTE_W` (Write): Cho phép ghi.
        - `PTE_X` (Execute): Cho phép thực thi.
        - `PTE_U` (User): Cho phép truy cập từ user mode.
- **Thanh ghi `satp` và TLB:** Kernel ghi địa chỉ vật lý của bảng trang gốc vào thanh ghi `satp` để kích hoạt nó. Để tăng tốc độ dịch địa chỉ, CPU sử dụng một bộ đệm gọi là Translation Look-aside Buffer (TLB) để cache các bản dịch gần đây.

### 4.2. Không gian địa chỉ Kernel và Tiến trình

- **Kernel Address Space:** Một lựa chọn thiết kế quan trọng trong xv6 là sử dụng **"ánh xạ trực tiếp" (direct mapping)**, nơi các địa chỉ ảo của kernel bằng với địa chỉ vật lý tương ứng của chúng. Ví dụ, `KERNBASE` tại `0x80000000` được ánh xạ đến địa chỉ vật lý `0x80000000`. Điều này đơn giản hóa đáng kể việc triển khai kernel, chẳng hạn như khi bộ cấp phát trả về một địa chỉ vật lý, kernel có thể sử dụng nó trực tiếp như một địa chỉ ảo. Tuy nhiên, có một số ngoại lệ đáng chú ý:
    - **Trampoline page:** Được ánh xạ ở một địa chỉ ảo rất cao.
    - **Kernel stacks:** Được ánh xạ ở vùng nhớ cao, với một trang không được ánh xạ (guard page) ngay bên dưới để phát hiện lỗi tràn ngăn xếp.
- **Process Address Space:** Không gian địa chỉ của một tiến trình người dùng bắt đầu từ địa chỉ ảo 0 và có thể phát triển lên đến `MAXVA`. Nó bao gồm các vùng mã lệnh (text), dữ liệu (data), ngăn xếp (stack), và vùng nhớ cấp phát động (heap).

### 4.3. Cấp phát Bộ nhớ Vật lý và `exec`

- **Bộ cấp phát bộ nhớ vật lý (`kalloc.c`):** xv6 quản lý các trang bộ nhớ vật lý trống bằng một danh sách liên kết đơn giản. Điều thú vị là các nút của danh sách liên kết này được lưu trữ ngay trong chính các trang trống đó, giúp tiết kiệm không gian.
- **Lời gọi hệ thống `exec`:** Đây là một ví dụ điển hình về việc thao tác với không gian địa chỉ. Khi một tiến trình gọi `exec`, kernel thực hiện các bước sau:
    1. Đọc header của tệp thực thi ELF để lấy thông tin về các đoạn mã và dữ liệu.
    2. Cấp phát một bảng trang hoàn toàn mới cho tiến trình.
    3. Với mỗi đoạn trong tệp ELF, cấp phát bộ nhớ vật lý và ánh xạ nó vào bảng trang mới, sau đó sao chép nội dung từ tệp vào bộ nhớ.
    4. Cấp phát một trang cho ngăn xếp người dùng và khởi tạo nó với các đối số dòng lệnh.
    5. Giải phóng bảng trang cũ và các trang bộ nhớ vật lý liên quan.

### 4.4. Bài thực hành liên quan

**`Lab: Page tables`**

Kiến thức trong chương này là nền tảng cho bài thực hành về bảng trang. Bài tập này sẽ giúp bạn hiểu rõ cơ chế phân trang bằng cách yêu cầu triển khai các tính năng yêu cầu thao tác trực tiếp với các PTE trong bảng trang của tiến trình, ví dụ như thay đổi cờ quyền truy cập hoặc ánh xạ một trang mới.

Khi các chương trình người dùng thực hiện các hành động bất hợp pháp (như truy cập địa chỉ không hợp lệ) hoặc cần dịch vụ từ kernel, một cơ chế gọi là "bẫy" sẽ được kích hoạt, và đó là chủ đề tiếp theo của chúng ta.

# 5. Bẫy và Lời gọi Hệ thống (Traps and System Calls)

"Bẫy" (trap) là một thuật ngữ chung cho ba loại sự kiện làm gián đoạn luồng thực thi bình thường của một chương trình: **lời gọi hệ thống** (một yêu cầu có chủ đích đến kernel), **ngoại lệ** (một lỗi, ví dụ như chia cho 0 hoặc truy cập bộ nhớ không hợp lệ), và **ngắt thiết bị** (một tín hiệu từ phần cứng). Xử lý bẫy là cơ chế cốt lõi để kernel giành lại quyền kiểm soát từ chương trình người dùng, cung cấp dịch vụ, hoặc xử lý lỗi.

### 5.1. Cơ chế Bẫy của RISC-V

Kiến trúc RISC-V cung cấp một bộ thanh ghi điều khiển chuyên dụng để quản lý bẫy:

- `stvec`: Lưu địa chỉ của trình xử lý bẫy (trap handler) mà kernel đã đăng ký.
- `sepc`: Khi bẫy xảy ra, CPU tự động lưu program counter (PC) của lệnh gây ra bẫy vào thanh ghi này.
- `scause`: Lưu một mã số cho biết nguyên nhân của bẫy (ví dụ: mã 8 cho lời gọi hệ thống từ user mode).
- `sscratch`: Kernel sử dụng thanh ghi này để tạm thời lưu trữ một giá trị (ví dụ như con trỏ đến trapframe) trong quá trình chuyển đổi ngữ cảnh.
- `sstatus`: Chứa các bit điều khiển quan trọng, như `SIE` (Supervisor Interrupt Enable) để bật/tắt ngắt và `SPP` để cho biết chế độ trước khi bẫy xảy ra (user hay supervisor).

### 5.2. Bẫy từ Không gian Người dùng (User Space)

Khi một bẫy xảy ra ở user space, một chuỗi sự kiện phức tạp diễn ra để chuyển quyền kiểm soát sang kernel một cách an toàn.

- **Trampoline Page:** Đây là một trang bộ nhớ đặc biệt được ánh xạ ở cùng một địa chỉ ảo cao trong cả bảng trang của người dùng và của kernel. Nó chứa mã assembly (`uservec`) để bắt đầu quá trình xử lý bẫy. Việc ánh xạ kép này cho phép mã xử lý bẫy tiếp tục chạy một cách liền mạch ngay cả sau khi kernel chuyển sang bảng trang của chính nó.
- **Trapframe:** Là một cấu trúc dữ liệu được cấp phát cho mỗi tiến trình, dùng để lưu toàn bộ 32 thanh ghi của tiến trình người dùng khi bẫy xảy ra.
- **Luồng thực thi:**
    1. Bẫy xảy ra, CPU chuyển sang supervisor mode và nhảy đến `uservec`.
    2. `uservec` (assembly) lưu các thanh ghi người dùng vào trapframe và chuyển sang ngăn xếp kernel.
    3. `uservec` gọi hàm C `usertrap`.
    4. `usertrap` xác định nguyên nhân bẫy và gọi hàm xử lý tương ứng (ví dụ: `syscall()` cho lời gọi hệ thống).
    5. Sau khi xử lý xong, `usertrap` gọi `usertrapret` để chuẩn bị quay lại.
    6. `usertrapret` gọi `userret` (assembly), hàm này khôi phục các thanh ghi người dùng từ trapframe và thực thi lệnh `sret` để quay trở lại user space.

### 5.3. Ngoại lệ Page-Fault và Copy-on-Write (COW)

Các hệ điều hành hiện đại tận dụng ngoại lệ page-fault—một loại bẫy xảy ra khi chương trình truy cập một địa chỉ ảo không hợp lệ—để triển khai các tính năng quản lý bộ nhớ nâng cao.

- **Copy-on-Write (COW) fork:** Đây là một kỹ thuật tối ưu hóa cho lời gọi hệ thống `fork()`.
    - `fork()` truyền thống của xv6 sao chép toàn bộ bộ nhớ của tiến trình cha cho con, điều này rất tốn kém.
    - Với COW, thay vì sao chép, kernel cho tiến trình cha và con **chia sẻ** tất cả các trang bộ nhớ vật lý, nhưng đánh dấu các PTE tương ứng là **chỉ đọc (read-only)**.
    - Khi một trong hai tiến trình cố gắng **ghi** vào một trang chia sẻ, CPU sẽ gây ra một ngoại lệ page-fault.
    - Kernel sẽ bắt lỗi này, tạo một bản sao riêng của trang đó, cập nhật PTE của tiến trình gây lỗi để trỏ đến bản sao mới với quyền ghi đầy đủ, và sau đó cho tiến trình tiếp tục.
- **Các ứng dụng khác của Page-Fault:** Kỹ thuật này cũng là nền tảng cho **lazy allocation** (chỉ cấp phát bộ nhớ vật lý khi nó thực sự được truy cập lần đầu) và **demand paging** (tải các trang của một chương trình từ đĩa vào bộ nhớ chỉ khi chúng cần thiết).

### 5.4. Bài thực hành liên quan

**`Lab: Traps`** và **`Lab: Copy on-write`**

Kiến thức trong chương này là nền tảng cho hai bài thực hành quan trọng. `Lab: Traps` sẽ yêu cầu bạn mở rộng trình xử lý bẫy để hỗ trợ các tính năng mới, chẳng hạn như báo động định kỳ cho tiến trình. `Lab: Copy on-write` là một bài thực hành kinh điển, yêu cầu bạn triển khai một trong những tối ưu hóa quan trọng nhất của hệ điều hành hiện đại, mang lại cái nhìn sâu sắc về sự tương tác giữa phần cứng MMU và phần mềm kernel.

Một loại bẫy đặc biệt quan trọng là ngắt từ các thiết bị phần cứng, điều này dẫn chúng ta đến chủ đề về trình điều khiển thiết bị.

# 6. Ngắt và Trình điều khiển Thiết bị (Interrupts and Device Drivers)

Trình điều khiển (driver) là phần mã trong kernel chịu trách nhiệm quản lý một thiết bị phần cứng cụ thể. Một cấu trúc phổ biến cho driver được chia làm hai phần: "top half", thực thi trong ngữ cảnh của tiến trình gọi hệ thống (ví dụ `read`, `write`), và "bottom half", thực thi trong trình xử lý ngắt để phản hồi các sự kiện từ thiết bị.

### 6.1. Trình điều khiển Console

Trình điều khiển console trong xv6 là một ví dụ điển hình. Nó quản lý phần cứng UART (được mô phỏng bởi QEMU) thông qua các thanh ghi điều khiển được ánh xạ bộ nhớ (memory-mapped control registers).

- **Console Input (Đầu vào):**
    1. Khi người dùng gõ một phím, phần cứng UART tạo ra một ngắt.
    2. Trình xử lý ngắt `uartintr` được kích hoạt. Nó đọc ký tự từ thanh ghi của UART.
    3. Hàm `consoleintr` nhận ký tự này và lưu nó vào một bộ đệm đầu vào. Nó cũng xử lý các ký tự đặc biệt như backspace.
    4. Khi một dòng hoàn chỉnh (kết thúc bằng newline) được nhập, nó sẽ đánh thức bất kỳ tiến trình nào đang chờ (ngủ) trong lời gọi hệ thống `consoleread`.
- **Console Output (Đầu ra):**
    1. Khi một tiến trình gọi `write` để xuất ra console, hàm `uartputc` sẽ thêm từng ký tự vào một bộ đệm đầu ra.
    2. Nó kích hoạt UART để bắt đầu gửi ký tự đầu tiên.
    3. Mỗi khi UART gửi xong một ký tự, nó tạo ra một ngắt "transmit complete".
    4. Trình xử lý ngắt sẽ lấy ký tự tiếp theo từ bộ đệm và gửi đi, quá trình này lặp lại cho đến khi bộ đệm trống.
- **I/O Concurrency:** Việc sử dụng bộ đệm và ngắt cho phép tiến trình (ví dụ, shell) và thiết bị (UART) hoạt động đồng thời, giúp hệ thống phản hồi nhanh hơn và tăng hiệu suất.

### 6.2. Ngắt Hẹn giờ (Timer Interrupts)

Ngắt hẹn giờ là một công cụ thiết yếu để thực hiện đa nhiệm ưu tiên (preemptive multitasking).

- Trong xv6, kernel lập trình cho mỗi lõi CPU để tạo ra một ngắt định kỳ.
- Khi ngắt hẹn giờ xảy ra, trình xử lý `clockintr` được gọi.
- Hàm này sẽ tăng biến `ticks` (một biến đếm thời gian toàn cục). Quan trọng hơn, nó có thể gọi `yield()`, buộc tiến trình đang chạy phải nhường CPU. Điều này đảm bảo rằng không có tiến trình nào có thể chiếm giữ CPU vô thời hạn, thực hiện việc chia sẻ CPU một cách công bằng giữa các tiến trình.

### 6.3. Bài thực hành liên quan

**`Lab: network driver`**

Kiến thức về trình điều khiển và xử lý ngắt là điều kiện tiên quyết cho bài thực hành này. Bạn sẽ được yêu cầu viết một trình điều khiển cho một card mạng ảo (ví dụ, E1000). Bài thực hành này sẽ áp dụng các nguyên tắc về I/O được lập trình, truyền dữ liệu trực tiếp (DMA), quản lý bộ đệm gói tin, và xử lý ngắt để gửi và nhận dữ liệu mạng.

Việc nhiều tiến trình và trình xử lý ngắt chạy đồng thời trên các lõi CPU khác nhau tạo ra nguy cơ xung đột khi truy cập dữ liệu chung. Điều này đòi hỏi một cơ chế bảo vệ mạnh mẽ, được gọi là khóa (lock).

# 7. Cơ chế Khóa (Locking)

Trong các hệ thống đa lõi, nhiều luồng thực thi có thể chạy song song. Điều này dẫn đến vấn đề tương tranh (concurrency), trong đó các luồng có thể truy cập và sửa đổi cùng một dữ liệu chia sẻ, gây ra **tình trạng tranh đua (race condition)**. Ví dụ, nếu hai CPU cùng lúc gọi `kfree()` và thao tác trên cùng một danh sách liên kết các trang trống, một trong các thao tác có thể bị ghi đè, dẫn đến mất dữ liệu hoặc hỏng cấu trúc dữ liệu. Để ngăn chặn điều này, chúng ta cần cơ chế khóa.

### 7.1. Spinlocks

Spinlock là loại khóa cơ bản nhất.

- **Định nghĩa:** Một spinlock là một loại khóa mà một luồng khi cố gắng lấy nó sẽ "quay" (spin) trong một vòng lặp chờ đợi bận rộn (busy-waiting) cho đến khi khóa được giải phóng.
- **Triển khai:** Một triển khai đơn giản bằng cách kiểm tra và gán một biến `locked` là không đủ vì bản thân thao tác kiểm tra-và-gán không phải là nguyên tử. Để giải quyết vấn đề này, cần sử dụng các **lệnh nguyên tử (atomic instructions)** do phần cứng cung cấp, chẳng hạn như `amoswap` trên RISC-V. Lệnh này đọc giá trị tại một địa chỉ bộ nhớ và ghi một giá trị mới vào đó trong một thao tác duy nhất, không thể bị gián đoạn.
- **Khóa và Ngắt:** Để tránh deadlock giữa một luồng đang giữ khóa và một trình xử lý ngắt muốn lấy cùng khóa đó, xv6 tuân theo một quy tắc nghiêm ngặt: nó luôn **vô hiệu hóa ngắt** trên một CPU khi CPU đó đang giữ bất kỳ spinlock nào.

### 7.2. Sleep-locks

Spinlock không hiệu quả cho các tác vụ cần giữ khóa trong thời gian dài (ví dụ: các thao tác I/O trên đĩa) vì nó lãng phí thời gian CPU vào việc chờ đợi bận rộn.

- **Định nghĩa:** Sleep-lock là một loại khóa mà luồng chờ đợi sẽ "ngủ" (tức là nhường CPU cho một luồng khác) thay vì "quay".
- **Đặc điểm:** Vì sleep-lock cho phép nhường CPU, nó cũng cho phép ngắt xảy ra trong khi khóa được giữ. Do đó, sleep-lock **không thể** được sử dụng bên trong các trình xử lý ngắt.

### 7.3. Deadlock và Thứ tự Khóa

- **Định nghĩa Deadlock:** Deadlock là tình huống trong đó hai hoặc nhiều luồng chờ đợi nhau một cách vô thời hạn. Ví dụ kinh điển:
    - Luồng 1 giữ khóa A và đang chờ để lấy khóa B.
    - Luồng 2 giữ khóa B và đang chờ để lấy khóa A.
- **Giải pháp:** Để tránh deadlock, hệ thống phải tuân thủ một **thứ tự khóa toàn cục (global lock order)**. Mọi luồng mã khi cần lấy nhiều khóa cùng lúc phải luôn lấy chúng theo đúng thứ tự đã định trước.

### 7.4. Bài thực hành liên quan

**`Lab: Lock`**

Việc hiểu sâu về cơ chế khóa là điều kiện tiên quyết cho bài thực hành này. `Lab: Lock` thường yêu cầu bạn triển khai một cấu trúc dữ liệu an toàn luồng (thread-safe), chẳng hạn như một bảng băm tương tranh. Điều này buộc bạn phải xác định các vùng tới hạn (critical sections) và bảo vệ chúng một cách cẩn thận bằng các loại khóa thích hợp.

Khi một tiến trình phải chờ đợi (dù là chờ khóa hay chờ I/O), hệ điều hành cần một cơ chế để chuyển CPU sang một tiến trình khác sẵn sàng chạy. Cơ chế này được gọi là lập lịch.

# 8. Lập lịch (Scheduling)

Lập lịch là quá trình mà hệ điều hành quyết định tiến trình nào sẽ được chạy tiếp theo trên một CPU. Mục tiêu của nó là tạo ra ảo giác rằng mỗi tiến trình có một CPU riêng bằng cách ghép kênh (multiplexing) nhiều tiến trình lên các CPU vật lý có sẵn một cách hiệu quả và công bằng.

### 8.1. Chuyển đổi Ngữ cảnh (Context Switching)

Đây là cơ chế cấp thấp để chuyển CPU từ luồng này sang luồng khác.

- **Định nghĩa:** Chuyển đổi ngữ cảnh là hành động lưu trạng thái (chủ yếu là các thanh ghi) của luồng đang chạy và khôi phục trạng thái của một luồng khác.
- **Hàm `swtch`:** Trong xv6, công việc này được thực hiện bởi một hàm assembly nhỏ tên là `swtch`. Nó không lưu tất cả các thanh ghi, mà chỉ lưu các thanh ghi "callee-saved"—những thanh ghi mà theo quy ước, một hàm được gọi phải bảo toàn giá trị cho hàm gọi nó. Quan trọng nhất, nó lưu và khôi phục thanh ghi con trỏ ngăn xếp (`sp`) và thanh ghi địa chỉ trả về (`ra`), cho phép việc chuyển đổi giữa các ngăn xếp và các luồng thực thi khác nhau.

### 8.2. Sleep & Wakeup

Đây là cặp cơ chế phối hợp chính trong xv6, cho phép một tiến trình tự nguyện nhường CPU và chờ một sự kiện cụ thể.

- **Vấn đề "Lost Wakeup":** Một triển khai ngây thơ có thể dẫn đến tình trạng "đánh thức bị mất", khi một tiến trình quyết định đi ngủ, nhưng ngay trước khi nó thực sự ngủ, một tiến trình khác đã gửi tín hiệu đánh thức. Tín hiệu này bị mất và tiến trình đầu tiên sẽ ngủ mãi mãi.
- **Giải pháp của xv6:**
    - Tiến trình gọi `sleep(chan, lk)` phải đang giữ một spinlock (`lk`).
    - Bên trong `sleep`, kernel phải giải phóng khóa (`lk`) và đưa tiến trình vào trạng thái ngủ **trong cùng một thao tác nguyên tử**. Điều này ngăn chặn một `wakeup` can thiệp vào giữa và gây ra một "lost wakeup".
    - Hàm `wakeup(chan)` sẽ quét qua bảng tiến trình, tìm tất cả các tiến trình đang ngủ trên kênh `chan` và chuyển trạng thái của chúng thành `RUNNABLE`.
- **Quy tắc Vàng:** `sleep` phải luôn được gọi bên trong một vòng lặp `while` để kiểm tra lại điều kiện sau khi được đánh thức, vì có thể có những lần "đánh thức giả" (spurious wakeups).

### 8.3. Ví dụ: `wait`, `exit`, và `kill`

Sự phối hợp giữa các lời gọi hệ thống này là một ví dụ điển hình về việc sử dụng `sleep` và `wakeup`.

- `exit()`: Khi một tiến trình gọi `exit`, nó không biến mất ngay lập tức. Thay vào đó, nó dọn dẹp tài nguyên, chuyển trạng thái thành `ZOMBIE`, và gọi `wakeup()` để đánh thức tiến trình cha (nếu cha đang chờ trong `wait`).
- `wait()`: Tiến trình cha quét qua danh sách con của mình. Nếu tìm thấy một tiến trình con ở trạng thái `ZOMBIE`, nó sẽ thu dọn hoàn toàn tài nguyên của con, lấy mã thoát, và trả về. Nếu không có con nào `ZOMBIE`, nó sẽ gọi `sleep()` để chờ.
- `kill()`: Lời gọi này không trực tiếp "giết" một tiến trình khác. Nó chỉ đơn giản là đặt cờ `p->killed` trong `struct proc` của tiến trình nạn nhân. Nếu nạn nhân đang ngủ, `kill` sẽ đánh thức nó. Tiến trình nạn nhân sẽ kiểm tra cờ này vào lần tiếp theo nó chuyển từ không gian người dùng sang không gian kernel (thông qua một lời gọi hệ thống hoặc ngắt) hoặc khi chuẩn bị quay trở lại không gian người dùng. Khi thấy cờ này được bật, nó sẽ không quay trở lại không gian người dùng mà thay vào đó sẽ gọi `exit()`.

Một trong những hệ thống con phức tạp nhất, nơi tất cả các khái niệm trên—từ khóa, lập lịch, đến quản lý bộ nhớ—hội tụ, chính là hệ thống tệp.

# 9. Hệ thống Tệp (File System)

Thiết kế một hệ thống tệp đặt ra nhiều thách thức lớn: làm thế nào để tạo ra các cấu trúc dữ liệu hiệu quả trên đĩa, làm sao để đảm bảo hệ thống có thể phục hồi sau sự cố đột ngột (crash recovery), làm thế nào để quản lý truy cập đồng thời từ nhiều tiến trình, và làm sao để tối ưu hóa hiệu suất bằng cách sử dụng bộ đệm trong bộ nhớ.

### 9.1. Các Lớp của Hệ thống Tệp

Hệ thống tệp của xv6 được tổ chức theo một kiến trúc bảy lớp rõ ràng, giúp quản lý sự phức tạp:

1. **File descriptor layer:** Cung cấp lớp trừu tượng mô tả tệp cho các tiến trình.
2. **Pathname layer:** Xử lý việc tra cứu đường dẫn như `/a/b/c`.
3. **Directory layer:** Triển khai các thư mục dưới dạng các tệp đặc biệt.
4. **Inode layer:** Quản lý các inode, đại diện cho các tệp riêng lẻ.
5. **Logging layer:** Đảm bảo các thao tác là nguyên tử đối với sự cố (crash-safe).
6. **Buffer cache layer:** Đệm các khối đĩa trong bộ nhớ để tăng hiệu suất và đồng bộ hóa truy cập.
7. **Disk layer:** Tương tác trực tiếp với phần cứng đĩa.

### 9.2. Buffer Cache và Logging

Đây là hai lớp quan trọng nhất, quyết định hiệu suất và độ tin cậy của hệ thống tệp.

- **Buffer Cache:**
    - **Vai trò:** (1) Đồng bộ hóa quyền truy cập vào các khối đĩa, đảm bảo chỉ một luồng có thể sửa đổi một khối tại một thời điểm. (2) Lưu trữ các khối thường dùng trong bộ nhớ (caching) để tránh phải đọc lại từ đĩa, một thao tác rất chậm.
    - **Chiến lược thay thế:** xv6 sử dụng một chiến lược LRU (Least Recently Used) đơn giản để quyết định khối nào sẽ bị loại bỏ khỏi bộ đệm khi cần không gian.
- **Logging (Ghi nhật ký):** Đây là cơ chế phục hồi sau sự cố của xv6.
    - **Luồng hoạt động:** Thay vì ghi trực tiếp các thay đổi vào cấu trúc dữ liệu của hệ thống tệp trên đĩa, các lời gọi hệ thống trước tiên ghi một **mô tả** về các thay đổi đó vào một khu vực riêng trên đĩa gọi là **log**.
    - **Commit:** Sau khi tất cả các bản ghi cho một thao tác đã được viết vào log, một bản ghi "commit" đặc biệt được ghi vào cuối log.
    - **Áp dụng:** Chỉ sau khi commit thành công, các thay đổi mới thực sự được ghi vào vị trí chính xác của chúng trong hệ thống tệp.
    - **Phục hồi:** Khi khởi động lại sau sự cố, hệ thống sẽ kiểm tra log. Nếu log có bản ghi commit, nó sẽ "phát lại" (replay) các thay đổi để hoàn tất thao tác. Nếu không có bản ghi commit, nó sẽ bỏ qua log, coi như thao tác chưa bao giờ xảy ra. Điều này đảm bảo tính nguyên tử.

### 9.3. Inodes và Thư mục

Đây là các cấu trúc dữ liệu cốt lõi trên đĩa.

- **Inodes:**
    - Mỗi tệp hoặc thư mục được đại diện bởi một inode. Có sự phân biệt giữa cấu trúc inode trên đĩa (`struct dinode`) và trong bộ nhớ (`struct inode`).
    - Inode chứa siêu dữ liệu quan trọng: `type` (tệp, thư mục, hay thiết bị), `nlink` (số lượng tên trỏ đến nó), `size` (kích thước tệp).
    - Mảng `addrs` trong inode chứa địa chỉ của các khối dữ liệu. Trong xv6, nó bao gồm các **khối trực tiếp (direct blocks)** và một **khối gián tiếp (indirect block)**, cho phép các tệp có kích thước lớn hơn.
- **Thư mục (Directories):**
    - Về cơ bản, một thư mục là một tệp đặc biệt có `type` là `T_DIR`.
    - Nội dung dữ liệu của nó là một chuỗi các mục `struct dirent`, mỗi mục chứa tên của một tệp/thư mục con và số inode (i-number) tương ứng của nó.

### 9.4. Bài thực hành liên quan

**`Lab: File system`** và **`Lab: mmap`**

Kiến thức trong chương này là cực kỳ quan trọng cho hai bài thực hành nâng cao. `Lab: File system` thường yêu cầu bạn mở rộng hệ thống tệp, chẳng hạn như triển khai hỗ trợ các tệp lớn hơn bằng cách thêm các khối gián tiếp cấp hai, hoặc thêm các liên kết tượng trưng (symbolic links). `Lab: mmap` là một bài thực hành thú vị, yêu cầu bạn liên kết hệ thống tệp với hệ thống quản lý bộ nhớ ảo bằng cách triển khai lời gọi hệ thống `mmap`, một tính năng tiêu chuẩn trong các hệ điều hành hiện đại.

# 10. Tổng kết và Hướng đi tiếp theo

Qua hành trình khám phá xv6, chúng ta đã đi sâu vào các khái niệm nền tảng nhất của một hệ điều hành: từ việc trừu tượng hóa phần cứng để tạo ra các **tiến trình**, quản lý **bộ nhớ ảo** thông qua bảng trang, xử lý các sự kiện không đồng bộ bằng cơ chế **bẫy và ngắt**, giải quyết các vấn đề **tương tranh** trên hệ thống đa lõi bằng khóa, cho đến việc xây dựng một **hệ thống tệp** bền vững và có khả năng phục hồi.

xv6, mặc dù đơn giản, đã cho thấy cách các thành phần này phối hợp với nhau để tạo thành một tổng thể hoạt động hài hòa. Kiến thức thu được từ việc nghiên cứu xv6 không chỉ giới hạn ở chính nó. Nó cung cấp cho bạn một bộ công cụ tư duy và một nền tảng vững chắc để tự tin tiếp cận, phân tích, và hiểu các hệ điều hành phức tạp hơn nhiều trong thực tế như Linux, Windows, hay macOS. Hãy xem đây là bước khởi đầu cho một hành trình khám phá sâu hơn vào thế giới kỳ diệu của kỹ thuật hệ điều hành.