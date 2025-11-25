
# xv6: một hệ điều hành dạy học đơn giản, giống Unix

**Tác giả: Russ Cox, Frans Kaashoek, Robert Morris**

**Ngày 31 tháng 8 năm 2024**

## Mục lục

*   [1 Giao diện hệ điều hành](#1-giao-diện-hệ-điều-hành)
    *   [1.1 Tiến trình và bộ nhớ](#11-tiến-trình-và-bộ-nhớ)
    *   [1.2 I/O và Mô tả tệp](#12-io-và-mô-tả-tệp)
    *   [1.3 Ống (Pipes)](#13-ống-pipes)
    *   [1.4 Hệ thống tệp](#14-hệ-thống-tệp)
    *   [1.5 Thế giới thực](#15-thế-giới-thực)
    *   [1.6 Bài tập](#16-bài-tập)
*   [2 Tổ chức hệ điều hành](#2-tổ-chức-hệ-điều-hành)
    *   [2.1 Trừu tượng hóa tài nguyên vật lý](#21-trừu-tượng-hóa-tài-nguyên-vật-lý)
    *   [2.2 Chế độ người dùng, chế độ giám sát và các lời gọi hệ thống](#22-chế-độ-người-dùng-chế-độ-giám-sát-và-các-lời-gọi-hệ-thống)
    *   [2.3 Tổ chức hạt nhân](#23-tổ-chức-hạt-nhân)
    *   [2.4 Mã: tổ chức xv6](#24-mã-tổ-chức-xv6)
    *   [2.5 Tổng quan về tiến trình](#25-tổng-quan-về-tiến-trình)
    *   [2.6 Mã: khởi động xv6, tiến trình đầu tiên và lời gọi hệ thống](#26-mã-khởi-động-xv6-tiến-trình-đầu-tiên-và-lời-gọi-hệ-thống)
    *   [2.7 Mô hình bảo mật](#27-mô-hình-bảo-mật)
    *   [2.8 Thế giới thực](#28-thế-giới-thực)
    *   [2.9 Bài tập](#29-bài-tập)
*   [3 Bảng trang](#3-bảng-trang)
    *   [3.1 Phần cứng phân trang](#31-phần-cứng-phân-trang)
    *   [3.2 Không gian địa chỉ hạt nhân](#32-không-gian-địa-chỉ-hạt-nhân)
    *   [3.3 Mã: tạo một không gian địa chỉ](#33-mã-tạo-một-không-gian-địa-chỉ)
    *   [3.4 Cấp phát bộ nhớ vật lý](#34-cấp-phát-bộ-nhớ-vật-lý)
    *   [3.5 Mã: Bộ cấp phát bộ nhớ vật lý](#35-mã-bộ-cấp-phát-bộ-nhớ-vật-lý)
    *   [3.6 Không gian địa chỉ tiến trình](#36-không-gian-địa-chỉ-tiến-trình)
    *   [3.7 Mã: sbrk](#37-mã-sbrk)
    *   [3.8 Mã: exec](#38-mã-exec)
    *   [3.9 Thế giới thực](#39-thế-giới-thực)
    *   [3.10 Bài tập](#310-bài-tập)
*   [4 Bẫy và các lời gọi hệ thống](#4-bẫy-và-các-lời-gọi-hệ-thống)
    *   [4.1 Cơ chế bẫy RISC-V](#41-cơ-chế-bẫy-risc-v)
    *   [4.2 Bẫy từ không gian người dùng](#42-bẫy-từ-không-gian-người-dùng)
    *   [4.3 Mã: Gọi các lời gọi hệ thống](#43-mã-gọi-các-lời-gọi-hệ-thống)
    *   [4.4 Mã: Đối số lời gọi hệ thống](#44-mã-đối-só-lời-gọi-hệ-thống)
    *   [4.5 Bẫy từ không gian hạt nhân](#45-bẫy-từ-không-gian-hạt-nhân)
    *   [4.6 Ngoại lệ lỗi trang](#46-ngoại-lệ-lỗi-trang)
    *   [4.7 Thế giới thực](#47-thế-giới-thực)
    *   [4.8 Bài tập](#48-bài-tập)
*   [5 Ngắt và trình điều khiển thiết bị](#5-ngắt-và-trình-điều-khiển-thiết-bị)
    *   [5.1 Mã: Nhập liệu từ giao diện điều khiển](#51-mã-nhập-liệu-từ-giao-diện-điều-khiển)
    *   [5.2 Mã: Xuất liệu ra giao diện điều khiển](#52-mã-xuất-liệu-ra-giao-diện-điều-khiển)
    *   [5.3 Đồng thời trong các trình điều khiển](#53-đồng-thời-trong-các-trình-điều-khiển)
    *   [5.4 Ngắt hẹn giờ](#54-ngắt-hẹn-giờ)
    *   [5.5 Thế giới thực](#55-thế-giới-thực)
    *   [5.6 Bài tập](#56-bài-tập)
*   [6 Khóa](#6-khóa)
    *   [6.1 Tranh chấp (Races)](#61-tranh-chấp-races)
    *   [6.2 Mã: Khóa](#62-mã-khóa)
    *   [6.3 Mã: Sử dụng khóa](#63-mã-sử-dụng-khóa)
    *   [6.4 Khóa chết và thứ tự khóa](#64-khóa-chết-và-thứ-tự-khóa)
    *   [6.5 Khóa đệ quy](#65-khóa-đệ-quy)
    *   [6.6 Khóa và trình xử lý ngắt](#66-khóa-và-trình-xử-lý-ngắt)
    *   [6.7 Thứ tự lệnh và bộ nhớ](#67-thứ-tự-lệnh-và-bộ-nhớ)
    *   [6.8 Khóa ngủ (Sleep locks)](#68-khóa-ngủ-sleep-locks)
    *   [6.9 Thế giới thực](#69-thế-giới-thực)
    *   [6.10 Bài tập](#610-bài-tập)
*   [7 Lập lịch](#7-lập-lịch)
    *   [7.1 Ghép kênh (Multiplexing)](#71-ghép-kênh-multiplexing)
    *   [7.2 Mã: Chuyển đổi ngữ cảnh](#72-mã-chuyển-đổi-ngữ-cảnh)
    *   [7.3 Mã: Lập lịch](#73-mã-lập-lịch)
    *   [7.4 Mã: mycpu và myproc](#74-mã-mycpu-và-myproc)
    *   [7.5 Ngủ và đánh thức](#75-ngủ-và-đánh-thức)
    *   [7.6 Mã: Ngủ và đánh thức](#76-mã-ngủ-và-đánh-thức)
    *   [7.7 Mã: Ống (Pipes)](#77-mã-ống-pipes)
    *   [7.8 Mã: Đợi, thoát và hủy](#78-mã-đợi-thoát-và-hủy)
    *   [7.9 Khóa tiến trình](#79-khóa-tiến-trình)
    *   [7.10 Thế giới thực](#710-thế-giới-thực)
    *   [7.11 Bài tập](#711-bài-tập)
*   [8 Hệ thống tệp](#8-hệ-thống-tệp)
    *   [8.1 Tổng quan](#81-tổng-quan)
    *   [8.2 Lớp đệm cache](#82-lớp-đệm-cache)
    *   [8.3 Mã: Đệm cache](#83-mã-đệm-cache)
    *   [8.4 Lớp ghi nhật ký](#84-lớp-ghi-nhật-ký)
    *   [8.5 Thiết kế nhật ký](#85-thiết-kế-nhật-ký)
    *   [8.6 Mã: ghi nhật ký](#86-mã-ghi-nhật-ký)
    *   [8.7 Mã: Bộ cấp phát khối](#87-mã-bộ-cấp-phát-khối)
    *   [8.8 Lớp Inode](#88-lớp-inode)
    *   [8.9 Mã: Inode](#89-mã-inode)
    *   [8.10 Mã: Nội dung Inode](#810-mã-nội-dung-inode)
    *   [8.11 Mã: lớp thư mục](#811-mã-lớp-thư-mục)
    *   [8.12 Mã: Tên đường dẫn](#812-mã-tên-đường-dẫn)
    *   [8.13 Lớp mô tả tệp](#813-lớp-mô-tả-tệp)
    *   [8.14 Mã: Lời gọi hệ thống](#814-mã-lời-gọi-hệ-thống)
    *   [8.15 Thế giới thực](#815-thế-giới-thực)
    *   [8.16 Bài tập](#816-bài-tập)
*   [9 Đồng thời được xem xét lại](#9-đồng-thời-được-xem-xét-lại)
    *   [9.1 Các mẫu khóa](#91-các-mẫu-khóa)
    *   [9.2 Các mẫu giống như khóa](#92-các-mẫu-giống-như-khóa)
    *   [9.3 Hoàn toàn không có khóa](#93-hoàn-toàn-không-có-khóa)
    *   [9.4 Song song](#94-song-song)
    *   [9.5 Bài tập](#95-bài-tập)
*   [10 Tóm tắt](#10-tóm-tắt)

## Lời nói đầu và lời cảm ơn

Đây là một bản nháp dùng cho một lớp học về hệ điều hành. Nó giải thích các khái niệm chính của hệ điều hành bằng cách nghiên cứu một hạt nhân ví dụ, có tên là xv6. Xv6 được mô phỏng theo Unix phiên bản 6 (v6) của Dennis Ritchie và Ken Thompson [17]. Xv6 tuân theo cấu trúc và phong cách của v6 một cách lỏng lẻo, nhưng được triển khai bằng ANSI C [7] cho một RISC-V đa lõi [15].

Cuốn sách này nên được đọc cùng với mã nguồn của xv6, một cách tiếp cận được truyền cảm hứng từ Chú giải về UNIX phiên bản thứ 6 của John Lions [11]; văn bản có các siêu liên kết đến mã nguồn tại https://github.com/mit-pdos/xv6-riscv. Xem https://pdos.csail.mit.edu/6.1810 để biết thêm các con trỏ đến các tài nguyên trực tuyến cho v6 và xv6, bao gồm một số bài tập trong phòng thí nghiệm sử dụng xv6.

Chúng tôi đã sử dụng văn bản này trong 6.828 và 6.1810, các lớp hệ điều hành tại MIT. Chúng tôi cảm ơn các giảng viên, trợ giảng và sinh viên của các lớp đó đã đóng góp trực tiếp hoặc gián tiếp cho xv6. Đặc biệt, chúng tôi muốn cảm ơn Adam Belay, Austin Clements và Nickolai Zeldovich. Cuối cùng, chúng tôi muốn cảm ơn những người đã gửi email cho chúng tôi về các lỗi trong văn bản hoặc các đề xuất cải tiến: Abutalib Aghayev, Sebastian Boehm, brandb97, Anton Burtsev, Raphael Carvalho, Tej Chajed, Brendan Davidson, Rasit Eskicioglu, Color Fuzzy, Wojciech Gac, Giuseppe, Tao Guo, Haibo Hao, Naoki Hayama, Chris Henderson, Robert Hilderman, Eden Hochbaum, Wolfgang Keller, Paweł Kraszewski, Henry Laih, Jin Li, Austin Liew, lyazj@github.com, Pavan Maddamsetti, Jacek Masiulaniec, Michael McConville, m3hm00d, miguelgvieira, Mark Morrissey, Muhammed Mourad, Harry Pan, Harry Porter, Siyuan Qian, Zhefeng Qiao, Askar Safin, Salman Shah, Huang Sha, Vikram Shenoy, Adeodato Simó, Ruslan Savchenko, Pawel Szczurko, Warren Toomey, tyfkda, tzerbib, Vanush Vaswani, Xi Wang, và Zou Chang Wei, Sam Whitlock, Qiongsi Wu, LucyShawYang, ykf11114@gmail.com, và Meng Zhou

Nếu bạn phát hiện lỗi hoặc có đề xuất cải tiến, vui lòng gửi email cho Frans Kaashoek và Robert Morris (kaashoek,rtm@csail.mit.edu).

## 1 Giao diện hệ điều hành

Công việc của một hệ điều hành là chia sẻ một máy tính cho nhiều chương trình và cung cấp một tập hợp các dịch vụ hữu ích hơn so với chỉ phần cứng. Một hệ điều hành quản lý và trừu tượng hóa phần cứng cấp thấp, để ví dụ, một trình xử lý văn bản không cần phải quan tâm đến loại phần cứng đĩa nào đang được sử dụng. Một hệ điều hành chia sẻ phần cứng cho nhiều chương trình để chúng chạy (hoặc dường như chạy) cùng một lúc. Cuối cùng, các hệ điều hành cung cấp các cách có kiểm soát để các chương trình tương tác, để chúng có thể chia sẻ dữ liệu hoặc làm việc cùng nhau.

Một hệ điều hành cung cấp các dịch vụ cho các chương trình người dùng thông qua một giao diện. Thiết kế một giao diện tốt hóa ra lại khó. Một mặt, chúng tôi muốn giao diện đơn giản và hẹp vì điều đó giúp việc triển khai đúng trở nên dễ dàng hơn. Mặt khác, chúng tôi có thể bị cám dỗ để cung cấp nhiều tính năng phức tạp cho các ứng dụng. Bí quyết để giải quyết sự căng thẳng này là thiết kế các giao diện dựa trên một vài cơ chế có thể được kết hợp để cung cấp nhiều tính tổng quát.

Cuốn sách này sử dụng một hệ điều hành duy nhất làm ví dụ cụ thể để minh họa các khái niệm về hệ điều hành. Hệ điều hành đó, xv6, cung cấp các giao diện cơ bản được giới thiệu bởi hệ điều hành Unix của Ken Thompson và Dennis Ritchie [17], cũng như bắt chước thiết kế nội bộ của Unix. Unix cung cấp một giao diện hẹp có các cơ chế kết hợp tốt, mang lại một mức độ tổng quát đáng ngạc nhiên. Giao diện này đã thành công đến nỗi các hệ điều hành hiện đại — BSD, Linux, macOS, Solaris và thậm chí, ở một mức độ thấp hơn, Microsoft Windows — đều có các giao diện giống Unix. Hiểu xv6 là một khởi đầu tốt để hiểu bất kỳ hệ thống nào trong số này và nhiều hệ thống khác.

Như Hình 1.1 cho thấy, xv6 có dạng truyền thống của một *kernel*, một chương trình đặc biệt cung cấp các dịch vụ cho các chương trình đang chạy. Mỗi chương trình đang chạy, được gọi là một *tiến trình*, có bộ nhớ chứa các lệnh, dữ liệu và một *ngăn xếp*. Các lệnh thực hiện tính toán của chương trình. Dữ liệu là các biến mà phép tính toán tác động lên. Ngăn xếp tổ chức các lệnh gọi thủ tục của chương trình. Một máy tính nhất định thường có nhiều tiến trình nhưng chỉ có một hạt nhân.

Khi một tiến trình cần gọi một dịch vụ hạt nhân, nó sẽ gọi một *lời gọi hệ thống*, một trong các lệnh gọi trong giao diện của hệ điều hành. Lời gọi hệ thống đi vào hạt nhân; hạt nhân thực hiện dịch vụ và trả về. Do đó, một tiến trình xen kẽ giữa việc thực thi trong *không gian người dùng* và *không gian hạt nhân*.

Như được mô tả chi tiết trong các chương tiếp theo, hạt nhân sử dụng các cơ chế bảo vệ phần cứng do CPU¹ cung cấp để đảm bảo rằng mỗi tiến trình thực thi trong không gian người dùng chỉ có thể truy cập vào bộ nhớ của chính nó. Hạt nhân thực thi với các đặc quyền phần cứng cần thiết để thực hiện các biện pháp bảo vệ này; các chương trình người dùng thực thi mà không có các đặc quyền đó. Khi một chương trình người dùng gọi một lời gọi hệ thống, phần cứng sẽ nâng mức đặc quyền và bắt đầu thực thi một hàm được sắp xếp trước trong hạt nhân.

Tập hợp các lời gọi hệ thống mà một hạt nhân cung cấp là giao diện mà các chương trình người dùng nhìn thấy. Hạt nhân xv6 cung cấp một tập hợp con các dịch vụ và lời gọi hệ thống mà các hạt nhân Unix thường cung cấp. Hình 1.2 liệt kê tất cả các lời gọi hệ thống của xv6.

Phần còn lại của chương này phác thảo các dịch vụ của xv6 — các tiến trình, bộ nhớ, mô tả tệp, ống dẫn và một hệ thống tệp — và minh họa chúng bằng các đoạn mã và thảo luận về cách *shell*, giao diện người dùng dòng lệnh của Unix, sử dụng chúng. Việc shell sử dụng các lời gọi hệ thống minh họa mức độ cẩn thận mà chúng đã được thiết kế.

Shell là một chương trình thông thường đọc các lệnh từ người dùng và thực thi chúng. Việc shell là một chương trình người dùng, và không phải là một phần của hạt nhân, minh họa sức mạnh của giao diện lời gọi hệ thống: không có gì đặc biệt về shell. Điều đó cũng có nghĩa là shell dễ dàng thay thế; kết quả là, các hệ thống Unix hiện đại có nhiều loại shell để lựa chọn, mỗi loại có giao diện người dùng và các tính năng kịch bản riêng. Shell xv6 là một triển khai đơn giản về bản chất của shell Bourne của Unix. Việc triển khai nó có thể được tìm thấy tại (user/sh.c:1).

¹Văn bản này thường đề cập đến phần tử phần cứng thực thi một phép tính bằng thuật ngữ CPU, một từ viết tắt của đơn vị xử lý trung tâm. Các tài liệu khác (ví dụ: đặc tả RISC-V) cũng sử dụng các từ bộ xử lý, lõi và hart thay cho CPU.

### 1.1 Tiến trình và bộ nhớ

Một tiến trình xv6 bao gồm bộ nhớ không gian người dùng (lệnh, dữ liệu và ngăn xếp) và trạng thái trên mỗi tiến trình riêng tư đối với hạt nhân. Xv6 *chia sẻ thời gian* các tiến trình: nó chuyển đổi một cách minh bạch các CPU có sẵn giữa tập hợp các tiến trình đang chờ thực thi. Khi một tiến trình không thực thi, xv6 sẽ lưu các thanh ghi CPU của tiến trình, khôi phục chúng khi nó chạy tiến trình tiếp theo. Hạt nhân liên kết một *định danh tiến trình*, hoặc *PID*, với mỗi tiến trình.

Một tiến trình có thể tạo một tiến trình mới bằng cách sử dụng lời gọi hệ thống `fork`. `fork` cung cấp cho tiến trình mới một bản sao chính xác của bộ nhớ của tiến trình đang gọi: nó sao chép các lệnh, dữ liệu và ngăn xếp của tiến trình đang gọi vào bộ nhớ của tiến trình mới. `fork` trả về trong cả tiến trình gốc và tiến trình mới. Trong tiến trình gốc, `fork` trả về PID của tiến trình mới. Trong tiến trình mới, `fork` trả về 0. Tiến trình gốc và tiến trình mới thường được gọi là *cha* và *con*.

Ví dụ, hãy xem xét đoạn chương trình sau được viết bằng ngôn ngữ lập trình C [7]:

```c
int pid = fork();
if(pid > 0) {
    printf("parent: child=%d\n", pid);
    pid = wait((int *) 0);
    printf("child %d is done\n", pid);
} else if(pid == 0) {
    printf("child: exiting\n");
    exit(0);
} else {
    printf("fork error\n");
}
```

Lời gọi hệ thống `exit` khiến tiến trình đang gọi ngừng thực thi và giải phóng các tài nguyên như bộ nhớ và các tệp đang mở. `exit` nhận một đối số trạng thái số nguyên, theo quy ước 0 để chỉ ra thành công và 1 để chỉ ra thất bại. Lời gọi hệ thống `wait` trả về PID của một đứa con đã thoát (hoặc bị giết) của tiến trình hiện tại và sao chép trạng thái thoát của đứa con vào địa chỉ được truyền cho `wait`; nếu không có đứa con nào của người gọi đã thoát, `wait` sẽ đợi một đứa con làm như vậy. Nếu người gọi không có con, `wait` sẽ trả về -1 ngay lập tức. Nếu cha mẹ không quan tâm đến trạng thái thoát của một đứa con, nó có thể truyền một địa chỉ 0 cho `wait`.

Trong ví dụ, các dòng đầu ra

```
parent: child=1234
child: exiting
```

có thể xuất hiện theo thứ tự nào (hoặc thậm chí xen kẽ), tùy thuộc vào việc cha mẹ hay con cái đến lệnh gọi `printf` của nó trước. Sau khi con cái thoát, `wait` của cha mẹ trả về, khiến cha mẹ in

```
parent: child 1234 is done
```

Mặc dù ban đầu con có cùng nội dung bộ nhớ với cha, nhưng cha và con đang thực thi với bộ nhớ và thanh ghi riêng biệt: việc thay đổi một biến trong một không ảnh hưởng đến biến kia. Ví dụ, khi giá trị trả về của `wait` được lưu trữ vào `pid` trong tiến trình cha, nó không thay đổi biến `pid` trong con. Giá trị của `pid` trong con vẫn sẽ bằng 0.

Lời gọi hệ thống `exec` thay thế bộ nhớ của tiến trình đang gọi bằng một hình ảnh bộ nhớ mới được tải từ một tệp được lưu trữ trong hệ thống tệp. Tệp phải có một định dạng cụ thể, chỉ định phần nào của tệp chứa lệnh, phần nào là dữ liệu, lệnh nào để bắt đầu, v.v. Xv6 sử dụng định dạng ELF, mà Chương 3 thảo luận chi tiết hơn. Thông thường, tệp là kết quả của việc biên dịch mã nguồn của một chương trình. Khi `exec` thành công, nó không trả về chương trình đang gọi; thay vào đó, các lệnh được tải từ tệp bắt đầu thực thi tại điểm vào được khai báo trong tiêu đề ELF. `exec` nhận hai đối số: tên của tệp chứa tệp thực thi và một mảng các đối số chuỗi. Ví dụ:

```c
char *argv[3];
argv[0] = "echo";
argv[1] = "hello";
argv[2] = 0;
exec("/bin/echo", argv);
printf("exec error\n");
```

Đoạn mã này thay thế chương trình đang gọi bằng một phiên bản của chương trình `/bin/echo` đang chạy với danh sách đối số `echo hello`. Hầu hết các chương trình bỏ qua phần tử đầu tiên của mảng đối số, theo quy ước là tên của chương trình.

Shell xv6 sử dụng các lệnh gọi trên để chạy các chương trình thay mặt cho người dùng. Cấu trúc chính của shell rất đơn giản; xem `main` (user/sh.c:146). Vòng lặp chính đọc một dòng đầu vào từ người dùng bằng `getcmd`. Sau đó, nó gọi `fork`, tạo một bản sao của tiến trình shell. Cha mẹ gọi `wait`, trong khi con chạy lệnh. Ví dụ, nếu người dùng đã nhập “echo hello” vào shell, `runcmd` sẽ được gọi với “echo hello” làm đối số. `runcmd` (user/sh.c:55) chạy lệnh thực tế. Đối với “echo hello”, nó sẽ gọi `exec` (user/sh.c:79). Nếu `exec` thành công thì con sẽ thực thi các lệnh từ `echo` thay vì `runcmd`. Tại một số điểm, `echo` sẽ gọi `exit`, điều này sẽ khiến cha mẹ trả về từ `wait` trong `main` (user/sh.c:146).

Bạn có thể tự hỏi tại sao `fork` và `exec` không được kết hợp thành một lệnh gọi duy nhất; chúng ta sẽ thấy sau rằng shell khai thác sự tách biệt trong việc triển khai chuyển hướng I/O của nó. Để tránh sự lãng phí của việc tạo một tiến trình trùng lặp và sau đó thay thế nó ngay lập tức (bằng `exec`), các hạt nhân hoạt động tối ưu hóa việc triển khai `fork` cho trường hợp sử dụng này bằng cách sử dụng các kỹ thuật bộ nhớ ảo như sao chép khi ghi (xem Phần 4.6).

Xv6 phân bổ hầu hết bộ nhớ không gian người dùng một cách ngầm định: `fork` phân bổ bộ nhớ cần thiết cho bản sao bộ nhớ của cha mẹ của con, và `exec` phân bổ đủ bộ nhớ để chứa tệp thực thi. Một tiến trình cần thêm bộ nhớ trong thời gian chạy (có lẽ cho `malloc`) có thể gọi `sbrk(n)` để tăng bộ nhớ dữ liệu của nó thêm n byte bằng không; `sbrk` trả về vị trí của bộ nhớ mới.

### 1.2 I/O và Mô tả tệp

Một *mô tả tệp* là một số nguyên nhỏ đại diện cho một đối tượng do hạt nhân quản lý mà một tiến trình có thể đọc hoặc ghi. Một tiến trình có thể có được một mô tả tệp bằng cách mở một tệp, thư mục hoặc thiết bị, hoặc bằng cách tạo một ống dẫn, hoặc bằng cách sao chép một mô tả hiện có. Để đơn giản, chúng ta thường sẽ đề cập đến đối tượng mà một mô tả tệp đề cập đến là một “tệp”; giao diện mô tả tệp trừu tượng hóa sự khác biệt giữa các tệp, ống dẫn và thiết bị, làm cho tất cả chúng trông giống như các luồng byte. Chúng ta sẽ đề cập đến đầu vào và đầu ra là *I/O*.

Bên trong, hạt nhân xv6 sử dụng mô tả tệp làm chỉ mục vào một bảng trên mỗi tiến trình, do đó mỗi tiến trình có một không gian riêng tư của các mô tả tệp bắt đầu từ 0. Theo quy ước, một tiến trình đọc từ mô tả tệp 0 (đầu vào tiêu chuẩn), ghi đầu ra vào mô tả tệp 1 (đầu ra tiêu chuẩn) và ghi thông báo lỗi vào mô tả tệp 2 (lỗi tiêu chuẩn). Như chúng ta sẽ thấy, shell khai thác quy ước để thực hiện chuyển hướng I/O và các đường ống. Shell đảm bảo rằng nó luôn có ba mô tả tệp đang mở (user/sh.c:152), theo mặc định là các mô tả tệp cho bảng điều khiển.

Các lời gọi hệ thống `read` và `write` đọc byte từ và ghi byte vào các tệp đang mở được đặt tên bằng các mô tả tệp. Lệnh gọi `read(fd, buf, n)` đọc nhiều nhất `n` byte từ mô tả tệp `fd`, sao chép chúng vào `buf` và trả về số byte đã đọc. Mỗi mô tả tệp tham chiếu đến một tệp có một *offset* được liên kết với nó. `read` đọc dữ liệu từ offset tệp hiện tại và sau đó tăng offset đó theo số byte đã đọc: một `read` tiếp theo sẽ trả về các byte theo sau các byte được trả về bởi `read` đầu tiên. Khi không còn byte nào để đọc, `read` trả về 0 để chỉ ra cuối tệp.

Lệnh gọi `write(fd, buf, n)` ghi `n` byte từ `buf` vào mô tả tệp `fd` và trả về số byte đã ghi. Ít hơn `n` byte chỉ được ghi khi có lỗi xảy ra. Giống như `read`, `write` ghi dữ liệu tại offset tệp hiện tại và sau đó tăng offset đó theo số byte đã ghi: mỗi `write` sẽ tiếp tục ở nơi `write` trước đó đã dừng lại.

Đoạn chương trình sau (tạo thành bản chất của chương trình `cat`) sao chép dữ liệu từ đầu vào tiêu chuẩn của nó sang đầu ra tiêu chuẩn của nó. Nếu xảy ra lỗi, nó sẽ ghi một thông báo vào lỗi tiêu chuẩn.

```c
char buf[512];
int n;

for(;;){
    n = read(0, buf, sizeof buf);
    if(n == 0)
        break;
    if(n < 0){
        fprintf(2, "read error\n");
        exit(1);
    }
    if(write(1, buf, n) != n){
        fprintf(2, "write error\n");
        exit(1);
    }
}
```

Điều quan trọng cần lưu ý trong đoạn mã là `cat` không biết liệu nó đang đọc từ tệp, bảng điều khiển hay ống dẫn. Tương tự, `cat` không biết liệu nó đang in ra bảng điều khiển, tệp hay bất cứ thứ gì khác. Việc sử dụng các mô tả tệp và quy ước rằng mô tả tệp 0 là đầu vào và mô tả tệp 1 là đầu ra cho phép triển khai `cat` đơn giản.

Lời gọi hệ thống `close` giải phóng một mô tả tệp, làm cho nó miễn phí để sử dụng lại bởi một lời gọi hệ thống `open`, `pipe` hoặc `dup` trong tương lai (xem bên dưới). Một mô tả tệp mới được phân bổ luôn là mô tả chưa sử dụng có số thấp nhất của tiến trình hiện tại.

Các mô tả tệp và `fork` tương tác để giúp chuyển hướng I/O dễ dàng thực hiện. `fork` sao chép bảng mô tả tệp của cha mẹ cùng với bộ nhớ của nó, do đó con bắt đầu với các tệp đang mở giống hệt cha mẹ. Lời gọi hệ thống `exec` thay thế bộ nhớ của tiến trình đang gọi nhưng giữ lại bảng tệp của nó. Hành vi này cho phép shell thực hiện chuyển hướng I/O bằng cách phân nhánh, mở lại các mô tả tệp đã chọn trong con, sau đó gọi `exec` để chạy chương trình mới. Đây là phiên bản đơn giản hóa của mã mà một shell chạy cho lệnh `cat < input.txt`:

```c
char *argv[2];
argv[0] = "cat";
argv[1] = 0;
if(fork() == 0) {
    close(0);
    open("input.txt", O_RDONLY);
    exec("cat", argv);
}
```

Sau khi con đóng mô tả tệp 0, `open` được đảm bảo sử dụng mô tả tệp đó cho `input.txt` mới được mở: 0 sẽ là mô tả tệp có sẵn nhỏ nhất. `cat` sau đó thực thi với mô tả tệp 0 (đầu vào tiêu chuẩn) tham chiếu đến `input.txt`. Các mô tả tệp của tiến trình cha không bị thay đổi bởi chuỗi này, vì nó chỉ sửa đổi các mô tả của con.

Mã cho chuyển hướng I/O trong shell xv6 hoạt động theo cách chính xác này (user/sh.c:83). Nhớ lại rằng tại thời điểm này trong mã, shell đã phân nhánh shell con và `runcmd` sẽ gọi `exec` để tải chương trình mới.

Đối số thứ hai cho `open` bao gồm một tập hợp các cờ, được biểu thị dưới dạng các bit, kiểm soát những gì `open` làm. Các giá trị có thể được định nghĩa trong tiêu đề điều khiển tệp (fcntl) (kernel/fcntl.h:1-5): `O_RDONLY`, `O_WRONLY`, `O_RDWR`, `O_CREATE` và `O_TRUNC`, hướng dẫn `open` mở tệp để đọc, hoặc để ghi, hoặc cho cả đọc và ghi, để tạo tệp nếu nó không tồn tại và để cắt ngắn tệp thành độ dài bằng không.

Bây giờ sẽ rõ ràng tại sao việc `fork` và `exec` là các lệnh gọi riêng biệt lại hữu ích: giữa hai lệnh gọi, shell có cơ hội chuyển hướng I/O của con mà không làm phiền thiết lập I/O của shell chính. Người ta có thể tưởng tượng một lệnh gọi hệ thống `forkexec` kết hợp giả định, nhưng các tùy chọn để thực hiện chuyển hướng I/O với một lệnh gọi như vậy có vẻ khó xử. Shell có thể sửa đổi thiết lập I/O của chính nó trước khi gọi `forkexec` (và sau đó hoàn tác các sửa đổi đó); hoặc `forkexec` có thể nhận các hướng dẫn chuyển hướng I/O làm đối số; hoặc (ít hấp dẫn nhất) mọi chương trình như `cat` có thể được dạy để thực hiện chuyển hướng I/O của riêng mình.

Mặc dù `fork` sao chép bảng mô tả tệp, mỗi offset tệp cơ bản được chia sẻ giữa cha và con. Hãy xem xét ví dụ này:

```c
if(fork() == 0) {
    write(1, "hello ", 6);
    exit(0);
} else {
    wait(0);
    write(1, "world\n", 6);
}
```

Ở cuối đoạn mã này, tệp được đính kèm với mô tả tệp 1 sẽ chứa dữ liệu `hello world`. `write` trong cha (nhờ `wait`, chỉ chạy sau khi con hoàn thành) tiếp tục ở nơi `write` của con đã dừng lại. Hành vi này giúp tạo ra đầu ra tuần tự từ các chuỗi lệnh shell, như `(echo hello; echo world) >output.txt`.

Lời gọi hệ thống `dup` sao chép một mô tả tệp hiện có, trả về một mô tả mới tham chiếu đến cùng một đối tượng I/O cơ bản. Cả hai mô tả tệp đều chia sẻ một offset, giống như các mô tả tệp được sao chép bởi `fork`. Đây là một cách khác để ghi `hello world` vào một tệp:

```c
fd = dup(1);
write(1, "hello ", 6);
write(fd, "world\n", 6);
```

Hai mô tả tệp chia sẻ một offset nếu chúng được bắt nguồn từ cùng một mô tả tệp ban đầu bằng một chuỗi các lệnh gọi `fork` và `dup`. Mặt khác, các mô tả tệp không chia sẻ các offset, ngay cả khi chúng là kết quả của các lệnh gọi `open` cho cùng một tệp. `dup` cho phép các shell thực hiện các lệnh như sau: `ls existing-file non-existing-file > tmp1 2>&1`. `2>&1` yêu cầu shell cung cấp cho lệnh một mô tả tệp 2 là một bản sao của mô tả tệp 1. Cả tên của tệp hiện có và thông báo lỗi cho tệp không tồn tại sẽ xuất hiện trong tệp `tmp1`. Shell xv6 không hỗ trợ chuyển hướng I/O cho mô tả tệp lỗi, nhưng bây giờ bạn đã biết cách triển khai nó.

Các mô tả tệp là một sự trừu tượng hóa mạnh mẽ, bởi vì chúng che giấu các chi tiết về những gì chúng được kết nối với: một tiến trình ghi vào mô tả tệp 1 có thể đang ghi vào một tệp, vào một thiết bị như bảng điều khiển hoặc vào một ống dẫn.

### 1.3 Ống (Pipes)

Một *ống* là một bộ đệm hạt nhân nhỏ được hiển thị cho các tiến trình dưới dạng một cặp mô tả tệp, một để đọc và một để ghi. Việc ghi dữ liệu vào một đầu của ống dẫn làm cho dữ liệu đó có sẵn để đọc từ đầu kia của ống dẫn. Các ống cung cấp một cách để các tiến trình giao tiếp.

Đoạn mã ví dụ sau chạy chương trình `wc` với đầu vào tiêu chuẩn được kết nối với đầu đọc của một ống dẫn.

```c
int p[2];
char *argv[2];
argv[0] = "wc";
argv[1] = 0;
pipe(p);
if(fork() == 0) {
    close(0);
    dup(p[0]);
    close(p[0]);
    close(p[1]);
    exec("/bin/wc", argv);
} else {
    close(p[0]);
    write(p[1], "hello world\n", 12);
    close(p[1]);
}
```

Chương trình gọi `pipe`, tạo một ống dẫn mới và ghi lại các mô tả tệp đọc và ghi trong mảng `p`. Sau `fork`, cả cha và con đều có các mô tả tệp tham chiếu đến ống dẫn. Con gọi `close` và `dup` để làm cho mô tả tệp 0 tham chiếu đến đầu đọc của ống dẫn, đóng các mô tả tệp trong `p` và gọi `exec` để chạy `wc`. Khi `wc` đọc từ đầu vào tiêu chuẩn của nó, nó sẽ đọc từ ống dẫn. Cha đóng đầu đọc của ống dẫn, ghi vào ống dẫn, sau đó đóng đầu ghi.

Nếu không có dữ liệu nào, một `read` trên một ống dẫn sẽ đợi cho đến khi có dữ liệu được ghi hoặc cho đến khi tất cả các mô tả tệp tham chiếu đến đầu ghi bị đóng; trong trường hợp sau, `read` sẽ trả về 0, giống như khi cuối tệp dữ liệu đã được đạt tới. Việc `read` chặn cho đến khi không thể có dữ liệu mới nào đến là một lý do tại sao điều quan trọng là con phải đóng đầu ghi của ống dẫn trước khi thực thi `wc` ở trên: nếu một trong các mô tả tệp của `wc` tham chiếu đến đầu ghi của ống dẫn, `wc` sẽ không bao giờ thấy cuối tệp.

Shell xv6 thực hiện các đường ống như `grep fork sh.c | wc -l` theo cách tương tự như mã trên (user/sh.c:101). Tiến trình con tạo một ống dẫn để kết nối đầu bên trái của đường ống với đầu bên phải. Sau đó, nó gọi `fork` và `runcmd` cho đầu bên trái của đường ống và `fork` và `runcmd` cho đầu bên phải, và đợi cả hai kết thúc. Đầu bên phải của đường ống có thể là một lệnh tự nó bao gồm một ống dẫn (ví dụ: `a | b | c`), tự nó phân nhánh hai tiến trình con mới (một cho `b` và một cho `c`). Do đó, shell có thể tạo một cây các tiến trình. Các lá của cây này là các lệnh và các nút bên trong là các tiến trình đợi cho đến khi các con bên trái và bên phải hoàn thành.

Các ống có vẻ không mạnh hơn các tệp tạm thời: đường ống

`echo hello world | wc`

có thể được thực hiện mà không cần ống dẫn như

`echo hello world >/tmp/xyz; wc </tmp/xyz`

Các ống có ít nhất ba lợi thế so với các tệp tạm thời trong tình huống này. Đầu tiên, các ống tự động dọn dẹp; với chuyển hướng tệp, một shell sẽ phải cẩn thận để xóa `/tmp/xyz` khi hoàn tất. Thứ hai, các ống có thể truyền các luồng dữ liệu dài tùy ý, trong khi chuyển hướng tệp yêu cầu đủ dung lượng trống trên đĩa để lưu trữ tất cả dữ liệu. Thứ ba, các ống cho phép thực thi song song các giai đoạn đường ống, trong khi cách tiếp cận tệp yêu cầu chương trình đầu tiên kết thúc trước khi chương trình thứ hai bắt đầu.

### 1.4 Hệ thống tệp

Hệ thống tệp xv6 cung cấp các tệp dữ liệu, chứa các mảng byte không được giải thích và các thư mục, chứa các tham chiếu được đặt tên đến các tệp dữ liệu và các thư mục khác. Các thư mục tạo thành một cây, bắt đầu từ một thư mục đặc biệt được gọi là *gốc*. Một *đường dẫn* như `/a/b/c` đề cập đến tệp hoặc thư mục có tên `c` bên trong thư mục có tên `b` bên trong thư mục có tên `a` trong thư mục gốc `/`. Các đường dẫn không bắt đầu bằng `/` được đánh giá tương đối so với *thư mục làm việc hiện tại* của tiến trình đang gọi, có thể được thay đổi bằng lệnh gọi hệ thống `chdir`. Cả hai đoạn mã này đều mở cùng một tệp (giả sử tất cả các thư mục liên quan đều tồn tại):

```c
chdir("/a");
chdir("b");
open("c", O_RDONLY);
```

```c
open("/a/b/c", O_RDONLY);
```

Đoạn mã đầu tiên thay đổi thư mục làm việc hiện tại của tiến trình thành `/a/b`; đoạn mã thứ hai không tham chiếu đến cũng không thay đổi thư mục làm việc hiện tại của tiến trình.

Có các lệnh gọi hệ thống để tạo các tệp và thư mục mới: `mkdir` tạo một thư mục mới, `open` với cờ `O_CREATE` tạo một tệp dữ liệu mới và `mknod` tạo một tệp thiết bị mới. Ví dụ này minh họa cả ba:

```c
mkdir("/dir");
fd = open("/dir/file", O_CREATE|O_WRONLY);
close(fd);
mknod("/console", 1, 1);
```

`mknod` tạo một tệp đặc biệt tham chiếu đến một thiết bị. Được liên kết với một tệp thiết bị là các số thiết bị chính và phụ (hai đối số cho `mknod`), xác định duy nhất một thiết bị hạt nhân. Khi một tiến trình sau đó mở một tệp thiết bị, hạt nhân sẽ chuyển hướng các lệnh gọi hệ thống `read` và `write` đến việc triển khai thiết bị hạt nhân thay vì chuyển chúng đến hệ thống tệp.

Tên của một tệp khác với chính tệp đó; cùng một tệp cơ bản, được gọi là *inode*, có thể có nhiều tên, được gọi là *liên kết*. Mỗi liên kết bao gồm một mục trong một thư mục; mục nhập chứa tên tệp và một tham chiếu đến một inode. Một inode chứa siêu dữ liệu về một tệp, bao gồm loại của nó (tệp hoặc thư mục hoặc thiết bị), độ dài của nó, vị trí nội dung của tệp trên đĩa và số lượng liên kết đến một tệp.

Lệnh gọi hệ thống `fstat` truy xuất thông tin từ inode mà một mô tả tệp tham chiếu đến. Nó điền vào một `struct stat`, được định nghĩa trong `stat.h` (kernel/stat.h) như sau:

```c
#define T_DIR 1 // Thư mục
#define T_FILE 2 // Tệp
#define T_DEVICE 3 // Thiết bị

struct stat {
    int dev; // Thiết bị đĩa của hệ thống tệp
    uint ino; // Số inode
    short type; // Loại tệp
    short nlink; // Số lượng liên kết đến tệp
    uint64 size; // Kích thước tệp tính bằng byte
};
```

Lệnh gọi hệ thống `link` tạo một tên hệ thống tệp khác tham chiếu đến cùng một inode với một tệp hiện có. Đoạn mã này tạo một tệp mới có tên cả `a` và `b`.

```c
open("a", O_CREATE|O_WRONLY);
link("a", "b");
```

Đọc từ hoặc ghi vào `a` cũng giống như đọc từ hoặc ghi vào `b`. Mỗi inode được xác định bằng một *số inode* duy nhất. Sau chuỗi mã ở trên, có thể xác định rằng `a` và `b` tham chiếu đến cùng một nội dung cơ bản bằng cách kiểm tra kết quả của `fstat`: cả hai sẽ trả về cùng một số inode (`ino`), và số lượng `nlink` sẽ được đặt thành 2.

Lệnh gọi hệ thống `unlink` xóa một tên khỏi hệ thống tệp. Inode của tệp và không gian đĩa chứa nội dung của nó chỉ được giải phóng khi số lượng liên kết của tệp bằng 0 và không có mô tả tệp nào tham chiếu đến nó. Do đó, việc thêm

`unlink("a");`

vào chuỗi mã cuối cùng sẽ để lại inode và nội dung tệp có thể truy cập dưới dạng `b`. Hơn nữa,

`fd = open("/tmp/xyz", O_CREATE|O_RDWR);`
`unlink("/tmp/xyz");`

là một cách thành ngữ để tạo một inode tạm thời không có tên sẽ được dọn dẹp khi tiến trình đóng `fd` hoặc thoát.

Unix cung cấp các tiện ích tệp có thể gọi từ shell dưới dạng các chương trình cấp người dùng, ví dụ `mkdir`, `ln` và `rm`. Thiết kế này cho phép bất kỳ ai mở rộng giao diện dòng lệnh bằng cách thêm các chương trình cấp người dùng mới. Nhìn lại, kế hoạch này có vẻ hiển nhiên, nhưng các hệ thống khác được thiết kế vào thời của Unix thường xây dựng các lệnh như vậy vào shell (và xây dựng shell vào hạt nhân).

Một ngoại lệ là `cd`, được tích hợp vào shell (user/sh.c:161). `cd` phải thay đổi thư mục làm việc hiện tại của chính shell. Nếu `cd` được chạy như một lệnh thông thường, thì shell sẽ phân nhánh một tiến trình con, tiến trình con sẽ chạy `cd` và `cd` sẽ thay đổi thư mục làm việc của con. Thư mục làm việc của cha (tức là của shell) sẽ không thay đổi.

### 1.5 Thế giới thực

Sự kết hợp của Unix giữa các mô tả tệp “tiêu chuẩn”, các ống dẫn và cú pháp shell thuận tiện cho các hoạt động trên chúng là một bước tiến lớn trong việc viết các chương trình có thể tái sử dụng cho mục đích chung. Ý tưởng này đã châm ngòi cho một nền văn hóa "công cụ phần mềm" chịu trách nhiệm cho phần lớn sức mạnh và sự phổ biến của Unix, và shell là cái gọi là “ngôn ngữ kịch bản” đầu tiên. Giao diện lời gọi hệ thống Unix vẫn tồn tại cho đến ngày nay trong các hệ thống như BSD, Linux và macOS.

Giao diện lời gọi hệ thống Unix đã được tiêu chuẩn hóa thông qua tiêu chuẩn Giao diện hệ điều hành di động (POSIX). Xv6 không tuân thủ POSIX: nó thiếu nhiều lời gọi hệ thống (bao gồm cả những lời gọi cơ bản như `lseek`), và nhiều lời gọi hệ thống mà nó cung cấp khác với tiêu chuẩn. Mục tiêu chính của chúng tôi đối với xv6 là sự đơn giản và rõ ràng trong khi cung cấp một giao diện giống UNIX đơn giản. Một số người đã mở rộng xv6 với một vài lời gọi hệ thống nữa và một thư viện C đơn giản để chạy các chương trình Unix cơ bản. Tuy nhiên, các hạt nhân hiện đại cung cấp nhiều lời gọi hệ thống hơn và nhiều loại dịch vụ hạt nhân hơn xv6. Ví dụ, chúng hỗ trợ mạng, hệ thống cửa sổ, luồng cấp người dùng, trình điều khiển cho nhiều thiết bị, v.v. Các hạt nhân hiện đại phát triển liên tục và nhanh chóng, và cung cấp nhiều tính năng ngoài POSIX.

Unix đã thống nhất quyền truy cập vào nhiều loại tài nguyên (tệp, thư mục và thiết bị) bằng một tập hợp các giao diện tên tệp và mô tả tệp duy nhất. Ý tưởng này có thể được mở rộng cho nhiều loại tài nguyên hơn; một ví dụ điển hình là Plan 9 [16], đã áp dụng khái niệm “tài nguyên là tệp” cho mạng, đồ họa, v.v. Tuy nhiên, hầu hết các hệ điều hành có nguồn gốc từ Unix đã không đi theo con đường này.

Hệ thống tệp và các mô tả tệp đã là những sự trừu tượng hóa mạnh mẽ. Mặc dù vậy, vẫn có những mô hình khác cho các giao diện hệ điều hành. Multics, một tiền thân của Unix, đã trừu tượng hóa việc lưu trữ tệp theo cách làm cho nó trông giống như bộ nhớ, tạo ra một hương vị giao diện rất khác. Sự phức tạp của thiết kế Multics đã có ảnh hưởng trực tiếp đến các nhà thiết kế của Unix, những người đã đặt mục tiêu xây dựng một cái gì đó đơn giản hơn.

Xv6 không cung cấp khái niệm về người dùng hoặc bảo vệ một người dùng khỏi người dùng khác; theo thuật ngữ Unix, tất cả các tiến trình xv6 đều chạy với quyền root.

Cuốn sách này xem xét cách xv6 triển khai giao diện giống Unix của nó, nhưng các ý tưởng và khái niệm áp dụng cho nhiều thứ hơn là chỉ Unix. Bất kỳ hệ điều hành nào cũng phải ghép kênh các tiến trình vào phần cứng cơ bản, cách ly các tiến trình với nhau và cung cấp các cơ chế để giao tiếp giữa các tiến trình có kiểm soát. Sau khi nghiên cứu xv6, bạn sẽ có thể xem xét các hệ điều hành khác, phức tạp hơn và thấy các khái niệm cơ bản của xv6 trong các hệ thống đó.

### 1.6 Bài tập

1.  Viết một chương trình sử dụng các lời gọi hệ thống UNIX để “ping-pong” một byte giữa hai tiến trình qua một cặp ống dẫn, một cho mỗi hướng. Đo lường hiệu suất của chương trình, tính bằng số lần trao đổi mỗi giây.

## 2 Tổ chức hệ điều hành

Một yêu cầu quan trọng đối với một hệ điều hành là hỗ trợ nhiều hoạt động cùng một lúc. Ví dụ, sử dụng giao diện lời gọi hệ thống được mô tả trong Chương 1, một tiến trình có thể bắt đầu các tiến trình mới bằng `fork`. Hệ điều hành phải chia sẻ thời gian các tài nguyên của máy tính giữa các tiến trình này. Ví dụ, ngay cả khi có nhiều tiến trình hơn so với CPU phần cứng, hệ điều hành phải đảm bảo rằng tất cả các tiến trình đều có cơ hội thực thi. Hệ điều hành cũng phải sắp xếp để *cô lập* giữa các tiến trình. Tức là, nếu một tiến trình có lỗi và hoạt động sai, nó không nên ảnh hưởng đến các tiến trình không phụ thuộc vào tiến trình bị lỗi. Tuy nhiên, sự cô lập hoàn toàn là quá mạnh, vì các tiến trình cần có thể tương tác một cách có chủ ý; các đường ống là một ví dụ. Do đó, một hệ điều hành phải đáp ứng ba yêu cầu: ghép kênh, cô lập và tương tác.

Chương này cung cấp một cái nhìn tổng quan về cách các hệ điều hành được tổ chức để đạt được ba yêu cầu này. Hóa ra có nhiều cách để làm như vậy, nhưng văn bản này tập trung vào các thiết kế chính thống tập trung vào một *hạt nhân nguyên khối*, được sử dụng bởi nhiều hệ điều hành Unix. Chương này cũng cung cấp một cái nhìn tổng quan về một tiến trình xv6, là đơn vị cô lập trong xv6, và việc tạo ra tiến trình đầu tiên khi xv6 khởi động.

Xv6 chạy trên một bộ vi xử lý RISC-V đa lõi¹, và phần lớn chức năng cấp thấp của nó (ví dụ, việc triển khai tiến trình của nó) là đặc trưng cho RISC-V. RISC-V là một CPU 64-bit, và xv6 được viết bằng C “LP64”, có nghĩa là `long` (L) và con trỏ (P) trong ngôn ngữ lập trình C là 64 bit, nhưng một `int` là 32 bit. Cuốn sách này giả định người đọc đã lập trình ở cấp độ máy một chút trên một số kiến trúc, và sẽ giới thiệu các ý tưởng đặc trưng của RISC-V khi chúng xuất hiện. Các tài liệu ISA cấp người dùng [2] và kiến trúc đặc quyền [3] là các đặc tả hoàn chỉnh. Bạn cũng có thể tham khảo “The RISC-V Reader: An Open Architecture Atlas" [15].

CPU trong một máy tính hoàn chỉnh được bao quanh bởi phần cứng hỗ trợ, phần lớn trong số đó ở dạng giao diện I/O. Xv6 được viết cho phần cứng hỗ trợ được mô phỏng bởi tùy chọn “-machine virt” của qemu. Điều này bao gồm RAM, một ROM chứa mã khởi động, một kết nối nối tiếp với bàn phím/màn hình của người dùng và một đĩa để lưu trữ.

¹Bằng "đa lõi”, văn bản này có nghĩa là nhiều CPU chia sẻ bộ nhớ nhưng thực thi song song, mỗi CPU có bộ thanh ghi riêng. Văn bản này đôi khi sử dụng thuật ngữ *đa bộ xử lý* như một từ đồng nghĩa với đa lõi, mặc dù đa bộ xử lý cũng có thể đề cập cụ thể hơn đến một máy tính có một số chip xử lý riêng biệt.

### 2.1 Trừu tượng hóa tài nguyên vật lý

Câu hỏi đầu tiên người ta có thể hỏi khi gặp một hệ điều hành là tại sao lại có nó? Tức là, người ta có thể triển khai các lời gọi hệ thống trong Hình 1.2 như một thư viện, mà các ứng dụng liên kết với. Trong kế hoạch này, mỗi ứng dụng thậm chí có thể có thư viện riêng được thiết kế riêng cho nhu cầu của nó. Các ứng dụng có thể tương tác trực tiếp với các tài nguyên phần cứng và sử dụng các tài nguyên đó theo cách tốt nhất cho ứng dụng (ví dụ: để đạt được hiệu suất cao hoặc có thể dự đoán được). Một số hệ điều hành cho các thiết bị nhúng hoặc các hệ thống thời gian thực được tổ chức theo cách này.

Nhược điểm của cách tiếp cận thư viện này là, nếu có nhiều hơn một ứng dụng đang chạy, các ứng dụng phải hoạt động tốt. Ví dụ, mỗi ứng dụng phải định kỳ từ bỏ CPU để các ứng dụng khác có thể chạy. Một lược đồ chia sẻ thời gian hợp tác như vậy có thể ổn nếu tất cả các ứng dụng tin tưởng lẫn nhau và không có lỗi. Điển hình hơn là các ứng dụng không tin tưởng lẫn nhau và có lỗi, vì vậy người ta thường muốn sự cô lập mạnh mẽ hơn so với một lược đồ hợp tác cung cấp.

Để đạt được sự cô lập mạnh mẽ, việc cấm các ứng dụng truy cập trực tiếp vào các tài nguyên phần cứng nhạy cảm là hữu ích, và thay vào đó là trừu tượng hóa các tài nguyên thành các dịch vụ. Ví dụ, các ứng dụng Unix chỉ tương tác với bộ nhớ lưu trữ thông qua các lời gọi hệ thống `open`, `read`, `write` và `close` của hệ thống tệp, thay vì đọc và ghi trực tiếp vào đĩa. Điều này cung cấp cho ứng dụng sự tiện lợi của các tên đường dẫn, và nó cho phép hệ điều hành (với tư cách là người triển khai giao diện) quản lý đĩa. Ngay cả khi sự cô lập không phải là một mối quan tâm, các chương trình tương tác một cách có chủ ý (hoặc chỉ muốn tránh xa nhau) có khả năng sẽ thấy một hệ thống tệp là một sự trừu tượng hóa thuận tiện hơn so với việc sử dụng trực tiếp đĩa.

Tương tự, Unix chuyển đổi một cách minh bạch các CPU phần cứng giữa các tiến trình, lưu và khôi phục trạng thái thanh ghi khi cần thiết, để các ứng dụng không cần phải biết về việc chia sẻ thời gian. Sự minh bạch này cho phép hệ điều hành chia sẻ CPU ngay cả khi một số ứng dụng ở trong các vòng lặp vô hạn.

Là một ví dụ khác, các tiến trình Unix sử dụng `exec` để xây dựng hình ảnh bộ nhớ của chúng, thay vì tương tác trực tiếp với bộ nhớ vật lý. Điều này cho phép hệ điều hành quyết định nơi đặt một tiến trình trong bộ nhớ; nếu bộ nhớ eo hẹp, hệ điều hành thậm chí có thể lưu trữ một số dữ liệu của một tiến trình trên đĩa. `exec` cũng cung cấp cho người dùng sự tiện lợi của một hệ thống tệp để lưu trữ các hình ảnh chương trình có thể thực thi.

Nhiều hình thức tương tác giữa các tiến trình Unix xảy ra thông qua các mô tả tệp. Các mô tả tệp không chỉ trừu tượng hóa nhiều chi tiết (ví dụ: dữ liệu trong một ống dẫn hoặc tệp được lưu trữ ở đâu), chúng còn được định nghĩa theo cách đơn giản hóa sự tương tác. Ví dụ, nếu một ứng dụng trong một đường ống bị lỗi, hạt nhân sẽ tạo ra một tín hiệu cuối tệp cho tiến trình tiếp theo trong đường ống.

Giao diện lời gọi hệ thống trong Hình 1.2 được thiết kế cẩn thận để cung cấp cả sự tiện lợi cho lập trình viên và khả năng cô lập mạnh mẽ. Giao diện Unix không phải là cách duy nhất để trừu tượng hóa các tài nguyên, nhưng nó đã được chứng minh là một cách tốt.

### 2.2 Chế độ người dùng, chế độ giám sát và các lời gọi hệ thống

Sự cô lập mạnh mẽ đòi hỏi một ranh giới cứng giữa các ứng dụng và hệ điều hành. Nếu ứng dụng mắc lỗi, chúng ta không muốn hệ điều hành bị lỗi hoặc các ứng dụng khác bị lỗi. Thay vào đó, hệ điều hành sẽ có thể dọn dẹp ứng dụng bị lỗi và tiếp tục chạy các ứng dụng khác. Để đạt được sự cô lập mạnh mẽ, hệ điều hành phải sắp xếp để các ứng dụng không thể sửa đổi (hoặc thậm chí đọc) các cấu trúc dữ liệu và lệnh của hệ điều hành và các ứng dụng không thể truy cập vào bộ nhớ của các tiến trình khác.

CPU cung cấp hỗ trợ phần cứng cho sự cô lập mạnh mẽ. Ví dụ, RISC-V có ba chế độ mà CPU có thể thực thi các lệnh: *chế độ máy*, *chế độ giám sát* và *chế độ người dùng*. Các lệnh thực thi ở chế độ máy có toàn quyền; một CPU khởi động ở chế độ máy. Chế độ máy chủ yếu dành cho việc thiết lập máy tính trong quá trình khởi động. Xv6 thực thi một vài dòng ở chế độ máy và sau đó chuyển sang chế độ giám sát.

Ở chế độ giám sát, CPU được phép thực thi các *lệnh đặc quyền*: ví dụ, bật và tắt ngắt, đọc và ghi thanh ghi chứa địa chỉ của một bảng trang, v.v. Nếu một ứng dụng ở chế độ người dùng cố gắng thực thi một lệnh đặc quyền, thì CPU không thực thi lệnh đó, mà chuyển sang chế độ giám sát để mã chế độ giám sát có thể chấm dứt ứng dụng, vì nó đã làm điều gì đó mà nó không nên làm. Hình 1.1 trong Chương 1 minh họa tổ chức này. Một ứng dụng chỉ có thể thực thi các lệnh ở chế độ người dùng (ví dụ: cộng số, v.v.) và được cho là đang chạy trong *không gian người dùng*, trong khi phần mềm ở chế độ giám sát cũng có thể thực thi các lệnh đặc quyền và được cho là đang chạy trong *không gian hạt nhân*. Phần mềm chạy trong không gian hạt nhân (hoặc trong chế độ giám sát) được gọi là *hạt nhân*.

Một ứng dụng muốn gọi một hàm hạt nhân (ví dụ: lời gọi hệ thống `read` trong xv6) phải chuyển sang hạt nhân; một ứng dụng không thể gọi trực tiếp một hàm hạt nhân. CPU cung cấp một lệnh đặc biệt chuyển CPU từ chế độ người dùng sang chế độ giám sát và đi vào hạt nhân tại một điểm vào do hạt nhân chỉ định. (RISC-V cung cấp lệnh `ecall` cho mục đích này.) Sau khi CPU đã chuyển sang chế độ giám sát, hạt nhân sau đó có thể xác thực các đối số của lời gọi hệ thống (ví dụ: kiểm tra xem địa chỉ được truyền cho lời gọi hệ thống có phải là một phần của bộ nhớ của ứng dụng hay không), quyết định xem ứng dụng có được phép thực hiện thao tác được yêu cầu hay không (ví dụ: kiểm tra xem ứng dụng có được phép ghi vào tệp được chỉ định hay không), và sau đó từ chối hoặc thực hiện nó. Điều quan trọng là hạt nhân kiểm soát điểm vào cho các chuyển đổi sang chế độ giám sát; nếu ứng dụng có thể quyết định điểm vào hạt nhân, một ứng dụng độc hại có thể, ví dụ, đi vào hạt nhân tại một điểm mà việc xác thực các đối số bị bỏ qua.

### 2.3 Tổ chức hạt nhân

Một câu hỏi thiết kế quan trọng là phần nào của hệ điều hành nên chạy ở chế độ giám sát. Một khả năng là toàn bộ hệ điều hành nằm trong hạt nhân, do đó việc triển khai tất cả các lời gọi hệ thống đều chạy ở chế độ giám sát. Tổ chức này được gọi là *hạt nhân nguyên khối*.

Trong tổ chức này, toàn bộ hệ điều hành bao gồm một chương trình duy nhất chạy với toàn quyền phần cứng. Tổ chức này thuận tiện vì nhà thiết kế HĐH không phải quyết định phần nào của hệ điều hành không cần toàn quyền phần cứng. Hơn nữa, các phần khác nhau của hệ điều hành hợp tác dễ dàng hơn. Ví dụ, một hệ điều hành có thể có một bộ đệm cache có thể được chia sẻ bởi cả hệ thống tệp và hệ thống bộ nhớ ảo.

Một nhược điểm của tổ chức nguyên khối là các tương tác giữa các phần khác nhau của hệ điều hành thường phức tạp (như chúng ta sẽ thấy trong phần còn lại của văn bản này), và do đó, một nhà phát triển hệ điều hành dễ mắc lỗi. Trong một hạt nhân nguyên khối, một sai lầm là chí mạng, bởi vì một lỗi ở chế độ giám sát thường sẽ khiến hạt nhân bị lỗi. Nếu hạt nhân bị lỗi, máy tính sẽ ngừng hoạt động, và do đó tất cả các ứng dụng cũng bị lỗi. Máy tính phải khởi động lại để bắt đầu lại.

Để giảm nguy cơ mắc lỗi trong hạt nhân, các nhà thiết kế HĐH có thể giảm thiểu lượng mã hệ điều hành chạy ở chế độ giám sát và thực thi phần lớn hệ điều hành ở chế độ người dùng. Tổ chức hạt nhân này được gọi là *vi hạt nhân*.

Hình 2.1 minh họa thiết kế vi hạt nhân này. Trong hình, hệ thống tệp chạy như một tiến trình cấp người dùng. Các dịch vụ HĐH chạy dưới dạng các tiến trình được gọi là *máy chủ*. Để cho phép các ứng dụng tương tác với máy chủ tệp, hạt nhân cung cấp một cơ chế giao tiếp giữa các tiến trình để gửi tin nhắn từ một tiến trình chế độ người dùng này sang một tiến trình khác. Ví dụ, nếu một ứng dụng như shell muốn đọc hoặc ghi một tệp, nó sẽ gửi một tin nhắn đến máy chủ tệp và đợi phản hồi.

Trong một vi hạt nhân, giao diện hạt nhân bao gồm một vài hàm cấp thấp để bắt đầu các ứng dụng, gửi tin nhắn, truy cập phần cứng thiết bị, v.v. Tổ chức này cho phép hạt nhân tương đối đơn giản, vì hầu hết hệ điều hành nằm trong các máy chủ cấp người dùng.

Trong thế giới thực, cả hạt nhân nguyên khối và vi hạt nhân đều phổ biến. Ví dụ, Linux có một hạt nhân nguyên khối, mặc dù một số chức năng HĐH chạy dưới dạng các máy chủ cấp người dùng (ví dụ: hệ thống cửa sổ). Linux mang lại hiệu suất cao cho các ứng dụng chuyên sâu về HĐH, một phần vì các hệ thống con của hạt nhân có thể được tích hợp chặt chẽ.

Các hệ điều hành như Minix, L4 và QNX được tổ chức như một vi hạt nhân với các máy chủ và đã được triển khai rộng rãi trong các môi trường nhúng. Một biến thể của L4, seL4, đủ nhỏ để nó đã được xác minh về an toàn bộ nhớ và các thuộc tính bảo mật khác [8].

Có nhiều tranh luận giữa các nhà phát triển hệ điều hành về tổ chức nào tốt hơn, và không có bằng chứng kết luận nào cho một cách hay cách khác. Hơn nữa, nó phụ thuộc nhiều vào ý nghĩa của "tốt hơn": hiệu suất nhanh hơn, kích thước mã nhỏ hơn, độ tin cậy của hạt nhân, độ tin cậy của toàn bộ hệ điều hành (bao gồm cả các dịch vụ cấp người dùng), v.v.

Cũng có những cân nhắc thực tế có thể quan trọng hơn câu hỏi về tổ chức. Một số hệ điều hành có một vi hạt nhân nhưng chạy một số dịch vụ cấp người dùng trong không gian hạt nhân vì lý do hiệu suất. Một số hệ điều hành có hạt nhân nguyên khối vì đó là cách chúng bắt đầu và có ít động lực để chuyển sang một tổ chức vi hạt nhân thuần túy, bởi vì các tính năng mới có thể quan trọng hơn việc viết lại hệ điều hành hiện có để phù hợp với một thiết kế vi hạt nhân.

Từ quan điểm của cuốn sách này, các hệ điều hành vi hạt nhân và nguyên khối chia sẻ nhiều ý tưởng chính. Chúng thực hiện các lời gọi hệ thống, chúng sử dụng các bảng trang, chúng xử lý các ngắt, chúng hỗ trợ các tiến trình, chúng sử dụng các khóa để kiểm soát đồng thời, chúng thực hiện một hệ thống tệp, v.v. Cuốn sách này tập trung vào các ý tưởng cốt lõi này.

Xv6 được triển khai như một hạt nhân nguyên khối, giống như hầu hết các hệ điều hành Unix. Do đó, giao diện hạt nhân xv6 tương ứng với giao diện hệ điều hành, và hạt nhân thực hiện toàn bộ hệ điều hành. Vì xv6 không cung cấp nhiều dịch vụ, hạt nhân của nó nhỏ hơn một số vi hạt nhân, nhưng về mặt khái niệm, xv6 là nguyên khối.

### 2.4 Mã: tổ chức xv6

Mã nguồn hạt nhân xv6 nằm trong thư mục con `kernel/`. Mã nguồn được chia thành các tệp, theo một khái niệm sơ bộ về tính mô-đun; Hình 2.2 liệt kê các tệp. Các giao diện giữa các mô-đun được định nghĩa trong `defs.h` (kernel/defs.h).

### 2.5 Tổng quan về tiến trình

Đơn vị cô lập trong xv6 (cũng như trong các hệ điều hành Unix khác) là một *tiến trình*. Sự trừu tượng hóa tiến trình ngăn một tiến trình phá hoại hoặc do thám bộ nhớ, CPU, mô tả tệp, v.v. của một tiến trình khác. Nó cũng ngăn một tiến trình phá hoại chính hạt nhân, để một tiến trình không thể phá vỡ các cơ chế cô lập của hạt nhân. Hạt nhân phải thực hiện sự trừu tượng hóa tiến trình một cách cẩn thận vì một ứng dụng bị lỗi hoặc độc hại có thể lừa hạt nhân hoặc phần cứng làm điều gì đó tồi tệ (ví dụ: phá vỡ sự cô lập). Các cơ chế được hạt nhân sử dụng để thực hiện các tiến trình bao gồm cờ chế độ người dùng/giám sát, không gian địa chỉ và chia sẻ thời gian của các luồng.

Để giúp thực thi sự cô lập, sự trừu tượng hóa tiến trình cung cấp cho một chương trình ảo tưởng rằng nó có máy riêng của mình. Một tiến trình cung cấp cho một chương trình những gì dường như là một hệ thống bộ nhớ riêng, hoặc *không gian địa chỉ*, mà các tiến trình khác không thể đọc hoặc ghi. Một tiến trình cũng cung cấp cho chương trình những gì dường như là CPU riêng của nó để thực thi các lệnh của chương trình.

Xv6 sử dụng các bảng trang (được thực hiện bởi phần cứng) để cung cấp cho mỗi tiến trình không gian địa chỉ riêng của nó. Bảng trang RISC-V dịch (hoặc “ánh xạ”) một *địa chỉ ảo* (địa chỉ mà một lệnh RISC-V thao tác) thành một *địa chỉ vật lý* (một địa chỉ mà CPU gửi đến bộ nhớ chính).

Xv6 duy trì một bảng trang riêng cho mỗi tiến trình xác định không gian địa chỉ của tiến trình đó. Như được minh họa trong Hình 2.3, một không gian địa chỉ bao gồm bộ nhớ người dùng của tiến trình bắt đầu từ địa chỉ ảo 0. Các lệnh đến trước, theo sau là các biến toàn cục, sau đó là ngăn xếp, và cuối cùng là một vùng "heap" (cho `malloc`) mà tiến trình có thể mở rộng khi cần thiết. Có một số yếu tố giới hạn kích thước tối đa của không gian địa chỉ của một tiến trình: các con trỏ trên RISC-V rộng 64 bit; phần cứng chỉ sử dụng 39 bit thấp khi tra cứu các địa chỉ ảo trong các bảng trang; và xv6 chỉ sử dụng 38 trong số 39 bit đó. Do đó, địa chỉ tối đa là 2³⁸ - 1 = 0x3fffffffff, là `MAXVA` (kernel/riscv.h:378). Ở đầu không gian địa chỉ, xv6 đặt một trang trampoline (4096 byte) và một trang trapframe. Xv6 sử dụng hai trang này để chuyển vào hạt nhân và quay lại; trang trampoline chứa mã để chuyển vào và ra khỏi hạt nhân, và trapframe là nơi hạt nhân lưu các thanh ghi người dùng của tiến trình, như Chương 4 giải thích.

Hạt nhân xv6 duy trì nhiều phần trạng thái cho mỗi tiến trình, mà nó tập hợp vào một `struct proc` (kernel/proc.h:85). Các phần trạng thái hạt nhân quan trọng nhất của một tiến trình là bảng trang của nó, ngăn xếp hạt nhân của nó và trạng thái chạy của nó. Chúng ta sẽ sử dụng ký hiệu `p->xxx` để chỉ các phần tử của cấu trúc `proc`; ví dụ, `p->pagetable` là một con trỏ đến bảng trang của tiến trình.

Mỗi tiến trình có một *luồng điều khiển* (hoặc *luồng* cho ngắn gọn) chứa trạng thái cần thiết để thực thi tiến trình. Tại bất kỳ thời điểm nào, một luồng có thể đang thực thi trên một CPU, hoặc bị tạm dừng (không thực thi, nhưng có khả năng tiếp tục thực thi trong tương lai). Để chuyển một CPU giữa các tiến trình, hạt nhân tạm dừng luồng hiện đang chạy trên CPU đó và lưu trạng thái của nó, và khôi phục trạng thái của một luồng đã bị tạm dừng trước đó của một tiến trình khác. Phần lớn trạng thái của một luồng (các biến cục bộ, các địa chỉ trả về của lệnh gọi hàm) được lưu trữ trên các ngăn xếp của luồng. Mỗi tiến trình có hai ngăn xếp: một ngăn xếp người dùng và một ngăn xếp hạt nhân (`p->kstack`). Khi tiến trình đang thực thi các lệnh người dùng, chỉ có ngăn xếp người dùng của nó được sử dụng, và ngăn xếp hạt nhân của nó trống. Khi tiến trình đi vào hạt nhân (cho một lời gọi hệ thống hoặc ngắt), mã hạt nhân thực thi trên ngăn xếp hạt nhân của tiến trình; trong khi một tiến trình ở trong hạt nhân, ngăn xếp người dùng của nó vẫn chứa dữ liệu đã lưu, nhưng không được sử dụng tích cực. Một luồng của một tiến trình xen kẽ giữa việc sử dụng tích cực ngăn xếp người dùng và ngăn xếp hạt nhân của nó. Ngăn xếp hạt nhân là riêng biệt (và được bảo vệ khỏi mã người dùng) để hạt nhân có thể thực thi ngay cả khi một tiến trình đã phá hỏng ngăn xếp người dùng của nó.

Một tiến trình có thể thực hiện một lời gọi hệ thống bằng cách thực thi lệnh `ecall` của RISC-V. Lệnh này nâng mức đặc quyền của phần cứng và thay đổi bộ đếm chương trình thành một điểm vào do hạt nhân xác định. Mã tại điểm vào chuyển sang ngăn xếp hạt nhân của tiến trình và thực thi các lệnh hạt nhân thực hiện lời gọi hệ thống. Khi lời gọi hệ thống hoàn tất, hạt nhân chuyển trở lại ngăn xếp người dùng và quay trở lại không gian người dùng bằng cách gọi lệnh `sret`, lệnh này hạ thấp mức đặc quyền của phần cứng và tiếp tục thực thi các lệnh người dùng ngay sau lệnh gọi hệ thống. Một luồng của một tiến trình có thể “chặn” trong hạt nhân để chờ I/O, và tiếp tục ở nơi nó đã dừng lại khi I/O đã kết thúc.

`p->state` cho biết liệu tiến trình được phân bổ, sẵn sàng chạy, hiện đang chạy trên một CPU, đang chờ I/O hay đang thoát.

`p->pagetable` chứa bảng trang của tiến trình, ở định dạng mà phần cứng RISC-V mong đợi. Xv6 làm cho phần cứng phân trang sử dụng `p->pagetable` của một tiến trình khi thực thi tiến trình đó trong không gian người dùng. Bảng trang của một tiến trình cũng đóng vai trò là bản ghi các địa chỉ của các trang vật lý được phân bổ để lưu trữ bộ nhớ của tiến trình.

Tóm lại, một tiến trình kết hợp hai ý tưởng thiết kế: một không gian địa chỉ để cung cấp cho một tiến trình ảo tưởng về bộ nhớ riêng của nó, và một luồng để cung cấp cho một tiến trình ảo tưởng về CPU riêng của nó. Trong xv6, một tiến trình bao gồm một không gian địa chỉ và một luồng. Trong các hệ điều hành thực, một tiến trình có thể có nhiều hơn một luồng để tận dụng nhiều CPU.

### 2.6 Mã: khởi động xv6, tiến trình đầu tiên và lời gọi hệ thống

Để làm cho xv6 cụ thể hơn, chúng ta sẽ phác thảo cách hạt nhân khởi động và chạy tiến trình đầu tiên. Các chương tiếp theo sẽ mô tả các cơ chế xuất hiện trong tổng quan này chi tiết hơn.

Khi một máy tính RISC-V bật nguồn, nó tự khởi tạo và chạy một bộ nạp khởi động được lưu trữ trong bộ nhớ chỉ đọc. Bộ nạp khởi động tải hạt nhân xv6 vào bộ nhớ. Sau đó, ở chế độ máy, CPU thực thi xv6 bắt đầu từ `_entry` (kernel/entry.S:7). RISC-V bắt đầu với phần cứng phân trang bị vô hiệu hóa: các địa chỉ ảo ánh xạ trực tiếp đến các địa chỉ vật lý.

Bộ nạp tải hạt nhân xv6 vào bộ nhớ tại địa chỉ vật lý 0x80000000. Lý do nó đặt hạt nhân ở 0x80000000 thay vì 0x0 là vì phạm vi địa chỉ 0x0:0x80000000 chứa các thiết bị I/O.

Các lệnh tại `_entry` thiết lập một ngăn xếp để xv6 có thể chạy mã C. Xv6 khai báo không gian cho một ngăn xếp ban đầu, `stack0`, trong tệp `start.c` (kernel/start.c:11). Mã tại `_entry` tải thanh ghi con trỏ ngăn xếp `sp` với địa chỉ `stack0+4096`, đỉnh của ngăn xếp, bởi vì ngăn xếp trên RISC-V phát triển xuống. Bây giờ hạt nhân có một ngăn xếp, `_entry` gọi vào mã C tại `start` (kernel/start.c:15).

Hàm `start` thực hiện một số cấu hình chỉ được phép ở chế độ máy, và sau đó chuyển sang chế độ giám sát. Để vào chế độ giám sát, RISC-V cung cấp lệnh `mret`. Lệnh này thường được sử dụng để trả về từ một lệnh gọi trước đó từ chế độ giám sát sang chế độ máy. `start` không trả về từ một lệnh gọi như vậy, nhưng thiết lập mọi thứ như thể nó đã làm: nó đặt chế độ đặc quyền trước đó thành giám sát trong thanh ghi `mstatus`, nó đặt địa chỉ trả về thành `main` bằng cách ghi địa chỉ của `main` vào thanh ghi `mepc`, vô hiệu hóa dịch địa chỉ ảo ở chế độ giám sát bằng cách ghi 0 vào thanh ghi bảng trang `satp`, và ủy quyền tất cả các ngắt và ngoại lệ cho chế độ giám sát.

Trước khi chuyển sang chế độ giám sát, `start` thực hiện thêm một nhiệm vụ: nó lập trình cho chip đồng hồ để tạo ra các ngắt hẹn giờ. Với việc dọn dẹp nhà cửa này đã xong, `start` “trả về” chế độ giám sát bằng cách gọi `mret`. Điều này làm cho bộ đếm chương trình thay đổi thành `main` (kernel/main.c:11), địa chỉ được lưu trữ trước đó trong `mepc`.

Sau khi `main` (kernel/main.c:11) khởi tạo một số thiết bị và hệ thống con, nó tạo ra tiến trình đầu tiên bằng cách gọi `userinit` (kernel/proc.c:233). Tiến trình đầu tiên thực thi một chương trình nhỏ được viết bằng hợp ngữ RISC-V, thực hiện lời gọi hệ thống đầu tiên trong xv6. `initcode.S` (user/initcode.S:3) tải số cho lời gọi hệ thống `exec`, `SYS_EXEC` (kernel/syscall.h:8), vào thanh ghi `a7`, và sau đó gọi `ecall` để vào lại hạt nhân.

Hạt nhân sử dụng số trong thanh ghi `a7` trong `syscall` (kernel/syscall.c:132) để gọi lời gọi hệ thống mong muốn. Bảng lời gọi hệ thống (kernel/syscall.c:107) ánh xạ `SYS_EXEC` đến hàm `sys_exec`, mà hạt nhân gọi. Như chúng ta đã thấy trong Chương 1, `exec` thay thế bộ nhớ và các thanh ghi của tiến trình hiện tại bằng một chương trình mới (trong trường hợp này là `/init`).

Sau khi hạt nhân đã hoàn thành `exec`, nó quay trở lại không gian người dùng trong tiến trình `/init`. `init` (user/init.c:15) tạo một tệp thiết bị giao diện điều khiển mới nếu cần và sau đó mở nó dưới dạng các mô tả tệp 0, 1 và 2. Sau đó, nó bắt đầu một shell trên bảng điều khiển. Hệ thống đã hoạt động.

### 2.7 Mô hình bảo mật

Bạn có thể tự hỏi hệ điều hành đối phó với mã bị lỗi hoặc độc hại như thế nào. Bởi vì đối phó với sự độc hại khó hơn nghiêm ngặt so với đối phó với các lỗi vô tình, việc tập trung chủ yếu vào việc cung cấp bảo mật chống lại sự độc hại là hợp lý. Đây là một cái nhìn cấp cao về các giả định và mục tiêu bảo mật điển hình trong thiết kế hệ điều hành.

Hệ điều hành phải giả định rằng mã cấp người dùng của một tiến trình sẽ cố gắng hết sức để phá hoại hạt nhân hoặc các tiến trình khác. Mã người dùng có thể cố gắng tham chiếu đến các con trỏ bên ngoài không gian địa chỉ được phép của nó; nó có thể cố gắng thực thi bất kỳ lệnh RISC-V nào, ngay cả những lệnh không dành cho mã người dùng; nó có thể cố gắng đọc và ghi bất kỳ thanh ghi điều khiển RISC-V nào; nó có thể cố gắng truy cập trực tiếp vào phần cứng thiết bị; và nó có thể truyền các giá trị thông minh cho các lời gọi hệ thống trong một nỗ lực lừa hạt nhân gặp sự cố hoặc làm điều gì đó ngu ngốc. Mục tiêu của hạt nhân là hạn chế mỗi tiến trình người dùng để tất cả những gì nó có thể làm là đọc/ghi/thực thi bộ nhớ người dùng của chính nó, sử dụng 32 thanh ghi RISC-V mục đích chung và ảnh hưởng đến hạt nhân và các tiến trình khác theo những cách mà các lời gọi hệ thống dự định cho phép. Hạt nhân phải ngăn chặn bất kỳ hành động nào khác. Đây thường là một yêu cầu tuyệt đối trong thiết kế hạt nhân.

Các kỳ vọng đối với mã của chính hạt nhân là khá khác nhau. Mã hạt nhân được cho là được viết bởi các lập trình viên có thiện chí và cẩn thận. Mã hạt nhân được cho là không có lỗi, và chắc chắn không chứa bất cứ điều gì độc hại. Giả định này ảnh hưởng đến cách chúng ta phân tích mã hạt nhân. Ví dụ, có nhiều hàm hạt nhân nội bộ (ví dụ: các khóa quay) sẽ gây ra các vấn đề nghiêm trọng nếu mã hạt nhân sử dụng chúng không chính xác. Khi kiểm tra bất kỳ đoạn mã hạt nhân cụ thể nào, chúng ta sẽ muốn tự thuyết phục rằng nó hoạt động chính xác. Tuy nhiên, chúng ta giả định rằng mã hạt nhân nói chung được viết chính xác và tuân theo tất cả các quy tắc về việc sử dụng các hàm và cấu trúc dữ liệu của chính hạt nhân. Ở cấp độ phần cứng, CPU RISC-V, RAM, đĩa, v.v. được cho là hoạt động như được quảng cáo trong tài liệu, không có lỗi phần cứng.

Tất nhiên trong cuộc sống thực, mọi thứ không đơn giản như vậy. Rất khó để ngăn chặn mã người dùng thông minh làm cho một hệ thống không thể sử dụng được (hoặc khiến nó hoảng loạn) bằng cách tiêu thụ các tài nguyên được bảo vệ bởi hạt nhân — không gian đĩa, thời gian CPU, các khe cắm bảng tiến trình, v.v. Thường không thể viết mã hạt nhân không có lỗi hoặc thiết kế phần cứng không có lỗi; nếu những người viết mã người dùng độc hại biết về các lỗi hạt nhân hoặc phần cứng, họ sẽ khai thác chúng. Ngay cả trong các hạt nhân trưởng thành, được sử dụng rộng rãi, chẳng hạn như Linux, mọi người liên tục phát hiện ra các lỗ hổng mới [1]. Việc thiết kế các biện pháp bảo vệ vào hạt nhân chống lại khả năng nó có lỗi là đáng giá: các xác nhận, kiểm tra kiểu, các trang bảo vệ ngăn xếp, v.v. Cuối cùng, sự phân biệt giữa mã người dùng và mã hạt nhân đôi khi bị xóa nhòa: một số tiến trình cấp người dùng đặc quyền có thể cung cấp các dịch vụ thiết yếu và thực sự là một phần của hệ điều hành, và trong một số hệ điều hành, mã người dùng đặc quyền có thể chèn mã mới vào hạt nhân (như với các mô-đun có thể tải của Linux).

### 2.8 Thế giới thực

Hầu hết các hệ điều hành đã áp dụng khái niệm tiến trình, và hầu hết các tiến trình trông tương tự như của xv6. Tuy nhiên, các hệ điều hành hiện đại hỗ trợ một số luồng trong một tiến trình, để cho phép một tiến trình duy nhất khai thác nhiều CPU. Hỗ trợ nhiều luồng trong một tiến trình liên quan đến khá nhiều máy móc mà xv6 không có, thường bao gồm các thay đổi giao diện (ví dụ: `clone` của Linux, một biến thể của `fork`), để kiểm soát các khía cạnh nào của một tiến trình mà các luồng chia sẻ.

### 2.9 Bài tập

1.  Thêm một lời gọi hệ thống vào xv6 trả về lượng bộ nhớ trống có sẵn.

## 3 Bảng trang

Bảng trang là cơ chế phổ biến nhất qua đó hệ điều hành cung cấp cho mỗi tiến trình không gian địa chỉ và bộ nhớ riêng của nó. Bảng trang xác định các địa chỉ bộ nhớ có ý nghĩa gì và các phần nào của bộ nhớ vật lý có thể được truy cập. Chúng cho phép xv6 cô lập các không gian địa chỉ của các tiến trình khác nhau và ghép kênh chúng vào một bộ nhớ vật lý duy nhất. Bảng trang là một thiết kế phổ biến vì chúng cung cấp một mức độ gián tiếp cho phép các hệ điều hành thực hiện nhiều thủ thuật. Xv6 thực hiện một vài thủ thuật: ánh xạ cùng một bộ nhớ (một trang trampoline) trong một số không gian địa chỉ, và bảo vệ các ngăn xếp hạt nhân và người dùng bằng một trang không được ánh xạ. Phần còn lại của chương này giải thích các bảng trang mà phần cứng RISC-V cung cấp và cách xv6 sử dụng chúng.

### 3.1 Phần cứng phân trang

Để nhắc lại, các lệnh RISC-V (cả người dùng và hạt nhân) thao tác các địa chỉ ảo. RAM của máy, hoặc bộ nhớ vật lý, được lập chỉ mục bằng các địa chỉ vật lý. Phần cứng bảng trang RISC-V kết nối hai loại địa chỉ này, bằng cách ánh xạ mỗi địa chỉ ảo thành một địa chỉ vật lý.

Xv6 chạy trên Sv39 RISC-V, có nghĩa là chỉ có 39 bit dưới cùng của một địa chỉ ảo 64-bit được sử dụng; 25 bit trên cùng không được sử dụng. Trong cấu hình Sv39 này, một bảng trang RISC-V về mặt logic là một mảng gồm 2²⁷ (134.217.728) *mục nhập bảng trang* (PTE). Mỗi PTE chứa một *số trang vật lý* (PPN) 44-bit và một số cờ. Phần cứng phân trang dịch một địa chỉ ảo bằng cách sử dụng 27 bit trên cùng của 39 bit để lập chỉ mục vào bảng trang để tìm một PTE, và tạo một địa chỉ vật lý 56-bit có 44 bit trên cùng đến từ PPN trong PTE và 12 bit dưới cùng được sao chép từ địa chỉ ảo ban đầu. Hình 3.1 cho thấy quá trình này với một cái nhìn logic về bảng trang như một mảng PTE đơn giản (xem Hình 3.2 để biết một câu chuyện đầy đủ hơn). Một bảng trang cung cấp cho hệ điều hành quyền kiểm soát các bản dịch địa chỉ ảo-vật lý ở mức độ chi tiết của các khối được căn chỉnh 4096 (2¹²) byte. Một khối như vậy được gọi là một *trang*.

Trong Sv39 RISC-V, 25 bit trên cùng của một địa chỉ ảo không được sử dụng để dịch. Địa chỉ vật lý cũng có chỗ để phát triển: có chỗ trong định dạng PTE để số trang vật lý tăng thêm 10 bit nữa. Các nhà thiết kế của RISC-V đã chọn những con số này dựa trên các dự đoán công nghệ. 2³⁹ byte là 512 GB, đủ không gian địa chỉ cho các ứng dụng chạy trên các máy tính RISC-V. 2⁵⁶ là đủ không gian bộ nhớ vật lý cho tương lai gần để phù hợp với nhiều thiết bị I/O và chip RAM. Nếu cần nhiều hơn, các nhà thiết kế RISC-V đã định nghĩa Sv48 với các địa chỉ ảo 48-bit [3].

Như Hình 3.2 cho thấy, một CPU RISC-V dịch một địa chỉ ảo thành một địa chỉ vật lý trong ba bước. Một bảng trang được lưu trữ trong bộ nhớ vật lý dưới dạng một cây ba cấp. Gốc của cây là một trang bảng trang 4096 byte chứa 512 PTE, chứa các địa chỉ vật lý cho các trang bảng trang ở cấp tiếp theo của cây. Mỗi trang đó chứa 512 PTE cho cấp cuối cùng trong cây. Phần cứng phân trang sử dụng 9 bit trên cùng của 27 bit để chọn một PTE trong trang bảng trang gốc, 9 bit giữa để chọn một PTE trong một trang bảng trang ở cấp tiếp theo của cây, và 9 bit dưới cùng để chọn PTE cuối cùng. (Trong Sv48 RISC-V, một bảng trang có bốn cấp, và các bit từ 39 đến 47 của một địa chỉ ảo lập chỉ mục vào cấp trên cùng.)

Nếu bất kỳ trong ba PTE nào cần thiết để dịch một địa chỉ không có mặt, phần cứng phân trang sẽ gây ra một *ngoại lệ lỗi trang*, để lại cho hạt nhân xử lý ngoại lệ (xem Chương 4).

Cấu trúc ba cấp của Hình 3.2 cho phép một cách hiệu quả về bộ nhớ để ghi lại các PTE, so với thiết kế một cấp của Hình 3.1. Trong trường hợp phổ biến trong đó các phạm vi lớn của các địa chỉ ảo không có ánh xạ, cấu trúc ba cấp có thể bỏ qua toàn bộ các thư mục trang. Ví dụ, nếu một ứng dụng chỉ sử dụng một vài trang bắt đầu từ địa chỉ 0, thì các mục từ 1 đến 511 của thư mục trang cấp cao nhất là không hợp lệ, và hạt nhân không phải phân bổ các trang đó cho 511 thư mục trang trung gian. Hơn nữa, hạt nhân cũng không phải phân bổ các trang cho các thư mục trang cấp thấp nhất cho 511 thư mục trang trung gian đó. Vì vậy, trong ví dụ này, thiết kế ba cấp tiết kiệm 511 trang cho các thư mục trang trung gian và 511 × 512 trang cho các thư mục trang cấp thấp nhất.

Mặc dù một CPU đi qua cấu trúc ba cấp trong phần cứng như một phần của việc thực thi một lệnh tải hoặc lưu trữ, một nhược điểm tiềm tàng của ba cấp là CPU phải tải ba PTE từ bộ nhớ để thực hiện việc dịch địa chỉ ảo trong lệnh tải/lưu trữ thành một địa chỉ vật lý. Để tránh chi phí tải PTE từ bộ nhớ vật lý, một CPU RISC-V lưu trữ các mục nhập bảng trang trong một *Bộ đệm tra cứu dịch địa chỉ* (TLB).

Mỗi PTE chứa các bit cờ cho phần cứng phân trang biết cách cho phép sử dụng địa chỉ ảo được liên kết. `PTE_V` cho biết liệu PTE có mặt hay không: nếu nó không được đặt, một tham chiếu đến trang sẽ gây ra một ngoại lệ (tức là, không được phép). `PTE_R` kiểm soát xem các lệnh có được phép đọc trang hay không. `PTE_W` kiểm soát xem các lệnh có được phép ghi vào trang hay không. `PTE_X` kiểm soát xem CPU có thể diễn giải nội dung của trang như các lệnh và thực thi chúng hay không. `PTE_U` kiểm soát xem các lệnh ở chế độ người dùng có được phép truy cập trang hay không; nếu `PTE_U` không được đặt, PTE chỉ có thể được sử dụng ở chế độ giám sát. Hình 3.2 cho thấy tất cả hoạt động như thế nào. Các cờ và tất cả các cấu trúc liên quan đến phần cứng trang khác được định nghĩa trong (kernel/riscv.h)

Để yêu cầu một CPU sử dụng một bảng trang, hạt nhân phải ghi địa chỉ vật lý của trang bảng trang gốc vào thanh ghi `satp`. Một CPU sẽ dịch tất cả các địa chỉ được tạo bởi các lệnh tiếp theo bằng cách sử dụng bảng trang được trỏ đến bởi `satp` của chính nó. Mỗi CPU có `satp` riêng để các CPU khác nhau có thể chạy các tiến trình khác nhau, mỗi tiến trình có một không gian địa chỉ riêng được mô tả bởi bảng trang riêng của nó.

Từ quan điểm của hạt nhân, một bảng trang là dữ liệu được lưu trữ trong bộ nhớ, và hạt nhân tạo và sửa đổi các bảng trang bằng cách sử dụng mã giống như bạn có thể thấy cho bất kỳ cấu trúc dữ liệu hình cây nào.

Một vài lưu ý về các thuật ngữ được sử dụng trong cuốn sách này. *Bộ nhớ vật lý* đề cập đến các ô lưu trữ trong RAM. Một byte của bộ nhớ vật lý có một địa chỉ, được gọi là một *địa chỉ vật lý*. Các lệnh tham chiếu đến các địa chỉ (chẳng hạn như tải, lưu trữ, nhảy và gọi hàm) chỉ sử dụng các *địa chỉ ảo*, mà phần cứng phân trang dịch thành các địa chỉ vật lý, và sau đó gửi đến phần cứng RAM để đọc hoặc ghi bộ nhớ. Một *không gian địa chỉ* là tập hợp các địa chỉ ảo hợp lệ trong một bảng trang nhất định; mỗi tiến trình xv6 có một không gian địa chỉ người dùng riêng, và hạt nhân xv6 có không gian địa chỉ riêng của nó. *Bộ nhớ người dùng* đề cập đến không gian địa chỉ người dùng của một tiến trình cộng với bộ nhớ vật lý mà bảng trang cho phép tiến trình truy cập. *Bộ nhớ ảo* đề cập đến các ý tưởng và kỹ thuật liên quan đến việc quản lý các bảng trang và sử dụng chúng để đạt được các mục tiêu như sự cô lập.

### 3.2 Không gian địa chỉ hạt nhân

Xv6 duy trì một bảng trang cho mỗi tiến trình, mô tả không gian địa chỉ người dùng của mỗi tiến trình, cộng với một bảng trang duy nhất mô tả không gian địa chỉ của hạt nhân. Hạt nhân cấu hình bố cục của không gian địa chỉ của nó để cung cấp cho chính nó quyền truy cập vào bộ nhớ vật lý và các tài nguyên phần cứng khác nhau tại các địa chỉ có thể dự đoán được. Hình 3.3 cho thấy bố cục này ánh xạ các địa chỉ ảo của hạt nhân đến các địa chỉ vật lý. Tệp (kernel/memlayout.h) khai báo các hằng số cho bố cục bộ nhớ hạt nhân của xv6.

QEMU mô phỏng một máy tính bao gồm RAM (bộ nhớ vật lý) bắt đầu từ địa chỉ vật lý 0x80000000 và tiếp tục qua ít nhất 0x88000000, mà xv6 gọi là `PHYSTOP`. Mô phỏng QEMU cũng bao gồm các thiết bị I/O như giao diện đĩa. QEMU hiển thị các giao diện thiết bị cho phần mềm dưới dạng các thanh ghi điều khiển được ánh xạ bộ nhớ nằm dưới 0x80000000 trong không gian địa chỉ vật lý. Hạt nhân có thể tương tác với các thiết bị bằng cách đọc/ghi các địa chỉ vật lý đặc biệt này; các lần đọc và ghi như vậy giao tiếp với phần cứng thiết bị thay vì với RAM. Chương 4 giải thích cách xv6 tương tác với các thiết bị.

Hạt nhân truy cập vào RAM và các thanh ghi thiết bị được ánh xạ bộ nhớ bằng cách sử dụng “ánh xạ trực tiếp”; nghĩa là, ánh xạ các tài nguyên tại các địa chỉ ảo bằng với địa chỉ vật lý. Ví dụ, chính hạt nhân được đặt tại `KERNBASE`=0x80000000 trong cả không gian địa chỉ ảo và trong bộ nhớ vật lý. Ánh xạ trực tiếp đơn giản hóa mã hạt nhân đọc hoặc ghi bộ nhớ vật lý. Ví dụ, khi `fork` phân bổ bộ nhớ người dùng cho tiến trình con, bộ cấp phát trả về địa chỉ vật lý của bộ nhớ đó; `fork` sử dụng địa chỉ đó trực tiếp như một địa chỉ ảo khi nó sao chép bộ nhớ người dùng của cha mẹ vào con.

Có một vài địa chỉ ảo của hạt nhân không được ánh xạ trực tiếp:

*   Trang trampoline. Nó được ánh xạ ở đầu không gian địa chỉ ảo; các bảng trang người dùng có cùng ánh xạ này. Chương 4 thảo luận về vai trò của trang trampoline, nhưng ở đây chúng ta thấy một trường hợp sử dụng thú vị của các bảng trang; một trang vật lý (chứa mã trampoline) được ánh xạ hai lần trong không gian địa chỉ ảo của hạt nhân: một lần ở đầu không gian địa chỉ ảo và một lần với một ánh xạ trực tiếp.

*   Các trang ngăn xếp hạt nhân. Mỗi tiến trình có ngăn xếp hạt nhân riêng của nó, được ánh xạ cao để bên dưới nó xv6 có thể để lại một *trang bảo vệ* không được ánh xạ. PTE của trang bảo vệ là không hợp lệ (tức là, `PTE_V` không được đặt), do đó nếu hạt nhân tràn ngăn xếp hạt nhân, nó có khả năng sẽ gây ra một ngoại lệ và hạt nhân sẽ hoảng loạn. Nếu không có trang bảo vệ, một ngăn xếp tràn sẽ ghi đè lên bộ nhớ hạt nhân khác, dẫn đến hoạt động không chính xác. Một sự cố hoảng loạn là thích hợp hơn.

Trong khi hạt nhân sử dụng các ngăn xếp của nó thông qua các ánh xạ bộ nhớ cao, chúng cũng có thể truy cập được đối với hạt nhân thông qua một địa chỉ được ánh xạ trực tiếp. Một thiết kế thay thế có thể chỉ có ánh xạ trực tiếp, và sử dụng các ngăn xếp tại địa chỉ được ánh xạ trực tiếp. Tuy nhiên, trong sự sắp xếp đó, việc cung cấp các trang bảo vệ sẽ liên quan đến việc bỏ ánh xạ các địa chỉ ảo mà nếu không sẽ tham chiếu đến bộ nhớ vật lý, điều này sau đó sẽ khó sử dụng.

Hạt nhân ánh xạ các trang cho trampoline và văn bản hạt nhân với các quyền `PTE_R` và `PTE_X`. Hạt nhân đọc và thực thi các lệnh từ các trang này. Hạt nhân ánh xạ các trang khác với các quyền `PTE_R` và `PTE_W`, để nó có thể đọc và ghi bộ nhớ trong các trang đó. Các ánh xạ cho các trang bảo vệ là không hợp lệ.

### 3.3 Mã: tạo một không gian địa chỉ

Hầu hết mã xv6 để thao tác các không gian địa chỉ và các bảng trang nằm trong `vm.c` (kernel/vm.c:1). Cấu trúc dữ liệu trung tâm là `pagetable_t`, thực sự là một con trỏ đến một trang bảng trang gốc RISC-V; một `pagetable_t` có thể là bảng trang hạt nhân, hoặc một trong các bảng trang trên mỗi tiến trình. Các hàm trung tâm là `walk`, tìm PTE cho một địa chỉ ảo, và `mappages`, cài đặt các PTE cho các ánh xạ mới. Các hàm bắt đầu bằng `kvm` thao tác bảng trang hạt nhân; các hàm bắt đầu bằng `uvm` thao tác một bảng trang người dùng; các hàm khác được sử dụng cho cả hai. `copyout` và `copyin` sao chép dữ liệu đến và từ các địa chỉ ảo người dùng được cung cấp dưới dạng các đối số lời gọi hệ thống; chúng ở trong `vm.c` vì chúng cần dịch rõ ràng các địa chỉ đó để tìm bộ nhớ vật lý tương ứng.

Đầu trong chuỗi khởi động, `main` gọi `kvminit` (kernel/vm.c:54) để tạo bảng trang của hạt nhân bằng cách sử dụng `kvmmake` (kernel/vm.c:20). Lệnh gọi này xảy ra trước khi xv6 đã bật phân trang trên RISC-V, vì vậy các địa chỉ tham chiếu trực tiếp đến bộ nhớ vật lý. `kvmmake` đầu tiên phân bổ một trang của bộ nhớ vật lý để chứa trang bảng trang gốc. Sau đó, nó gọi `kvmmap` để cài đặt các bản dịch mà hạt nhân cần. Các bản dịch bao gồm các lệnh và dữ liệu của hạt nhân, bộ nhớ vật lý lên đến `PHYSTOP`, và các phạm vi bộ nhớ thực sự là các thiết bị. `proc_mapstacks` (kernel/proc.c:33) phân bổ một ngăn xếp hạt nhân cho mỗi tiến trình. Nó gọi `kvmmap` để ánh xạ mỗi ngăn xếp tại địa chỉ ảo được tạo bởi `KSTACK`, để lại chỗ cho các trang bảo vệ ngăn xếp không hợp lệ.

`kvmmap` (kernel/vm.c:132) gọi `mappages` (kernel/vm.c:144), cài đặt các ánh xạ vào một bảng trang cho một phạm vi các địa chỉ ảo đến một phạm vi tương ứng của các địa chỉ vật lý. Nó thực hiện điều này riêng biệt cho mỗi địa chỉ ảo trong phạm vi, ở các khoảng trang. Đối với mỗi địa chỉ ảo được ánh xạ, `mappages` gọi `walk` để tìm địa chỉ của PTE cho địa chỉ đó. Sau đó, nó khởi tạo PTE để chứa số trang vật lý có liên quan, các quyền mong muốn (`PTE_W`, `PTE_X`, và/hoặc `PTE_R`), và `PTE_V` để đánh dấu PTE là hợp lệ (kernel/vm.c:165).

`walk` (kernel/vm.c:86) bắt chước phần cứng phân trang RISC-V khi nó tra cứu PTE cho một địa chỉ ảo (xem Hình 3.2). `walk` đi xuống bảng trang một cấp tại một thời điểm, sử dụng 9 bit của địa chỉ ảo của mỗi cấp để lập chỉ mục vào trang thư mục trang có liên quan. Ở mỗi cấp, nó tìm thấy PTE của trang thư mục trang của cấp tiếp theo, hoặc PTE của trang cuối cùng (kernel/vm.c:92). Nếu một PTE trong một trang thư mục trang cấp một hoặc hai không hợp lệ, thì trang thư mục được yêu cầu chưa được phân bổ; nếu đối số `alloc` được đặt, `walk` sẽ phân bổ một trang bảng trang mới và đặt địa chỉ vật lý của nó vào PTE. Nó trả về địa chỉ của PTE ở lớp thấp nhất trong cây (kernel/vm.c:102).

Mã trên phụ thuộc vào bộ nhớ vật lý được ánh xạ trực tiếp vào không gian địa chỉ ảo của hạt nhân. Ví dụ, khi `walk` đi xuống các cấp của bảng trang, nó lấy địa chỉ (vật lý) của trang bảng trang cấp dưới tiếp theo từ một PTE (kernel/vm.c:94), và sau đó sử dụng địa chỉ đó như một địa chỉ ảo để lấy PTE ở cấp dưới tiếp theo (kernel/vm.c:92).

`main` gọi `kvminithart` (kernel/vm.c:62) để cài đặt bảng trang hạt nhân. Nó ghi địa chỉ vật lý của trang bảng trang gốc vào thanh ghi `satp`. Sau đó, CPU sẽ dịch các địa chỉ bằng cách sử dụng bảng trang hạt nhân. Vì hạt nhân sử dụng một ánh xạ trực tiếp, địa chỉ ảo hiện tại của lệnh tiếp theo sẽ ánh xạ đến đúng địa chỉ bộ nhớ vật lý.

Mỗi CPU RISC-V lưu trữ các mục nhập bảng trang trong một *Bộ đệm tra cứu dịch địa chỉ* (TLB), và khi xv6 thay đổi một bảng trang, nó phải yêu cầu CPU vô hiệu hóa các mục nhập TLB được lưu trong bộ nhớ cache tương ứng. Nếu không, thì tại một thời điểm nào đó sau đó, TLB có thể sử dụng một ánh xạ được lưu trong bộ nhớ cache cũ, trỏ đến một trang vật lý trong thời gian đó đã được phân bổ cho một tiến trình khác, và kết quả là, một tiến trình có thể ghi nguệch ngoạc lên bộ nhớ của một tiến trình khác. RISC-V có một lệnh `sfence.vma` xóa TLB của CPU hiện tại. Xv6 thực thi `sfence.vma` trong `kvminithart` sau khi tải lại thanh ghi `satp`, và trong mã trampoline chuyển sang một bảng trang người dùng trước khi quay trở lại không gian người dùng (kernel/trampoline.S:89).

Cũng cần phải phát hành `sfence.vma` trước khi thay đổi `satp`, để đợi hoàn thành tất cả các lần tải và lưu trữ đang chờ xử lý. Việc chờ đợi này đảm bảo rằng các cập nhật trước đó cho bảng trang đã hoàn tất, và đảm bảo rằng các lần tải và lưu trữ trước đó sử dụng bảng trang cũ, chứ không phải bảng trang mới.

Để tránh xóa toàn bộ TLB, các CPU RISC-V có thể hỗ trợ các *định danh không gian địa chỉ* (ASID) [3]. Hạt nhân sau đó có thể xóa chỉ các mục nhập TLB cho một không gian địa chỉ cụ thể. Xv6 không sử dụng tính năng này.

### 3.4 Cấp phát bộ nhớ vật lý

Hạt nhân phải phân bổ và giải phóng bộ nhớ vật lý trong thời gian chạy cho các bảng trang, bộ nhớ người dùng, ngăn xếp hạt nhân và bộ đệm ống dẫn.

Xv6 sử dụng bộ nhớ vật lý giữa cuối hạt nhân và `PHYSTOP` để phân bổ trong thời gian chạy. Nó phân bổ và giải phóng toàn bộ các trang 4096 byte tại một thời điểm. Nó theo dõi các trang nào miễn phí bằng cách luồn một danh sách liên kết qua chính các trang đó. Việc phân bổ bao gồm việc xóa một trang khỏi danh sách liên kết; việc giải phóng bao gồm việc thêm trang đã giải phóng vào danh sách.

### 3.5 Mã: Bộ cấp phát bộ nhớ vật lý

Bộ cấp phát nằm trong `kalloc.c` (kernel/kalloc.c:1). Cấu trúc dữ liệu của bộ cấp phát là một *danh sách miễn phí* các trang bộ nhớ vật lý có sẵn để phân bổ. Mỗi phần tử danh sách của trang miễn phí là một `struct run` (kernel/kalloc.c:17). Bộ cấp phát lấy bộ nhớ để chứa cấu trúc dữ liệu đó ở đâu? Nó lưu trữ cấu trúc `run` của mỗi trang miễn phí trong chính trang miễn phí đó, vì không có gì khác được lưu trữ ở đó. Danh sách miễn phí được bảo vệ bởi một khóa quay (kernel/kalloc.c:21-24). Danh sách và khóa được gói trong một `struct` để làm rõ rằng khóa bảo vệ các trường trong `struct`. Hiện tại, hãy bỏ qua khóa và các lệnh gọi đến `acquire` và `release`; Chương 6 sẽ xem xét chi tiết về khóa.

Hàm `main` gọi `kinit` để khởi tạo bộ cấp phát (kernel/kalloc.c:27). `kinit` khởi tạo danh sách miễn phí để chứa mọi trang giữa cuối hạt nhân và `PHYSTOP`. Xv6 nên xác định có bao nhiêu bộ nhớ vật lý có sẵn bằng cách phân tích thông tin cấu hình do phần cứng cung cấp. Thay vào đó, xv6 giả định rằng máy có 128 megabyte RAM. `kinit` gọi `freerange` để thêm bộ nhớ vào danh sách miễn phí thông qua các lệnh gọi trên mỗi trang đến `kfree`. Một PTE chỉ có thể tham chiếu đến một địa chỉ vật lý được căn chỉnh trên một ranh giới 4096 byte (là một bội số của 4096), vì vậy `freerange` sử dụng `PGROUNDUP` để đảm bảo rằng nó chỉ giải phóng các địa chỉ vật lý được căn chỉnh. Bộ cấp phát bắt đầu không có bộ nhớ; các lệnh gọi đến `kfree` này cung cấp cho nó một số để quản lý.

Bộ cấp phát đôi khi coi các địa chỉ là các số nguyên để thực hiện các phép toán số học trên chúng (ví dụ: duyệt qua tất cả các trang trong `freerange`), và đôi khi sử dụng các địa chỉ làm con trỏ để đọc và ghi bộ nhớ (ví dụ: thao tác cấu trúc `run` được lưu trữ trong mỗi trang); việc sử dụng kép các địa chỉ này là lý do chính khiến mã bộ cấp phát đầy các ép kiểu C.

Hàm `kfree` (kernel/kalloc.c:47) bắt đầu bằng cách đặt mọi byte trong bộ nhớ đang được giải phóng thành giá trị 1. Điều này sẽ khiến mã sử dụng bộ nhớ sau khi giải phóng nó (sử dụng “các tham chiếu lơ lửng”) đọc rác thay vì nội dung hợp lệ cũ; hy vọng rằng điều đó sẽ khiến mã như vậy bị hỏng nhanh hơn. Sau đó, `kfree` đặt trước trang vào danh sách miễn phí: nó ép kiểu `pa` thành một con trỏ đến `struct run`, ghi lại phần bắt đầu cũ của danh sách miễn phí trong `r->next`, và đặt danh sách miễn phí bằng `r`. `kalloc` xóa và trả về phần tử đầu tiên trong danh sách miễn phí.

### 3.6 Không gian địa chỉ tiến trình

Mỗi tiến trình có bảng trang riêng của nó, và khi xv6 chuyển đổi giữa các tiến trình, nó cũng thay đổi các bảng trang. Hình 3.4 cho thấy không gian địa chỉ của một tiến trình chi tiết hơn Hình 2.3. Bộ nhớ người dùng của một tiến trình bắt đầu từ địa chỉ ảo 0 và có thể tăng lên đến `MAXVA` (kernel/riscv.h:375), cho phép một tiến trình về nguyên tắc có thể địa chỉ 256 Gigabyte bộ nhớ.

Không gian địa chỉ của một tiến trình bao gồm các trang chứa văn bản của chương trình (mà xv6 ánh xạ với các quyền `PTE_R`, `PTE_X` và `PTE_U`), các trang chứa dữ liệu được khởi tạo trước của chương trình, một trang cho ngăn xếp và các trang cho heap. Xv6 ánh xạ dữ liệu, ngăn xếp và heap với các quyền `PTE_R`, `PTE_W` và `PTE_U`.

Sử dụng các quyền trong một không gian địa chỉ người dùng là một kỹ thuật phổ biến để làm cứng một tiến trình người dùng. Nếu văn bản được ánh xạ với `PTE_W`, thì một tiến trình có thể vô tình sửa đổi chương trình của chính nó; ví dụ, một lỗi lập trình có thể khiến chương trình ghi vào một con trỏ null, sửa đổi các lệnh tại địa chỉ 0, và sau đó tiếp tục chạy, có lẽ tạo ra nhiều sự tàn phá hơn. Để phát hiện các lỗi như vậy ngay lập tức, xv6 ánh xạ văn bản mà không có `PTE_W`; nếu một chương trình vô tình cố gắng lưu trữ vào địa chỉ 0, phần cứng sẽ từ chối thực hiện việc lưu trữ và gây ra một lỗi trang (xem Phần 4.6). Hạt nhân sau đó sẽ giết tiến trình và in ra một thông báo thông tin để nhà phát triển có thể theo dõi vấn đề.

Tương tự, bằng cách ánh xạ dữ liệu mà không có `PTE_X`, một chương trình người dùng không thể vô tình nhảy đến một địa chỉ trong dữ liệu của chương trình và bắt đầu thực thi tại đó.

Trong thế giới thực, việc làm cứng một tiến trình bằng cách đặt các quyền một cách cẩn thận cũng hỗ trợ trong việc phòng thủ chống lại các cuộc tấn công bảo mật. Một kẻ tấn công có thể cung cấp đầu vào được xây dựng cẩn thận cho một chương trình (ví dụ: một máy chủ Web) gây ra một lỗi trong chương trình với hy vọng biến lỗi đó thành một khai thác [14]. Việc đặt các quyền một cách cẩn thận và các kỹ thuật khác, chẳng hạn như ngẫu nhiên hóa bố cục của không gian địa chỉ người dùng, làm cho các cuộc tấn công như vậy khó khăn hơn.

Ngăn xếp là một trang duy nhất, và được hiển thị với nội dung ban đầu được tạo bởi `exec`. Các chuỗi chứa các đối số dòng lệnh, cũng như một mảng các con trỏ đến chúng, nằm ở ngay đầu ngăn xếp. Ngay bên dưới đó là các giá trị cho phép một chương trình bắt đầu tại `main` như thể hàm `main(argc, argv)` vừa được gọi.

Để phát hiện một ngăn xếp người dùng tràn bộ nhớ ngăn xếp được phân bổ, xv6 đặt một trang bảo vệ không thể truy cập ngay bên dưới ngăn xếp bằng cách xóa cờ `PTE_U`. Nếu ngăn xếp người dùng tràn và tiến trình cố gắng sử dụng một địa chỉ bên dưới ngăn xếp, phần cứng sẽ tạo ra một ngoại lệ lỗi trang vì trang bảo vệ không thể truy cập được đối với một chương trình chạy ở chế độ người dùng. Một hệ điều hành trong thế giới thực có thể thay vào đó tự động phân bổ thêm bộ nhớ cho ngăn xếp người dùng khi nó tràn.

Khi một tiến trình yêu cầu xv6 thêm bộ nhớ người dùng, xv6 sẽ phát triển heap của tiến trình. Xv6 đầu tiên sử dụng `kalloc` để phân bổ các trang vật lý. Sau đó, nó thêm các PTE vào bảng trang của tiến trình trỏ đến các trang vật lý mới. Xv6 đặt các cờ `PTE_W`, `PTE_R`, `PTE_U` và `PTE_V` trong các PTE này. Hầu hết các tiến trình không sử dụng toàn bộ không gian địa chỉ người dùng; xv6 để `PTE_V` trống trong các PTE không sử dụng.

Ở đây chúng ta thấy một vài ví dụ hay về việc sử dụng các bảng trang. Đầu tiên, các bảng trang của các tiến trình khác nhau dịch các địa chỉ người dùng thành các trang khác nhau của bộ nhớ vật lý, do đó mỗi tiến trình có bộ nhớ người dùng riêng. Thứ hai, mỗi tiến trình thấy bộ nhớ của nó có các địa chỉ ảo liền kề bắt đầu từ 0, trong khi bộ nhớ vật lý của tiến trình có thể không liền kề. Thứ ba, hạt nhân ánh xạ một trang với mã trampoline ở đầu không gian địa chỉ người dùng (không có `PTE_U`), do đó một trang duy nhất của bộ nhớ vật lý xuất hiện trong tất cả các không gian địa chỉ, nhưng chỉ có thể được sử dụng bởi hạt nhân.

### 3.7 Mã: sbrk

`sbrk` là lời gọi hệ thống để một tiến trình thu nhỏ hoặc phát triển bộ nhớ của nó. Lời gọi hệ thống được triển khai bởi hàm `growproc` (kernel/proc.c:260). `growproc` gọi `uvmalloc` hoặc `uvmdealloc`, tùy thuộc vào việc `n` là dương hay âm. `uvmalloc` (kernel/vm.c:233) phân bổ bộ nhớ vật lý bằng `kalloc`, không hóa bộ nhớ được phân bổ, và thêm các PTE vào bảng trang người dùng bằng `mappages`. `uvmdealloc` gọi `uvmunmap` (kernel/vm.c:178), sử dụng `walk` để tìm các PTE và `kfree` để giải phóng bộ nhớ vật lý mà chúng tham chiếu đến.

Xv6 sử dụng bảng trang của một tiến trình không chỉ để cho phần cứng biết cách ánh xạ các địa chỉ ảo người dùng, mà còn là bản ghi duy nhất về các trang bộ nhớ vật lý nào được phân bổ cho tiến trình đó. Đó là lý do tại sao việc giải phóng bộ nhớ người dùng (trong `uvmunmap`) yêu cầu kiểm tra bảng trang người dùng.

### 3.8 Mã: exec

`exec` là một lời gọi hệ thống thay thế không gian địa chỉ người dùng của một tiến trình bằng dữ liệu được đọc từ một tệp, được gọi là một tệp nhị phân hoặc tệp thực thi. Một tệp nhị phân thường là đầu ra của trình biên dịch và trình liên kết, và chứa các lệnh máy và dữ liệu chương trình. `exec` (kernel/exec.c:23) mở đường dẫn nhị phân được đặt tên bằng `namei` (kernel/exec.c:36), được giải thích trong Chương 8. Sau đó, nó đọc tiêu đề ELF. Các tệp nhị phân Xv6 được định dạng theo định dạng ELF được sử dụng rộng rãi, được định nghĩa trong (kernel/elf.h). Một tệp nhị phân ELF bao gồm một tiêu đề ELF, `struct elfhdr` (kernel/elf.h:6), theo sau là một chuỗi các tiêu đề phần chương trình, `struct proghdr` (kernel/elf.h:25). Mỗi `proghdr` mô tả một phần của ứng dụng phải được tải vào bộ nhớ; các chương trình xv6 có hai tiêu đề phần chương trình: một cho các lệnh và một cho dữ liệu.

Bước đầu tiên là một kiểm tra nhanh rằng tệp có thể chứa một tệp nhị phân ELF. Một tệp nhị phân ELF bắt đầu bằng “số ma thuật” bốn byte 0x7F, `E`, `L`, `F`, hoặc `ELF_MAGIC` (kernel/elf.h:3). Nếu tiêu đề ELF có số ma thuật đúng, `exec` giả định rằng tệp nhị phân được định dạng tốt.

`exec` phân bổ một bảng trang mới không có ánh xạ người dùng với `proc_pagetable` (kernel/exec.c:49), phân bổ bộ nhớ cho mỗi phân đoạn ELF với `uvmalloc` (kernel/exec.c:65), và tải mỗi phân đoạn vào bộ nhớ với `loadseg` (kernel/exec.c:10). `loadseg` sử dụng `walkaddr` để tìm địa chỉ vật lý của bộ nhớ được phân bổ tại đó để ghi mỗi trang của phân đoạn ELF, và `readi` để đọc từ tệp.

Tiêu đề phần chương trình cho `/init`, chương trình người dùng đầu tiên được tạo bằng `exec`, trông như thế này:

```
# objdump -p user/_init

user/_init:     file format elf64-little

Program Header:
    0x70000003 off    0x0000000000006bb0 vaddr 0x0000000000000000
         paddr 0x0000000000000000 align 2**0
         filesz 0x000000000000004a memsz 0x0000000000000000 flags r--
    LOAD off    0x0000000000001000 vaddr 0x0000000000000000
         paddr 0x0000000000000000 align 2**12
         filesz 0x0000000000001000 memsz 0x0000000000001000 flags r-x
    LOAD off    0x0000000000002000 vaddr 0x0000000000001000
         paddr 0x0000000000001000 align 2**12
         filesz 0x0000000000000010 memsz 0x0000000000000030 flags rw-
   STACK off    0x0000000000000000 vaddr 0x0000000000000000
         paddr 0x0000000000000000 align 2**4
         filesz 0x0000000000000000 memsz 0x0000000000000000 flags rw-
```

Chúng ta thấy rằng phân đoạn văn bản nên được tải tại địa chỉ ảo 0x0 trong bộ nhớ (không có quyền ghi) từ nội dung tại offset 0x1000 trong tệp. Chúng ta cũng thấy rằng dữ liệu nên được tải tại địa chỉ 0x1000, là một ranh giới trang, và không có quyền thực thi.

`filesz` của một tiêu đề phần chương trình có thể nhỏ hơn `memsz`, cho biết rằng khoảng cách giữa chúng nên được điền bằng các số không (cho các biến toàn cục C) thay vì đọc từ tệp. Đối với `/init`, `filesz` dữ liệu là 0x10 byte và `memsz` là 0x30 byte, và do đó `uvmalloc` phân bổ đủ bộ nhớ vật lý để chứa 0x30 byte, nhưng chỉ đọc 0x10 byte từ tệp `/init`.

Bây giờ `exec` phân bổ và khởi tạo ngăn xếp người dùng. Nó chỉ phân bổ một trang ngăn xếp. `exec` sao chép các chuỗi đối số lên đầu ngăn xếp một lần, ghi lại các con trỏ đến chúng trong `ustack`. Nó đặt một con trỏ null ở cuối danh sách `argv` sẽ được truyền cho `main`. Các giá trị cho `argc` và `argv` được truyền cho `main` thông qua đường dẫn trả về của lời gọi hệ thống: `argc` được truyền qua giá trị trả về của lời gọi hệ thống, đi vào `a0`, và `argv` được truyền qua mục `a1` của trapframe của tiến trình.

`exec` đặt một trang không thể truy cập ngay bên dưới trang ngăn xếp, để các chương trình cố gắng sử dụng nhiều hơn một trang sẽ bị lỗi. Trang không thể truy cập này cũng cho phép `exec` đối phó với các đối số quá lớn; trong tình huống đó, hàm `copyout` (kernel/vm.c:359) mà `exec` sử dụng để sao chép các đối số vào ngăn xếp sẽ nhận thấy rằng trang đích không thể truy cập được, và sẽ trả về -1.

Trong quá trình chuẩn bị hình ảnh bộ nhớ mới, nếu `exec` phát hiện một lỗi như một phân đoạn chương trình không hợp lệ, nó sẽ nhảy đến nhãn `bad`, giải phóng hình ảnh mới, và trả về -1. `exec` phải đợi để giải phóng hình ảnh cũ cho đến khi nó chắc chắn rằng lời gọi hệ thống sẽ thành công: nếu hình ảnh cũ đã biến mất, lời gọi hệ thống không thể trả về -1 cho nó. Các trường hợp lỗi duy nhất trong `exec` xảy ra trong quá trình tạo hình ảnh. Sau khi hình ảnh hoàn tất, `exec` có thể cam kết với bảng trang mới (kernel/exec.c:125) và giải phóng bảng trang cũ (kernel/exec.c:129).

`exec` tải các byte từ tệp ELF vào bộ nhớ tại các địa chỉ được chỉ định bởi tệp ELF. Người dùng hoặc các tiến trình có thể đặt bất kỳ địa chỉ nào họ muốn vào một tệp ELF. Do đó, `exec` là rủi ro, bởi vì các địa chỉ trong tệp ELF có thể tham chiếu đến hạt nhân, một cách vô tình hoặc có chủ ý. Hậu quả đối với một hạt nhân không cẩn thận có thể từ một sự cố đến một sự lật đổ độc hại của các cơ chế cô lập của hạt nhân (tức là, một khai thác bảo mật). Xv6 thực hiện một số kiểm tra để tránh những rủi ro này. Ví dụ, nếu `(ph.vaddr + ph.memsz < ph.vaddr)` kiểm tra xem tổng có tràn một số nguyên 64-bit hay không. Nguy hiểm là một người dùng có thể xây dựng một tệp nhị phân ELF với một `ph.vaddr` trỏ đến một địa chỉ do người dùng chọn, và `ph.memsz` đủ lớn để tổng tràn đến 0x1000, sẽ trông giống như một giá trị hợp lệ. Trong một phiên bản cũ hơn của xv6 trong đó không gian địa chỉ người dùng cũng chứa hạt nhân (nhưng không thể đọc/ghi ở chế độ người dùng), người dùng có thể chọn một địa chỉ tương ứng với bộ nhớ hạt nhân và do đó sao chép dữ liệu từ tệp nhị phân ELF vào hạt nhân. Trong phiên bản RISC-V của xv6, điều này không thể xảy ra, bởi vì hạt nhân có bảng trang riêng của nó; `loadseg` tải vào bảng trang của tiến trình, không phải trong bảng trang của hạt nhân.

Một nhà phát triển hạt nhân dễ dàng bỏ qua một kiểm tra quan trọng, và các hạt nhân trong thế giới thực có một lịch sử dài về việc thiếu các kiểm tra mà sự vắng mặt của chúng có thể bị khai thác bởi các chương trình người dùng để có được các đặc quyền hạt nhân. Có khả năng xv6 không thực hiện một công việc hoàn chỉnh trong việc xác thực dữ liệu cấp người dùng được cung cấp cho hạt nhân, mà một chương trình người dùng độc hại có thể khai thác để phá vỡ sự cô lập của xv6.

### 3.9 Thế giới thực

Giống như hầu hết các hệ điều hành, xv6 sử dụng phần cứng phân trang để bảo vệ và ánh xạ bộ nhớ. Hầu hết các hệ điều hành sử dụng phân trang một cách tinh vi hơn nhiều so với xv6 bằng cách kết hợp phân trang và các ngoại lệ lỗi trang, mà chúng ta sẽ thảo luận trong Chương 4.

Xv6 được đơn giản hóa bởi việc hạt nhân sử dụng một ánh xạ trực tiếp giữa các địa chỉ ảo và vật lý, và bởi giả định của nó rằng có RAM vật lý tại địa chỉ 0x80000000, nơi hạt nhân mong đợi được tải. Điều này hoạt động với QEMU, nhưng trên phần cứng thực, hóa ra đó là một ý tưởng tồi; phần cứng thực đặt RAM và các thiết bị tại các địa chỉ vật lý không thể đoán trước, do đó (ví dụ) có thể không có RAM tại 0x80000000, nơi xv6 mong đợi có thể lưu trữ hạt nhân. Các thiết kế hạt nhân nghiêm túc hơn khai thác bảng trang để biến các bố cục bộ nhớ vật lý phần cứng tùy ý thành các bố cục địa chỉ ảo hạt nhân có thể dự đoán được.

RISC-V hỗ trợ bảo vệ ở cấp độ các địa chỉ vật lý, nhưng xv6 không sử dụng tính năng đó.

Trên các máy có nhiều bộ nhớ, việc sử dụng hỗ trợ của RISC-V cho “siêu trang” có thể có ý nghĩa. Các trang nhỏ có ý nghĩa khi bộ nhớ vật lý nhỏ, để cho phép phân bổ và phân trang ra đĩa với độ chi tiết tốt. Ví dụ, nếu một chương trình chỉ sử dụng 8 kilobyte bộ nhớ, việc cung cấp cho nó một siêu trang 4 megabyte bộ nhớ vật lý là lãng phí. Các trang lớn hơn có ý nghĩa trên các máy có nhiều RAM, và có thể giảm chi phí cho việc thao tác bảng trang.

Việc hạt nhân xv6 thiếu một bộ cấp phát giống như `malloc` có thể cung cấp bộ nhớ cho các đối tượng nhỏ ngăn cản hạt nhân sử dụng các cấu trúc dữ liệu tinh vi sẽ yêu cầu phân bổ động. Một hạt nhân phức tạp hơn có khả năng sẽ phân bổ nhiều kích thước khác nhau của các khối nhỏ, thay vì (như trong xv6) chỉ các khối 4096 byte; một bộ cấp phát hạt nhân thực sự sẽ cần phải xử lý các phân bổ nhỏ cũng như các phân bổ lớn.

Phân bổ bộ nhớ là một chủ đề nóng lâu năm, các vấn đề cơ bản là sử dụng hiệu quả bộ nhớ hạn chế và chuẩn bị cho các yêu cầu không xác định trong tương lai [9]. Ngày nay, mọi người quan tâm nhiều hơn đến tốc độ hơn là hiệu quả không gian.

### 3.10 Bài tập

1.  Phân tích cây thiết bị của RISC-V để tìm lượng bộ nhớ vật lý mà máy tính có.

2.  Viết một chương trình người dùng phát triển không gian địa chỉ của nó thêm một byte bằng cách gọi `sbrk(1)`. Chạy chương trình và điều tra bảng trang cho chương trình trước lệnh gọi đến `sbrk` và sau lệnh gọi đến `sbrk`. Hạt nhân đã phân bổ bao nhiêu không gian? PTE cho bộ nhớ mới chứa gì?

3.  Sửa đổi xv6 để sử dụng các siêu trang cho hạt nhân.

4.  Các triển khai Unix của `exec` theo truyền thống bao gồm việc xử lý đặc biệt cho các kịch bản shell. Nếu tệp để thực thi bắt đầu bằng văn bản `#!`, thì dòng đầu tiên được coi là một chương trình để chạy để diễn giải tệp. Ví dụ, nếu `exec` được gọi để chạy `myprog arg1` và dòng đầu tiên của `myprog` là `#!/interp`, thì `exec` sẽ chạy `/interp` với dòng lệnh `/interp myprog arg1`. Triển khai hỗ trợ cho quy ước này trong xv6.

5.  Triển khai ngẫu nhiên hóa bố cục không gian địa chỉ cho hạt nhân.

## 4 Bẫy và các lời gọi hệ thống

Có ba loại sự kiện khiến CPU gạt bỏ việc thực thi các lệnh thông thường và buộc chuyển quyền điều khiển sang mã đặc biệt xử lý sự kiện. Một tình huống là một *lời gọi hệ thống*, khi một chương trình người dùng thực thi lệnh `ecall` để yêu cầu hạt nhân làm gì đó cho nó. Một tình huống khác là một *ngoại lệ*: một lệnh (người dùng hoặc hạt nhân) làm điều gì đó bất hợp pháp, chẳng hạn như chia cho 0 hoặc sử dụng một địa chỉ ảo không hợp lệ. Tình huống thứ ba là một *ngắt thiết bị*, khi một thiết bị báo hiệu rằng nó cần chú ý, ví dụ như khi phần cứng đĩa hoàn thành một yêu cầu đọc hoặc ghi.

Cuốn sách này sử dụng *bẫy* như một thuật ngữ chung cho các tình huống này. Thông thường, bất kỳ mã nào đang thực thi tại thời điểm xảy ra bẫy sẽ cần tiếp tục sau đó, và không cần phải biết rằng có điều gì đặc biệt đã xảy ra. Tức là, chúng ta thường muốn các bẫy là *trong suốt*; điều này đặc biệt quan trọng đối với các ngắt thiết bị, mà mã bị ngắt thường không mong đợi. Chuỗi thông thường là một bẫy buộc chuyển quyền điều khiển vào hạt nhân; hạt nhân lưu các thanh ghi và trạng thái khác để việc thực thi có thể được tiếp tục; hạt nhân thực thi mã xử lý thích hợp (ví dụ: một triển khai lời gọi hệ thống hoặc trình điều khiển thiết bị); hạt nhân khôi phục trạng thái đã lưu và quay trở lại từ bẫy; và mã ban đầu tiếp tục ở nơi nó đã dừng lại.

Xv6 xử lý tất cả các bẫy trong hạt nhân; các bẫy không được gửi đến mã người dùng. Việc xử lý các bẫy trong hạt nhân là tự nhiên đối với các lời gọi hệ thống. Nó có ý nghĩa đối với các ngắt vì sự cô lập đòi hỏi rằng chỉ có hạt nhân mới được phép sử dụng các thiết bị, và bởi vì hạt nhân là một cơ chế thuận tiện để chia sẻ các thiết bị giữa nhiều tiến trình. Nó cũng có ý nghĩa đối với các ngoại lệ vì xv6 đáp ứng tất cả các ngoại lệ từ không gian người dùng bằng cách giết chương trình vi phạm.

Việc xử lý bẫy của Xv6 tiến hành theo bốn giai đoạn: các hành động phần cứng được thực hiện bởi CPU RISC-V, một số lệnh hợp ngữ chuẩn bị đường cho mã C của hạt nhân, một hàm C quyết định phải làm gì với bẫy, và lời gọi hệ thống hoặc quy trình dịch vụ trình điều khiển thiết bị. Mặc dù sự tương đồng giữa ba loại bẫy cho thấy rằng một hạt nhân có thể xử lý tất cả các bẫy bằng một đường dẫn mã duy nhất, nhưng hóa ra việc có mã riêng cho hai trường hợp riêng biệt là thuận tiện: các bẫy từ không gian người dùng và các bẫy từ không gian hạt nhân. Mã hạt nhân (hợp ngữ hoặc C) xử lý một bẫy thường được gọi là một *trình xử lý*; các lệnh xử lý đầu tiên thường được viết bằng hợp ngữ (thay vì C) và đôi khi được gọi là một *vectơ*.

### 4.1 Cơ chế bẫy RISC-V

Mỗi CPU RISC-V có một tập hợp các thanh ghi điều khiển mà hạt nhân ghi để cho CPU biết cách xử lý các bẫy, và hạt nhân có thể đọc để tìm hiểu về một bẫy đã xảy ra. Các tài liệu RISC-V chứa câu chuyện đầy đủ [3]. `riscv.h` (kernel/riscv.h:1) chứa các định nghĩa mà xv6 sử dụng. Đây là một phác thảo về các thanh ghi quan trọng nhất:

*   `stvec`: Hạt nhân ghi địa chỉ của trình xử lý bẫy của nó vào đây; RISC-V nhảy đến địa chỉ trong `stvec` để xử lý một bẫy.

*   `sepc`: Khi một bẫy xảy ra, RISC-V lưu bộ đếm chương trình vào đây (vì `pc` sau đó được ghi đè bằng giá trị trong `stvec`). Lệnh `sret` (trở về từ bẫy) sao chép `sepc` vào `pc`. Hạt nhân có thể ghi `sepc` để kiểm soát nơi `sret` đi.

*   `scause`: RISC-V đặt một số vào đây mô tả lý do của bẫy.

*   `sscratch`: Mã xử lý bẫy sử dụng `sscratch` để giúp nó tránh ghi đè lên các thanh ghi người dùng trước khi lưu chúng.

*   `sstatus`: Bit `SIE` trong `sstatus` kiểm soát xem các ngắt thiết bị có được bật hay không. Nếu hạt nhân xóa `SIE`, RISC-V sẽ trì hoãn các ngắt thiết bị cho đến khi hạt nhân đặt `SIE`. Bit `SPP` cho biết liệu một bẫy đến từ chế độ người dùng hay chế độ giám sát, và kiểm soát chế độ mà `sret` trả về.

Các thanh ghi trên liên quan đến các bẫy được xử lý ở chế độ giám sát, và chúng không thể được đọc hoặc ghi ở chế độ người dùng.

Mỗi CPU trên một chip đa lõi có bộ thanh ghi riêng của nó, và nhiều hơn một CPU có thể đang xử lý một bẫy tại bất kỳ thời điểm nào.

Khi nó cần buộc một bẫy, phần cứng RISC-V thực hiện các bước sau cho tất cả các loại bẫy:

1.  Nếu bẫy là một ngắt thiết bị, và bit `sstatus` `SIE` bị xóa, không làm bất kỳ điều gì sau đây.

2.  Vô hiệu hóa các ngắt bằng cách xóa bit `SIE` trong `sstatus`.

3.  Sao chép `pc` vào `sepc`.

4.  Lưu chế độ hiện tại (người dùng hoặc giám sát) trong bit `SPP` trong `sstatus`.

5.  Đặt `scause` để phản ánh nguyên nhân của bẫy.

6.  Đặt chế độ thành giám sát.

7.  Sao chép `stvec` vào `pc`.

8.  Bắt đầu thực thi tại `pc` mới.

Lưu ý rằng CPU không chuyển sang bảng trang hạt nhân, không chuyển sang một ngăn xếp trong hạt nhân, và không lưu bất kỳ thanh ghi nào khác ngoài `pc`. Phần mềm hạt nhân phải thực hiện các tác vụ này. Một lý do mà CPU thực hiện công việc tối thiểu trong một bẫy là để cung cấp sự linh hoạt cho phần mềm; ví dụ, một số hệ điều hành bỏ qua việc chuyển đổi bảng trang trong một số tình huống để tăng hiệu suất bẫy.

Đáng để suy nghĩ xem liệu có bất kỳ bước nào được liệt kê ở trên có thể được bỏ qua, có lẽ để tìm kiếm các bẫy nhanh hơn. Mặc dù có những tình huống trong đó một chuỗi đơn giản hơn có thể hoạt động, nhiều bước sẽ nguy hiểm nếu bỏ qua nói chung. Ví dụ, giả sử rằng CPU không chuyển đổi bộ đếm chương trình. Sau đó, một bẫy từ không gian người dùng có thể chuyển sang chế độ giám sát trong khi vẫn chạy các lệnh người dùng. Các lệnh người dùng đó có thể phá vỡ sự cô lập người dùng/hạt nhân, ví dụ bằng cách sửa đổi thanh ghi `satp` để trỏ đến một bảng trang cho phép truy cập tất cả bộ nhớ vật lý. Do đó, điều quan trọng là CPU chuyển sang một địa chỉ lệnh do hạt nhân chỉ định, cụ thể là `stvec`.

### 4.2 Bẫy từ không gian người dùng

Xv6 xử lý các bẫy khác nhau tùy thuộc vào việc bẫy xảy ra khi đang thực thi trong hạt nhân hay trong mã người dùng. Đây là câu chuyện cho các bẫy từ mã người dùng; Phần 4.5 mô tả các bẫy từ mã hạt nhân.

Một bẫy có thể xảy ra khi đang thực thi trong không gian người dùng nếu chương trình người dùng thực hiện một lời gọi hệ thống (lệnh `ecall`), hoặc làm điều gì đó bất hợp pháp, hoặc nếu một thiết bị ngắt. Đường dẫn cấp cao của một bẫy từ không gian người dùng là `uservec` (kernel/trampoline.S:22), sau đó là `usertrap` (kernel/trap.c:37); và khi quay trở lại, `usertrapret` (kernel/trap.c:90) và sau đó là `userret` (kernel/trampoline.S:101).

Một ràng buộc chính đối với thiết kế xử lý bẫy của xv6 là thực tế là phần cứng RISC-V không chuyển đổi các bảng trang khi nó buộc một bẫy. Điều này có nghĩa là địa chỉ xử lý bẫy trong `stvec` phải có một ánh xạ hợp lệ trong bảng trang người dùng, vì đó là bảng trang có hiệu lực khi mã xử lý bẫy bắt đầu thực thi. Hơn nữa, mã xử lý bẫy của xv6 cần chuyển sang bảng trang hạt nhân; để có thể tiếp tục thực thi sau khi chuyển đổi đó, bảng trang hạt nhân cũng phải có một ánh xạ cho trình xử lý được trỏ đến bởi `stvec`.

Xv6 đáp ứng các yêu cầu này bằng cách sử dụng một *trang trampoline*. Trang trampoline chứa `uservec`, mã xử lý bẫy của xv6 mà `stvec` trỏ đến. Trang trampoline được ánh xạ trong bảng trang của mỗi tiến trình tại địa chỉ `TRAMPOLINE`, ở đầu không gian địa chỉ ảo để nó sẽ ở trên bộ nhớ mà các chương trình sử dụng cho chính chúng. Trang trampoline cũng được ánh xạ tại địa chỉ `TRAMPOLINE` trong bảng trang hạt nhân. Xem Hình 2.3 và Hình 3.3. Bởi vì trang trampoline được ánh xạ trong bảng trang người dùng, các bẫy có thể bắt đầu thực thi ở đó trong chế độ giám sát. Bởi vì trang trampoline được ánh xạ tại cùng một địa chỉ trong không gian địa chỉ hạt nhân, trình xử lý bẫy có thể tiếp tục thực thi sau khi nó chuyển sang bảng trang hạt nhân.

Mã cho trình xử lý bẫy `uservec` nằm trong `trampoline.S` (kernel/trampoline.S:22). Khi `uservec` bắt đầu, tất cả 32 thanh ghi đều chứa các giá trị thuộc sở hữu của mã người dùng bị ngắt. 32 giá trị này cần được lưu ở đâu đó trong bộ nhớ, để sau này hạt nhân có thể khôi phục chúng trước khi quay trở lại không gian người dùng. Việc lưu trữ vào bộ nhớ yêu cầu sử dụng một thanh ghi để chứa địa chỉ, nhưng tại thời điểm này không có thanh ghi mục đích chung nào có sẵn! May mắn thay, RISC-V cung cấp một sự giúp đỡ dưới dạng thanh ghi `sscratch`. Lệnh `csrw` ở đầu `uservec` lưu `a0` vào `sscratch`. Bây giờ `uservec` có một thanh ghi (`a0`) để chơi.

Nhiệm vụ tiếp theo của `uservec` là lưu 32 thanh ghi người dùng. Hạt nhân phân bổ, cho mỗi tiến trình, một trang bộ nhớ cho một cấu trúc `trapframe` (trong số những thứ khác) có không gian để lưu 32 thanh ghi người dùng (kernel/proc.h:43). Bởi vì `satp` vẫn tham chiếu đến bảng trang người dùng, `uservec` cần trapframe được ánh xạ trong không gian địa chỉ người dùng. Xv6 ánh xạ trapframe của mỗi tiến trình tại địa chỉ ảo `TRAPFRAME` trong bảng trang người dùng của tiến trình đó; `TRAPFRAME` ngay bên dưới `TRAMPOLINE`. `p->trapframe` của tiến trình cũng trỏ đến trapframe, mặc dù tại địa chỉ vật lý của nó để hạt nhân có thể sử dụng nó thông qua bảng trang hạt nhân.

Do đó, `uservec` tải địa chỉ `TRAPFRAME` vào `a0` và lưu tất cả các thanh ghi người dùng ở đó, bao gồm cả `a0` của người dùng, được đọc lại từ `sscratch`.

`trapframe` chứa địa chỉ của ngăn xếp hạt nhân của tiến trình hiện tại, hartid của CPU hiện tại, địa chỉ của hàm `usertrap`, và địa chỉ của bảng trang hạt nhân. `uservec` truy xuất các giá trị này, chuyển `satp` sang bảng trang hạt nhân, và nhảy đến `usertrap`.

Công việc của `usertrap` là xác định nguyên nhân của bẫy, xử lý nó, và quay trở lại (kernel/trap.c:37). Đầu tiên, nó thay đổi `stvec` để một bẫy trong khi ở trong hạt nhân sẽ được xử lý bởi `kernelvec` thay vì `uservec`. Nó lưu thanh ghi `sepc` (bộ đếm chương trình người dùng đã lưu), bởi vì `usertrap` có thể gọi `yield` để chuyển sang luồng hạt nhân của một tiến trình khác, và tiến trình đó có thể quay trở lại không gian người dùng, trong quá trình đó nó sẽ sửa đổi `sepc`. Nếu bẫy là một lời gọi hệ thống, `usertrap` gọi `syscall` để xử lý nó; nếu là một ngắt thiết bị, `devintr`; nếu không, đó là một ngoại lệ, và hạt nhân sẽ giết tiến trình bị lỗi. Đường dẫn lời gọi hệ thống cộng bốn vào bộ đếm chương trình người dùng đã lưu vì RISC-V, trong trường hợp một lời gọi hệ thống, để lại bộ đếm chương trình trỏ đến lệnh `ecall` nhưng mã người dùng cần tiếp tục thực thi tại lệnh tiếp theo. Trên đường ra, `usertrap` kiểm tra xem tiến trình đã bị giết hay nên nhường CPU (nếu bẫy này là một ngắt hẹn giờ).

Bước đầu tiên để quay trở lại không gian người dùng là lệnh gọi đến `usertrapret` (kernel/trap.c:90). Hàm này thiết lập các thanh ghi điều khiển RISC-V để chuẩn bị cho một bẫy trong tương lai từ không gian người dùng: đặt `stvec` thành `uservec` và chuẩn bị các trường trapframe mà `uservec` dựa vào. `usertrapret` đặt `sepc` thành bộ đếm chương trình người dùng đã lưu trước đó. Cuối cùng, `usertrapret` gọi `userret` trên trang trampoline được ánh xạ trong cả bảng trang người dùng và hạt nhân; lý do là mã hợp ngữ trong `userret` sẽ chuyển đổi các bảng trang.

Lệnh gọi của `usertrapret` đến `userret` truyền một con trỏ đến bảng trang người dùng của tiến trình trong `a0` (kernel/trampoline.S:101). `userret` chuyển `satp` sang bảng trang người dùng của tiến trình. Nhớ lại rằng bảng trang người dùng ánh xạ cả trang trampoline và `TRAPFRAME`, nhưng không có gì khác từ hạt nhân. Ánh xạ trang trampoline tại cùng một địa chỉ ảo trong các bảng trang người dùng và hạt nhân cho phép `userret` tiếp tục thực thi sau khi thay đổi `satp`. Từ thời điểm này trở đi, dữ liệu duy nhất mà `userret` có thể sử dụng là nội dung thanh ghi và nội dung của trapframe. `userret` tải địa chỉ `TRAPFRAME` vào `a0`, khôi phục các thanh ghi người dùng đã lưu từ trapframe thông qua `a0`, khôi phục `a0` người dùng đã lưu, và thực thi `sret` để quay trở lại không gian người dùng.

### 4.3 Mã: Gọi các lời gọi hệ thống

Chương 2 kết thúc với `initcode.S` gọi lời gọi hệ thống `exec` (user/initcode.S:11). Hãy xem cách lệnh gọi người dùng đi đến việc triển khai lời gọi hệ thống `exec` trong hạt nhân.

`initcode.S` đặt các đối số cho `exec` vào các thanh ghi `a0` và `a1`, và đặt số lời gọi hệ thống vào `a7`. Các số lời gọi hệ thống khớp với các mục trong mảng `syscalls`, một bảng các con trỏ hàm (kernel/syscall.c:107). Lệnh `ecall` bẫy vào hạt nhân và gây ra việc thực thi `uservec`, `usertrap`, và sau đó là `syscall`, như chúng ta đã thấy ở trên.

`syscall` (kernel/syscall.c:132) truy xuất số lời gọi hệ thống từ `a7` đã lưu trong trapframe và sử dụng nó để lập chỉ mục vào `syscalls`. Đối với lời gọi hệ thống đầu tiên, `a7` chứa `SYS_exec` (kernel/syscall.h:8), dẫn đến một lệnh gọi đến hàm triển khai lời gọi hệ thống `sys_exec`.

Khi `sys_exec` trả về, `syscall` ghi lại giá trị trả về của nó trong `p->trapframe->a0`. Điều này sẽ khiến lệnh gọi không gian người dùng ban đầu đến `exec()` trả về giá trị đó, vì quy ước gọi C trên RISC-V đặt các giá trị trả về trong `a0`. Các lời gọi hệ thống theo quy ước trả về các số âm để chỉ ra các lỗi, và các số không hoặc dương để chỉ ra thành công. Nếu số lời gọi hệ thống không hợp lệ, `syscall` sẽ in một lỗi và trả về -1.

### 4.4 Mã: Đối số lời gọi hệ thống

Các triển khai lời gọi hệ thống trong hạt nhân cần tìm các đối số được truyền bởi mã người dùng. Bởi vì mã người dùng gọi các hàm bao bọc lời gọi hệ thống, các đối số ban đầu ở nơi quy ước gọi C của RISC-V đặt chúng: trong các thanh ghi. Mã bẫy hạt nhân lưu các thanh ghi người dùng vào trapframe của tiến trình hiện tại, nơi mã hạt nhân có thể tìm thấy chúng. Các hàm hạt nhân `argint`, `argaddr`, và `argfd` truy xuất đối số lời gọi hệ thống thứ n từ trapframe dưới dạng một số nguyên, một con trỏ hoặc một mô tả tệp. Tất cả chúng đều gọi `argraw` để truy xuất thanh ghi người dùng đã lưu thích hợp (kernel/syscall.c:34).

Một số lời gọi hệ thống truyền các con trỏ làm đối số, và hạt nhân phải sử dụng các con trỏ đó để đọc hoặc ghi bộ nhớ người dùng. Lời gọi hệ thống `exec`, ví dụ, truyền cho hạt nhân một mảng các con trỏ tham chiếu đến các đối số chuỗi trong không gian người dùng. Các con trỏ này đặt ra hai thách thức. Đầu tiên, chương trình người dùng có thể bị lỗi hoặc độc hại, và có thể truyền cho hạt nhân một con trỏ không hợp lệ hoặc một con trỏ nhằm lừa hạt nhân truy cập bộ nhớ hạt nhân thay vì bộ nhớ người dùng. Thứ hai, các ánh xạ bảng trang hạt nhân xv6 không giống như các ánh xạ bảng trang người dùng, vì vậy hạt nhân không thể sử dụng các lệnh tải hoặc lưu trữ thông thường từ các địa chỉ do người dùng cung cấp.

Hạt nhân thực hiện các hàm chuyển dữ liệu một cách an toàn đến và từ các địa chỉ do người dùng cung cấp. `fetchstr` là một ví dụ (kernel/syscall.c:25). Các lời gọi hệ thống tệp như `exec` sử dụng `fetchstr` để truy xuất các đối số tên tệp chuỗi từ không gian người dùng. `fetchstr` gọi `copyinstr` để thực hiện công việc khó khăn.

`copyinstr` (kernel/vm.c:415) sao chép tối đa `max` byte đến `dst` từ địa chỉ ảo `srcva` trong bảng trang người dùng `pagetable`. Vì `pagetable` không phải là bảng trang hiện tại, `copyinstr` sử dụng `walkaddr` (gọi `walk`) để tra cứu `srcva` trong `pagetable`, mang lại địa chỉ vật lý `pa0`. Bảng trang của hạt nhân ánh xạ tất cả RAM vật lý tại các địa chỉ ảo bằng với địa chỉ vật lý của RAM. Điều này cho phép `copyinstr` sao chép trực tiếp các byte chuỗi từ `pa0` đến `dst`. `walkaddr` (kernel/vm.c:109) kiểm tra xem địa chỉ ảo do người dùng cung cấp có phải là một phần của không gian địa chỉ của tiến trình hay không, để các chương trình không thể lừa hạt nhân đọc bộ nhớ khác. Một hàm tương tự, `copyout`, sao chép dữ liệu từ hạt nhân đến một địa chỉ do người dùng cung cấp.

### 4.5 Bẫy từ không gian hạt nhân

Xv6 xử lý các bẫy từ mã hạt nhân theo một cách khác với các bẫy từ mã người dùng. Khi vào hạt nhân, `usertrap` trỏ `stvec` đến mã hợp ngữ tại `kernelvec` (kernel/kernelvec.S:12). Vì `kernelvec` chỉ thực thi nếu xv6 đã ở trong hạt nhân, `kernelvec` có thể dựa vào `satp` được đặt thành bảng trang hạt nhân, và con trỏ ngăn xếp tham chiếu đến một ngăn xếp hạt nhân hợp lệ. `kernelvec` đẩy tất cả 32 thanh ghi lên ngăn xếp, từ đó nó sẽ khôi phục chúng sau này để mã hạt nhân bị ngắt có thể tiếp tục mà không bị xáo trộn.

`kernelvec` lưu các thanh ghi trên ngăn xếp của luồng hạt nhân bị ngắt, điều này có ý nghĩa vì các giá trị thanh ghi thuộc về luồng đó. Điều này đặc biệt quan trọng nếu bẫy gây ra một sự chuyển đổi sang một luồng khác – trong trường hợp đó, bẫy sẽ thực sự trả về từ ngăn xếp của luồng mới, để lại các thanh ghi đã lưu của luồng bị ngắt một cách an toàn trên ngăn xếp của nó.

`kernelvec` nhảy đến `kerneltrap` (kernel/trap.c:135) sau khi lưu các thanh ghi. `kerneltrap` được chuẩn bị cho hai loại bẫy: ngắt thiết bị và ngoại lệ. Nó gọi `devintr` (kernel/trap.c:185) để kiểm tra và xử lý cái trước. Nếu bẫy không phải là một ngắt thiết bị, nó phải là một ngoại lệ, và đó luôn là một lỗi nghiêm trọng nếu nó xảy ra trong hạt nhân xv6; hạt nhân gọi `panic` và dừng thực thi.

Nếu `kerneltrap` được gọi do một ngắt hẹn giờ, và luồng hạt nhân của một tiến trình đang chạy (trái ngược với một luồng lập lịch), `kerneltrap` gọi `yield` để cho các luồng khác có cơ hội chạy. Tại một thời điểm nào đó, một trong những luồng đó sẽ nhường, và để cho luồng của chúng ta và `kerneltrap` của nó tiếp tục lại. Chương 7 giải thích những gì xảy ra trong `yield`.

Khi công việc của `kerneltrap` hoàn tất, nó cần quay trở lại bất kỳ mã nào đã bị ngắt bởi bẫy. Bởi vì một `yield` có thể đã làm xáo trộn `sepc` và chế độ trước đó trong `sstatus`, `kerneltrap` lưu chúng khi nó bắt đầu. Bây giờ nó khôi phục các thanh ghi điều khiển đó và quay trở lại `kernelvec` (kernel/kernelvec.S:38). `kernelvec` bật các thanh ghi đã lưu từ ngăn xếp và thực thi `sret`, sao chép `sepc` vào `pc` và tiếp tục mã hạt nhân bị ngắt.

Đáng để suy nghĩ xem việc trả về bẫy xảy ra như thế nào nếu `kerneltrap` gọi `yield` do một ngắt hẹn giờ.

Xv6 đặt `stvec` của một CPU thành `kernelvec` khi CPU đó vào hạt nhân từ không gian người dùng; bạn có thể thấy điều này trong `usertrap` (kernel/trap.c:29). Có một cửa sổ thời gian khi hạt nhân đã bắt đầu thực thi nhưng `stvec` vẫn được đặt thành `uservec`, và điều quan trọng là không có ngắt thiết bị nào xảy ra trong cửa sổ đó. May mắn thay, RISC-V luôn vô hiệu hóa các ngắt khi nó bắt đầu thực hiện một bẫy, và `usertrap` không bật lại chúng cho đến sau khi nó đặt `stvec`.

### 4.6 Ngoại lệ lỗi trang

Phản ứng của Xv6 đối với các ngoại lệ khá nhàm chán: nếu một ngoại lệ xảy ra trong không gian người dùng, hạt nhân sẽ giết tiến trình bị lỗi. Nếu một ngoại lệ xảy ra trong hạt nhân, hạt nhân sẽ hoảng loạn. Các hệ điều hành thực thường phản ứng theo những cách thú vị hơn nhiều.

Ví dụ, nhiều hạt nhân sử dụng các lỗi trang để thực hiện *sao chép khi ghi* (COW) `fork`. Để giải thích `fork` sao chép khi ghi, hãy xem xét `fork` của xv6, được mô tả trong Chương 3. `fork` làm cho nội dung bộ nhớ ban đầu của con giống như của cha tại thời điểm `fork`. Xv6 thực hiện `fork` với `uvmcopy` (kernel/vm.c:313), phân bổ bộ nhớ vật lý cho con và sao chép bộ nhớ của cha vào đó. Sẽ hiệu quả hơn nếu con và cha có thể chia sẻ bộ nhớ vật lý của cha. Tuy nhiên, một triển khai đơn giản của điều này sẽ không hoạt động, vì nó sẽ khiến cha và con phá vỡ việc thực thi của nhau bằng các lần ghi của chúng vào ngăn xếp và heap được chia sẻ.

Cha và con có thể chia sẻ bộ nhớ vật lý một cách an toàn bằng cách sử dụng hợp lý các quyền của bảng trang và các lỗi trang. CPU gây ra một *ngoại lệ lỗi trang* khi một địa chỉ ảo được sử dụng không có ánh xạ trong bảng trang, hoặc có một ánh xạ có cờ `PTE_V` bị xóa, hoặc một ánh xạ có các bit quyền (`PTE_R`, `PTE_W`, `PTE_X`, `PTE_U`) cấm thao tác đang được thực hiện. RISC-V phân biệt ba loại lỗi trang: lỗi trang tải (gây ra bởi các lệnh tải), lỗi trang lưu trữ (gây ra bởi các lệnh lưu trữ), và lỗi trang lệnh (gây ra bởi các lần tìm nạp các lệnh để thực thi). Thanh ghi `scause` cho biết loại lỗi trang và thanh ghi `stval` chứa địa chỉ không thể dịch được.

Kế hoạch cơ bản trong `fork` COW là để cha và con ban đầu chia sẻ tất cả các trang vật lý, nhưng để mỗi trang ánh xạ chúng chỉ đọc (với cờ `PTE_W` bị xóa). Cha và con có thể đọc từ bộ nhớ vật lý được chia sẻ. Nếu một trong hai ghi vào một trang nhất định, CPU RISC-V sẽ gây ra một ngoại lệ lỗi trang. Trình xử lý bẫy của hạt nhân đáp ứng bằng cách phân bổ một trang bộ nhớ vật lý mới và sao chép vào đó trang vật lý mà địa chỉ bị lỗi ánh xạ đến. Hạt nhân thay đổi PTE có liên quan trong bảng trang của tiến trình bị lỗi để trỏ đến bản sao và cho phép ghi cũng như đọc, và sau đó tiếp tục tiến trình bị lỗi tại lệnh đã gây ra lỗi. Bởi vì PTE bây giờ cho phép ghi, lệnh được thực thi lại sẽ thực thi mà không có lỗi. Sao chép khi ghi yêu cầu ghi sổ sách để giúp quyết định khi nào các trang vật lý có thể được giải phóng, vì mỗi trang có thể được tham chiếu bởi một số lượng khác nhau của các bảng trang tùy thuộc vào lịch sử của các `fork`, các lỗi trang, `exec`, và `exit`. Việc ghi sổ sách này cho phép một tối ưu hóa quan trọng: nếu một tiến trình gặp một lỗi trang lưu trữ và trang vật lý chỉ được tham chiếu từ bảng trang của tiến trình đó, không cần sao chép.

Sao chép khi ghi làm cho `fork` nhanh hơn, vì `fork` không cần sao chép bộ nhớ. Một số bộ nhớ sẽ phải được sao chép sau đó, khi được ghi, nhưng thường thì hầu hết bộ nhớ không bao giờ phải được sao chép. Một ví dụ phổ biến là `fork` theo sau là `exec`: một vài trang có thể được ghi sau `fork`, nhưng sau đó `exec` của con giải phóng phần lớn bộ nhớ được kế thừa từ cha. `fork` sao chép khi ghi loại bỏ nhu cầu sao chép bộ nhớ này.

Sự kết hợp của các bảng trang và các lỗi trang mở ra một loạt các khả năng thú vị ngoài `fork` COW. Một tính năng được sử dụng rộng rãi khác được gọi là *phân bổ lười biếng*, có hai phần. Đầu tiên, khi một ứng dụng yêu cầu thêm bộ nhớ bằng cách gọi `sbrk`, hạt nhân ghi nhận sự gia tăng kích thước, nhưng không phân bổ bộ nhớ vật lý và không tạo PTE cho phạm vi địa chỉ ảo mới. Thứ hai, trên một lỗi trang trên một trong những địa chỉ mới đó, hạt nhân phân bổ một trang bộ nhớ vật lý và ánh xạ nó vào bảng trang. Giống như `fork` COW, hạt nhân có thể thực hiện phân bổ lười biếng một cách minh bạch cho các ứng dụng.

Vì các ứng dụng thường yêu cầu nhiều bộ nhớ hơn chúng cần, phân bổ lười biếng là một chiến thắng: hạt nhân không phải thực hiện bất kỳ công việc nào cho các trang mà ứng dụng không bao giờ sử dụng. Hơn nữa, nếu ứng dụng đang yêu cầu phát triển không gian địa chỉ thêm rất nhiều, thì `sbrk` không có phân bổ lười biếng là tốn kém: nếu một ứng dụng yêu cầu một gigabyte bộ nhớ, hạt nhân phải phân bổ và không hóa 262.144 trang 4096 byte. Phân bổ lười biếng cho phép chi phí này được trải đều theo thời gian. Mặt khác, phân bổ lười biếng phải chịu thêm chi phí của các lỗi trang, liên quan đến một quá trình chuyển đổi người dùng/hạt nhân. Các hệ điều hành có thể giảm chi phí này bằng cách phân bổ một loạt các trang liên tiếp cho mỗi lỗi trang thay vì một trang và bằng cách chuyên môn hóa mã vào/ra hạt nhân cho các lỗi trang như vậy.

Một tính năng được sử dụng rộng rãi khác khai thác các lỗi trang là *phân trang theo yêu cầu*. Trong `exec`, xv6 tải tất cả văn bản và dữ liệu của một ứng dụng vào bộ nhớ trước khi bắt đầu ứng dụng. Vì các ứng dụng có thể lớn và việc đọc từ đĩa mất thời gian, chi phí khởi động này có thể đáng chú ý đối với người dùng. Để giảm thời gian khởi động, một hạt nhân hiện đại ban đầu không tải tệp thực thi vào bộ nhớ, mà chỉ tạo bảng trang người dùng với tất cả các PTE được đánh dấu là không hợp lệ. Hạt nhân bắt đầu chương trình chạy; mỗi khi chương trình sử dụng một trang lần đầu tiên, một lỗi trang xảy ra, và để đáp lại, hạt nhân đọc nội dung của trang từ đĩa và ánh xạ nó vào không gian địa chỉ người dùng. Giống như `fork` COW và phân bổ lười biếng, hạt nhân có thể thực hiện tính năng này một cách minh bạch cho các ứng dụng.

Các chương trình chạy trên một máy tính có thể cần nhiều bộ nhớ hơn so với RAM của máy tính. Để đối phó một cách duyên dáng, hệ điều hành có thể thực hiện *phân trang ra đĩa*. Ý tưởng là chỉ lưu trữ một phần nhỏ các trang người dùng trong RAM, và lưu trữ phần còn lại trên đĩa trong một *vùng phân trang*. Hạt nhân đánh dấu các PTE tương ứng với bộ nhớ được lưu trữ trong vùng phân trang (và do đó không có trong RAM) là không hợp lệ. Nếu một ứng dụng cố gắng sử dụng một trong các trang đã được *phân trang ra đĩa*, ứng dụng sẽ gặp một lỗi trang, và trang đó phải được *phân trang vào*: trình xử lý bẫy hạt nhân sẽ phân bổ một trang RAM vật lý, đọc trang từ đĩa vào RAM, và sửa đổi PTE có liên quan để trỏ đến RAM.

Điều gì xảy ra nếu một trang cần được phân trang vào, nhưng không có RAM vật lý miễn phí? Trong trường hợp đó, hạt nhân trước tiên phải giải phóng một trang vật lý bằng cách *phân trang ra* hoặc *loại bỏ* nó vào vùng phân trang trên đĩa, và đánh dấu các PTE tham chiếu đến trang vật lý đó là không hợp lệ. Việc loại bỏ là tốn kém, vì vậy việc phân trang hoạt động tốt nhất nếu nó không thường xuyên: nếu các ứng dụng chỉ sử dụng một tập hợp con của các trang bộ nhớ của chúng và hợp của các tập hợp con vừa với RAM. Thuộc tính này thường được gọi là có *tính cục bộ tốt của tham chiếu*. Như với nhiều kỹ thuật bộ nhớ ảo, các hạt nhân thường thực hiện việc phân trang ra đĩa theo cách minh bạch cho các ứng dụng.

Các máy tính thường hoạt động với ít hoặc không có bộ nhớ vật lý miễn phí, bất kể phần cứng cung cấp bao nhiêu RAM. Ví dụ, các nhà cung cấp đám mây ghép kênh nhiều khách hàng trên một máy duy nhất để sử dụng phần cứng của họ một cách hiệu quả về chi phí. Là một ví dụ khác, người dùng chạy nhiều ứng dụng trên điện thoại thông minh trong một lượng nhỏ bộ nhớ vật lý. Trong các cài đặt như vậy, việc phân bổ một trang có thể yêu cầu trước tiên phải loại bỏ một trang hiện có. Do đó, khi bộ nhớ vật lý miễn phí khan hiếm, việc phân bổ là tốn kém.

Phân bổ lười biếng và phân trang theo yêu cầu đặc biệt thuận lợi khi bộ nhớ miễn phí khan hiếm và các chương trình chỉ tích cực sử dụng một phần nhỏ bộ nhớ được phân bổ của chúng. Các kỹ thuật này cũng có thể tránh được công việc lãng phí khi một trang được phân bổ hoặc tải nhưng không bao giờ được sử dụng hoặc bị loại bỏ trước khi nó có thể được sử dụng.

Các tính năng khác kết hợp các ngoại lệ phân trang và lỗi trang bao gồm tự động mở rộng các ngăn xếp và các *tệp được ánh xạ bộ nhớ*, là các tệp mà một chương trình đã ánh xạ vào không gian địa chỉ của nó bằng cách sử dụng lời gọi hệ thống `mmap` để chương trình có thể đọc và ghi chúng bằng cách sử dụng tải và lưu trữ các lệnh.

### 4.7 Thế giới thực

Trampoline và trapframe có vẻ phức tạp quá mức. Một động lực là RISC-V cố ý làm ít nhất có thể khi buộc một bẫy, để cho phép khả năng xử lý bẫy rất nhanh, điều này hóa ra lại quan trọng. Kết quả là, một vài lệnh đầu tiên của trình xử lý bẫy hạt nhân thực sự phải thực thi trong môi trường người dùng: bảng trang người dùng và nội dung thanh ghi người dùng. Và trình xử lý bẫy ban đầu không biết các sự kiện hữu ích như danh tính của tiến trình đang chạy hoặc địa chỉ của bảng trang hạt nhân. Một giải pháp là có thể bởi vì RISC-V cung cấp các nơi được bảo vệ trong đó hạt nhân có thể cất giấu thông tin trước khi vào không gian người dùng: thanh ghi `sscratch`, và các mục nhập bảng trang người dùng trỏ đến bộ nhớ hạt nhân nhưng được bảo vệ bởi việc thiếu `PTE_U`. Trampoline và trapframe của Xv6 khai thác các tính năng RISC-V này.

Nhu cầu về các trang trampoline đặc biệt có thể được loại bỏ nếu bộ nhớ hạt nhân được ánh xạ vào bảng trang người dùng của mỗi tiến trình (với `PTE_U` bị xóa). Điều đó cũng sẽ loại bỏ nhu cầu chuyển đổi bảng trang khi bẫy từ không gian người dùng vào hạt nhân. Điều đó đến lượt nó sẽ cho phép các triển khai lời gọi hệ thống trong hạt nhân tận dụng việc bộ nhớ người dùng của tiến trình hiện tại được ánh xạ, cho phép mã hạt nhân tham chiếu trực tiếp đến các con trỏ người dùng. Nhiều hệ điều hành đã sử dụng những ý tưởng này để tăng hiệu quả. Xv6 tránh chúng để giảm khả năng xảy ra các lỗi bảo mật trong hạt nhân do việc sử dụng vô ý các con trỏ người dùng, và để giảm một số phức tạp sẽ được yêu cầu để đảm bảo rằng các địa chỉ ảo của người dùng và hạt nhân không chồng chéo.

Các hệ điều hành sản xuất thực hiện `fork` sao chép khi ghi, phân bổ lười biếng, phân trang theo yêu cầu, phân trang ra đĩa, các tệp được ánh xạ bộ nhớ, v.v. Hơn nữa, các hệ điều hành sản xuất cố gắng lưu trữ một cái gì đó hữu ích trong tất cả các vùng của bộ nhớ vật lý, thường là lưu trữ nội dung tệp trong bộ nhớ không được các tiến trình sử dụng.

Các hệ điều hành sản xuất cũng cung cấp cho các ứng dụng các lời gọi hệ thống để quản lý các không gian địa chỉ của chúng và thực hiện việc xử lý lỗi trang của riêng chúng thông qua các lời gọi hệ thống `mmap`, `munmap`, và `sigaction`, cũng như cung cấp các lệnh gọi để ghim bộ nhớ vào RAM (xem `mlock`) và để tư vấn cho hạt nhân cách một ứng dụng dự định sử dụng bộ nhớ của nó (xem `madvise`).

### 4.8 Bài tập

1.  Các hàm `copyin` và `copyinstr` đi qua bảng trang người dùng trong phần mềm. Thiết lập bảng trang hạt nhân để hạt nhân có bộ nhớ chương trình người dùng được ánh xạ, và `copyin` và `copyinstr` có thể sử dụng `memcpy` để sao chép các đối số lời gọi hệ thống vào không gian hạt nhân, dựa vào phần cứng để thực hiện việc đi qua bảng trang.

2.  Triển khai phân bổ bộ nhớ lười biếng.

3.  Triển khai `fork` COW.

4.  Có cách nào để loại bỏ ánh xạ trang `TRAPFRAME` đặc biệt trong mọi không gian địa chỉ người dùng không? Ví dụ, `uservec` có thể được sửa đổi để chỉ cần đẩy 32 thanh ghi người dùng lên ngăn xếp hạt nhân, hoặc lưu chúng trong cấu trúc `proc` không?

5.  Xv6 có thể được sửa đổi để loại bỏ ánh xạ trang `TRAMPOLINE` đặc biệt không?

6.  Triển khai `mmap`.

## 5 Ngắt và trình điều khiển thiết bị

Một *trình điều khiển* là mã trong một hệ điều hành quản lý một thiết bị cụ thể: nó cấu hình phần cứng thiết bị, yêu cầu thiết bị thực hiện các hoạt động, xử lý các ngắt kết quả, và tương tác với các tiến trình có thể đang chờ I/O từ thiết bị. Mã trình điều khiển có thể phức tạp vì một trình điều khiển thực thi đồng thời với thiết bị mà nó quản lý. Ngoài ra, trình điều khiển phải hiểu giao diện phần cứng của thiết bị, có thể phức tạp và được ghi chép kém.

Các thiết bị cần sự chú ý từ hệ điều hành thường có thể được cấu hình để tạo ra các *ngắt*, là một loại bẫy. Mã xử lý bẫy của hạt nhân nhận ra khi một thiết bị đã gây ra một ngắt và gọi trình xử lý ngắt của trình điều khiển; trong xv6, việc điều phối này xảy ra trong `devintr` (kernel/trap.c:185).

Nhiều trình điều khiển thiết bị thực thi mã trong hai bối cảnh: một *nửa trên* chạy trong luồng hạt nhân của một tiến trình, và một *nửa dưới* thực thi tại thời điểm ngắt. Nửa trên được gọi thông qua các lời gọi hệ thống như `read` và `write` muốn thiết bị thực hiện I/O. Mã này có thể yêu cầu phần cứng bắt đầu một hoạt động (ví dụ: yêu cầu đĩa đọc một khối); sau đó mã đợi cho hoạt động hoàn tất. Cuối cùng, thiết bị hoàn thành hoạt động và gây ra một ngắt. Trình xử lý ngắt của trình điều khiển, hoạt động như nửa dưới, tìm ra hoạt động nào đã hoàn tất, đánh thức một tiến trình đang chờ nếu thích hợp, và yêu cầu phần cứng bắt đầu làm việc trên bất kỳ hoạt động tiếp theo nào đang chờ.

### 5.1 Mã: Nhập liệu từ giao diện điều khiển

Trình điều khiển giao diện điều khiển (kernel/console.c) là một minh họa đơn giản về cấu trúc trình điều khiển. Trình điều khiển giao diện điều khiển chấp nhận các ký tự được gõ bởi một người, thông qua phần cứng cổng nối tiếp UART được gắn vào RISC-V. Trình điều khiển giao diện điều khiển tích lũy một dòng đầu vào tại một thời điểm, xử lý các ký tự đầu vào đặc biệt như xóa lùi và control-u. Các tiến trình người dùng, chẳng hạn như shell, sử dụng lời gọi hệ thống `read` để lấy các dòng đầu vào từ giao diện điều khiển. Khi bạn nhập đầu vào cho xv6 trong QEMU, các lần nhấn phím của bạn được gửi đến xv6 thông qua phần cứng UART được mô phỏng của QEMU.

Phần cứng UART mà trình điều khiển nói chuyện là một chip 16550 [13] được mô phỏng bởi QEMU. Trên một máy tính thực, một 16550 sẽ quản lý một liên kết nối tiếp RS232 kết nối với một thiết bị đầu cuối hoặc một máy tính khác. Khi chạy QEMU, nó được kết nối với bàn phím và màn hình của bạn.

Phần cứng UART xuất hiện cho phần mềm dưới dạng một tập hợp các thanh ghi điều khiển được ánh xạ bộ nhớ. Tức là, có một số địa chỉ vật lý mà phần cứng RISC-V kết nối với thiết bị UART, do đó các lần tải và lưu trữ tương tác với phần cứng thiết bị thay vì RAM. Các địa chỉ được ánh xạ bộ nhớ cho UART bắt đầu tại 0x10000000, hoặc `UART0` (kernel/memlayout.h:21). Có một số thanh ghi điều khiển UART, mỗi thanh ghi có chiều rộng một byte. Các offset của chúng từ `UART0` được định nghĩa trong (kernel/uart.c:22). Ví dụ, thanh ghi `LSR` chứa các bit cho biết liệu các ký tự đầu vào có đang chờ được phần mềm đọc hay không. Các ký tự này (nếu có) có sẵn để đọc từ thanh ghi `RHR`. Mỗi khi một ký tự được đọc, phần cứng UART sẽ xóa nó khỏi một FIFO nội bộ của các ký tự đang chờ, và xóa bit “sẵn sàng” trong `LSR` khi FIFO trống. Phần cứng truyền UART phần lớn độc lập với phần cứng nhận; nếu phần mềm ghi một byte vào `THR`, UART sẽ truyền byte đó.

`main` của Xv6 gọi `consoleinit` (kernel/console.c:182) để khởi tạo phần cứng UART. Mã này cấu hình UART để tạo ra một ngắt nhận khi UART nhận được mỗi byte đầu vào, và một ngắt hoàn thành truyền mỗi khi UART hoàn thành việc gửi một byte đầu ra (kernel/uart.c:53).

Shell xv6 đọc từ giao diện điều khiển thông qua một mô tả tệp được mở bởi `init.c` (user/init.c:19). Các lệnh gọi đến lời gọi hệ thống `read` đi qua hạt nhân đến `consoleread` (kernel/console.c:80). `consoleread` đợi đầu vào đến (thông qua các ngắt) và được đệm trong `cons.buf`, sao chép đầu vào vào không gian người dùng, và (sau khi một dòng đầy đủ đã đến) quay trở lại tiến trình người dùng. Nếu người dùng chưa gõ một dòng đầy đủ, bất kỳ tiến trình đọc nào cũng sẽ đợi trong lệnh gọi `sleep` (kernel/console.c:96) (Chương 7 giải thích chi tiết về `sleep`).

Khi người dùng gõ một ký tự, phần cứng UART yêu cầu RISC-V gây ra một ngắt, kích hoạt trình xử lý bẫy của xv6. Trình xử lý bẫy gọi `devintr` (kernel/trap.c:185), xem thanh ghi `scause` của RISC-V để phát hiện ra rằng ngắt đến từ một thiết bị bên ngoài. Sau đó, nó yêu cầu một đơn vị phần cứng được gọi là PLIC [3] cho nó biết thiết bị nào đã ngắt (kernel/trap.c:193). Nếu đó là UART, `devintr` sẽ gọi `uartintr`.

`uartintr` (kernel/uart.c:177) đọc bất kỳ ký tự đầu vào nào đang chờ từ phần cứng UART và chuyển chúng cho `consoleintr` (kernel/console.c:136); nó không đợi các ký tự, vì đầu vào trong tương lai sẽ gây ra một ngắt mới. Công việc của `consoleintr` là tích lũy các ký tự đầu vào trong `cons.buf` cho đến khi một dòng đầy đủ đến. `consoleintr` xử lý xóa lùi và một vài ký tự đặc biệt khác. Khi một dòng mới đến, `consoleintr` sẽ đánh thức một `consoleread` đang chờ (nếu có).

Sau khi được đánh thức, `consoleread` sẽ quan sát một dòng đầy đủ trong `cons.buf`, sao chép nó vào không gian người dùng, và quay trở lại (thông qua bộ máy lời gọi hệ thống) cho không gian người dùng.

### 5.2 Mã: Xuất liệu ra giao diện điều khiển

Một lời gọi hệ thống `write` trên một mô tả tệp được kết nối với giao diện điều khiển cuối cùng sẽ đến `uartputc` (kernel/uart.c:87). Trình điều khiển thiết bị duy trì một bộ đệm đầu ra (`uart_tx_buf`) để các tiến trình ghi không phải đợi UART hoàn thành việc gửi; thay vào đó, `uartputc` nối mỗi ký tự vào bộ đệm, gọi `uartstart` để bắt đầu thiết bị truyền (nếu nó chưa làm như vậy), và trả về. Tình huống duy nhất mà `uartputc` đợi là nếu bộ đệm đã đầy.

Mỗi khi UART hoàn thành việc gửi một byte, nó sẽ tạo ra một ngắt. `uartintr` gọi `uartstart`, kiểm tra xem thiết bị đã thực sự hoàn thành việc gửi hay chưa, và đưa cho thiết bị ký tự đầu ra được đệm tiếp theo. Do đó, nếu một tiến trình ghi nhiều byte vào giao diện điều khiển, thông thường byte đầu tiên sẽ được gửi bởi lệnh gọi của `uartputc` đến `uartstart`, và các byte được đệm còn lại sẽ được gửi bởi các lệnh gọi của `uartstart` từ `uartintr` khi các ngắt hoàn thành truyền đến.

Một mẫu chung cần lưu ý là sự tách rời hoạt động của thiết bị khỏi hoạt động của tiến trình thông qua việc đệm và các ngắt. Trình điều khiển giao diện điều khiển có thể xử lý đầu vào ngay cả khi không có tiến trình nào đang chờ đọc nó; một lần đọc tiếp theo sẽ thấy đầu vào. Tương tự, các tiến trình có thể gửi đầu ra mà không cần phải đợi thiết bị. Sự tách rời này có thể tăng hiệu suất bằng cách cho phép các tiến trình thực thi đồng thời với I/O của thiết bị, và đặc biệt quan trọng khi thiết bị chậm (như với UART) hoặc cần sự chú ý ngay lập tức (như với việc lặp lại các ký tự đã gõ). Ý tưởng này đôi khi được gọi là *đồng thời I/O*.

### 5.3 Đồng thời trong các trình điều khiển

Bạn có thể đã nhận thấy các lệnh gọi đến `acquire` trong `consoleread` và trong `consoleintr`. Các lệnh gọi này có được một khóa, bảo vệ các cấu trúc dữ liệu của trình điều khiển giao diện điều khiển khỏi việc truy cập đồng thời. Có ba nguy cơ đồng thời ở đây: hai tiến trình trên các CPU khác nhau có thể gọi `consoleread` cùng một lúc; phần cứng có thể yêu cầu một CPU cung cấp một ngắt giao diện điều khiển (thực sự là UART) trong khi CPU đó đã đang thực thi bên trong `consoleread`; và phần cứng có thể cung cấp một ngắt giao diện điều khiển trên một CPU khác trong khi `consoleread` đang thực thi. Chương 6 giải thích cách sử dụng các khóa để đảm bảo rằng những nguy cơ này không dẫn đến kết quả không chính xác.

Một cách khác mà sự đồng thời đòi hỏi sự cẩn thận trong các trình điều khiển là một tiến trình có thể đang chờ đầu vào từ một thiết bị, nhưng ngắt báo hiệu sự xuất hiện của đầu vào có thể đến khi một tiến trình khác (hoặc hoàn toàn không có tiến trình nào) đang chạy. Do đó, các trình xử lý ngắt không được phép nghĩ về tiến trình hoặc mã mà chúng đã ngắt. Ví dụ, một trình xử lý ngắt không thể gọi `copyout` một cách an toàn với bảng trang của tiến trình hiện tại. Các trình xử lý ngắt thường thực hiện công việc tương đối ít (ví dụ: chỉ sao chép dữ liệu đầu vào vào một bộ đệm), và đánh thức mã nửa trên để thực hiện phần còn lại.

### 5.4 Ngắt hẹn giờ

Xv6 sử dụng các ngắt hẹn giờ để duy trì ý tưởng của nó về thời gian hiện tại và để chuyển đổi giữa các tiến trình bị ràng buộc bởi tính toán. Các ngắt hẹn giờ đến từ phần cứng đồng hồ được gắn vào mỗi CPU RISC-V. Xv6 lập trình cho phần cứng đồng hồ của mỗi CPU để ngắt CPU định kỳ.

Mã trong `start.c` (kernel/start.c:53) đặt một số bit điều khiển cho phép truy cập chế độ giám sát vào các thanh ghi điều khiển hẹn giờ, và sau đó yêu cầu ngắt hẹn giờ đầu tiên. Thanh ghi điều khiển thời gian chứa một bộ đếm mà phần cứng tăng lên với một tốc độ ổn định; điều này phục vụ như một khái niệm về thời gian hiện tại. Thanh ghi `stimecmp` chứa một thời điểm mà CPU sẽ gây ra một ngắt hẹn giờ; việc đặt `stimecmp` thành giá trị hiện tại của `time` cộng với `x` sẽ lên lịch một ngắt `x` đơn vị thời gian trong tương lai. Đối với mô phỏng RISC-V của qemu, 1.000.000 đơn vị thời gian là khoảng một phần mười giây.

Các ngắt hẹn giờ đến qua `usertrap` hoặc `kerneltrap` và `devintr`, giống như các ngắt thiết bị khác. Các ngắt hẹn giờ đến với các bit thấp của `scause` được đặt thành năm; `devintr` trong `trap.c` phát hiện tình huống này và gọi `clockintr` (kernel/trap.c:164). Hàm sau tăng `ticks`, cho phép hạt nhân theo dõi thời gian trôi qua. Việc tăng chỉ xảy ra trên một CPU, để tránh thời gian trôi qua nhanh hơn nếu có nhiều CPU. `clockintr` đánh thức bất kỳ tiến trình nào đang chờ trong lời gọi hệ thống `sleep`, và lên lịch ngắt hẹn giờ tiếp theo bằng cách ghi `stimecmp`.

`devintr` trả về 2 cho một ngắt hẹn giờ để chỉ ra cho `kerneltrap` hoặc `usertrap` rằng chúng nên gọi `yield` để các CPU có thể được ghép kênh giữa các tiến trình có thể chạy được.

Việc mã hạt nhân có thể bị ngắt bởi một ngắt hẹn giờ buộc phải chuyển đổi ngữ cảnh thông qua `yield` là một phần lý do tại sao mã sớm trong `usertrap` cẩn thận lưu trạng thái như `sepc` trước khi bật các ngắt. Các chuyển đổi ngữ cảnh này cũng có nghĩa là mã hạt nhân phải được viết với kiến thức rằng nó có thể di chuyển từ một CPU này sang một CPU khác mà không có cảnh báo.

### 5.5 Thế giới thực

Xv6, giống như nhiều hệ điều hành, cho phép các ngắt và thậm chí cả các chuyển đổi ngữ cảnh (thông qua `yield`) trong khi thực thi trong hạt nhân. Lý do cho điều này là để giữ thời gian phản hồi nhanh trong các lời gọi hệ thống phức tạp chạy trong một thời gian dài. Tuy nhiên, như đã lưu ý ở trên, việc cho phép các ngắt trong hạt nhân là nguồn gốc của một số phức tạp; kết quả là, một vài hệ điều hành chỉ cho phép các ngắt trong khi thực thi mã người dùng.

Hỗ trợ tất cả các thiết bị trên một máy tính điển hình một cách đầy đủ là rất nhiều công việc, bởi vì có nhiều thiết bị, các thiết bị có nhiều tính năng, và giao thức giữa thiết bị và trình điều khiển có thể phức tạp và được ghi chép kém. Trong nhiều hệ điều hành, các trình điều khiển chiếm nhiều mã hơn so với hạt nhân cốt lõi.

Trình điều khiển UART truy xuất dữ liệu một byte tại một thời điểm bằng cách đọc các thanh ghi điều khiển UART; mẫu này được gọi là *I/O được lập trình*, vì phần mềm đang điều khiển việc di chuyển dữ liệu. I/O được lập trình là đơn giản, nhưng quá chậm để được sử dụng ở tốc độ dữ liệu cao. Các thiết bị cần di chuyển nhiều dữ liệu ở tốc độ cao thường sử dụng *truy cập bộ nhớ trực tiếp* (DMA). Phần cứng thiết bị DMA ghi trực tiếp dữ liệu đến vào RAM, và đọc dữ liệu đi từ RAM. Các thiết bị đĩa và mạng hiện đại sử dụng DMA. Một trình điều khiển cho một thiết bị DMA sẽ chuẩn bị dữ liệu trong RAM, và sau đó sử dụng một lần ghi duy nhất vào một thanh ghi điều khiển để yêu cầu thiết bị xử lý dữ liệu đã chuẩn bị.

Các ngắt có ý nghĩa khi một thiết bị cần sự chú ý vào những thời điểm không thể đoán trước, và không quá thường xuyên. Nhưng các ngắt có chi phí CPU cao. Do đó, các thiết bị tốc độ cao, chẳng hạn như các bộ điều khiển mạng và đĩa, sử dụng các thủ thuật để giảm nhu cầu về các ngắt. Một thủ thuật là gây ra một ngắt duy nhất cho cả một lô các yêu cầu đến hoặc đi. Một thủ thuật khác là để trình điều khiển vô hiệu hóa hoàn toàn các ngắt, và kiểm tra thiết bị định kỳ để xem nó có cần chú ý hay không. Kỹ thuật này được gọi là *thăm dò*. Thăm dò có ý nghĩa nếu thiết bị thực hiện các hoạt động ở tốc độ cao, nhưng nó lãng phí thời gian CPU nếu thiết bị chủ yếu không hoạt động. Một số trình điều khiển chuyển đổi động giữa thăm dò và các ngắt tùy thuộc vào tải thiết bị hiện tại.

Trình điều khiển UART sao chép dữ liệu đến trước tiên vào một bộ đệm trong hạt nhân, và sau đó vào không gian người dùng. Điều này có ý nghĩa ở tốc độ dữ liệu thấp, nhưng một bản sao kép như vậy có thể làm giảm đáng kể hiệu suất cho các thiết bị tạo ra hoặc tiêu thụ dữ liệu rất nhanh. Một số hệ điều hành có thể di chuyển trực tiếp dữ liệu giữa các bộ đệm không gian người dùng và phần cứng thiết bị, thường là với DMA.

Như đã đề cập trong Chương 1, giao diện điều khiển xuất hiện cho các ứng dụng dưới dạng một tệp thông thường, và các ứng dụng đọc đầu vào và ghi đầu ra bằng cách sử dụng các lời gọi hệ thống `read` và `write`. Các ứng dụng có thể muốn kiểm soát các khía cạnh của một thiết bị không thể được thể hiện thông qua các lời gọi hệ thống tệp tiêu chuẩn (ví dụ: bật/tắt bộ đệm dòng trong trình điều khiển giao diện điều khiển). Các hệ điều hành Unix hỗ trợ lời gọi hệ thống `ioctl` cho các trường hợp như vậy.

Một số cách sử dụng máy tính yêu cầu hệ thống phải phản hồi trong một thời gian giới hạn. Ví dụ, trong các hệ thống an toàn-quan trọng, việc bỏ lỡ một thời hạn có thể dẫn đến thảm họa. Xv6 không phù hợp cho các cài đặt thời gian thực cứng. Các hệ điều hành cho thời gian thực cứng có xu hướng là các thư viện liên kết với ứng dụng theo cách cho phép phân tích để xác định thời gian phản hồi trong trường hợp xấu nhất. Xv6 cũng không phù hợp cho các ứng dụng thời gian thực mềm, khi việc bỏ lỡ một thời hạn đôi khi là chấp nhận được, bởi vì bộ lập lịch của xv6 quá đơn giản và nó có đường dẫn mã hạt nhân nơi các ngắt bị vô hiệu hóa trong một thời gian dài.

### 5.6 Bài tập

1.  Sửa đổi `uart.c` để không sử dụng các ngắt. Bạn cũng có thể cần phải sửa đổi `console.c`.

2.  Thêm một trình điều khiển cho một thẻ Ethernet.

## 6 Khóa

Hầu hết các hạt nhân, bao gồm cả xv6, xen kẽ việc thực thi nhiều hoạt động. Một nguồn xen kẽ là phần cứng đa bộ xử lý: các máy tính có nhiều CPU thực thi độc lập, chẳng hạn như RISC-V của xv6. Nhiều CPU này chia sẻ RAM vật lý, và xv6 khai thác việc chia sẻ để duy trì các cấu trúc dữ liệu mà tất cả các CPU đọc và ghi. Việc chia sẻ này làm tăng khả năng một CPU đọc một cấu trúc dữ liệu trong khi một CPU khác đang ở giữa quá trình cập nhật nó, hoặc thậm chí nhiều CPU cập nhật cùng một dữ liệu đồng thời; nếu không có thiết kế cẩn thận, việc truy cập song song như vậy có khả năng sẽ mang lại kết quả không chính xác hoặc một cấu trúc dữ liệu bị hỏng. Ngay cả trên một bộ xử lý đơn, hạt nhân có thể chuyển CPU giữa một số luồng, khiến việc thực thi của chúng bị xen kẽ. Cuối cùng, một trình xử lý ngắt thiết bị sửa đổi cùng một dữ liệu với một số mã có thể bị ngắt có thể làm hỏng dữ liệu nếu ngắt xảy ra vào đúng thời điểm sai. Từ *đồng thời* đề cập đến các tình huống trong đó nhiều luồng lệnh được xen kẽ, do song song đa bộ xử lý, chuyển đổi luồng hoặc ngắt.

Các hạt nhân có đầy đủ dữ liệu được truy cập đồng thời. Ví dụ, hai CPU có thể gọi `kalloc` đồng thời, do đó đồng thời bật ra khỏi đầu danh sách miễn phí. Các nhà thiết kế hạt nhân thích cho phép nhiều sự đồng thời, vì nó có thể mang lại hiệu suất tăng thông qua song song và tăng khả năng phản hồi. Tuy nhiên, kết quả là các nhà thiết kế hạt nhân phải tự thuyết phục về tính đúng đắn mặc dù có sự đồng thời như vậy. Có nhiều cách để đạt được mã đúng, một số dễ suy luận hơn những cách khác. Các chiến lược nhằm vào tính đúng đắn dưới sự đồng thời, và các sự trừu tượng hóa hỗ trợ chúng, được gọi là *kỹ thuật kiểm soát đồng thời*.

Xv6 sử dụng một số kỹ thuật kiểm soát đồng thời, tùy thuộc vào tình huống; nhiều kỹ thuật khác là có thể. Chương này tập trung vào một kỹ thuật được sử dụng rộng rãi: *khóa*. Một khóa cung cấp *loại trừ lẫn nhau*, đảm bảo rằng chỉ một CPU tại một thời điểm có thể giữ khóa. Nếu lập trình viên liên kết một khóa với mỗi mục dữ liệu được chia sẻ, và mã luôn giữ khóa được liên kết khi sử dụng một mục, thì mục đó sẽ chỉ được sử dụng bởi một CPU tại một thời điểm. Trong tình huống này, chúng ta nói rằng khóa *bảo vệ* mục dữ liệu. Mặc dù các khóa là một cơ chế kiểm soát đồng thời dễ hiểu, nhược điểm của các khóa là chúng có thể hạn chế hiệu suất, bởi vì chúng tuần tự hóa các hoạt động đồng thời.

Phần còn lại của chương này giải thích tại sao xv6 cần các khóa, cách xv6 triển khai chúng, và cách nó sử dụng chúng.

### 6.1 Tranh chấp (Races)

Là một ví dụ về lý do tại sao chúng ta cần các khóa, hãy xem xét hai tiến trình có các con đã thoát gọi `wait` trên hai CPU khác nhau. `wait` giải phóng bộ nhớ của con. Do đó, trên mỗi CPU, hạt nhân sẽ gọi `kfree` để giải phóng các trang bộ nhớ của các con. Bộ cấp phát hạt nhân duy trì một danh sách liên kết: `kalloc()` (kernel/kalloc.c:69) bật một trang bộ nhớ ra khỏi một danh sách các trang miễn phí, và `kfree()` (kernel/kalloc.c:47) đẩy một trang vào danh sách miễn phí. Để có hiệu suất tốt nhất, chúng ta có thể hy vọng rằng các `kfree` của hai tiến trình cha mẹ sẽ thực thi song song mà không cần phải đợi nhau, nhưng điều này sẽ không đúng với việc triển khai `kfree` của xv6.

Hình 6.1 minh họa chi tiết hơn về cài đặt: danh sách liên kết của các trang miễn phí nằm trong bộ nhớ được chia sẻ bởi hai CPU, thao tác danh sách bằng cách sử dụng các lệnh tải và lưu trữ. (Trong thực tế, các bộ xử lý có các bộ nhớ cache, nhưng các hệ thống đa bộ xử lý về mặt khái niệm hoạt động như thể có một bộ nhớ chia sẻ duy nhất.) Nếu không có các yêu cầu đồng thời, bạn có thể triển khai một hoạt động đẩy danh sách như sau:

```c
1  struct element {
2      int data;
3      struct element *next;
4  };
5
6  struct element *list = 0;
7
8  void
9  push(int data)
10 {
11     struct element *l;
12
13     l = malloc(sizeof *l);
14     l->data = data;
15     l->next = list;
16     list = l;
17 }
```

Việc triển khai này là đúng nếu được thực thi một cách cô lập. Tuy nhiên, mã không đúng nếu nhiều hơn một bản sao thực thi đồng thời. Nếu hai CPU thực thi `push` cùng một lúc, cả hai có thể thực thi dòng 15 như trong Hình 6.1, trước khi một trong hai thực thi dòng 16, điều này dẫn đến một kết quả không chính xác như được minh họa trong Hình 6.2. Sau đó, sẽ có hai phần tử danh sách với `next` được đặt thành giá trị cũ của `list`. Khi hai phép gán cho `list` xảy ra ở dòng 16, phép gán thứ hai sẽ ghi đè lên phép gán đầu tiên; phần tử liên quan đến phép gán đầu tiên sẽ bị mất.

Việc cập nhật bị mất ở dòng 16 là một ví dụ về một *tranh chấp*. Một tranh chấp là một tình huống trong đó một vị trí bộ nhớ được truy cập đồng thời, và ít nhất một lần truy cập là một lần ghi. Một tranh chấp thường là một dấu hiệu của một lỗi, hoặc là một cập nhật bị mất (nếu các lần truy cập là các lần ghi) hoặc một lần đọc của một cấu trúc dữ liệu được cập nhật không hoàn chỉnh. Kết quả của một tranh chấp phụ thuộc vào mã máy được tạo bởi trình biên dịch, thời gian của hai CPU liên quan, và cách các hoạt động bộ nhớ của chúng được sắp xếp bởi hệ thống bộ nhớ, điều này có thể làm cho các lỗi do tranh chấp gây ra khó tái tạo và gỡ lỗi. Ví dụ, việc thêm các câu lệnh in trong khi gỡ lỗi `push` có thể thay đổi thời gian của việc thực thi đủ để làm cho tranh chấp biến mất.

Cách thông thường để tránh các tranh chấp là sử dụng một khóa. Các khóa đảm bảo *loại trừ lẫn nhau*, do đó chỉ một CPU tại một thời điểm có thể thực thi các dòng nhạy cảm của `push`; điều này làm cho kịch bản trên không thể xảy ra. Phiên bản được khóa chính xác của mã trên chỉ thêm một vài dòng (được tô sáng màu vàng):

```c
6  struct element *list = 0;
7  struct lock listlock;
8
9  void
10 push(int data)
11 {
12     struct element *l;
13     l = malloc(sizeof *l);
14     l->data = data;
15     acquire(&listlock);
16     l->next = list;
17     list = l;
18     release(&listlock);
19 }
```

Chuỗi các lệnh giữa `acquire` và `release` thường được gọi là một *phần quan trọng*. Khóa thường được cho là đang bảo vệ `list`.

Khi chúng ta nói rằng một khóa bảo vệ dữ liệu, chúng ta thực sự có nghĩa là khóa bảo vệ một tập hợp các *bất biến* áp dụng cho dữ liệu. Các bất biến là các thuộc tính của các cấu trúc dữ liệu được duy trì qua các hoạt động. Thông thường, hành vi đúng của một hoạt động phụ thuộc vào các bất biến là đúng khi hoạt động bắt đầu. Hoạt động có thể tạm thời vi phạm các bất biến nhưng phải thiết lập lại chúng trước khi kết thúc. Ví dụ, trong trường hợp danh sách liên kết, bất biến là `list` trỏ đến phần tử đầu tiên trong danh sách và trường `next` của mỗi phần tử trỏ đến phần tử tiếp theo. Việc triển khai `push` vi phạm bất biến này một cách tạm thời: trong dòng 17, `l` trỏ đến phần tử danh sách tiếp theo, nhưng `list` chưa trỏ đến `l` (được thiết lập lại ở dòng 18). Tranh chấp mà chúng ta đã kiểm tra ở trên đã xảy ra bởi vì một CPU thứ hai đã thực thi mã phụ thuộc vào các bất biến của danh sách trong khi chúng (tạm thời) bị vi phạm. Việc sử dụng đúng một khóa đảm bảo rằng chỉ một CPU tại một thời điểm có thể hoạt động trên cấu trúc dữ liệu trong phần quan trọng, do đó không có CPU nào sẽ thực thi một hoạt động cấu trúc dữ liệu khi các bất biến của cấu trúc dữ liệu không giữ.

Bạn có thể nghĩ về một khóa như là *tuần tự hóa* các phần quan trọng đồng thời để chúng chạy một lần, và do đó bảo tồn các bất biến (giả sử các phần quan trọng là đúng trong sự cô lập). Bạn cũng có thể nghĩ về các phần quan trọng được bảo vệ bởi cùng một khóa là *nguyên tử* đối với nhau, do đó mỗi phần chỉ thấy tập hợp các thay đổi hoàn chỉnh từ các phần quan trọng trước đó, và không bao giờ thấy các cập nhật được hoàn thành một phần.

Mặc dù hữu ích cho tính đúng đắn, các khóa vốn đã hạn chế hiệu suất. Ví dụ, nếu hai tiến trình gọi `kfree` đồng thời, các khóa sẽ tuần tự hóa hai phần quan trọng, do đó không có lợi ích gì từ việc chạy chúng trên các CPU khác nhau. Chúng ta nói rằng nhiều tiến trình *xung đột* nếu chúng muốn cùng một khóa cùng một lúc, hoặc khóa đó gặp phải *sự tranh chấp*. Một thách thức lớn trong thiết kế hạt nhân là tránh sự tranh chấp khóa để theo đuổi tính song song. Xv6 làm ít điều đó, nhưng các hạt nhân tinh vi tổ chức các cấu trúc dữ liệu và các thuật toán đặc biệt để tránh sự tranh chấp khóa. Trong ví dụ danh sách, một hạt nhân có thể duy trì một danh sách miễn phí riêng cho mỗi CPU và chỉ chạm vào danh sách miễn phí của một CPU khác nếu danh sách của CPU hiện tại trống và nó phải đánh cắp bộ nhớ từ một CPU khác. Các trường hợp sử dụng khác có thể yêu cầu các thiết kế phức tạp hơn.

Việc đặt các khóa cũng quan trọng đối với hiệu suất. Ví dụ, sẽ đúng nếu di chuyển `acquire` sớm hơn trong `push`, trước dòng 13. Nhưng điều này có khả năng sẽ làm giảm hiệu suất bởi vì sau đó các lệnh gọi đến `malloc` sẽ được tuần tự hóa. Phần “Sử dụng các khóa” bên dưới cung cấp một số hướng dẫn về nơi chèn các lệnh gọi `acquire` và `release`.

### 6.2 Mã: Khóa

Xv6 có hai loại khóa: khóa quay và khóa ngủ. Chúng ta sẽ bắt đầu với các khóa quay. Xv6 biểu diễn một khóa quay như một `struct spinlock` (kernel/spinlock.h:2). Trường quan trọng trong cấu trúc là `locked`, một từ là 0 khi khóa có sẵn và khác 0 khi nó được giữ. Về mặt logic, xv6 nên có được một khóa bằng cách thực thi mã như

```c
21 void
22 acquire(struct spinlock *lk) // không hoạt động!
23 {
24     for(;;) {
25         if(lk->locked == 0) {
26             lk->locked = 1;
27             break;
28         }
29     }
30 }
```

Thật không may, việc triển khai này không đảm bảo loại trừ lẫn nhau trên một bộ đa xử lý. Có thể xảy ra trường hợp hai CPU đồng thời đạt đến dòng 25, thấy rằng `lk->locked` là 0, và sau đó cả hai đều lấy khóa bằng cách thực thi dòng 26. Tại thời điểm này, hai CPU khác nhau giữ khóa, vi phạm thuộc tính loại trừ lẫn nhau. Điều chúng ta cần là một cách để làm cho các dòng 25 và 26 thực thi như một bước *nguyên tử* (tức là, không thể chia cắt).

Bởi vì các khóa được sử dụng rộng rãi, các bộ xử lý đa lõi thường cung cấp các lệnh thực hiện một phiên bản nguyên tử của các dòng 25 và 26. Trên RISC-V, lệnh này là `amoswap r, a`. `amoswap` đọc giá trị tại địa chỉ bộ nhớ `a`, ghi nội dung của thanh ghi `r` vào địa chỉ đó, và đặt giá trị mà nó đã đọc vào `r`. Tức là, nó hoán đổi nội dung của thanh ghi và địa chỉ bộ nhớ. Nó thực hiện chuỗi này một cách nguyên tử, sử dụng phần cứng đặc biệt để ngăn bất kỳ CPU nào khác sử dụng địa chỉ bộ nhớ giữa lần đọc và lần ghi.

`acquire` của Xv6 (kernel/spinlock.c:22) sử dụng lệnh gọi thư viện C di động `__sync_lock_test_and_set`, rút gọn thành lệnh `amoswap`; giá trị trả về là nội dung cũ (đã hoán đổi) của `lk->locked`. Hàm `acquire` bao bọc việc hoán đổi trong một vòng lặp, thử lại (quay) cho đến khi nó có được khóa. Mỗi lần lặp hoán đổi một vào `lk->locked` và kiểm tra giá trị trước đó; nếu giá trị trước đó là 0, thì chúng ta đã có được khóa, và việc hoán đổi sẽ đặt `lk->locked` thành một. Nếu giá trị trước đó là một, thì một CPU khác giữ khóa, và thực tế là chúng ta đã hoán đổi một cách nguyên tử một vào `lk->locked` không thay đổi giá trị của nó.

Sau khi khóa được lấy, `acquire` ghi lại, để gỡ lỗi, CPU đã lấy khóa. Trường `lk->cpu` được bảo vệ bởi khóa và chỉ phải được thay đổi trong khi giữ khóa.

Hàm `release` (kernel/spinlock.c:47) là ngược lại của `acquire`: nó xóa trường `lk->cpu` và sau đó giải phóng khóa. Về mặt khái niệm, việc giải phóng chỉ yêu cầu gán 0 cho `lk->locked`. Tiêu chuẩn C cho phép các trình biên dịch thực hiện một phép gán với nhiều lệnh lưu trữ, vì vậy một phép gán C có thể không nguyên tử đối với mã đồng thời. Thay vào đó, `release` sử dụng hàm thư viện C `__sync_lock_release` thực hiện một phép gán nguyên tử. Hàm này cũng rút gọn thành một lệnh `amoswap` của RISC-V.

### 6.3 Mã: Sử dụng các khóa

Xv6 sử dụng các khóa ở nhiều nơi để tránh các tranh chấp. Như đã mô tả ở trên, `kalloc` (kernel/kalloc.c:69) và `kfree` (kernel/kalloc.c:47) tạo thành một ví dụ điển hình. Hãy thử các Bài tập 1 và 2 để xem điều gì xảy ra nếu các hàm đó bỏ qua các khóa. Bạn có thể sẽ thấy rằng rất khó để kích hoạt hành vi không chính xác, cho thấy rằng rất khó để kiểm tra một cách đáng tin cậy xem mã có không có các lỗi khóa và các tranh chấp hay không. Xv6 rất có thể có các tranh chấp chưa được phát hiện.

Một phần khó khăn về việc sử dụng các khóa là quyết định có bao nhiêu khóa để sử dụng và dữ liệu và các bất biến nào mỗi khóa nên bảo vệ. Có một vài nguyên tắc cơ bản. Đầu tiên, bất cứ khi nào một biến có thể được ghi bởi một CPU cùng lúc một CPU khác có thể đọc hoặc ghi nó, một khóa nên được sử dụng để giữ cho hai hoạt động không chồng chéo. Thứ hai, hãy nhớ rằng các khóa bảo vệ các bất biến: nếu một bất biến liên quan đến nhiều vị trí bộ nhớ, thông thường tất cả chúng cần được bảo vệ bởi một khóa duy nhất để đảm bảo rằng bất biến được duy trì.

Các quy tắc trên cho biết khi nào các khóa là cần thiết nhưng không nói gì về khi nào các khóa là không cần thiết, và điều quan trọng đối với hiệu quả là không khóa quá nhiều, bởi vì các khóa làm giảm tính song song. Nếu tính song song không quan trọng, thì người ta có thể sắp xếp để chỉ có một luồng duy nhất và không phải lo lắng về các khóa. Một hạt nhân đơn giản có thể làm điều này trên một bộ đa xử lý bằng cách có một khóa duy nhất phải được lấy khi vào hạt nhân và được giải phóng khi thoát khỏi hạt nhân (mặc dù các lời gọi hệ thống chặn như đọc ống dẫn hoặc `wait` sẽ gây ra một vấn đề). Nhiều hệ điều hành bộ xử lý đơn đã được chuyển đổi để chạy trên các bộ đa xử lý bằng cách sử dụng cách tiếp cận này, đôi khi được gọi là “khóa hạt nhân lớn”, nhưng cách tiếp cận này hy sinh tính song song: chỉ một CPU có thể thực thi trong hạt nhân tại một thời điểm. Nếu hạt nhân thực hiện bất kỳ tính toán nặng nào, sẽ hiệu quả hơn nếu sử dụng một tập hợp lớn hơn các khóa chi tiết hơn, để hạt nhân có thể thực thi trên nhiều CPU đồng thời.

Là một ví dụ về khóa hạt thô, bộ cấp phát `kalloc.c` của xv6 có một danh sách miễn phí duy nhất được bảo vệ bởi một khóa duy nhất. Nếu nhiều tiến trình trên các CPU khác nhau cố gắng phân bổ các trang cùng một lúc, mỗi tiến trình sẽ phải đợi đến lượt mình bằng cách quay trong `acquire`. Việc quay lãng phí thời gian CPU, vì đó không phải là công việc hữu ích. Nếu sự tranh chấp cho khóa lãng phí một phần đáng kể thời gian CPU, có lẽ hiệu suất có thể được cải thiện bằng cách thay đổi thiết kế bộ cấp phát để có nhiều danh sách miễn phí, mỗi danh sách có khóa riêng, để cho phép phân bổ song song thực sự.

Là một ví dụ về khóa hạt mịn, xv6 có một khóa riêng cho mỗi tệp, để các tiến trình thao tác các tệp khác nhau thường có thể tiến hành mà không cần đợi khóa của nhau. Lược đồ khóa tệp có thể được làm chi tiết hơn nữa nếu người ta muốn cho phép các tiến trình ghi đồng thời vào các vùng khác nhau của cùng một tệp. Cuối cùng, các quyết định về độ chi tiết của khóa cần được thúc đẩy bởi các phép đo hiệu suất cũng như các cân nhắc về độ phức tạp.

Khi các chương tiếp theo giải thích từng phần của xv6, chúng sẽ đề cập đến các ví dụ về việc xv6 sử dụng các khóa để đối phó với sự đồng thời. Để xem trước, Hình 6.3 liệt kê tất cả các khóa trong xv6.

### 6.4 Khóa chết và thứ tự khóa

Nếu một đường dẫn mã thông qua hạt nhân phải giữ nhiều khóa cùng một lúc, điều quan trọng là tất cả các đường dẫn mã đều lấy các khóa đó theo cùng một thứ tự. Nếu không, có nguy cơ *khóa chết*. Giả sử hai đường dẫn mã trong xv6 cần các khóa A và B, nhưng đường dẫn mã 1 lấy các khóa theo thứ tự A rồi B, và đường dẫn mã khác lấy chúng theo thứ tự B rồi A. Giả sử luồng T1 thực thi đường dẫn mã 1 và lấy khóa A, và luồng T2 thực thi đường dẫn mã 2 và lấy khóa B. Tiếp theo, T1 sẽ cố gắng lấy khóa B, và T2 sẽ cố gắng lấy khóa A. Cả hai lần lấy sẽ chặn vô thời hạn, bởi vì trong cả hai trường hợp, luồng khác giữ khóa cần thiết, và sẽ không giải phóng nó cho đến khi lần lấy của nó trả về. Để tránh các khóa chết như vậy, tất cả các đường dẫn mã phải lấy các khóa theo cùng một thứ tự. Nhu cầu về một thứ tự lấy khóa toàn cục có nghĩa là các khóa thực sự là một phần của đặc tả của mỗi hàm: người gọi phải gọi các hàm theo cách khiến các khóa được lấy theo thứ tự đã thỏa thuận.

Xv6 có nhiều chuỗi thứ tự khóa có độ dài hai liên quan đến các khóa trên mỗi tiến trình (khóa trong mỗi `struct proc`) do cách `sleep` hoạt động (xem Chương 7). Ví dụ, `consoleintr` (kernel/console.c:136) là quy trình ngắt xử lý các ký tự đã gõ. Khi một dòng mới đến, bất kỳ tiến trình nào đang chờ đầu vào giao diện điều khiển sẽ được đánh thức. Để làm điều này, `consoleintr` giữ `cons.lock` trong khi gọi `wakeup`, `wakeup` lấy khóa của tiến trình đang chờ để đánh thức nó. Do đó, thứ tự khóa toàn cục tránh khóa chết bao gồm quy tắc rằng `cons.lock` phải được lấy trước bất kỳ khóa tiến trình nào. Mã hệ thống tệp chứa các chuỗi khóa dài nhất của xv6. Ví dụ, việc tạo một tệp yêu cầu đồng thời giữ một khóa trên thư mục, một khóa trên inode của tệp mới, một khóa trên một bộ đệm khối đĩa, `vdisk_lock` của trình điều khiển đĩa, và `p->lock` của tiến trình đang gọi. Để tránh khóa chết, mã hệ thống tệp luôn lấy các khóa theo thứ tự được đề cập trong câu trước.

Việc tôn trọng một thứ tự tránh khóa chết toàn cục có thể khó khăn một cách đáng ngạc nhiên. Đôi khi thứ tự khóa xung đột với cấu trúc chương trình logic, ví dụ: có lẽ mô-đun mã M1 gọi mô-đun M2, nhưng thứ tự khóa yêu cầu một khóa trong M2 phải được lấy trước một khóa trong M1. Đôi khi danh tính của các khóa không được biết trước, có lẽ bởi vì một khóa phải được giữ để khám phá danh tính của khóa sẽ được lấy tiếp theo. Loại tình huống này phát sinh trong hệ thống tệp khi nó tra cứu các thành phần kế tiếp trong một tên đường dẫn, và trong mã cho `wait` và `exit` khi chúng tìm kiếm bảng các tiến trình tìm kiếm các tiến trình con. Cuối cùng, nguy cơ khóa chết thường là một ràng buộc về mức độ chi tiết mà người ta có thể tạo ra một lược đồ khóa, vì nhiều khóa hơn thường có nghĩa là có nhiều cơ hội hơn cho khóa chết. Nhu cầu tránh khóa chết thường là một yếu tố chính trong việc triển khai hạt nhân.

### 6.5 Khóa đệ quy

Có vẻ như một số khóa chết và các thách thức về thứ tự khóa có thể được tránh bằng cách sử dụng *khóa đệ quy*, còn được gọi là *khóa đệ quy*. Ý tưởng là nếu khóa được giữ bởi một tiến trình và nếu tiến trình đó cố gắng lấy lại khóa, thì hạt nhân có thể chỉ cho phép điều này (vì tiến trình đã có khóa), thay vì gọi `panic`, như hạt nhân xv6 làm.

Tuy nhiên, hóa ra các khóa đệ quy làm cho việc suy luận về sự đồng thời khó khăn hơn: các khóa đệ quy phá vỡ trực giác rằng các khóa gây ra các phần quan trọng là nguyên tử đối với các phần quan trọng khác. Hãy xem xét các hàm `f` và `g` sau, và một hàm giả định `h`:

```c
struct spinlock lock;
int data = 0; // được bảo vệ bởi khóa

f() {
    acquire(&lock);
    if (data == 0) {
        call_once();
        h();
        data = 1;
    }
    release(&lock);
}

g() {
    acquire(&lock);
    if (data == 0) {
        call_once();
        data = 1;
    }
    release(&lock);
}

h() {
    ...
}
```

Nhìn vào đoạn mã này, trực giác là `call_once` sẽ chỉ được gọi một lần: hoặc bởi `f`, hoặc bởi `g`, nhưng không phải cả hai. Nhưng nếu các khóa đệ quy được cho phép, và `h` tình cờ gọi `g`, `call_once` sẽ được gọi hai lần. Nếu các khóa đệ quy không được cho phép, thì `h` gọi `g` dẫn đến một khóa chết, điều này cũng không tuyệt vời. Nhưng, giả sử sẽ là một lỗi nghiêm trọng nếu gọi `call_once`, một khóa chết là thích hợp hơn. Nhà phát triển hạt nhân sẽ quan sát khóa chết (hạt nhân hoảng loạn) và có thể sửa mã để tránh nó, trong khi việc gọi `call_once` hai lần có thể âm thầm dẫn đến một lỗi khó theo dõi.

Vì lý do này, xv6 sử dụng các khóa không đệ quy đơn giản hơn để hiểu. Tuy nhiên, miễn là các lập trình viên ghi nhớ các quy tắc khóa, cả hai cách tiếp cận đều có thể được thực hiện. Nếu xv6 sử dụng các khóa đệ quy, người ta sẽ phải sửa đổi `acquire` để nhận thấy rằng khóa hiện đang được giữ bởi luồng đang gọi. Người ta cũng sẽ phải thêm một bộ đếm các lần lấy lồng nhau vào `struct spinlock`, theo phong cách tương tự như `push_off`, được thảo luận tiếp theo.

### 6.6 Khóa và trình xử lý ngắt

Một số khóa quay của xv6 bảo vệ dữ liệu được sử dụng bởi cả các luồng và các trình xử lý ngắt. Ví dụ, trình xử lý ngắt hẹn giờ `clockintr` có thể tăng `ticks` (kernel/trap.c:164) vào khoảng cùng thời gian một luồng hạt nhân đọc `ticks` trong `sys_sleep` (kernel/sysproc.c:61). Khóa `tickslock` tuần tự hóa hai lần truy cập.

Sự tương tác của các khóa quay và các ngắt làm tăng một nguy cơ tiềm tàng. Giả sử `sys_sleep` giữ `tickslock`, và CPU của nó bị ngắt bởi một ngắt hẹn giờ. `clockintr` sẽ cố gắng lấy `tickslock`, thấy nó đã được giữ, và đợi cho đến khi nó được giải phóng. Trong tình huống này, `tickslock` sẽ không bao giờ được giải phóng: chỉ có `sys_sleep` mới có thể giải phóng nó, nhưng `sys_sleep` sẽ không tiếp tục chạy cho đến khi `clockintr` trả về. Vì vậy, CPU sẽ bị khóa chết, và bất kỳ mã nào cần một trong hai khóa cũng sẽ bị đóng băng.

Để tránh tình huống này, nếu một khóa quay được sử dụng bởi một trình xử lý ngắt, một CPU không bao giờ được giữ khóa đó với các ngắt được bật. Xv6 bảo thủ hơn: khi một CPU lấy bất kỳ khóa nào, xv6 luôn vô hiệu hóa các ngắt trên CPU đó. Các ngắt vẫn có thể xảy ra trên các CPU khác, vì vậy `acquire` của một ngắt có thể đợi một luồng giải phóng một khóa quay; chỉ là không phải trên cùng một CPU.

Xv6 bật lại các ngắt khi một CPU không giữ khóa quay nào; nó phải thực hiện một chút ghi sổ để đối phó với các phần quan trọng lồng nhau. `acquire` gọi `push_off` (kernel/spinlock.c:89) và `release` gọi `pop_off` (kernel/spinlock.c:100) để theo dõi mức độ lồng nhau của các khóa trên CPU hiện tại. Khi số lượng đó đạt đến 0, `pop_off` sẽ khôi phục trạng thái bật ngắt đã tồn tại khi bắt đầu phần quan trọng ngoài cùng. Các hàm `intr_off` và `intr_on` thực thi các lệnh RISC-V để vô hiệu hóa và bật các ngắt, tương ứng.

Điều quan trọng là `acquire` gọi `push_off` nghiêm ngặt trước khi đặt `lk->locked` (kernel/spinlock.c:28). Nếu hai cái bị đảo ngược, sẽ có một cửa sổ ngắn khi khóa được giữ với các ngắt được bật, và một ngắt được hẹn giờ không may sẽ làm khóa chết hệ thống. Tương tự, điều quan trọng là `release` gọi `pop_off` chỉ sau khi giải phóng khóa (kernel/spinlock.c:66).

### 6.7 Thứ tự lệnh và bộ nhớ

Việc nghĩ về các chương trình thực thi theo thứ tự mà các câu lệnh mã nguồn xuất hiện là tự nhiên. Đó là một mô hình tinh thần hợp lý cho mã đơn luồng, nhưng không chính xác khi nhiều luồng tương tác thông qua bộ nhớ chia sẻ [2, 4]. Một lý do là các trình biên dịch phát ra các lệnh tải và lưu trữ theo các thứ tự khác với các thứ tự được ngụ ý bởi mã nguồn, và có thể bỏ qua chúng hoàn toàn (ví dụ bằng cách lưu trữ dữ liệu trong các thanh ghi). Một lý do khác là CPU có thể thực thi các lệnh không theo thứ tự để tăng hiệu suất. Ví dụ, một CPU có thể nhận thấy rằng trong một chuỗi các lệnh A và B nối tiếp, chúng không phụ thuộc vào nhau. CPU có thể bắt đầu lệnh B trước, hoặc vì các đầu vào của nó đã sẵn sàng trước các đầu vào của A, hoặc để chồng chéo việc thực thi của A và B.

Là một ví dụ về những gì có thể sai, trong mã này cho `push`, sẽ là một thảm họa nếu trình biên dịch hoặc CPU di chuyển việc lưu trữ tương ứng với dòng 4 đến một điểm sau khi giải phóng ở dòng 6:

```c
1  l = malloc(sizeof *l);
2  l->data = data;
3  acquire(&listlock);
4  l->next = list;
5  list = l;
6  release(&listlock);
```

Nếu một sự sắp xếp lại như vậy xảy ra, sẽ có một cửa sổ trong đó một CPU khác có thể lấy khóa và quan sát `list` được cập nhật, nhưng thấy một `list->next` chưa được khởi tạo.

Tin tốt là các trình biên dịch và CPU giúp các lập trình viên đồng thời bằng cách tuân theo một tập hợp các quy tắc được gọi là *mô hình bộ nhớ*, và bằng cách cung cấp một số nguyên thủy để giúp các lập trình viên kiểm soát việc sắp xếp lại.

Để yêu cầu phần cứng và trình biên dịch không sắp xếp lại, xv6 sử dụng `__sync_synchronize()` trong cả `acquire` (kernel/spinlock.c:22) và `release` (kernel/spinlock.c:47). `__sync_synchronize()` là một *rào cản bộ nhớ*: nó yêu cầu trình biên dịch và CPU không sắp xếp lại các lần tải hoặc lưu trữ qua rào cản. Các rào cản trong `acquire` và `release` của xv6 buộc phải có thứ tự trong hầu hết các trường hợp quan trọng, vì xv6 sử dụng các khóa xung quanh các lần truy cập vào dữ liệu được chia sẻ. Chương 9 thảo luận về một vài ngoại lệ.

### 6.8 Khóa ngủ (Sleep locks)

Đôi khi xv6 cần giữ một khóa trong một thời gian dài. Ví dụ, hệ thống tệp (Chương 8) giữ một tệp bị khóa trong khi đọc và ghi nội dung của nó trên đĩa, và các hoạt động đĩa này có thể mất hàng chục mili giây. Việc giữ một khóa quay lâu như vậy sẽ dẫn đến lãng phí nếu một tiến trình khác muốn lấy nó, vì tiến trình lấy sẽ lãng phí thời gian CPU trong một thời gian dài trong khi quay. Một nhược điểm khác của các khóa quay là một tiến trình không thể nhường CPU trong khi giữ một khóa quay; chúng ta muốn làm điều này để các tiến trình khác có thể sử dụng CPU trong khi tiến trình có khóa đợi đĩa. Việc nhường trong khi giữ một khóa quay là bất hợp pháp vì nó có thể dẫn đến khóa chết nếu một luồng thứ hai sau đó cố gắng lấy khóa quay; vì `acquire` không nhường CPU, việc quay của luồng thứ hai có thể ngăn luồng đầu tiên chạy và giải phóng khóa. Việc nhường trong khi giữ một khóa cũng sẽ vi phạm yêu cầu rằng các ngắt phải tắt trong khi một khóa quay được giữ. Do đó, chúng ta muốn một loại khóa nhường CPU trong khi chờ đợi để lấy, và cho phép nhường (và các ngắt) trong khi khóa được giữ.

Xv6 cung cấp các khóa như vậy dưới dạng *khóa ngủ*. `acquiresleep` (kernel/sleeplock.c:22) nhường CPU trong khi chờ đợi, sử dụng các kỹ thuật sẽ được giải thích trong Chương 7. Ở cấp độ cao, một khóa ngủ có một trường `locked` được bảo vệ bởi một khóa quay, và lệnh gọi của `acquiresleep` đến `sleep` nhường CPU một cách nguyên tử và giải phóng khóa quay. Kết quả là các luồng khác có thể thực thi trong khi `acquiresleep` đợi.

Bởi vì các khóa ngủ để các ngắt được bật, chúng không thể được sử dụng trong các trình xử lý ngắt. Bởi vì `acquiresleep` có thể nhường CPU, các khóa ngủ không thể được sử dụng bên trong các phần quan trọng của khóa quay (mặc dù các khóa quay có thể được sử dụng bên trong các phần quan trọng của khóa ngủ).

Các khóa quay phù hợp nhất với các phần quan trọng ngắn, vì việc chờ đợi chúng lãng phí thời gian CPU; các khóa ngủ hoạt động tốt cho các hoạt động dài.

### 6.9 Thế giới thực

Lập trình với các khóa vẫn còn nhiều thách thức mặc dù nhiều năm nghiên cứu về các nguyên thủy đồng thời và tính song song. Tốt nhất là che giấu các khóa trong các cấu trúc cấp cao hơn như các hàng đợi được đồng bộ hóa, mặc dù xv6 không làm điều này. Nếu bạn lập trình với các khóa, nên sử dụng một công cụ cố gắng xác định các tranh chấp, bởi vì rất dễ bỏ sót một bất biến yêu cầu một khóa.

Hầu hết các hệ điều hành đều hỗ trợ các luồng POSIX (Pthreads), cho phép một tiến trình người dùng có nhiều luồng chạy đồng thời trên các CPU khác nhau. Pthreads có hỗ trợ cho các khóa cấp người dùng, các rào cản, v.v. Pthreads cũng cho phép một lập trình viên tùy chọn chỉ định rằng một khóa nên là đệ quy.

Hỗ trợ Pthreads ở cấp người dùng yêu cầu sự hỗ trợ từ hệ điều hành. Ví dụ, nên có trường hợp nếu một pthread chặn trong một lời gọi hệ thống, một pthread khác của cùng một tiến trình sẽ có thể chạy trên CPU đó. Là một ví dụ khác, nếu một pthread thay đổi không gian địa chỉ của tiến trình của nó (ví dụ: ánh xạ hoặc bỏ ánh xạ bộ nhớ), hạt nhân phải sắp xếp để các CPU khác chạy các luồng của cùng một tiến trình cập nhật các bảng trang phần cứng của chúng để phản ánh sự thay đổi trong không gian địa chỉ.

Có thể triển khai các khóa mà không cần các lệnh nguyên tử [10], nhưng nó tốn kém, và hầu hết các hệ điều hành đều sử dụng các lệnh nguyên tử.

Các khóa có thể tốn kém nếu nhiều CPU cố gắng lấy cùng một khóa cùng một lúc. Nếu một CPU có một khóa được lưu trong bộ nhớ cache cục bộ của nó, và một CPU khác phải lấy khóa, thì lệnh nguyên tử để cập nhật dòng cache chứa khóa phải di chuyển dòng từ bộ nhớ cache của một CPU sang bộ nhớ cache của CPU khác, và có lẽ vô hiệu hóa bất kỳ bản sao nào khác của dòng cache. Việc tìm nạp một dòng cache từ bộ nhớ cache của một CPU khác có thể tốn kém hơn nhiều bậc so với việc tìm nạp một dòng từ bộ nhớ cache cục bộ.

Để tránh các chi phí liên quan đến các khóa, nhiều hệ điều hành sử dụng các cấu trúc dữ liệu và các thuật toán không có khóa [6, 12]. Ví dụ, có thể triển khai một danh sách liên kết như danh sách ở đầu chương không yêu cầu khóa trong quá trình tìm kiếm danh sách, và một lệnh nguyên tử để chèn một mục vào danh sách. Lập trình không có khóa phức tạp hơn, tuy nhiên, so với lập trình khóa; ví dụ, người ta phải lo lắng về việc sắp xếp lại lệnh và bộ nhớ. Lập trình với các khóa đã khó, vì vậy xv6 tránh sự phức tạp bổ sung của lập trình không có khóa.

### 6.10 Bài tập

1.  Bình luận các lệnh gọi đến `acquire` và `release` trong `kalloc` (kernel/kalloc.c:69). Điều này dường như sẽ gây ra các vấn đề cho mã hạt nhân gọi `kalloc`; bạn mong đợi thấy những triệu chứng nào? Khi bạn chạy xv6, bạn có thấy những triệu chứng này không? Còn khi chạy `usertests` thì sao? Nếu bạn không thấy vấn đề, tại sao không? Xem liệu bạn có thể gây ra một vấn đề bằng cách chèn các vòng lặp giả vào phần quan trọng của `kalloc` không.

2.  Giả sử rằng bạn thay vào đó đã bình luận việc khóa trong `kfree` (sau khi khôi phục việc khóa trong `kalloc`). Bây giờ có thể sai điều gì? Việc thiếu các khóa trong `kfree` có ít gây hại hơn so với trong `kalloc` không?

3.  Nếu hai CPU gọi `kalloc` cùng một lúc, một CPU sẽ phải đợi CPU kia, điều này không tốt cho hiệu suất. Sửa đổi `kalloc.c` để có nhiều tính song song hơn, để các lệnh gọi đồng thời đến `kalloc` từ các CPU khác nhau có thể tiến hành mà không cần đợi nhau.

4.  Viết một chương trình song song sử dụng các luồng POSIX, được hỗ trợ trên hầu hết các hệ điều hành. Ví dụ, triển khai một bảng băm song song và đo xem số lượng puts/gets có tỷ lệ với số lượng CPU tăng lên hay không.

5.  Triển khai một tập hợp con của Pthreads trong xv6. Tức là, triển khai một thư viện luồng cấp người dùng để một tiến trình người dùng có thể có nhiều hơn 1 luồng và sắp xếp để các luồng này có thể chạy song song trên các CPU khác nhau. Đưa ra một thiết kế xử lý chính xác một luồng thực hiện một lời gọi hệ thống chặn và thay đổi không gian địa chỉ được chia sẻ của nó.

## 7 Lập lịch

Bất kỳ hệ điều hành nào cũng có khả năng chạy với nhiều tiến trình hơn so với số CPU mà máy tính có, vì vậy cần có một kế hoạch để chia sẻ thời gian các CPU giữa các tiến trình. Lý tưởng nhất là việc chia sẻ sẽ trong suốt đối với các tiến trình người dùng. Một cách tiếp cận phổ biến là cung cấp cho mỗi tiến trình ảo tưởng rằng nó có CPU ảo riêng của mình bằng cách *ghép kênh* các tiến trình vào các CPU phần cứng. Chương này giải thích cách xv6 đạt được việc ghép kênh này.

### 7.1 Ghép kênh (Multiplexing)

Xv6 ghép kênh bằng cách chuyển mỗi CPU từ một tiến trình này sang một tiến trình khác trong hai tình huống. Đầu tiên, cơ chế `sleep` và `wakeup` của xv6 chuyển đổi khi một tiến trình thực hiện một lời gọi hệ thống bị chặn (phải đợi một sự kiện), thường là trong `read`, `wait`, hoặc `sleep`. Thứ hai, xv6 định kỳ buộc một sự chuyển đổi để đối phó với các tiến trình tính toán trong thời gian dài mà không bị chặn. Các chuyển đổi trước là *tự nguyện*; các chuyển đổi sau được gọi là *không tự nguyện*. Việc ghép kênh này tạo ra ảo tưởng rằng mỗi tiến trình có CPU riêng của nó.

Việc triển khai ghép kênh đặt ra một vài thách thức. Đầu tiên, làm thế nào để chuyển từ một tiến trình này sang một tiến trình khác? Ý tưởng cơ bản là lưu và khôi phục các thanh ghi CPU, mặc dù thực tế là điều này không thể được thể hiện bằng C làm cho nó trở nên phức tạp. Thứ hai, làm thế nào để buộc các chuyển đổi theo cách trong suốt đối với các tiến trình người dùng? Xv6 sử dụng kỹ thuật tiêu chuẩn trong đó các ngắt của một bộ hẹn giờ phần cứng điều khiển các chuyển đổi ngữ cảnh. Thứ ba, tất cả các CPU chuyển đổi giữa cùng một tập hợp các tiến trình, vì vậy cần có một kế hoạch khóa để tránh các tranh chấp. Thứ tư, bộ nhớ của một tiến trình và các tài nguyên khác phải được giải phóng khi tiến trình thoát, nhưng nó không thể tự làm tất cả những điều này bởi vì (ví dụ) nó không thể giải phóng ngăn xếp hạt nhân của chính nó trong khi vẫn đang sử dụng nó. Thứ năm, mỗi CPU của một máy đa lõi phải nhớ tiến trình nào nó đang thực thi để các lời gọi hệ thống ảnh hưởng đến trạng thái hạt nhân của đúng tiến trình. Cuối cùng, `sleep` và `wakeup` cho phép một tiến trình từ bỏ CPU và đợi được một tiến trình hoặc ngắt khác đánh thức. Cần phải cẩn thận để tránh các tranh chấp dẫn đến việc mất các thông báo đánh thức.

### 7.2 Mã: Chuyển đổi ngữ cảnh

Hình 7.1 phác thảo các bước liên quan đến việc chuyển từ một tiến trình người dùng này sang một tiến trình khác: một bẫy (lời gọi hệ thống hoặc ngắt) từ không gian người dùng đến luồng hạt nhân của tiến trình cũ, một chuyển đổi ngữ cảnh sang luồng lập lịch của CPU hiện tại, một chuyển đổi ngữ cảnh sang luồng hạt nhân của một tiến trình mới, và một lần trả về bẫy cho tiến trình cấp người dùng. Xv6 có các luồng riêng biệt (các thanh ghi và ngăn xếp đã lưu) trong đó để thực thi bộ lập lịch vì không an toàn cho bộ lập lịch thực thi trên ngăn xếp hạt nhân của bất kỳ tiến trình nào: một CPU khác có thể đánh thức tiến trình và chạy nó, và sẽ là một thảm họa nếu sử dụng cùng một ngăn xếp trên hai CPU khác nhau. Có một luồng lập lịch riêng cho mỗi CPU để đối phó với các tình huống trong đó nhiều hơn một CPU đang chạy một tiến trình muốn từ bỏ CPU. Trong phần này, chúng ta sẽ xem xét các cơ chế chuyển đổi giữa một luồng hạt nhân và một luồng lập lịch.

Việc chuyển từ một luồng này sang một luồng khác liên quan đến việc lưu các thanh ghi CPU của luồng cũ, và khôi phục các thanh ghi đã lưu trước đó của luồng mới; thực tế là con trỏ ngăn xếp và bộ đếm chương trình được lưu và khôi phục có nghĩa là CPU sẽ chuyển đổi các ngăn xếp và chuyển đổi mã nào nó đang thực thi.

Hàm `swtch` lưu và khôi phục các thanh ghi cho một chuyển đổi luồng hạt nhân. `swtch` không biết trực tiếp về các luồng; nó chỉ lưu và khôi phục các tập hợp các thanh ghi RISC-V, được gọi là *ngữ cảnh*. Khi đến lúc một tiến trình từ bỏ CPU, luồng hạt nhân của tiến trình gọi `swtch` để lưu ngữ cảnh của chính nó và khôi phục ngữ cảnh của bộ lập lịch. Mỗi ngữ cảnh được chứa trong một `struct context` (kernel/proc.h:2), tự nó được chứa trong `struct proc` của một tiến trình hoặc `struct cpu` của một CPU. `swtch` nhận hai đối số: `struct context *old` và `struct context *new`. Nó lưu các thanh ghi hiện tại trong `old`, tải các thanh ghi từ `new`, và trả về.

Hãy theo dõi một tiến trình thông qua `swtch` vào bộ lập lịch. Chúng ta đã thấy trong Chương 4 rằng một khả năng ở cuối một ngắt là `usertrap` gọi `yield`. `yield` đến lượt nó gọi `sched`, `sched` gọi `swtch` để lưu ngữ cảnh hiện tại trong `p->context` và chuyển sang ngữ cảnh của bộ lập lịch đã được lưu trước đó trong `cpu->context` (kernel/proc.c:506).

`swtch` (kernel/swtch.S:3) chỉ lưu các thanh ghi *được lưu bởi người được gọi*; trình biên dịch C tạo mã trong người gọi để lưu các thanh ghi *được lưu bởi người gọi* trên ngăn xếp. `swtch` biết offset của mỗi trường thanh ghi trong `struct context`. Nó không lưu bộ đếm chương trình. Thay vào đó, `swtch` lưu thanh ghi `ra`, chứa địa chỉ trả về mà từ đó `swtch` được gọi. Bây giờ `swtch` khôi phục các thanh ghi từ ngữ cảnh mới, chứa các giá trị thanh ghi đã được lưu bởi một `swtch` trước đó. Khi `swtch` trả về, nó trả về các lệnh được trỏ đến bởi thanh ghi `ra` đã khôi phục, tức là, lệnh mà từ đó luồng mới đã gọi `swtch` trước đó. Ngoài ra, nó trả về trên ngăn xếp của luồng mới, vì đó là nơi `sp` đã khôi phục trỏ đến.

Trong ví dụ của chúng ta, `sched` đã gọi `swtch` để chuyển sang `cpu->context`, ngữ cảnh lập lịch trên mỗi CPU. Ngữ cảnh đó đã được lưu tại thời điểm trong quá khứ khi `scheduler` gọi `swtch` (kernel/proc.c:466) để chuyển sang tiến trình hiện đang từ bỏ CPU. Khi `swtch` mà chúng ta đã theo dõi trả về, nó không trả về `sched` mà trả về `scheduler`, với con trỏ ngăn xếp trong ngăn xếp lập lịch của CPU hiện tại.

### 7.3 Mã: Lập lịch

Phần cuối cùng đã xem xét các chi tiết cấp thấp của `swtch`; bây giờ hãy xem `swtch` như một cái đã cho và kiểm tra việc chuyển từ luồng hạt nhân của một tiến trình thông qua bộ lập lịch sang một tiến trình khác. Bộ lập lịch tồn tại dưới dạng một luồng đặc biệt cho mỗi CPU, mỗi luồng chạy hàm `scheduler`. Hàm này chịu trách nhiệm chọn tiến trình nào để chạy tiếp theo. Một tiến trình muốn từ bỏ CPU phải lấy khóa tiến trình riêng của nó `p->lock`, giải phóng bất kỳ khóa nào khác mà nó đang giữ, cập nhật trạng thái riêng của nó (`p->state`), và sau đó gọi `sched`. Bạn có thể thấy chuỗi này trong `yield` (kernel/proc.c:512), `sleep` và `exit`. `sched` kiểm tra lại một số yêu cầu đó (kernel/proc.c:496-501) và sau đó kiểm tra một hệ quả: vì một khóa được giữ, các ngắt phải bị vô hiệu hóa. Cuối cùng, `sched` gọi `swtch` để lưu ngữ cảnh hiện tại trong `p->context` và chuyển sang ngữ cảnh của bộ lập lịch trong `cpu->context`. `swtch` trả về trên ngăn xếp của bộ lập lịch như thể `swtch` của `scheduler` đã trả về (kernel/proc.c:466). Bộ lập lịch tiếp tục vòng lặp `for` của nó, tìm một tiến trình để chạy, chuyển sang nó, và chu kỳ lặp lại.

Chúng ta vừa thấy rằng xv6 giữ `p->lock` qua các lệnh gọi đến `swtch`: người gọi `swtch` phải đã giữ khóa, và quyền kiểm soát khóa được chuyển cho mã được chuyển sang. Sự sắp xếp này là không bình thường: phổ biến hơn là luồng lấy một khóa cũng giải phóng nó. Việc chuyển đổi ngữ cảnh của Xv6 phải phá vỡ quy ước này bởi vì `p->lock` bảo vệ các bất biến trên các trường `state` và `context` của tiến trình không đúng trong khi thực thi trong `swtch`. Ví dụ, nếu `p->lock` không được giữ trong `swtch`, một CPU khác có thể quyết định chạy tiến trình sau khi `yield` đã đặt trạng thái của nó thành `RUNNABLE`, nhưng trước khi `swtch` khiến nó ngừng sử dụng ngăn xếp hạt nhân của chính nó. Kết quả sẽ là hai CPU chạy trên cùng một ngăn xếp, điều này sẽ gây ra sự hỗn loạn. Sau khi `yield` đã bắt đầu sửa đổi trạng thái của một tiến trình đang chạy để làm cho nó `RUNNABLE`, `p->lock` phải được giữ cho đến khi các bất biến được khôi phục: điểm giải phóng chính xác sớm nhất là sau khi `scheduler` (chạy trên ngăn xếp riêng của nó) xóa `c->proc`. Tương tự, sau khi `scheduler` bắt đầu chuyển đổi một tiến trình `RUNNABLE` thành `RUNNING`, khóa không thể được giải phóng cho đến khi luồng hạt nhân của tiến trình hoàn toàn chạy (sau `swtch`, ví dụ trong `yield`).

Nơi duy nhất một luồng hạt nhân từ bỏ CPU của nó là trong `sched`, và nó luôn chuyển đến cùng một vị trí trong `scheduler`, (hầu như) luôn chuyển đến một số luồng hạt nhân đã gọi `sched` trước đó. Do đó, nếu người ta in ra các số dòng nơi xv6 chuyển đổi các luồng, người ta sẽ quan sát một mẫu đơn giản sau: (kernel/proc.c:466), (kernel/proc.c:506), (kernel/proc.c:466), (kernel/proc.c:506), v.v. Các thủ tục cố ý chuyển quyền điều khiển cho nhau thông qua chuyển đổi luồng đôi khi được gọi là *coroutines*; trong ví dụ này, `sched` và `scheduler` là các coroutine của nhau.

Có một trường hợp khi lệnh gọi của bộ lập lịch đến `swtch` không kết thúc trong `sched`. `allocproc` đặt thanh ghi `ra` ngữ cảnh của một tiến trình mới thành `forkret` (kernel/proc.c:524), do đó `swtch` đầu tiên của nó "trả về" đầu của hàm đó. `forkret` tồn tại để giải phóng `p->lock`; nếu không, vì tiến trình mới cần quay trở lại không gian người dùng như thể quay trở lại từ `fork`, nó có thể thay vào đó bắt đầu tại `usertrapret`.

`scheduler` (kernel/proc.c:445) chạy một vòng lặp: tìm một tiến trình để chạy, chạy nó cho đến khi nó nhường, lặp lại. Bộ lập lịch lặp qua bảng tiến trình tìm kiếm một tiến trình có thể chạy được, một tiến trình có `p->state == RUNNABLE`. Sau khi nó tìm thấy một tiến trình, nó đặt biến tiến trình hiện tại trên mỗi CPU `c->proc`, đánh dấu tiến trình là `RUNNING`, và sau đó gọi `swtch` để bắt đầu chạy nó (kernel/proc.c:461-466).

### 7.4 Mã: mycpu và myproc

Xv6 thường cần một con trỏ đến `proc` của tiến trình hiện tại. Trên một bộ xử lý đơn, người ta có thể có một biến toàn cục trỏ đến `proc` hiện tại. Điều này không hoạt động trên một máy đa lõi, vì mỗi CPU thực thi một tiến trình khác nhau. Cách để giải quyết vấn đề này là khai thác thực tế là mỗi CPU có bộ thanh ghi riêng của nó.

Trong khi một CPU nhất định đang thực thi trong hạt nhân, xv6 đảm bảo rằng thanh ghi `tp` của CPU luôn chứa hartid của CPU. RISC-V đánh số các CPU của nó, cung cấp cho mỗi CPU một *hartid* duy nhất. `mycpu` (kernel/proc.c:74) sử dụng `tp` để lập chỉ mục một mảng các cấu trúc `cpu` và trả về cấu trúc cho CPU hiện tại. Một `struct cpu` (kernel/proc.h:22) chứa một con trỏ đến cấu trúc `proc` của tiến trình hiện đang chạy trên CPU đó (nếu có), các thanh ghi đã lưu cho luồng lập lịch của CPU, và số lượng các khóa quay lồng nhau cần thiết để quản lý việc vô hiệu hóa ngắt.

Việc đảm bảo rằng `tp` của một CPU chứa hartid của CPU có một chút phức tạp, vì mã người dùng có thể tự do sửa đổi `tp`. `start` đặt thanh ghi `tp` sớm trong chuỗi khởi động của CPU, trong khi vẫn ở chế độ máy (kernel/start.c:45). `usertrapret` lưu `tp` trong trang trampoline, trong trường hợp mã người dùng sửa đổi nó. Cuối cùng, `uservec` khôi phục `tp` đã lưu đó khi vào hạt nhân từ không gian người dùng (kernel/trampoline.S:78). Trình biên dịch đảm bảo không bao giờ sửa đổi `tp` trong mã hạt nhân. Sẽ thuận tiện hơn nếu xv6 có thể yêu cầu phần cứng RISC-V cho hartid hiện tại bất cứ khi nào cần, nhưng RISC-V chỉ cho phép điều đó ở chế độ máy, không phải ở chế độ giám sát.

Các giá trị trả về của `cpuid` và `mycpu` là mong manh: nếu bộ hẹn giờ ngắt và khiến luồng nhường và sau đó tiếp tục thực thi trên một CPU khác, một giá trị trả về trước đó sẽ không còn đúng nữa. Để tránh vấn đề này, xv6 yêu cầu người gọi vô hiệu hóa các ngắt, và chỉ bật chúng sau khi họ đã sử dụng xong `struct cpu` được trả về.

Hàm `myproc` (kernel/proc.c:83) trả về con trỏ `struct proc` cho tiến trình đang chạy trên CPU hiện tại. `myproc` vô hiệu hóa các ngắt, gọi `mycpu`, lấy con trỏ tiến trình hiện tại (`c->proc`) ra khỏi `struct cpu`, và sau đó bật các ngắt. Giá trị trả về của `myproc` là an toàn để sử dụng ngay cả khi các ngắt được bật: nếu một ngắt hẹn giờ di chuyển tiến trình đang gọi sang một CPU khác, con trỏ `struct proc` của nó sẽ vẫn giữ nguyên.

### 7.5 Ngủ và đánh thức

Lập lịch và các khóa giúp che giấu các hành động của một luồng khỏi một luồng khác, nhưng chúng ta cũng cần các sự trừu tượng hóa giúp các luồng tương tác một cách có chủ ý. Ví dụ, người đọc một ống dẫn trong xv6 có thể cần đợi một tiến trình ghi để tạo ra dữ liệu; lệnh gọi `wait` của một cha mẹ có thể cần đợi một con thoát; và một tiến trình đọc đĩa cần đợi phần cứng đĩa hoàn thành việc đọc. Hạt nhân xv6 sử dụng một cơ chế được gọi là `sleep` và `wakeup` trong các tình huống này (và nhiều tình huống khác). `sleep` cho phép một luồng hạt nhân đợi một sự kiện cụ thể; một luồng khác có thể gọi `wakeup` để chỉ ra rằng các luồng đang đợi một sự kiện cụ thể sẽ tiếp tục. `sleep` và `wakeup` thường được gọi là các cơ chế *điều phối trình tự* hoặc *đồng bộ hóa có điều kiện*.

`sleep` và `wakeup` cung cấp một giao diện đồng bộ hóa tương đối cấp thấp. Để thúc đẩy cách chúng hoạt động trong xv6, chúng ta sẽ sử dụng chúng để xây dựng một cơ chế đồng bộ hóa cấp cao hơn được gọi là *semaphore* [5] điều phối các nhà sản xuất và người tiêu dùng (xv6 không sử dụng semaphore). Một semaphore duy trì một bộ đếm và cung cấp hai hoạt động. Hoạt động “V” (cho nhà sản xuất) tăng bộ đếm. Hoạt động “P” (cho người tiêu dùng) đợi cho đến khi bộ đếm khác không, và sau đó giảm nó và trả về. Nếu chỉ có một luồng nhà sản xuất và một luồng người tiêu dùng, và chúng thực thi trên các CPU khác nhau, và trình biên dịch không tối ưu hóa quá mạnh, việc triển khai này sẽ đúng:

```c
100 struct semaphore {
101     struct spinlock lock;
102     int count;
103 };
104
105 void
106 V(struct semaphore *s)
107 {
108     acquire(&s->lock);
109     s->count += 1;
110     release(&s->lock);
111 }
112
113 void
114 P(struct semaphore *s)
115 {
116     while(s->count == 0)
117         ;
118     acquire(&s->lock);
119     s->count -= 1;
120     release(&s->lock);
121 }
```

Việc triển khai trên là tốn kém. Nếu nhà sản xuất hoạt động hiếm khi, người tiêu dùng sẽ dành phần lớn thời gian của mình quay trong vòng lặp `while` hy vọng một bộ đếm khác không. CPU của người tiêu dùng có lẽ có thể tìm thấy công việc hiệu quả hơn so với việc *chờ đợi bận rộn* bằng cách thăm dò `s->count` lặp đi lặp lại.

Việc tránh chờ đợi bận rộn đòi hỏi một cách để người tiêu dùng nhường CPU và chỉ tiếp tục sau khi V tăng bộ đếm. Đây là một bước đi theo hướng đó, mặc dù như chúng ta sẽ thấy nó là không đủ. Hãy tưởng tượng một cặp lệnh gọi, `sleep` và `wakeup`, hoạt động như sau. `sleep(chan)` đợi một sự kiện được chỉ định bởi giá trị của `chan`, được gọi là *kênh chờ*. `sleep` đặt tiến trình đang gọi vào trạng thái ngủ, giải phóng CPU cho công việc khác. `wakeup(chan)` đánh thức tất cả các tiến trình đang ở trong các lệnh gọi đến `sleep` với cùng một `chan` (nếu có), khiến các lệnh gọi `sleep` của chúng trả về. Nếu không có tiến trình nào đang chờ trên `chan`, `wakeup` không làm gì cả. Chúng ta có thể thay đổi việc triển khai semaphore để sử dụng `sleep` và `wakeup` (các thay đổi được tô sáng màu vàng):

```c
200 void
201 V(struct semaphore *s)
202 {
203     acquire(&s->lock);
204     s->count += 1;
205     wakeup(s);
206     release(&s->lock);
207 }
208
209 void
210 P(struct semaphore *s)
211 {
212     while(s->count == 0)
213         sleep(s);
214     acquire(&s->lock);
215     s->count -= 1;
216     release(&s->lock);
217 }
```

`P` bây giờ nhường CPU thay vì quay, điều này tốt. Tuy nhiên, hóa ra không đơn giản để thiết kế `sleep` và `wakeup` với giao diện này mà không bị *vấn đề đánh thức bị mất*. Giả sử rằng `P` thấy rằng `s->count == 0` ở dòng 212. Trong khi `P` ở giữa dòng 212 và 213, `V` chạy trên một CPU khác: nó thay đổi `s->count` thành khác không và gọi `wakeup`, không tìm thấy tiến trình nào đang ngủ và do đó không làm gì cả. Bây giờ `P` tiếp tục thực thi ở dòng 213: nó gọi `sleep` và đi ngủ. Điều này gây ra một vấn đề: `P` đang ngủ chờ một lệnh gọi `V` đã xảy ra. Trừ khi chúng ta gặp may và nhà sản xuất gọi `V` một lần nữa, người tiêu dùng sẽ đợi mãi mãi mặc dù bộ đếm khác không.

Vấn đề gốc rễ của vấn đề này là bất biến rằng `P` chỉ ngủ khi `s->count == 0` bị vi phạm bởi `V` chạy vào đúng thời điểm sai. Một cách không chính xác để bảo vệ bất biến sẽ là di chuyển việc lấy khóa (được tô sáng màu vàng bên dưới) trong `P` để việc kiểm tra bộ đếm và lệnh gọi đến `sleep` của nó là nguyên tử:

```c
300 void
301 V(struct semaphore *s)
302 {
303     acquire(&s->lock);
304     s->count += 1;
305     wakeup(s);
306     release(&s->lock);
307 }
308
309 void
310 P(struct semaphore *s)
311 {
312     acquire(&s->lock);
313     while(s->count == 0)
314         sleep(s);
315     s->count -= 1;
316     release(&s->lock);
317 }
```

Người ta có thể hy vọng rằng phiên bản `P` này sẽ tránh được việc đánh thức bị mất bởi vì khóa ngăn `V` thực thi giữa các dòng 313 và 314. Nó làm điều đó, nhưng nó cũng bị khóa chết: `P` giữ khóa trong khi nó ngủ, vì vậy `V` sẽ chặn mãi mãi chờ khóa.

Chúng ta sẽ khắc phục lược đồ trước đó bằng cách thay đổi giao diện của `sleep`: người gọi phải truyền *khóa điều kiện* cho `sleep` để nó có thể giải phóng khóa sau khi tiến trình đang gọi được đánh dấu là đang ngủ và đang chờ trên kênh ngủ. Khóa sẽ buộc một `V` đồng thời phải đợi cho đến khi `P` đã hoàn thành việc tự đặt mình vào trạng thái ngủ, để `wakeup` sẽ tìm thấy người tiêu dùng đang ngủ và đánh thức nó. Sau khi người tiêu dùng được đánh thức lại, `sleep` sẽ lấy lại khóa trước khi trả về. Lược đồ ngủ/đánh thức mới, chính xác của chúng ta có thể sử dụng được như sau (thay đổi được tô sáng màu vàng):

```c
400 void
401 V(struct semaphore *s)
402 {
403     acquire(&s->lock);
404     s->count += 1;
405     wakeup(s);
406     release(&s->lock);
407 }
408
409 void
410 P(struct semaphore *s)
411 {
412     acquire(&s->lock);
413     while(s->count == 0)
414         sleep(s, &s->lock);
415     s->count -= 1;
416     release(&s->lock);
417 }
```

Việc `P` giữ `s->lock` ngăn `V` cố gắng đánh thức nó giữa việc kiểm tra `s->count` của `P` và lệnh gọi đến `sleep` của nó. Tuy nhiên, `sleep` phải giải phóng `s->lock` và đặt tiến trình tiêu thụ vào trạng thái ngủ theo cách nguyên tử theo quan điểm của `wakeup`, để tránh các lần đánh thức bị mất.

### 7.6 Mã: Ngủ và đánh thức

`sleep` của Xv6 (kernel/proc.c:548) và `wakeup` (kernel/proc.c:579) cung cấp giao diện được sử dụng trong ví dụ cuối cùng ở trên. Ý tưởng cơ bản là để `sleep` đánh dấu tiến trình hiện tại là `SLEEPING` và sau đó gọi `sched` để giải phóng CPU; `wakeup` tìm một tiến trình đang ngủ trên kênh chờ đã cho và đánh dấu nó là `RUNNABLE`. Người gọi `sleep` và `wakeup` có thể sử dụng bất kỳ số nào thuận tiện cho cả hai làm kênh. Xv6 thường sử dụng địa chỉ của một cấu trúc dữ liệu hạt nhân liên quan đến việc chờ đợi.

`sleep` lấy `p->lock` (kernel/proc.c:559) và chỉ sau đó mới giải phóng `lk`. Như chúng ta sẽ thấy, thực tế là `sleep` giữ một trong hai khóa này tại mọi thời điểm là điều ngăn cản một `wakeup` đồng thời (phải lấy và giữ cả hai) hoạt động. Bây giờ `sleep` chỉ giữ `p->lock`, nó có thể đặt tiến trình vào trạng thái ngủ bằng cách ghi lại kênh ngủ, thay đổi trạng thái tiến trình thành `SLEEPING`, và gọi `sched` (kernel/proc.c:563-566). Trong một lát nữa sẽ rõ tại sao điều quan trọng là `p->lock` không được giải phóng (bởi `scheduler`) cho đến sau khi tiến trình được đánh dấu là `SLEEPING`.

Tại một thời điểm nào đó, một tiến trình sẽ lấy khóa điều kiện, đặt điều kiện mà người ngủ đang chờ, và gọi `wakeup(chan)`. Điều quan trọng là `wakeup` được gọi trong khi giữ khóa điều kiện¹. `wakeup` lặp qua bảng tiến trình (kernel/proc.c:579). Nó lấy `p->lock` của mỗi tiến trình mà nó kiểm tra. Khi `wakeup` tìm thấy một tiến trình ở trạng thái `SLEEPING` với một `chan` khớp, nó sẽ thay đổi trạng thái của tiến trình đó thành `RUNNABLE`. Lần tiếp theo `scheduler` chạy, nó sẽ thấy rằng tiến trình đã sẵn sàng để chạy.

Tại sao các quy tắc khóa cho `sleep` và `wakeup` đảm bảo rằng một tiến trình sắp đi ngủ sẽ không bỏ lỡ một `wakeup` đồng thời? Tiến trình sắp đi ngủ giữ hoặc khóa điều kiện hoặc `p->lock` của chính nó hoặc cả hai từ trước khi nó kiểm tra điều kiện cho đến sau khi nó được đánh dấu là `SLEEPING`. Tiến trình gọi `wakeup` giữ cả hai khóa trong vòng lặp của `wakeup`. Do đó, người đánh thức hoặc làm cho điều kiện đúng trước khi luồng tiêu thụ kiểm tra điều kiện; hoặc `wakeup` của người đánh thức kiểm tra luồng đang ngủ nghiêm ngặt sau khi nó đã được đánh dấu là `SLEEPING`. Sau đó, `wakeup` sẽ thấy tiến trình đang ngủ và đánh thức nó (trừ khi có thứ gì đó khác đánh thức nó trước).

Đôi khi nhiều tiến trình đang ngủ trên cùng một kênh; ví dụ, nhiều hơn một tiến trình đọc từ một ống dẫn. Một lệnh gọi duy nhất đến `wakeup` sẽ đánh thức tất cả chúng. Một trong số chúng sẽ chạy trước và lấy khóa mà `sleep` đã được gọi với, và (trong trường hợp các ống dẫn) đọc bất kỳ dữ liệu nào đang chờ. Các tiến trình khác sẽ thấy rằng, mặc dù đã được đánh thức, không có dữ liệu nào để đọc. Theo quan điểm của chúng, việc đánh thức là “giả mạo”, và chúng phải ngủ lại. Vì lý do này, `sleep` luôn được gọi bên trong một vòng lặp kiểm tra điều kiện.

Không có hại gì nếu hai lần sử dụng `sleep`/`wakeup` vô tình chọn cùng một kênh: chúng sẽ thấy các lần đánh thức giả mạo, nhưng việc lặp lại như được mô tả ở trên sẽ chịu đựng được vấn đề này. Phần lớn sự quyến rũ của `sleep`/`wakeup` là nó vừa nhẹ (không cần tạo các cấu trúc dữ liệu đặc biệt để hoạt động như các kênh ngủ) và cung cấp một lớp gián tiếp (người gọi không cần biết họ đang tương tác với tiến trình cụ thể nào).

¹Nói một cách chính xác, chỉ cần `wakeup` theo sau `acquire` (tức là, người ta có thể gọi `wakeup` sau khi `release`).

### 7.7 Mã: Ống (Pipes)

Một ví dụ phức tạp hơn sử dụng `sleep` và `wakeup` để đồng bộ hóa các nhà sản xuất và người tiêu dùng là việc triển khai các ống dẫn của xv6. Chúng ta đã thấy giao diện cho các ống dẫn trong Chương 1: các byte được ghi vào một đầu của một ống dẫn được sao chép vào một bộ đệm trong hạt nhân và sau đó có thể được đọc từ đầu kia của ống dẫn. Các chương trong tương lai sẽ xem xét sự hỗ trợ của mô tả tệp xung quanh các ống dẫn, nhưng bây giờ hãy xem xét việc triển khai `pipewrite` và `piperead`.

Mỗi ống dẫn được biểu diễn bằng một `struct pipe`, chứa một khóa và một bộ đệm dữ liệu. Các trường `nread` và `nwrite` đếm tổng số byte đã đọc từ và ghi vào bộ đệm. Bộ đệm bao quanh: byte tiếp theo được ghi sau `buf[PIPESIZE-1]` là `buf[0]`. Các bộ đếm không bao quanh. Quy ước này cho phép việc triển khai phân biệt một bộ đệm đầy (`nwrite == nread+PIPESIZE`) với một bộ đệm trống (`nwrite == nread`), nhưng nó có nghĩa là việc lập chỉ mục vào bộ đệm phải sử dụng `buf[nread % PIPESIZE]` thay vì chỉ `buf[nread]` (và tương tự cho `nwrite`).

Giả sử rằng các lệnh gọi đến `piperead` và `pipewrite` xảy ra đồng thời trên hai CPU khác nhau. `pipewrite` (kernel/pipe.c:77) bắt đầu bằng cách lấy khóa của ống dẫn, bảo vệ các bộ đếm, dữ liệu và các bất biến liên quan của chúng. `piperead` (kernel/pipe.c:106) sau đó cố gắng lấy khóa, nhưng không thể. Nó quay trong `acquire` (kernel/spinlock.c:22) chờ khóa. Trong khi `piperead` đợi, `pipewrite` lặp qua các byte đang được ghi (`addr[0..n-1]`), thêm từng byte vào ống dẫn lần lượt (kernel/pipe.c:95). Trong vòng lặp này, có thể xảy ra trường hợp bộ đệm đầy (kernel/pipe.c:88). Trong trường hợp này, `pipewrite` gọi `wakeup` để cảnh báo bất kỳ người đọc nào đang ngủ về thực tế là có dữ liệu đang chờ trong bộ đệm và sau đó ngủ trên `&pi->nwrite` để đợi một người đọc lấy một số byte ra khỏi bộ đệm. `sleep` giải phóng khóa của ống dẫn như một phần của việc đặt tiến trình của `pipewrite` vào trạng thái ngủ.

`piperead` bây giờ lấy khóa của ống dẫn và vào phần quan trọng của nó: nó thấy rằng `pi->nread != pi->nwrite` (kernel/pipe.c:113) (`pipewrite` đã đi ngủ vì `pi->nwrite == pi->nread + PIPESIZE` (kernel/pipe.c:88)), vì vậy nó đi qua vòng lặp `for`, sao chép dữ liệu ra khỏi ống dẫn (kernel/pipe.c:120), và tăng `nread` theo số byte đã sao chép. Nhiều byte đó hiện có sẵn để ghi, vì vậy `piperead` gọi `wakeup` (kernel/pipe.c:127) để đánh thức bất kỳ người ghi nào đang ngủ trước khi nó trả về. `wakeup` tìm thấy một tiến trình đang ngủ trên `&pi->nwrite`, tiến trình đang chạy `pipewrite` nhưng đã dừng lại khi bộ đệm đầy. Nó đánh dấu tiến trình đó là `RUNNABLE`.

Mã ống dẫn sử dụng các kênh ngủ riêng cho người đọc và người ghi (`pi->nread` và `pi->nwrite`); điều này có thể làm cho hệ thống hiệu quả hơn trong trường hợp không chắc có nhiều người đọc và người ghi đang chờ cùng một ống dẫn. Mã ống dẫn ngủ bên trong một vòng lặp kiểm tra điều kiện ngủ; nếu có nhiều người đọc hoặc người ghi, tất cả trừ tiến trình đầu tiên thức dậy sẽ thấy điều kiện vẫn sai và ngủ lại.

### 7.8 Mã: Đợi, thoát và hủy

`sleep` và `wakeup` có thể được sử dụng cho nhiều loại chờ đợi. Một ví dụ thú vị, được giới thiệu trong Chương 1, là sự tương tác giữa `exit` của một đứa con và `wait` của cha mẹ nó. Tại thời điểm cái chết của đứa con, cha mẹ có thể đã đang ngủ trong `wait`, hoặc có thể đang làm gì đó khác; trong trường hợp sau, một lệnh gọi tiếp theo đến `wait` phải quan sát cái chết của đứa con, có lẽ rất lâu sau khi nó gọi `exit`. Cách mà xv6 ghi lại cái chết của đứa con cho đến khi `wait` quan sát nó là để `exit` đặt người gọi vào trạng thái `ZOMBIE`, nơi nó ở lại cho đến khi `wait` của cha mẹ nhận thấy nó, thay đổi trạng thái của đứa con thành `UNUSED`, sao chép trạng thái thoát của đứa con, và trả về ID tiến trình của đứa con cho cha mẹ. Nếu cha mẹ thoát trước con, cha mẹ sẽ giao con cho tiến trình `init`, tiến trình này vĩnh viễn gọi `wait`; do đó, mọi đứa con đều có một cha mẹ để dọn dẹp sau nó. Một thách thức là tránh các tranh chấp và khóa chết giữa `wait` và `exit` của cha mẹ và con đồng thời, cũng như `exit` và `exit` đồng thời.

`wait` bắt đầu bằng cách lấy `wait_lock` (kernel/proc.c:391), hoạt động như khóa điều kiện giúp đảm bảo rằng `wait` không bỏ lỡ một lần đánh thức từ một đứa con đang thoát. Sau đó, `wait` quét bảng tiến trình. Nếu nó tìm thấy một đứa con ở trạng thái `ZOMBIE`, nó sẽ giải phóng các tài nguyên của đứa con đó và cấu trúc `proc` của nó, sao chép trạng thái thoát của đứa con vào địa chỉ được cung cấp cho `wait` (nếu nó không phải là 0), và trả về ID tiến trình của đứa con. Nếu `wait` tìm thấy các con nhưng không có con nào đã thoát, nó sẽ gọi `sleep` để đợi bất kỳ đứa con nào của nó thoát (kernel/proc.c:433), sau đó quét lại. `wait` thường giữ hai khóa, `wait_lock` và `pp->lock` của một tiến trình nào đó; thứ tự tránh khóa chết là đầu tiên là `wait_lock` và sau đó là `pp->lock`.

`exit` (kernel/proc.c:347) ghi lại trạng thái thoát, giải phóng một số tài nguyên, gọi `reparent` để giao các con của nó cho tiến trình `init`, đánh thức cha mẹ trong trường hợp nó ở trong `wait`, đánh dấu người gọi là một zombie, và nhường CPU vĩnh viễn. `exit` giữ cả `wait_lock` và `p->lock` trong chuỗi này. Nó giữ `wait_lock` vì nó là khóa điều kiện cho `wakeup(p->parent)`, ngăn cản một cha mẹ trong `wait` mất lần đánh thức. `exit` cũng phải giữ `p->lock` cho chuỗi này, để ngăn một cha mẹ trong `wait` thấy rằng đứa con ở trạng thái `ZOMBIE` trước khi đứa con đã gọi `swtch` cuối cùng. `exit` lấy các khóa này theo cùng một thứ tự như `wait` để tránh khóa chết.

Có vẻ không chính xác khi `exit` đánh thức cha mẹ trước khi đặt trạng thái của nó thành `ZOMBIE`, nhưng điều đó là an toàn: mặc dù `wakeup` có thể khiến cha mẹ chạy, vòng lặp trong `wait` không thể kiểm tra đứa con cho đến khi `p->lock` của đứa con được giải phóng bởi `scheduler`, vì vậy `wait` không thể xem xét tiến trình đang thoát cho đến sau khi `exit` đã đặt trạng thái của nó thành `ZOMBIE` (kernel/proc.c:379).

Trong khi `exit` cho phép một tiến trình tự chấm dứt, `kill` (kernel/proc.c:598) cho phép một tiến trình yêu cầu một tiến trình khác chấm dứt. Sẽ quá phức tạp nếu `kill` trực tiếp phá hủy tiến trình nạn nhân, vì nạn nhân có thể đang thực thi trên một CPU khác, có lẽ ở giữa một chuỗi các cập nhật nhạy cảm đối với các cấu trúc dữ liệu hạt nhân. Do đó, `kill` làm rất ít: nó chỉ đặt `p->killed` của nạn nhân và, nếu nó đang ngủ, đánh thức nó. Cuối cùng, nạn nhân sẽ vào hoặc rời khỏi hạt nhân, tại thời điểm đó mã trong `usertrap` sẽ gọi `exit` nếu `p->killed` được đặt (nó kiểm tra bằng cách gọi `killed` (kernel/proc.c:627)). Nếu nạn nhân đang chạy trong không gian người dùng, nó sẽ sớm vào hạt nhân bằng cách thực hiện một lời gọi hệ thống hoặc vì bộ hẹn giờ (hoặc một thiết bị khác) ngắt.

Nếu tiến trình nạn nhân đang ở trong `sleep`, lệnh gọi của `kill` đến `wakeup` sẽ khiến nạn nhân quay trở lại từ `sleep`. Điều này có khả năng nguy hiểm vì điều kiện đang được chờ đợi có thể không đúng. Tuy nhiên, các lệnh gọi của xv6 đến `sleep` luôn được gói trong một vòng lặp `while` kiểm tra lại điều kiện sau khi `sleep` trả về. Một số lệnh gọi đến `sleep` cũng kiểm tra `p->killed` trong vòng lặp, và từ bỏ hoạt động hiện tại nếu nó được đặt. Điều này chỉ được thực hiện khi việc từ bỏ như vậy sẽ đúng. Ví dụ, mã đọc và ghi ống dẫn (kernel/pipe.c:84) trả về nếu cờ `killed` được đặt; cuối cùng mã sẽ quay trở lại `trap`, `trap` sẽ kiểm tra lại `p->killed` và thoát.

Một số vòng lặp ngủ của xv6 không kiểm tra `p->killed` bởi vì mã đang ở giữa một lời gọi hệ thống nhiều bước nên là nguyên tử. Trình điều khiển virtio (kernel/virtio_disk.c:285) là một ví dụ: nó không kiểm tra `p->killed` bởi vì một hoạt động đĩa có thể là một trong một tập hợp các lần ghi mà tất cả đều cần thiết để hệ thống tệp được để lại trong một trạng thái chính xác. Một tiến trình bị giết trong khi chờ I/O đĩa sẽ không thoát cho đến khi nó hoàn thành lời gọi hệ thống hiện tại và `usertrap` thấy cờ `killed`.

### 7.9 Khóa tiến trình

Khóa được liên kết với mỗi tiến trình (`p->lock`) là khóa phức tạp nhất trong xv6. Một cách đơn giản để nghĩ về `p->lock` là nó phải được giữ trong khi đọc hoặc ghi bất kỳ trường `struct proc` nào sau đây: `p->state`, `p->chan`, `p->killed`, `p->xstate`, và `p->pid`. Các trường này có thể được sử dụng bởi các tiến trình khác, hoặc bởi các luồng lập lịch trên các CPU khác, vì vậy việc chúng phải được bảo vệ bởi một khóa là tự nhiên.

Tuy nhiên, hầu hết các lần sử dụng `p->lock` là để bảo vệ các khía cạnh cấp cao hơn của các cấu trúc dữ liệu và các thuật toán tiến trình của xv6. Đây là tập hợp đầy đủ những gì `p->lock` làm:

*   Cùng với `p->state`, nó ngăn chặn các tranh chấp trong việc phân bổ các khe `proc[]` cho các tiến trình mới.

*   Nó che giấu một tiến trình khỏi bị xem trong khi nó đang được tạo hoặc phá hủy.

*   Nó ngăn `wait` của một cha mẹ thu thập một tiến trình đã đặt trạng thái của nó thành `ZOMBIE` nhưng chưa nhường CPU.

*   Nó ngăn bộ lập lịch của một CPU khác quyết định chạy một tiến trình đang nhường sau khi nó đã đặt trạng thái của nó thành `RUNNABLE` nhưng trước khi nó hoàn thành `swtch`.

*   Nó đảm bảo rằng chỉ có bộ lập lịch của một CPU quyết định chạy một tiến trình `RUNNABLE`.

*   Nó ngăn một ngắt hẹn giờ gây ra một tiến trình nhường trong khi nó ở trong `swtch`.

*   Cùng với khóa điều kiện, nó giúp `wakeup` không bỏ qua một tiến trình đang gọi `sleep` nhưng chưa hoàn thành việc nhường CPU.

*   Nó ngăn tiến trình nạn nhân của `kill` thoát và có lẽ được phân bổ lại giữa việc kiểm tra `p->pid` của `kill` và việc đặt `p->killed`.

*   Nó làm cho việc kiểm tra và ghi `p->state` của `kill` là nguyên tử.

Trường `p->parent` được bảo vệ bởi khóa toàn cục `wait_lock` thay vì bởi `p->lock`. Chỉ có cha mẹ của một tiến trình mới sửa đổi `p->parent`, mặc dù trường này được đọc bởi cả chính tiến trình và bởi các tiến trình khác đang tìm kiếm con của chúng. Mục đích của `wait_lock` là hoạt động như khóa điều kiện khi `wait` ngủ chờ bất kỳ đứa con nào thoát. Một đứa con đang thoát giữ hoặc `wait_lock` hoặc `p->lock` cho đến sau khi nó đã đặt trạng thái của nó thành `ZOMBIE`, đánh thức cha mẹ của nó, và nhường CPU. `wait_lock` cũng tuần tự hóa các lần thoát đồng thời của một cha mẹ và một con, để tiến trình `init` (kế thừa đứa con) được đảm bảo sẽ được đánh thức từ `wait` của nó. `wait_lock` là một khóa toàn cục thay vì một khóa trên mỗi tiến trình trong mỗi cha mẹ, bởi vì, cho đến khi một tiến trình lấy nó, nó không thể biết cha mẹ của nó là ai.

### 7.10 Thế giới thực

Bộ lập lịch xv6 thực hiện một chính sách lập lịch đơn giản, chạy mỗi tiến trình lần lượt. Chính sách này được gọi là *round robin*. Các hệ điều hành thực thực hiện các chính sách phức tạp hơn, ví dụ, cho phép các tiến trình có các ưu tiên. Ý tưởng là một tiến trình có thể chạy có ưu tiên cao sẽ được bộ lập lịch ưa thích hơn một tiến trình có thể chạy có ưu tiên thấp. Các chính sách này có thể trở nên phức tạp nhanh chóng vì thường có các mục tiêu cạnh tranh: ví dụ, hệ điều hành cũng có thể muốn đảm bảo sự công bằng và thông lượng cao. Ngoài ra, các chính sách phức tạp có thể dẫn đến các tương tác không mong muốn như *đảo ngược ưu tiên* và *đoàn xe*. Đảo ngược ưu tiên có thể xảy ra khi một tiến trình có ưu tiên thấp và một tiến trình có ưu tiên cao đều sử dụng một khóa cụ thể, khi được một tiến trình có ưu tiên thấp lấy có thể ngăn tiến trình có ưu tiên cao tiến triển. Một đoàn xe dài các tiến trình đang chờ có thể hình thành khi nhiều tiến trình có ưu tiên cao đang chờ một tiến trình có ưu tiên thấp lấy một khóa được chia sẻ; một khi một đoàn xe đã hình thành, nó có thể tồn tại trong một thời gian dài. Để tránh các loại vấn đề này, các cơ chế bổ sung là cần thiết trong các bộ lập lịch tinh vi.

`sleep` và `wakeup` là một phương pháp đồng bộ hóa đơn giản và hiệu quả, nhưng có nhiều phương pháp khác. Thách thức đầu tiên trong tất cả chúng là tránh vấn đề “đánh thức bị mất” mà chúng ta đã thấy ở đầu chương. `sleep` của hạt nhân Unix ban đầu chỉ đơn giản là vô hiệu hóa các ngắt, điều này đủ vì Unix chạy trên một hệ thống CPU đơn. Bởi vì xv6 chạy trên các bộ đa xử lý, nó thêm một khóa rõ ràng vào `sleep`. `msleep` của FreeBSD có cùng cách tiếp cận. `sleep` của Plan 9 sử dụng một hàm gọi lại chạy với khóa lập lịch được giữ ngay trước khi đi ngủ; hàm này đóng vai trò là một kiểm tra cuối cùng của điều kiện ngủ, để tránh các lần đánh thức bị mất. `sleep` của hạt nhân Linux sử dụng một hàng đợi tiến trình rõ ràng, được gọi là một *hàng đợi chờ*, thay vì một kênh chờ; hàng đợi có khóa nội bộ riêng của nó.

Việc quét toàn bộ tập hợp các tiến trình trong `wakeup` là không hiệu quả. Một giải pháp tốt hơn là thay thế `chan` trong cả `sleep` và `wakeup` bằng một cấu trúc dữ liệu chứa một danh sách các tiến trình đang ngủ trên cấu trúc đó, chẳng hạn như hàng đợi chờ của Linux. `sleep` và `wakeup` của Plan 9 gọi cấu trúc đó là một *điểm hẹn*. Nhiều thư viện luồng đề cập đến cùng một cấu trúc như một *biến điều kiện*; trong bối cảnh đó, các hoạt động `sleep` và `wakeup` được gọi là `wait` và `signal`. Tất cả các cơ chế này đều có cùng một hương vị: điều kiện ngủ được bảo vệ bởi một loại khóa nào đó được thả một cách nguyên tử trong khi ngủ.

Việc triển khai `wakeup` đánh thức tất cả các tiến trình đang chờ trên một kênh cụ thể, và có thể có trường hợp nhiều tiến trình đang chờ trên kênh cụ thể đó. Hệ điều hành sẽ lên lịch cho tất cả các tiến trình này và chúng sẽ tranh nhau để kiểm tra điều kiện ngủ. Các tiến trình hoạt động theo cách này đôi khi được gọi là một *đàn gia súc sấm sét*, và tốt nhất là nên tránh. Hầu hết các biến điều kiện đều có hai nguyên thủy cho `wakeup`: `signal`, đánh thức một tiến trình, và `broadcast`, đánh thức tất cả các tiến trình đang chờ.

Các semaphore thường được sử dụng để đồng bộ hóa. Bộ đếm thường tương ứng với một cái gì đó như số byte có sẵn trong một bộ đệm ống dẫn hoặc số lượng các con zombie mà một tiến trình có. Việc sử dụng một bộ đếm rõ ràng như một phần của sự trừu tượng hóa tránh được vấn đề “đánh thức bị mất”: có một bộ đếm rõ ràng về số lần đánh thức đã xảy ra. Bộ đếm cũng tránh được các vấn đề đánh thức giả mạo và đàn gia súc sấm sét.

Việc chấm dứt các tiến trình và dọn dẹp chúng giới thiệu nhiều sự phức tạp trong xv6. Trong hầu hết các hệ điều hành, nó thậm chí còn phức tạp hơn, bởi vì, ví dụ, tiến trình nạn nhân có thể đang ngủ sâu trong hạt nhân, và việc gỡ rối ngăn xếp của nó đòi hỏi sự cẩn thận, vì mỗi hàm trên ngăn xếp cuộc gọi có thể cần thực hiện một số dọn dẹp. Một số ngôn ngữ giúp đỡ bằng cách cung cấp một cơ chế ngoại lệ, nhưng không phải C. Hơn nữa, có những sự kiện khác có thể khiến một tiến trình đang ngủ được đánh thức, mặc dù sự kiện mà nó đang chờ đợi chưa xảy ra. Ví dụ, khi một tiến trình Unix đang ngủ, một tiến trình khác có thể gửi một *tín hiệu* cho nó. Trong trường hợp này, tiến trình sẽ quay trở lại từ lời gọi hệ thống bị ngắt với giá trị -1 và với mã lỗi được đặt thành `EINTR`. Ứng dụng có thể kiểm tra các giá trị này và quyết định phải làm gì. Xv6 không hỗ trợ các tín hiệu và sự phức tạp này không phát sinh.

Hỗ trợ của Xv6 cho `kill` không hoàn toàn thỏa đáng: có các vòng lặp ngủ có lẽ nên kiểm tra `p->killed`. Một vấn đề liên quan là, ngay cả đối với các vòng lặp ngủ kiểm tra `p->killed`, có một cuộc đua giữa `sleep` và `kill`; cái sau có thể đặt `p->killed` và cố gắng đánh thức nạn nhân ngay sau khi vòng lặp của nạn nhân kiểm tra `p->killed` nhưng trước khi nó gọi `sleep`. Nếu vấn đề này xảy ra, nạn nhân sẽ không nhận thấy `p->killed` cho đến khi điều kiện mà nó đang chờ đợi xảy ra. Điều này có thể khá lâu sau hoặc thậm chí không bao giờ (ví dụ: nếu nạn nhân đang chờ đầu vào từ giao diện điều khiển, nhưng người dùng không gõ bất kỳ đầu vào nào).

Một hệ điều hành thực sẽ tìm thấy các cấu trúc `proc` miễn phí với một danh sách miễn phí rõ ràng trong thời gian không đổi thay vì tìm kiếm tuyến tính trong `allocproc`; xv6 sử dụng quét tuyến tính để đơn giản.

### 7.11 Bài tập

1.  Triển khai các semaphore trong xv6 mà không cần sử dụng `sleep` và `wakeup` (nhưng được phép sử dụng các khóa quay). Chọn một vài lần sử dụng `sleep` và `wakeup` của xv6 và thay thế chúng bằng các semaphore. Đánh giá kết quả.

2.  Khắc phục cuộc đua được đề cập ở trên giữa `kill` và `sleep`, để một `kill` xảy ra sau khi vòng lặp ngủ của nạn nhân kiểm tra `p->killed` nhưng trước khi nó gọi `sleep` dẫn đến việc nạn nhân từ bỏ lời gọi hệ thống hiện tại.

3.  Thiết kế một kế hoạch để mọi vòng lặp ngủ đều kiểm tra `p->killed` để, ví dụ, một tiến trình ở trong trình điều khiển virtio có thể nhanh chóng quay trở lại từ vòng lặp `while` nếu nó bị một tiến trình khác giết.

4.  Sửa đổi xv6 để chỉ sử dụng một chuyển đổi ngữ cảnh khi chuyển từ luồng hạt nhân của một tiến trình này sang một tiến trình khác, thay vì chuyển qua luồng lập lịch. Luồng nhường sẽ cần tự chọn luồng tiếp theo và gọi `swtch`. Các thách thức sẽ là ngăn chặn nhiều CPU thực thi cùng một luồng một cách vô tình; để có được việc khóa đúng; và để tránh các khóa chết.

5.  Sửa đổi bộ lập lịch của xv6 để sử dụng lệnh `WFI` (chờ ngắt) của RISC-V khi không có tiến trình nào có thể chạy được. Cố gắng đảm bảo rằng, bất cứ khi nào có các tiến trình có thể chạy được đang chờ chạy, không có CPU nào đang tạm dừng trong `WFI`.

## 8 Hệ thống tệp

Mục đích của một hệ thống tệp là để tổ chức và lưu trữ dữ liệu. Các hệ thống tệp thường hỗ trợ việc chia sẻ dữ liệu giữa những người dùng và các ứng dụng, cũng như *sự bền bỉ* để dữ liệu vẫn có sẵn sau khi khởi động lại.

Hệ thống tệp xv6 cung cấp các tệp, thư mục và tên đường dẫn giống Unix (xem Chương 1), và lưu trữ dữ liệu của nó trên một đĩa virtio để có sự bền bỉ. Hệ thống tệp giải quyết một số thách thức:

*   Hệ thống tệp cần các cấu trúc dữ liệu trên đĩa để biểu diễn cây các thư mục và tệp được đặt tên, để ghi lại danh tính của các khối chứa nội dung của mỗi tệp, và để ghi lại các vùng nào của đĩa là miễn phí.

*   Hệ thống tệp phải hỗ trợ *phục hồi sau sự cố*. Tức là, nếu một sự cố (ví dụ: mất điện) xảy ra, hệ thống tệp vẫn phải hoạt động chính xác sau khi khởi động lại. Rủi ro là một sự cố có thể ngắt một chuỗi các cập nhật và để lại các cấu trúc dữ liệu trên đĩa không nhất quán (ví dụ: một khối vừa được sử dụng trong một tệp vừa được đánh dấu là miễn phí).

*   Các tiến trình khác nhau có thể hoạt động trên hệ thống tệp cùng một lúc, vì vậy mã hệ thống tệp phải phối hợp để duy trì các bất biến.

*   Việc truy cập một đĩa chậm hơn nhiều bậc so với việc truy cập bộ nhớ, vì vậy hệ thống tệp phải duy trì một bộ nhớ cache trong bộ nhớ của các khối phổ biến.

Phần còn lại của chương này giải thích cách xv6 giải quyết những thách thức này.

### 8.1 Tổng quan

Việc triển khai hệ thống tệp xv6 được tổ chức thành bảy lớp, được hiển thị trong Hình 8.1. Lớp đĩa đọc và ghi các khối trên một ổ đĩa cứng virtio. Lớp bộ đệm cache lưu trữ các khối đĩa và đồng bộ hóa quyền truy cập vào chúng, đảm bảo rằng chỉ một tiến trình hạt nhân tại một thời điểm có thể sửa đổi dữ liệu được lưu trữ trong bất kỳ khối cụ thể nào. Lớp ghi nhật ký cho phép các lớp cao hơn gói các cập nhật cho một số khối trong một *giao dịch*, và đảm bảo rằng các khối được cập nhật một cách nguyên tử khi có sự cố (tức là, tất cả chúng đều được cập nhật hoặc không có cái nào). Lớp inode cung cấp các tệp riêng lẻ, mỗi tệp được biểu diễn dưới dạng một *inode* với một số i duy nhất và một số khối chứa dữ liệu của tệp. Lớp thư mục thực hiện mỗi thư mục như một loại inode đặc biệt có nội dung là một chuỗi các mục nhập thư mục, mỗi mục chứa tên tệp và số i. Lớp tên đường dẫn cung cấp các tên đường dẫn phân cấp như `/usr/rtm/xv6/fs.c`, và giải quyết chúng bằng cách tra cứu đệ quy. Lớp mô tả tệp trừu tượng hóa nhiều tài nguyên Unix (ví dụ: các ống dẫn, thiết bị, tệp, v.v.) bằng cách sử dụng giao diện hệ thống tệp, đơn giản hóa cuộc sống của các lập trình viên ứng dụng.

Phần cứng đĩa theo truyền thống trình bày dữ liệu trên đĩa dưới dạng một chuỗi được đánh số của các khối 512 byte (còn được gọi là các *sector*): sector 0 là 512 byte đầu tiên, sector 1 là tiếp theo, v.v. Kích thước khối mà một hệ điều hành sử dụng cho hệ thống tệp của nó có thể khác với kích thước sector mà một đĩa sử dụng, nhưng thông thường kích thước khối là một bội số của kích thước sector. Xv6 chứa các bản sao của các khối mà nó đã đọc vào bộ nhớ trong các đối tượng thuộc loại `struct buf` (kernel/buf.h:1). Dữ liệu được lưu trữ trong cấu trúc này đôi khi không đồng bộ với đĩa: nó có thể chưa được đọc từ đĩa (đĩa đang làm việc trên nó nhưng chưa trả về nội dung của sector), hoặc nó có thể đã được cập nhật bởi phần mềm nhưng chưa được ghi vào đĩa.

Hệ thống tệp phải có một kế hoạch về nơi nó lưu trữ các inode và các khối nội dung trên đĩa. Để làm như vậy, xv6 chia đĩa thành nhiều phần, như Hình 8.2 cho thấy. Hệ thống tệp không sử dụng khối 0 (nó chứa sector khởi động). Khối 1 được gọi là *siêu khối*; nó chứa siêu dữ liệu về hệ thống tệp (kích thước hệ thống tệp tính bằng khối, số lượng khối dữ liệu, số lượng inode, và số lượng khối trong nhật ký). Các khối bắt đầu từ 2 chứa nhật ký. Sau nhật ký là các inode, với nhiều inode trên mỗi khối. Sau đó là các khối bitmap theo dõi các khối dữ liệu nào đang được sử dụng. Các khối còn lại là các khối dữ liệu; mỗi khối hoặc được đánh dấu là miễn phí trong khối bitmap, hoặc chứa nội dung cho một tệp hoặc thư mục. Siêu khối được điền bởi một chương trình riêng biệt, được gọi là `mkfs`, xây dựng một hệ thống tệp ban đầu.

Phần còn lại của chương này thảo luận về từng lớp, bắt đầu với bộ đệm cache. Hãy tìm kiếm các tình huống trong đó các sự trừu tượng hóa được lựa chọn tốt ở các lớp thấp hơn giúp thiết kế các lớp cao hơn dễ dàng hơn.

### 8.2 Lớp đệm cache

Bộ đệm cache có hai công việc: (1) đồng bộ hóa quyền truy cập vào các khối đĩa để đảm bảo rằng chỉ một bản sao của một khối ở trong bộ nhớ và chỉ một luồng hạt nhân tại một thời điểm sử dụng bản sao đó; (2) lưu trữ các khối phổ biến để chúng không cần phải được đọc lại từ đĩa chậm. Mã nằm trong `bio.c`.

Giao diện chính được xuất bởi bộ đệm cache bao gồm `bread` và `bwrite`; cái trước lấy một `buf` chứa một bản sao của một khối có thể được đọc hoặc sửa đổi trong bộ nhớ, và cái sau ghi một bộ đệm đã sửa đổi vào khối thích hợp trên đĩa. Một luồng hạt nhân phải giải phóng một bộ đệm bằng cách gọi `brelse` khi nó đã hoàn thành với nó. Bộ đệm cache sử dụng một khóa ngủ trên mỗi bộ đệm để đảm bảo rằng chỉ một luồng tại một thời điểm sử dụng mỗi bộ đệm (và do đó mỗi khối đĩa); `bread` trả về một bộ đệm bị khóa, và `brelse` giải phóng khóa.

Hãy quay trở lại bộ đệm cache. Bộ đệm cache có một số lượng bộ đệm cố định để chứa các khối đĩa, điều này có nghĩa là nếu hệ thống tệp yêu cầu một khối chưa có trong bộ đệm, bộ đệm cache phải tái chế một bộ đệm hiện đang chứa một khối khác. Bộ đệm cache tái chế bộ đệm được sử dụng ít gần đây nhất cho khối mới. Giả định là bộ đệm được sử dụng ít gần đây nhất là bộ đệm ít có khả năng được sử dụng lại sớm nhất.

### 8.3 Mã: Đệm cache

Bộ đệm cache là một danh sách liên kết đôi của các bộ đệm. Hàm `binit`, được gọi bởi `main` (kernel/main.c:27), khởi tạo danh sách với `NBUF` bộ đệm trong mảng tĩnh `buf` (kernel/bio.c:43-52). Tất cả các quyền truy cập khác vào bộ đệm cache đều tham chiếu đến danh sách liên kết thông qua `bcache.head`, không phải mảng `buf`.

Một bộ đệm có hai trường trạng thái được liên kết với nó. Trường `valid` cho biết rằng bộ đệm chứa một bản sao của khối. Trường `disk` cho biết rằng nội dung bộ đệm đã được chuyển cho đĩa, đĩa có thể thay đổi bộ đệm (ví dụ: ghi dữ liệu từ đĩa vào `data`).

`bread` (kernel/bio.c:93) gọi `bget` để lấy một bộ đệm cho sector đã cho (kernel/bio.c:97). Nếu bộ đệm cần được đọc từ đĩa, `bread` sẽ gọi `virtio_disk_rw` để làm điều đó trước khi trả về bộ đệm.

`bget` (kernel/bio.c:59) quét danh sách bộ đệm để tìm một bộ đệm với các số thiết bị và sector đã cho (kernel/bio.c:65-73). Nếu có một bộ đệm như vậy, `bget` sẽ lấy khóa ngủ cho bộ đệm. `bget` sau đó trả về bộ đệm bị khóa.

Nếu không có bộ đệm được lưu trong bộ nhớ cache cho sector đã cho, `bget` phải tạo một bộ đệm, có thể tái sử dụng một bộ đệm đã chứa một sector khác. Nó quét danh sách bộ đệm lần thứ hai, tìm kiếm một bộ đệm không được sử dụng (`b->refcnt == 0`); bất kỳ bộ đệm nào như vậy đều có thể được sử dụng. `bget` chỉnh sửa siêu dữ liệu của bộ đệm để ghi lại số thiết bị và sector mới và lấy khóa ngủ của nó. Lưu ý rằng việc gán `b->valid = 0` đảm bảo rằng `bread` sẽ đọc dữ liệu khối từ đĩa thay vì sử dụng không chính xác nội dung trước đó của bộ đệm.

Điều quan trọng là có nhiều nhất một bộ đệm được lưu trong bộ nhớ cache cho mỗi sector đĩa, để đảm bảo rằng người đọc thấy các lần ghi, và bởi vì hệ thống tệp sử dụng các khóa trên các bộ đệm để đồng bộ hóa. `bget` đảm bảo bất biến này bằng cách giữ `bcache.lock` `bcache.lock` liên tục từ lần kiểm tra của vòng lặp đầu tiên về việc khối có được lưu trong bộ nhớ cache hay không thông qua khai báo của vòng lặp thứ hai rằng khối hiện đã được lưu trong bộ nhớ cache (bằng cách đặt `dev`, `blockno`, và `refcnt`). Điều này làm cho việc kiểm tra sự hiện diện của một khối và (nếu không có) việc chỉ định một bộ đệm để chứa khối là nguyên tử.

An toàn khi `bget` lấy khóa ngủ của bộ đệm bên ngoài phần quan trọng của `bcache.lock`, vì `b->refcnt` khác không ngăn bộ đệm bị tái sử dụng cho một khối đĩa khác. Khóa ngủ bảo vệ các lần đọc và ghi của nội dung được đệm của khối, trong khi `bcache.lock` bảo vệ thông tin về các khối nào được lưu trong bộ nhớ cache.

Nếu tất cả các bộ đệm đều bận, thì quá nhiều tiến trình đang thực thi các lời gọi hệ thống tệp đồng thời; `bget` hoảng loạn. Một phản ứng duyên dáng hơn có thể là ngủ cho đến khi một bộ đệm trở nên miễn phí, mặc dù sau đó sẽ có khả năng xảy ra khóa chết.

Sau khi `bread` đã đọc đĩa (nếu cần) và trả về bộ đệm cho người gọi của nó, người gọi có độc quyền sử dụng bộ đệm và có thể đọc hoặc ghi các byte dữ liệu. Nếu người gọi sửa đổi bộ đệm, nó phải gọi `bwrite` để ghi dữ liệu đã thay đổi vào đĩa trước khi giải phóng bộ đệm. `bwrite` (kernel/bio.c:107) gọi `virtio_disk_rw` để nói chuyện với phần cứng đĩa.

Khi người gọi đã hoàn thành với một bộ đệm, nó phải gọi `brelse` để giải phóng nó. (Tên `brelse`, một cách viết tắt của b-release, là khó hiểu nhưng đáng để học: nó bắt nguồn từ Unix và cũng được sử dụng trong BSD, Linux và Solaris.) `brelse` (kernel/bio.c:117) giải phóng khóa ngủ và di chuyển bộ đệm đến đầu danh sách liên kết (kernel/bio.c:128-133). Việc di chuyển bộ đệm làm cho danh sách được sắp xếp theo mức độ gần đây mà các bộ đệm đã được sử dụng (có nghĩa là được giải phóng): bộ đệm đầu tiên trong danh sách là bộ đệm được sử dụng gần đây nhất, và bộ đệm cuối cùng là bộ đệm được sử dụng ít gần đây nhất. Hai vòng lặp trong `bget` tận dụng điều này: việc quét tìm một bộ đệm hiện có phải xử lý toàn bộ danh sách trong trường hợp xấu nhất, nhưng việc kiểm tra các bộ đệm được sử dụng gần đây nhất trước (bắt đầu từ `bcache.head` và theo các con trỏ `next`) sẽ giảm thời gian quét khi có tính cục bộ tốt của tham chiếu. Việc quét để chọn một bộ đệm để tái sử dụng sẽ chọn bộ đệm được sử dụng ít gần đây nhất bằng cách quét ngược (theo các con trỏ `prev`).

### 8.4 Lớp ghi nhật ký

Một trong những vấn đề thú vị nhất trong thiết kế hệ thống tệp là phục hồi sau sự cố. Vấn đề phát sinh bởi vì nhiều hoạt động của hệ thống tệp liên quan đến nhiều lần ghi vào đĩa, và một sự cố sau một tập hợp con các lần ghi có thể để lại hệ thống tệp trên đĩa trong một trạng thái không nhất quán. Ví dụ, giả sử một sự cố xảy ra trong quá trình cắt ngắn tệp (đặt độ dài của một tệp thành 0 và giải phóng các khối nội dung của nó). Tùy thuộc vào thứ tự của các lần ghi đĩa, sự cố có thể để lại một inode với một tham chiếu đến một khối nội dung được đánh dấu là miễn phí, hoặc nó có thể để lại một khối nội dung được phân bổ nhưng không được tham chiếu.

Cái sau tương đối lành tính, nhưng một inode tham chiếu đến một khối đã được giải phóng có khả năng gây ra các vấn đề nghiêm trọng sau khi khởi động lại. Sau khi khởi động lại, hạt nhân có thể phân bổ khối đó cho một tệp khác, và bây giờ chúng ta có hai tệp khác nhau trỏ vô tình đến cùng một khối. Nếu xv6 hỗ trợ nhiều người dùng, tình huống này có thể là một vấn đề bảo mật, vì chủ sở hữu của tệp cũ sẽ có thể đọc và ghi các khối trong tệp mới, thuộc sở hữu của một người dùng khác.

Xv6 giải quyết vấn đề sự cố trong các hoạt động của hệ thống tệp bằng một hình thức ghi nhật ký đơn giản. Một lời gọi hệ thống xv6 không ghi trực tiếp các cấu trúc dữ liệu của hệ thống tệp trên đĩa. Thay vào đó, nó đặt một mô tả về tất cả các lần ghi đĩa mà nó muốn thực hiện trong một *nhật ký* trên đĩa. Sau khi lời gọi hệ thống đã ghi lại tất cả các lần ghi của nó, nó sẽ ghi một bản ghi *cam kết* đặc biệt vào đĩa cho biết rằng nhật ký chứa một hoạt động hoàn chỉnh. Tại thời điểm đó, lời gọi hệ thống sao chép các lần ghi vào các cấu trúc dữ liệu của hệ thống tệp trên đĩa. Sau khi các lần ghi đó đã hoàn tất, lời gọi hệ thống sẽ xóa nhật ký trên đĩa.

Nếu hệ thống gặp sự cố và khởi động lại, mã hệ thống tệp sẽ phục hồi sau sự cố như sau, trước khi chạy bất kỳ tiến trình nào. Nếu nhật ký được đánh dấu là chứa một hoạt động hoàn chỉnh, thì mã phục hồi sẽ sao chép các lần ghi vào nơi chúng thuộc về trong hệ thống tệp trên đĩa. Nếu nhật ký không được đánh dấu là chứa một hoạt động hoàn chỉnh, mã phục hồi sẽ bỏ qua nhật ký. Mã phục hồi kết thúc bằng cách xóa nhật ký.

Tại sao nhật ký của xv6 giải quyết được vấn đề sự cố trong các hoạt động của hệ thống tệp? Nếu sự cố xảy ra trước khi hoạt động cam kết, thì nhật ký trên đĩa sẽ không được đánh dấu là hoàn chỉnh, mã phục hồi sẽ bỏ qua nó, và trạng thái của đĩa sẽ như thể hoạt động chưa bao giờ bắt đầu. Nếu sự cố xảy ra sau khi hoạt động cam kết, thì quá trình phục hồi sẽ phát lại tất cả các lần ghi của hoạt động, có lẽ lặp lại chúng nếu hoạt động đã bắt đầu ghi chúng vào cấu trúc dữ liệu trên đĩa. Trong cả hai trường hợp, nhật ký làm cho các hoạt động trở nên *nguyên tử* đối với các sự cố: sau khi phục hồi, hoặc tất cả các lần ghi của hoạt động xuất hiện trên đĩa, hoặc không có lần ghi nào xuất hiện.

### 8.5 Thiết kế nhật ký

Nhật ký nằm ở một vị trí cố định đã biết, được chỉ định trong siêu khối. Nó bao gồm một khối tiêu đề theo sau là một chuỗi các bản sao khối được cập nhật (“các khối được ghi nhật ký”). Khối tiêu đề chứa một mảng các số sector, một cho mỗi khối được ghi nhật ký, và số lượng các khối nhật ký. Số lượng trong khối tiêu đề trên đĩa hoặc là 0, cho biết rằng không có giao dịch nào trong nhật ký, hoặc khác 0, cho biết rằng nhật ký chứa một giao dịch đã cam kết hoàn chỉnh với số lượng các khối được ghi nhật ký đã cho. Xv6 ghi khối tiêu đề khi một giao dịch cam kết, nhưng không phải trước đó, và đặt số lượng thành 0 sau khi sao chép các khối được ghi nhật ký vào hệ thống tệp. Do đó, một sự cố giữa chừng một giao dịch sẽ dẫn đến một số lượng bằng 0 trong khối tiêu đề của nhật ký; một sự cố sau một cam kết sẽ dẫn đến một số lượng khác 0.

Mã của mỗi lời gọi hệ thống cho biết sự bắt đầu và kết thúc của chuỗi các lần ghi phải là nguyên tử đối với các sự cố. Để cho phép thực thi đồng thời các hoạt động của hệ thống tệp bởi các tiến trình khác nhau, hệ thống ghi nhật ký có thể tích lũy các lần ghi của nhiều lời gọi hệ thống vào một giao dịch. Do đó, một cam kết duy nhất có thể liên quan đến các lần ghi của nhiều lời gọi hệ thống hoàn chỉnh. Để tránh chia một lời gọi hệ thống qua các giao dịch, hệ thống ghi nhật ký chỉ cam kết khi không có lời gọi hệ thống tệp nào đang diễn ra.

Ý tưởng cam kết nhiều giao dịch cùng nhau được gọi là *cam kết nhóm*. Cam kết nhóm làm giảm số lượng các hoạt động đĩa bởi vì nó phân bổ chi phí cố định của một cam kết cho nhiều hoạt động. Cam kết nhóm cũng cung cấp cho hệ thống đĩa nhiều lần ghi đồng thời cùng một lúc, có lẽ cho phép đĩa ghi tất cả chúng trong một vòng quay đĩa duy nhất. Trình điều khiển virtio của Xv6 không hỗ trợ loại phân lô này, nhưng thiết kế hệ thống tệp của xv6 cho phép nó.

Xv6 dành một lượng không gian cố định trên đĩa để chứa nhật ký. Tổng số khối được ghi bởi các lời gọi hệ thống trong một giao dịch phải vừa với không gian đó. Điều này có hai hệ quả. Không một lời gọi hệ thống nào được phép ghi nhiều khối riêng biệt hơn so với không gian có trong nhật ký. Đây không phải là vấn đề đối với hầu hết các lời gọi hệ thống, nhưng hai trong số chúng có khả năng ghi nhiều khối: `write` và `unlink`. Một lần ghi tệp lớn có thể ghi nhiều khối dữ liệu và nhiều khối bitmap cũng như một khối inode; việc hủy liên kết một tệp lớn có thể ghi nhiều khối bitmap và một inode. Lời gọi hệ thống `write` của Xv6 chia các lần ghi lớn thành nhiều lần ghi nhỏ hơn vừa với nhật ký, và `unlink` không gây ra vấn đề bởi vì trong thực tế, hệ thống tệp xv6 chỉ sử dụng một khối bitmap. Hệ quả khác của không gian nhật ký hạn chế là hệ thống ghi nhật ký không thể cho phép một lời gọi hệ thống bắt đầu trừ khi nó chắc chắn rằng các lần ghi của lời gọi hệ thống sẽ vừa với không gian còn lại trong nhật ký.

### 8.6 Mã: ghi nhật ký

Một cách sử dụng điển hình của nhật ký trong một lời gọi hệ thống trông như thế này:

```c
begin_op();
...
bp = bread(...);
bp->data[...] = ...;
log_write(bp);
...
end_op();
```

`begin_op` (kernel/log.c:127) đợi cho đến khi hệ thống ghi nhật ký hiện không cam kết, và cho đến khi có đủ không gian nhật ký chưa được đặt trước để chứa các lần ghi từ lệnh gọi này. `log.outstanding` đếm số lượng các lời gọi hệ thống đã đặt trước không gian nhật ký; tổng không gian được đặt trước là `log.outstanding` nhân `MAXOPBLOCKS`. Việc tăng `log.outstanding` vừa đặt trước không gian vừa ngăn cản một cam kết xảy ra trong lời gọi hệ thống này. Mã này giả định một cách bảo thủ rằng mỗi lời gọi hệ thống có thể ghi tới `MAXOPBLOCKS` khối riêng biệt.

`log_write` (kernel/log.c:215) hoạt động như một proxy cho `bwrite`. Nó ghi lại số sector của khối trong bộ nhớ, đặt trước cho nó một khe trong nhật ký trên đĩa, và ghim bộ đệm vào bộ đệm khối để ngăn bộ đệm khối loại bỏ nó. Khối phải ở trong bộ đệm cho đến khi được cam kết: cho đến lúc đó, bản sao được lưu trong bộ nhớ cache là bản ghi duy nhất của sửa đổi; nó không thể được ghi vào vị trí của nó trên đĩa cho đến sau khi cam kết; và các lần đọc khác trong cùng một giao dịch phải thấy các sửa đổi. `log_write` nhận thấy khi một khối được ghi nhiều lần trong một giao dịch duy nhất, và phân bổ cho khối đó cùng một khe trong nhật ký. Tối ưu hóa này thường được gọi là *hấp thụ*. Điều phổ biến là, ví dụ, khối đĩa chứa các inode của một số tệp được ghi nhiều lần trong một giao dịch. Bằng cách hấp thụ nhiều lần ghi đĩa thành một, hệ thống tệp có thể tiết kiệm không gian nhật ký và có thể đạt được hiệu suất tốt hơn vì chỉ có một bản sao của khối đĩa phải được ghi.

`end_op` (kernel/log.c:147) đầu tiên giảm số lượng các lời gọi hệ thống đang chờ xử lý. Nếu số lượng bây giờ là 0, nó sẽ cam kết giao dịch hiện tại bằng cách gọi `commit()`. Có bốn giai đoạn trong quá trình này. `write_log()` (kernel/log.c:179) sao chép mỗi khối đã được sửa đổi trong giao dịch từ bộ đệm khối vào khe của nó trong nhật ký trên đĩa. `write_head()` (kernel/log.c:103) ghi khối tiêu đề vào đĩa: đây là điểm cam kết, và một sự cố sau khi ghi sẽ dẫn đến việc phục hồi phát lại các lần ghi của giao dịch từ nhật ký. `install_trans` (kernel/log.c:69) đọc mỗi khối từ nhật ký và ghi nó vào vị trí thích hợp trong hệ thống tệp. Cuối cùng, `end_op` ghi tiêu đề nhật ký với một số lượng bằng 0; điều này phải xảy ra trước khi giao dịch tiếp theo bắt đầu ghi các khối được ghi nhật ký, để một sự cố không dẫn đến việc phục hồi sử dụng tiêu đề của một giao dịch với các khối được ghi nhật ký của giao dịch tiếp theo.

`recover_from_log` (kernel/log.c:117) được gọi từ `initlog` (kernel/log.c:55), được gọi từ `fsinit` (kernel/fs.c:42) trong quá trình khởi động trước khi tiến trình người dùng đầu tiên chạy (kernel/proc.c:535). Nó đọc tiêu đề nhật ký, và bắt chước các hành động của `end_op` nếu tiêu đề cho biết rằng nhật ký chứa một giao dịch đã cam kết.

Một ví dụ về việc sử dụng nhật ký xảy ra trong `filewrite` (kernel/file.c:135). Giao dịch trông như thế này:

```c
begin_op();
ilock(f->ip);
r = writei(f->ip, ...);
iunlock(f->ip);
end_op();
```

Mã này được gói trong một vòng lặp chia các lần ghi lớn thành các giao dịch riêng lẻ chỉ gồm một vài sector tại một thời điểm, để tránh làm tràn nhật ký. Lệnh gọi đến `writei` ghi nhiều khối như một phần của giao dịch này: inode của tệp, một hoặc nhiều khối bitmap, và một số khối dữ liệu.

### 8.7 Mã: Bộ cấp phát khối

Nội dung tệp và thư mục được lưu trữ trong các khối đĩa, phải được phân bổ từ một nhóm miễn phí. Bộ cấp phát khối của Xv6 duy trì một bitmap miễn phí trên đĩa, với một bit cho mỗi khối. Một bit 0 cho biết rằng khối tương ứng là miễn phí; một bit 1 cho biết rằng nó đang được sử dụng. Chương trình `mkfs` đặt các bit tương ứng với sector khởi động, siêu khối, các khối nhật ký, các khối inode, và các khối bitmap.

Bộ cấp phát khối cung cấp hai hàm: `balloc` phân bổ một khối đĩa mới, và `bfree` giải phóng một khối. `balloc` Vòng lặp trong `balloc` tại (kernel/fs.c:72) xem xét mọi khối, bắt đầu từ khối 0 lên đến `sb.size`, số lượng khối trong hệ thống tệp. Nó tìm kiếm một khối có bit bitmap là 0, cho biết rằng nó là miễn phí. Nếu `balloc` tìm thấy một khối như vậy, nó sẽ cập nhật bitmap và trả về khối. Để hiệu quả, vòng lặp được chia thành hai phần. Vòng lặp bên ngoài đọc mỗi khối của các bit bitmap. Vòng lặp bên trong kiểm tra tất cả các bit `Bits-Per-Block` (BPB) trong một khối bitmap duy nhất. Cuộc đua có thể xảy ra nếu hai tiến trình cố gắng phân bổ một khối cùng một lúc được ngăn chặn bởi thực tế là bộ đệm khối chỉ cho phép một tiến trình sử dụng bất kỳ một khối bitmap nào tại một thời điểm.

`bfree` (kernel/fs.c:92) tìm khối bitmap phù hợp và xóa bit phù hợp. Một lần nữa, việc sử dụng độc quyền được ngụ ý bởi `bread` và `brelse` tránh sự cần thiết phải khóa rõ ràng.

Như với phần lớn mã được mô tả trong phần còn lại của chương này, `balloc` và `bfree` phải được gọi bên trong một giao dịch.

### 8.8 Lớp Inode

Thuật ngữ *inode* có thể có một trong hai ý nghĩa liên quan. Nó có thể đề cập đến cấu trúc dữ liệu trên đĩa chứa kích thước của một tệp và danh sách các số khối dữ liệu. Hoặc “inode” có thể đề cập đến một inode trong bộ nhớ, chứa một bản sao của inode trên đĩa cũng như thông tin bổ sung cần thiết trong hạt nhân.

Các inode trên đĩa được đóng gói vào một vùng liền kề của đĩa được gọi là các khối inode. Mỗi inode có cùng kích thước, vì vậy rất dễ dàng, với một số `n`, để tìm inode thứ n trên đĩa. Trên thực tế, số `n` này, được gọi là *số inode* hoặc *số i*, là cách các inode được xác định trong việc triển khai.

Inode trên đĩa được định nghĩa bởi một `struct dinode` (kernel/fs.h:32). Trường `type` phân biệt giữa các tệp, thư mục và các tệp đặc biệt (thiết bị). Một loại bằng 0 cho biết rằng một inode trên đĩa là miễn phí. Trường `nlink` đếm số lượng các mục nhập thư mục tham chiếu đến inode này, để nhận ra khi nào inode trên đĩa và các khối dữ liệu của nó sẽ được giải phóng. Trường `size` ghi lại số byte nội dung trong tệp. Mảng `addrs` ghi lại các số khối của các khối đĩa chứa nội dung của tệp.

Hạt nhân giữ tập hợp các inode đang hoạt động trong bộ nhớ trong một bảng được gọi là `itable`; `struct inode` (kernel/file.h:17) là bản sao trong bộ nhớ của một `struct dinode` trên đĩa. Hạt nhân chỉ lưu trữ một inode trong bộ nhớ nếu có các con trỏ C tham chiếu đến inode đó. Trường `ref` đếm số lượng các con trỏ C tham chiếu đến inode trong bộ nhớ, và hạt nhân loại bỏ inode khỏi bộ nhớ nếu số lượng tham chiếu giảm xuống 0. Các hàm `iget` và `iput` lấy và giải phóng các con trỏ đến một inode, sửa đổi số lượng tham chiếu. Các con trỏ đến một inode có thể đến từ các mô tả tệp, các thư mục làm việc hiện tại, và mã hạt nhân tạm thời như `exec`.

Có bốn cơ chế khóa hoặc giống như khóa trong mã inode của xv6. `itable.lock` bảo vệ bất biến rằng một inode có mặt trong bảng inode nhiều nhất một lần, và bất biến rằng trường `ref` của một inode trong bộ nhớ đếm số lượng các con trỏ trong bộ nhớ đến inode. Mỗi inode trong bộ nhớ có một trường `lock` chứa một khóa ngủ, đảm bảo quyền truy cập độc quyền vào các trường của inode (chẳng hạn như độ dài tệp) cũng như vào các khối nội dung tệp hoặc thư mục của inode. `ref` của một inode, nếu nó lớn hơn 0, khiến hệ thống duy trì inode trong bảng, và không sử dụng lại mục nhập bảng cho một inode khác. Cuối cùng, mỗi inode chứa một trường `nlink` (trên đĩa và được sao chép trong bộ nhớ nếu trong bộ nhớ) đếm số lượng các mục nhập thư mục tham chiếu đến một tệp; xv6 sẽ không giải phóng một inode nếu số lượng liên kết của nó lớn hơn 0.

Một con trỏ `struct inode` được trả về bởi `iget()` được đảm bảo là hợp lệ cho đến lệnh gọi tương ứng đến `iput()`; inode sẽ không bị xóa, và bộ nhớ được tham chiếu bởi con trỏ sẽ không được sử dụng lại cho một inode khác. `iget()` cung cấp quyền truy cập không độc quyền vào một inode, do đó có thể có nhiều con trỏ đến cùng một inode. Nhiều phần của mã hệ thống tệp phụ thuộc vào hành vi này của `iget()`, cả để giữ các tham chiếu dài hạn đến các inode (như các tệp đang mở và các thư mục hiện tại) và để ngăn chặn các tranh chấp trong khi tránh khóa chết trong mã thao tác nhiều inode (chẳng hạn như tra cứu tên đường dẫn).

`struct inode` mà `iget` trả về có thể không có nội dung hữu ích nào. Để đảm bảo nó chứa một bản sao của inode trên đĩa, mã phải gọi `ilock`. Điều này khóa inode (để không có tiến trình nào khác có thể `ilock` nó) và đọc inode từ đĩa, nếu nó chưa được đọc. `iunlock` giải phóng khóa trên inode. Việc tách việc lấy các con trỏ inode khỏi việc khóa giúp tránh khóa chết trong một số tình huống, ví dụ như trong quá trình tra cứu thư mục. Nhiều tiến trình có thể giữ một con trỏ C đến một inode được trả về bởi `iget`, nhưng chỉ một tiến trình có thể khóa inode tại một thời điểm.

Bảng inode chỉ lưu trữ các inode mà mã hạt nhân hoặc các cấu trúc dữ liệu giữ các con trỏ C. Công việc chính của nó là đồng bộ hóa quyền truy cập bởi nhiều tiến trình. Bảng inode cũng tình cờ lưu trữ các inode được sử dụng thường xuyên, nhưng việc lưu trữ là thứ yếu; nếu một inode được sử dụng thường xuyên, bộ đệm cache có thể sẽ giữ nó trong bộ nhớ. Mã sửa đổi một inode trong bộ nhớ sẽ ghi nó vào đĩa bằng `iupdate`.

### 8.9 Mã: Inode

Để phân bổ một inode mới (ví dụ, khi tạo một tệp), xv6 gọi `ialloc` (kernel/fs.c:199). `ialloc` tương tự như `balloc`: nó lặp qua các cấu trúc inode trên đĩa, một khối tại một thời điểm, tìm kiếm một cấu trúc được đánh dấu là miễn phí. Khi nó tìm thấy một cấu trúc, nó yêu cầu nó bằng cách ghi loại mới vào đĩa và sau đó trả về một mục từ bảng inode với lệnh gọi đuôi đến `iget` (kernel/fs.c:213). Hoạt động chính xác của `ialloc` phụ thuộc vào thực tế là chỉ một tiến trình tại một thời điểm có thể giữ một tham chiếu đến `bp`: `ialloc` có thể chắc chắn rằng một tiến trình khác không đồng thời thấy rằng inode có sẵn và cố gắng yêu cầu nó.

`iget` (kernel/fs.c:247) tìm kiếm trong bảng inode để tìm một mục đang hoạt động (`ip->ref > 0`) với số thiết bị và inode mong muốn. Nếu nó tìm thấy một mục, nó sẽ trả về một tham chiếu mới đến inode đó (kernel/fs.c:256-260). Khi `iget` quét, nó ghi lại vị trí của khe trống đầu tiên (kernel/fs.c:261-262), mà nó sử dụng nếu nó cần phân bổ một mục nhập bảng.

Mã phải khóa inode bằng `ilock` trước khi đọc hoặc ghi siêu dữ liệu hoặc nội dung của nó. `ilock` (kernel/fs.c:293) sử dụng một khóa ngủ cho mục đích này. Sau khi `ilock` có quyền truy cập độc quyền vào inode, nó sẽ đọc inode từ đĩa (nhiều khả năng là bộ đệm cache) nếu cần. Hàm `iunlock` (kernel/fs.c:321) giải phóng khóa ngủ, có thể khiến bất kỳ tiến trình nào đang ngủ được đánh thức.

`iput` (kernel/fs.c:337) giải phóng một con trỏ C đến một inode bằng cách giảm số lượng tham chiếu (kernel/fs.c:360). Nếu đây là tham chiếu cuối cùng, khe của inode trong bảng inode bây giờ là miễn phí và có thể được sử dụng lại cho một inode khác.

Nếu `iput` thấy rằng không có tham chiếu con trỏ C nào đến một inode và inode không có liên kết nào đến nó (xảy ra trong không có thư mục nào), thì inode và các khối dữ liệu của nó phải được giải phóng. `iput` gọi `itrunc` để cắt ngắn tệp thành 0 byte, giải phóng các khối dữ liệu; đặt loại inode thành 0 (chưa được phân bổ); và ghi inode vào đĩa (kernel/fs.c:342).

Giao thức khóa trong `iput` trong trường hợp nó giải phóng inode đáng được xem xét kỹ hơn. Một nguy cơ là một luồng đồng thời có thể đang chờ trong `ilock` để sử dụng inode này (ví dụ: để đọc một tệp hoặc liệt kê một thư mục), và sẽ không chuẩn bị để thấy rằng inode không còn được phân bổ nữa. Điều này không thể xảy ra bởi vì không có cách nào để một lời gọi hệ thống có được một con trỏ đến một inode trong bộ nhớ nếu nó không có liên kết nào đến nó và `ip->ref` là một. Tham chiếu đó là tham chiếu thuộc sở hữu của luồng gọi `iput`. Nguy cơ chính khác là một lệnh gọi đồng thời đến `ialloc` có thể chọn cùng một inode mà `iput` đang giải phóng. Điều này chỉ có thể xảy ra sau khi `iupdate` ghi vào đĩa để inode có loại 0. Cuộc đua này là lành tính; luồng phân bổ sẽ lịch sự đợi để lấy khóa ngủ của inode trước khi đọc hoặc ghi inode, tại thời điểm đó `iput` đã hoàn thành với nó.

`iput()` có thể ghi vào đĩa. Điều này có nghĩa là bất kỳ lời gọi hệ thống nào sử dụng hệ thống tệp đều có thể ghi vào đĩa, bởi vì lời gọi hệ thống có thể là lời gọi cuối cùng có một tham chiếu đến tệp. Ngay cả các lệnh gọi như `read()` có vẻ như chỉ đọc, cũng có thể kết thúc bằng việc gọi `iput()`. Điều này, đến lượt nó, có nghĩa là ngay cả các lời gọi hệ thống chỉ đọc cũng phải được gói trong các giao dịch nếu chúng sử dụng hệ thống tệp.

Có một sự tương tác đầy thách thức giữa `iput()` và các sự cố. `iput()` không cắt ngắn một tệp ngay lập tức khi số lượng liên kết của tệp giảm xuống 0, bởi vì một số tiến trình vẫn có thể giữ một tham chiếu đến inode trong bộ nhớ: một tiến trình vẫn có thể đang đọc và ghi vào tệp, bởi vì nó đã mở nó thành công. Nhưng, nếu một sự cố xảy ra trước khi tiến trình cuối cùng đóng mô tả tệp cho tệp, thì tệp sẽ được đánh dấu là đã được phân bổ trên đĩa nhưng không có mục nhập thư mục nào trỏ đến nó.

Các hệ thống tệp xử lý trường hợp này theo một trong hai cách. Giải pháp đơn giản là khi phục hồi, sau khi khởi động lại, hệ thống tệp quét toàn bộ hệ thống tệp để tìm các tệp được đánh dấu là đã được phân bổ, nhưng không có mục nhập thư mục nào trỏ đến chúng. Nếu có bất kỳ tệp nào như vậy, thì nó có thể giải phóng các tệp đó.

Giải pháp thứ hai không yêu cầu quét hệ thống tệp. Trong giải pháp này, hệ thống tệp ghi lại trên đĩa (ví dụ: trong siêu khối) số inode của một tệp có số lượng liên kết giảm xuống 0 nhưng số lượng tham chiếu của nó không phải là 0. Nếu hệ thống tệp xóa tệp khi số lượng tham chiếu của nó đạt đến 0, thì nó sẽ cập nhật danh sách trên đĩa bằng cách xóa inode đó khỏi danh sách. Khi phục hồi, hệ thống tệp sẽ giải phóng bất kỳ tệp nào trong danh sách.

Xv6 không thực hiện giải pháp nào, điều này có nghĩa là các inode có thể được đánh dấu là đã được phân bổ trên đĩa, mặc dù chúng không còn được sử dụng nữa. Điều này có nghĩa là theo thời gian, xv6 có nguy cơ hết dung lượng đĩa.

### 8.10 Mã: Nội dung Inode

Cấu trúc inode trên đĩa, `struct dinode`, chứa một kích thước và một mảng các số khối (xem Hình 8.3). Dữ liệu inode được tìm thấy trong các khối được liệt kê trong mảng `addrs` của `dinode`. `NDIRECT` khối dữ liệu đầu tiên được liệt kê trong `NDIRECT` mục đầu tiên trong mảng; các khối này được gọi là *khối trực tiếp*. `NINDIRECT` khối dữ liệu tiếp theo được liệt kê không phải trong inode mà trong một khối dữ liệu được gọi là *khối gián tiếp*. Mục cuối cùng trong mảng `addrs` cung cấp địa chỉ của khối gián tiếp. Do đó, 12 kB đầu tiên (`NDIRECT` x `BSIZE`) của một tệp có thể được tải từ các khối được liệt kê trong inode, trong khi 256 kB tiếp theo (`NINDIRECT` × `BSIZE`) chỉ có thể được tải sau khi tham khảo khối gián tiếp. Đây là một biểu diễn trên đĩa tốt nhưng phức tạp đối với các máy khách. Hàm `bmap` quản lý biểu diễn để các quy trình cấp cao hơn, chẳng hạn như `readi` và `writei`, mà chúng ta sẽ thấy ngay sau đây, không cần phải quản lý sự phức tạp này. `bmap` trả về số khối đĩa của khối dữ liệu thứ `bn` cho inode `ip`. Nếu `ip` chưa có một khối như vậy, `bmap` sẽ phân bổ một khối.

Hàm `bmap` (kernel/fs.c:383) bắt đầu bằng cách chọn trường hợp dễ dàng: `NDIRECT` khối đầu tiên được liệt kê trong chính inode (kernel/fs.c:388-396). `NINDIRECT` khối tiếp theo được liệt kê trong khối gián tiếp tại `ip->addrs[NDIRECT]`. `bmap` đọc khối gián tiếp (kernel/fs.c:407) và sau đó đọc một số khối từ vị trí phù hợp trong khối (kernel/fs.c:408). Nếu số khối vượt quá `NDIRECT+NINDIRECT`, `bmap` sẽ hoảng loạn; `writei` chứa kiểm tra ngăn điều này xảy ra (kernel/fs.c:513).

`bmap` phân bổ các khối khi cần thiết. Một mục `ip->addrs[]` hoặc gián tiếp bằng 0 cho biết rằng không có khối nào được phân bổ. Khi `bmap` gặp các số không, nó sẽ thay thế chúng bằng các số của các khối mới, được phân bổ theo yêu cầu (kernel/fs.c:389-390) (kernel/fs.c:401-402).

`itrunc` giải phóng các khối của một tệp, đặt lại kích thước của inode thành 0. `itrunc` (kernel/fs.c:426) bắt đầu bằng cách giải phóng các khối trực tiếp (kernel/fs.c:432-437), sau đó là các khối được liệt kê trong khối gián tiếp (kernel/fs.c:442-445), và cuối cùng là chính khối gián tiếp (kernel/fs.c:447-448).

`bmap` giúp `readi` và `writei` dễ dàng truy cập vào dữ liệu của một inode. `readi` (kernel/fs.c:472) bắt đầu bằng cách đảm bảo rằng offset và số lượng không vượt quá cuối tệp. Các lần đọc bắt đầu vượt quá cuối tệp trả về một lỗi (kernel/fs.c:477-478) trong khi các lần đọc bắt đầu tại hoặc vượt qua cuối tệp trả về ít byte hơn yêu cầu (kernel/fs.c:479-480). Vòng lặp chính xử lý mỗi khối của tệp, sao chép dữ liệu từ bộ đệm vào `dst` (kernel/fs.c:482-494). `writei` (kernel/fs.c:506) giống hệt `readi`, với ba ngoại lệ: các lần ghi bắt đầu tại hoặc vượt qua cuối tệp sẽ phát triển tệp, lên đến kích thước tệp tối đa (kernel/fs.c:513-514); vòng lặp sao chép dữ liệu vào các bộ đệm thay vì ra ngoài (kernel/fs.c:522); và nếu việc ghi đã mở rộng tệp, `writei` phải cập nhật kích thước của nó (kernel/fs.c:530-531).

Hàm `stati` (kernel/fs.c:458) sao chép siêu dữ liệu inode vào cấu trúc `stat`, được hiển thị cho các chương trình người dùng thông qua lời gọi hệ thống `stat`.

### 8.11 Mã: lớp thư mục

Một thư mục được triển khai bên trong giống như một tệp. Inode của nó có loại `T_DIR` và dữ liệu của nó là một chuỗi các mục nhập thư mục. Mỗi mục nhập là một `struct dirent` (kernel/fs.h:56), chứa một tên và một số inode. Tên có nhiều nhất `DIRSIZ` (14) ký tự; nếu ngắn hơn, nó được kết thúc bằng một byte `NULL` (0). Các mục nhập thư mục với số inode 0 là miễn phí.

Hàm `dirlookup` (kernel/fs.c:552) tìm kiếm một thư mục để tìm một mục nhập với tên đã cho. Nếu nó tìm thấy một mục, nó sẽ trả về một con trỏ đến inode tương ứng, đã được mở khóa, và đặt `*poff` thành offset byte của mục nhập trong thư mục, trong trường hợp người gọi muốn chỉnh sửa nó. Nếu `dirlookup` tìm thấy một mục nhập với tên phù hợp, nó sẽ cập nhật `*poff` và trả về một inode đã được mở khóa thu được thông qua `iget`. `dirlookup` là lý do tại sao `iget` trả về các inode đã được mở khóa. Người gọi đã khóa `dp`, vì vậy nếu tra cứu là cho `.`, một bí danh cho thư mục hiện tại, việc cố gắng khóa inode trước khi trả về sẽ cố gắng khóa lại `dp` và bị khóa chết. (Có những kịch bản khóa chết phức tạp hơn liên quan đến nhiều tiến trình và `..`, một bí danh cho thư mục cha; `.` không phải là vấn đề duy nhất.) Người gọi có thể mở khóa `dp` và sau đó khóa `ip`, đảm bảo rằng nó chỉ giữ một khóa tại một thời điểm.

Hàm `dirlink` (kernel/fs.c:580) ghi một mục nhập thư mục mới với tên và số inode đã cho vào thư mục `dp`. Nếu tên đã tồn tại, `dirlink` sẽ trả về một lỗi (kernel/fs.c:586-590). Vòng lặp chính đọc các mục nhập thư mục tìm kiếm một mục nhập chưa được phân bổ. Khi nó tìm thấy một mục, nó sẽ dừng vòng lặp sớm (kernel/fs.c:592-597), với `off` được đặt thành offset của mục nhập có sẵn. Mặt khác, vòng lặp kết thúc với `off` được đặt thành `dp->size`. Dù bằng cách nào, `dirlink` sau đó thêm một mục nhập mới vào thư mục bằng cách ghi tại offset `off` (kernel/fs.c:602-603).

### 8.12 Mã: Tên đường dẫn

Việc tra cứu tên đường dẫn liên quan đến một chuỗi các lệnh gọi đến `dirlookup`, một cho mỗi thành phần đường dẫn. `namei` (kernel/fs.c:687) đánh giá `path` và trả về inode tương ứng. Hàm `nameiparent` là một biến thể: nó dừng lại trước phần tử cuối cùng, trả về inode của thư mục cha và sao chép phần tử cuối cùng vào `name`. Cả hai đều gọi hàm tổng quát `namex` để thực hiện công việc thực sự.

`namex` (kernel/fs.c:652) bắt đầu bằng cách quyết định nơi bắt đầu đánh giá đường dẫn. Nếu đường dẫn bắt đầu bằng một dấu gạch chéo, việc đánh giá bắt đầu từ gốc; nếu không, thư mục hiện tại (kernel/fs.c:656-659). Sau đó, nó sử dụng `skipelem` để xem xét từng phần tử của đường dẫn lần lượt (kernel/fs.c:661). Mỗi lần lặp của vòng lặp phải tra cứu `name` trong inode hiện tại `ip`. Lần lặp bắt đầu bằng cách khóa `ip` và kiểm tra xem nó có phải là một thư mục hay không. Nếu không, việc tra cứu sẽ thất bại (kernel/fs.c:662-666). (Việc khóa `ip` là cần thiết không phải vì `ip->type` có thể thay đổi dưới chân — nó không thể — mà bởi vì cho đến khi `ilock` chạy, `ip->type` không được đảm bảo đã được tải từ đĩa.) Nếu lệnh gọi là `nameiparent` và đây là phần tử đường dẫn cuối cùng, vòng lặp sẽ dừng sớm, theo định nghĩa của `nameiparent`; phần tử đường dẫn cuối cùng đã được sao chép vào `name`, vì vậy `namex` chỉ cần trả về `ip` đã được mở khóa (kernel/fs.c:667-671). Cuối cùng, vòng lặp tìm kiếm phần tử đường dẫn bằng cách sử dụng `dirlookup` và chuẩn bị cho lần lặp tiếp theo bằng cách đặt `ip = next` (kernel/fs.c:672-677). Khi vòng lặp hết các phần tử đường dẫn, nó sẽ trả về `ip`.

Thủ tục `namex` có thể mất một thời gian dài để hoàn thành: nó có thể liên quan đến một số hoạt động đĩa để đọc các inode và các khối thư mục cho các thư mục được duyệt trong tên đường dẫn (nếu chúng không có trong bộ đệm cache). Xv6 được thiết kế cẩn thận để nếu một lần gọi `namex` của một luồng hạt nhân bị chặn trên một I/O đĩa, một luồng hạt nhân khác đang tra cứu một tên đường dẫn khác có thể tiến hành đồng thời. `namex` khóa mỗi thư mục trong đường dẫn một cách riêng biệt để các tra cứu trong các thư mục khác nhau có thể tiến hành song song.

Sự đồng thời này giới thiệu một số thách thức. Ví dụ, trong khi một luồng hạt nhân đang tra cứu một tên đường dẫn, một luồng hạt nhân khác có thể đang thay đổi cây thư mục bằng cách hủy liên kết một thư mục. Một rủi ro tiềm tàng là một tra cứu có thể đang tìm kiếm một thư mục đã bị một luồng hạt nhân khác xóa và các khối của nó đã được sử dụng lại cho một thư mục hoặc tệp khác.

Xv6 tránh các cuộc đua như vậy. Ví dụ, khi thực thi `dirlookup` trong `namex`, luồng tra cứu giữ khóa trên thư mục và `dirlookup` trả về một inode đã được lấy bằng `iget`. `iget` tăng số lượng tham chiếu của inode. Chỉ sau khi nhận được inode từ `dirlookup`, `namex` mới giải phóng khóa trên thư mục. Bây giờ một luồng khác có thể hủy liên kết inode khỏi thư mục nhưng xv6 sẽ chưa xóa inode, bởi vì số lượng tham chiếu của inode vẫn lớn hơn 0.

Một rủi ro khác là khóa chết. Ví dụ, `next` trỏ đến cùng một inode với `ip` khi tra cứu ".". Việc khóa `next` trước khi giải phóng khóa trên `ip` sẽ dẫn đến một khóa chết. Để tránh khóa chết này, `namex` mở khóa thư mục trước khi lấy một khóa trên `next`. Ở đây một lần nữa chúng ta thấy tại sao sự tách biệt giữa `iget` và `ilock` là quan trọng.

### 8.13 Lớp mô tả tệp

Một khía cạnh thú vị của giao diện Unix là hầu hết các tài nguyên trong Unix được biểu diễn dưới dạng các tệp, bao gồm các thiết bị như giao diện điều khiển, các ống dẫn, và tất nhiên, các tệp thực. Lớp mô tả tệp là lớp đạt được sự đồng nhất này.

Xv6 cung cấp cho mỗi tiến trình bảng riêng của nó về các tệp đang mở, hoặc các *mô tả tệp*, như chúng ta đã thấy trong Chương 1. Mỗi tệp đang mở được biểu diễn bằng một `struct file` (kernel/file.h:1), là một trình bao bọc xung quanh một inode hoặc một ống dẫn, cộng với một offset I/O. Mỗi lệnh gọi đến `open` tạo một tệp đang mở mới (một `struct file` mới): nếu nhiều tiến trình mở cùng một tệp một cách độc lập, các phiên bản khác nhau sẽ có các offset I/O khác nhau. Mặt khác, một tệp đang mở duy nhất (cùng một `struct file`) có thể xuất hiện nhiều lần trong bảng tệp của một tiến trình và cũng trong các bảng tệp của nhiều tiến trình. Điều này sẽ xảy ra nếu một tiến trình sử dụng `open` để mở tệp và sau đó tạo các bí danh bằng `dup` hoặc chia sẻ nó với một đứa con bằng `fork`. Một số lượng tham chiếu theo dõi số lượng tham chiếu đến một tệp đang mở cụ thể. Một tệp có thể được mở để đọc hoặc ghi hoặc cả hai. Các trường `readable` và `writable` theo dõi điều này.

Tất cả các tệp đang mở trong hệ thống được giữ trong một bảng tệp toàn cục, `ftable`. Bảng tệp có các hàm để phân bổ một tệp (`filealloc`), tạo một tham chiếu trùng lặp (`filedup`), giải phóng một tham chiếu (`fileclose`), và đọc và ghi dữ liệu (`fileread` và `filewrite`).

Ba cái đầu tiên theo một hình thức quen thuộc. `filealloc` (kernel/file.c:30) quét bảng tệp để tìm một tệp không được tham chiếu (`f->ref == 0`) và trả về một tham chiếu mới; `filedup` (kernel/file.c:48) tăng số lượng tham chiếu; và `fileclose` (kernel/file.c:60) giảm nó. Khi số lượng tham chiếu của một tệp đạt đến 0, `fileclose` sẽ giải phóng ống dẫn hoặc inode cơ bản, tùy thuộc vào loại.

Các hàm `filestat`, `fileread`, và `filewrite` thực hiện các hoạt động `stat`, `read`, và `write` trên các tệp. `filestat` (kernel/file.c:88) chỉ được phép trên các inode và gọi `stati`. `fileread` và `filewrite` kiểm tra xem hoạt động có được phép bởi chế độ mở hay không và sau đó chuyển lệnh gọi đến việc triển khai ống dẫn hoặc inode. Nếu tệp đại diện cho một inode, `fileread` và `filewrite` sử dụng offset I/O làm offset cho hoạt động và sau đó tăng nó (kernel/file.c:122-123) (kernel/file.c:153-154). Các ống dẫn không có khái niệm về offset. Nhớ lại rằng các hàm inode yêu cầu người gọi xử lý việc khóa (kernel/file.c:94-96) (kernel/file.c:121-124) (kernel/file.c:163-166). Việc khóa inode có tác dụng phụ thuận tiện là các offset đọc và ghi được cập nhật một cách nguyên tử, do đó nhiều lần ghi vào cùng một tệp đồng thời không thể ghi đè lên dữ liệu của nhau, mặc dù các lần ghi của chúng có thể bị xen kẽ.

### 8.14 Mã: Lời gọi hệ thống

Với các hàm mà các lớp thấp hơn cung cấp, việc triển khai hầu hết các lời gọi hệ thống là tầm thường (xem (kernel/sysfile.c)). Có một vài lệnh gọi đáng được xem xét kỹ hơn.

Các hàm `sys_link` và `sys_unlink` chỉnh sửa các thư mục, tạo hoặc xóa các tham chiếu đến các inode. Chúng là một ví dụ điển hình khác về sức mạnh của việc sử dụng các giao dịch. `sys_link` (kernel/sysfile.c:124) bắt đầu bằng cách lấy các đối số của nó, hai chuỗi `old` và `new` (kernel/sysfile.c:129). Giả sử `old` tồn tại và không phải là một thư mục (kernel/sysfile.c:133-136), `sys_link` sẽ tăng `ip->nlink` của nó. Sau đó, `sys_link` gọi `nameiparent` để tìm thư mục cha và phần tử đường dẫn cuối cùng của `new` (kernel/sysfile.c:149) và tạo một mục nhập thư mục mới trỏ đến inode của `old` (kernel/sysfile.c:152). Thư mục cha mới phải tồn tại và phải ở trên cùng một thiết bị với inode hiện có: các số inode chỉ có một ý nghĩa duy nhất trên một đĩa duy nhất. Nếu một lỗi như thế này xảy ra, `sys_link` phải quay lại và giảm `ip->nlink`.

Các giao dịch đơn giản hóa việc triển khai bởi vì nó yêu cầu cập nhật nhiều khối đĩa, nhưng chúng ta không phải lo lắng về thứ tự chúng ta thực hiện chúng. Chúng hoặc sẽ thành công tất cả hoặc không có cái nào. Ví dụ, nếu không có các giao dịch, việc cập nhật `ip->nlink` trước khi tạo một liên kết, sẽ đặt hệ thống tệp tạm thời vào một trạng thái không an toàn, và một sự cố ở giữa có thể gây ra sự tàn phá. Với các giao dịch, chúng ta không phải lo lắng về điều này.

`sys_link` tạo một tên mới cho một inode hiện có. Hàm `create` (kernel/sysfile.c:246) tạo một tên mới cho một inode mới. Nó là một sự tổng quát hóa của ba lời gọi hệ thống tạo tệp: `open` với cờ `O_CREATE` tạo một tệp thông thường mới, `mkdir` tạo một thư mục mới, và `mkdev` tạo một tệp thiết bị mới. Giống như `sys_link`, `create` bắt đầu bằng cách gọi `nameiparent` để lấy inode của thư mục cha. Sau đó, nó gọi `dirlookup` để kiểm tra xem tên đã tồn tại hay chưa (kernel/sysfile.c:256). Nếu tên đã tồn tại, hành vi của `create` phụ thuộc vào lời gọi hệ thống nào nó đang được sử dụng: `open` có ngữ nghĩa khác với `mkdir` và `mkdev`. Nếu `create` đang được sử dụng thay mặt cho `open` (`type == T_FILE`) và tên tồn tại là một tệp thông thường, thì `open` coi đó là một thành công, vì vậy `create` cũng vậy (kernel/sysfile.c:260). Mặt khác, đó là một lỗi (kernel/sysfile.c:261-262). Nếu tên chưa tồn tại, `create` bây giờ sẽ phân bổ một inode mới với `ialloc` (kernel/sysfile.c:265). Nếu inode mới là một thư mục, `create` sẽ khởi tạo nó với các mục `.` và `..`. Cuối cùng, bây giờ dữ liệu đã được khởi tạo đúng cách, `create` có thể liên kết nó vào thư mục cha (kernel/sysfile.c:278). `create`, giống như `sys_link`, giữ hai khóa inode đồng thời: `ip` và `dp`. Không có khả năng xảy ra khóa chết bởi vì inode `ip` mới được phân bổ: không có tiến trình nào khác trong hệ thống sẽ giữ khóa của `ip` và sau đó cố gắng khóa `dp`.

Sử dụng `create`, rất dễ dàng để triển khai `sys_open`, `sys_mkdir`, và `sys_mknod`. `sys_open` (kernel/sysfile.c:305) là phức tạp nhất, bởi vì việc tạo một tệp mới chỉ là một phần nhỏ của những gì nó có thể làm. Nếu `open` được truyền cờ `O_CREATE`, nó sẽ gọi `create` (kernel/sysfile.c:320). Mặt khác, nó gọi `namei` (kernel/sysfile.c:326). `create` trả về một inode bị khóa, nhưng `namei` thì không, vì vậy `sys_open` phải tự khóa inode. Điều này cung cấp một nơi thuận tiện để kiểm tra xem các thư mục chỉ được mở để đọc, không phải để ghi. Giả sử inode đã được lấy bằng cách này hay cách khác, `sys_open` sẽ phân bổ một tệp và một mô tả tệp (kernel/sysfile.c:344) và sau đó điền vào tệp (kernel/sysfile.c:356-361). Lưu ý rằng không có tiến trình nào khác có thể truy cập vào tệp được khởi tạo một phần vì nó chỉ có trong bảng của tiến trình hiện tại.

Chương 7 đã xem xét việc triển khai các ống dẫn trước khi chúng ta có một hệ thống tệp. Hàm `sys_pipe` kết nối việc triển khai đó với hệ thống tệp bằng cách cung cấp một cách để tạo một cặp ống dẫn. Đối số của nó là một con trỏ đến không gian cho hai số nguyên, nơi nó sẽ ghi lại hai mô tả tệp mới. Sau đó, nó phân bổ ống dẫn và cài đặt các mô tả tệp.

### 8.15 Thế giới thực

Bộ đệm cache trong một hệ điều hành trong thế giới thực phức tạp hơn đáng kể so với của xv6, nhưng nó phục vụ cùng hai mục đích: lưu trữ và đồng bộ hóa quyền truy cập vào đĩa. Bộ đệm cache của Xv6, giống như của V6, sử dụng một chính sách loại bỏ *ít được sử dụng gần đây* (LRU) đơn giản; có nhiều chính sách phức tạp hơn có thể được triển khai, mỗi chính sách tốt cho một số khối lượng công việc và không tốt bằng cho những chính sách khác. Một bộ đệm cache LRU hiệu quả hơn sẽ loại bỏ danh sách liên kết, thay vào đó sử dụng một bảng băm để tra cứu và một heap cho các lần loại bỏ LRU. Các bộ đệm cache hiện đại thường được tích hợp với hệ thống bộ nhớ ảo để hỗ trợ các tệp được ánh xạ bộ nhớ.

Hệ thống ghi nhật ký của Xv6 không hiệu quả. Một cam kết không thể xảy ra đồng thời với các lời gọi hệ thống tệp. Hệ thống ghi nhật ký toàn bộ các khối, ngay cả khi chỉ một vài byte trong một khối bị thay đổi. Nó thực hiện các lần ghi nhật ký đồng bộ, một khối tại một thời điểm, mỗi lần có khả năng yêu cầu một vòng quay đĩa hoàn chỉnh. Các hệ thống ghi nhật ký thực giải quyết tất cả các vấn đề này.

Ghi nhật ký không phải là cách duy nhất để cung cấp phục hồi sau sự cố. Các hệ thống tệp ban đầu đã sử dụng một *kẻ dọn dẹp* trong quá trình khởi động lại (ví dụ, chương trình `fsck` của UNIX) để kiểm tra mọi tệp và thư mục và các danh sách miễn phí khối và inode, tìm kiếm và giải quyết các sự không nhất quán. Việc dọn dẹp có thể mất hàng giờ đối với các hệ thống tệp lớn, và có những tình huống không thể giải quyết các sự không nhất quán theo cách làm cho các lời gọi hệ thống ban đầu là nguyên tử. Việc phục hồi từ một nhật ký nhanh hơn nhiều và làm cho các lời gọi hệ thống là nguyên tử khi có sự cố.

Xv6 sử dụng cùng một bố cục trên đĩa cơ bản của các inode và các thư mục như UNIX ban đầu; lược đồ này đã tồn tại một cách đáng kể trong nhiều năm. UFS/FFS của BSD và ext2/ext3 của Linux sử dụng về cơ bản cùng một cấu trúc dữ liệu. Phần không hiệu quả nhất của bố cục hệ thống tệp là thư mục, yêu cầu một lần quét tuyến tính qua tất cả các khối đĩa trong mỗi lần tra cứu. Điều này là hợp lý khi các thư mục chỉ có một vài khối đĩa, nhưng tốn kém đối với các thư mục chứa nhiều tệp. NTFS của Microsoft Windows, HFS của macOS, và ZFS của Solaris, chỉ để kể tên một vài, thực hiện một thư mục như một cây cân bằng trên đĩa của các khối. Điều này phức tạp nhưng đảm bảo thời gian tra cứu thư mục logarit.

Xv6 ngây thơ về các lỗi đĩa: nếu một hoạt động đĩa thất bại, xv6 sẽ hoảng loạn. Liệu điều này có hợp lý hay không phụ thuộc vào phần cứng: nếu một hệ điều hành nằm trên phần cứng đặc biệt sử dụng sự dư thừa để che giấu các lỗi đĩa, có lẽ hệ điều hành thấy các lỗi không thường xuyên đến mức việc hoảng loạn là được. Mặt khác, các hệ điều hành sử dụng các đĩa thông thường nên mong đợi các lỗi và xử lý chúng một cách duyên dáng hơn, để việc mất một khối trong một tệp không ảnh hưởng đến việc sử dụng phần còn lại của hệ thống tệp.

Xv6 yêu cầu hệ thống tệp phải vừa trên một thiết bị đĩa và không thay đổi kích thước. Khi các cơ sở dữ liệu lớn và các tệp đa phương tiện thúc đẩy các yêu cầu lưu trữ ngày càng cao, các hệ điều hành đang phát triển các cách để loại bỏ nút thắt cổ chai “một đĩa cho mỗi hệ thống tệp”. Cách tiếp cận cơ bản là kết hợp nhiều đĩa thành một đĩa logic duy nhất. Các giải pháp phần cứng như RAID vẫn là phổ biến nhất, nhưng xu hướng hiện tại đang chuyển sang việc triển khai càng nhiều logic này trong phần mềm càng tốt. Các triển khai phần mềm này thường cho phép các chức năng phong phú như phát triển hoặc thu nhỏ thiết bị logic bằng cách thêm hoặc xóa các đĩa một cách nhanh chóng. Tất nhiên, một lớp lưu trữ có thể phát triển hoặc thu nhỏ một cách nhanh chóng đòi hỏi một hệ thống tệp có thể làm điều tương tự: mảng kích thước cố định của các khối inode được sử dụng bởi xv6 sẽ không hoạt động tốt trong các môi trường như vậy. Việc tách quản lý đĩa khỏi hệ thống tệp có thể là thiết kế sạch sẽ nhất, nhưng giao diện phức tạp giữa hai đã khiến một số hệ thống, như ZFS của Sun, kết hợp chúng.

Hệ thống tệp của Xv6 thiếu nhiều tính năng khác của các hệ thống tệp hiện đại; ví dụ, nó thiếu hỗ trợ cho các ảnh chụp nhanh và sao lưu gia tăng.

Các hệ thống Unix hiện đại cho phép nhiều loại tài nguyên được truy cập bằng cùng một lời gọi hệ thống như lưu trữ trên đĩa: các ống dẫn được đặt tên, các kết nối mạng, các hệ thống tệp mạng được truy cập từ xa, và các giao diện giám sát và điều khiển như `/proc`. Thay vì các câu lệnh `if` của xv6 trong `fileread` và `filewrite`, các hệ thống này thường cung cấp cho mỗi tệp đang mở một bảng các con trỏ hàm, một cho mỗi hoạt động, và gọi con trỏ hàm để gọi việc triển khai của inode đó. Các hệ thống tệp mạng và các hệ thống tệp cấp người dùng cung cấp các hàm biến các lệnh gọi đó thành các RPC mạng và đợi phản hồi trước khi trả về.

### 8.16 Bài tập

1.  Tại sao lại hoảng loạn trong `balloc`? xv6 có thể phục hồi không?

2.  Tại sao lại hoảng loạn trong `ialloc`? xv6 có thể phục hồi không?

3.  Tại sao `filealloc` không hoảng loạn khi nó hết tệp? Tại sao điều này phổ biến hơn và do đó đáng để xử lý?

4.  Giả sử tệp tương ứng với `ip` bị hủy liên kết bởi một tiến trình khác giữa các lệnh gọi của `sys_link` đến `iunlock(ip)` và `dirlink`. Liên kết có được tạo chính xác không? Tại sao có hoặc tại sao không?

5.  `create` thực hiện bốn lệnh gọi hàm (một đến `ialloc` và ba đến `dirlink`) mà nó yêu cầu phải thành công. Nếu bất kỳ cái nào không thành công, `create` sẽ hoảng loạn. Tại sao điều này có thể chấp nhận được? Tại sao không có cái nào trong bốn lệnh gọi đó có thể thất bại?

6.  `sys_chdir` gọi `iunlock(ip)` trước `iput(cp->cwd)`, có thể cố gắng khóa `cp->cwd`, nhưng việc hoãn `iunlock(ip)` cho đến sau `iput` sẽ không gây ra khóa chết. Tại sao không?

7.  Triển khai lời gọi hệ thống `lseek`. Việc hỗ trợ `lseek` cũng sẽ yêu cầu bạn sửa đổi `filewrite` để điền các lỗ trong tệp bằng 0 nếu `lseek` đặt `off` vượt quá `f->ip->size`.

8.  Thêm `O_TRUNC` và `O_APPEND` vào `open`, để các toán tử `>` và `>>` hoạt động trong shell.

9.  Sửa đổi hệ thống tệp để hỗ trợ các liên kết tượng trưng.

10. Sửa đổi hệ thống tệp để hỗ trợ các ống dẫn được đặt tên.

11. Sửa đổi hệ thống tệp và hệ thống VM để hỗ trợ các tệp được ánh xạ bộ nhớ.

## 9 Đồng thời được xem xét lại

Đồng thời đạt được hiệu suất song song tốt, tính đúng đắn mặc dù có sự đồng thời, và mã dễ hiểu là một thách thức lớn trong thiết kế hạt nhân. Việc sử dụng các khóa một cách đơn giản là con đường tốt nhất để đạt được tính đúng đắn, nhưng không phải lúc nào cũng có thể. Chương này nêu bật các ví dụ trong đó xv6 buộc phải sử dụng các khóa một cách phức tạp, và các ví dụ trong đó xv6 sử dụng các kỹ thuật giống như khóa nhưng không phải là khóa.

### 9.1 Các mẫu khóa

Các mục được lưu trong bộ nhớ cache thường là một thách thức đối với việc khóa. Ví dụ, bộ đệm khối của hệ thống tệp (kernel/bio.c:26) lưu trữ các bản sao của tối đa `NBUF` khối đĩa. Điều quan trọng là một khối đĩa nhất định có nhiều nhất một bản sao trong bộ đệm; nếu không, các tiến trình khác nhau có thể thực hiện các thay đổi xung đột đối với các bản sao khác nhau của những gì được cho là cùng một khối. Mỗi khối được lưu trong bộ nhớ cache được lưu trữ trong một `struct buf` (kernel/buf.h:1). Một `struct buf` có một trường khóa giúp đảm bảo rằng chỉ một tiến trình sử dụng một khối đĩa nhất định tại một thời điểm. Tuy nhiên, khóa đó là không đủ: điều gì sẽ xảy ra nếu một khối không có trong bộ đệm, và hai tiến trình muốn sử dụng nó cùng một lúc? Không có `struct buf` (vì khối chưa được lưu trong bộ nhớ cache), và do đó không có gì để khóa. Xv6 đối phó với tình huống này bằng cách liên kết một khóa bổ sung (`bcache.lock`) với tập hợp các danh tính của các khối được lưu trong bộ nhớ cache. Mã cần kiểm tra xem một khối có được lưu trong bộ nhớ cache hay không (ví dụ: `bget` (kernel/bio.c:59)), hoặc thay đổi tập hợp các khối được lưu trong bộ nhớ cache, phải giữ `bcache.lock`; sau khi mã đó đã tìm thấy khối và `struct buf` mà nó cần, nó có thể giải phóng `bcache.lock` và khóa chỉ khối cụ thể. Đây là một mẫu phổ biến: một khóa cho tập hợp các mục, cộng với một khóa cho mỗi mục.

Thông thường, cùng một hàm lấy một khóa sẽ giải phóng nó. Nhưng một cách nhìn chính xác hơn về mọi thứ là một khóa được lấy ở đầu một chuỗi phải xuất hiện nguyên tử, và được giải phóng khi chuỗi kết thúc. Nếu chuỗi bắt đầu và kết thúc ở các hàm khác nhau, hoặc các luồng khác nhau, hoặc trên các CPU khác nhau, thì việc lấy và giải phóng khóa phải làm điều tương tự. Chức năng của khóa là buộc các lần sử dụng khác phải đợi, không phải để ghim một mẩu dữ liệu vào một tác nhân cụ thể. Một ví dụ là `acquire` trong `yield` (kernel/proc.c:512), được giải phóng trong luồng lập lịch thay vì trong quy trình lấy. Một ví dụ khác là `acquiresleep` trong `ilock` (kernel/fs.c:293); mã này thường ngủ trong khi đọc đĩa; nó có thể thức dậy trên một CPU khác, có nghĩa là khóa có thể được lấy và giải phóng trên các CPU khác nhau.

Việc giải phóng một đối tượng được bảo vệ bởi một khóa được nhúng trong đối tượng là một công việc tinh tế, vì việc sở hữu khóa không đủ để đảm bảo rằng việc giải phóng sẽ đúng. Trường hợp vấn đề phát sinh khi một số luồng khác đang chờ trong `acquire` để sử dụng đối tượng; việc giải phóng đối tượng ngầm giải phóng khóa được nhúng, điều này sẽ khiến luồng đang chờ bị trục trặc. Một giải pháp là theo dõi có bao nhiêu tham chiếu đến đối tượng tồn tại, để nó chỉ được giải phóng khi tham chiếu cuối cùng biến mất. Xem `pipeclose` (kernel/pipe.c:59) để biết một ví dụ; `pi->readopen` và `pi->writeopen` theo dõi xem ống dẫn có các mô tả tệp tham chiếu đến nó hay không.

Thông thường, người ta thấy các khóa xung quanh các chuỗi các lần đọc và ghi vào các tập hợp các mục liên quan; các khóa đảm bảo rằng các luồng khác chỉ thấy các chuỗi cập nhật hoàn chỉnh (miễn là chúng cũng khóa). Còn những tình huống mà việc cập nhật là một lần ghi đơn giản vào một biến được chia sẻ thì sao? Ví dụ, `setkilled` và `killed` (kernel/proc.c:619) khóa xung quanh các lần sử dụng đơn giản của chúng đối với `p->killed`. Nếu không có khóa, một luồng có thể ghi `p->killed` cùng lúc một luồng khác đọc nó. Đây là một cuộc đua, và đặc tả ngôn ngữ C nói rằng một cuộc đua mang lại *hành vi không xác định*, có nghĩa là chương trình có thể gặp sự cố hoặc mang lại kết quả không chính xác¹. Các khóa ngăn chặn cuộc đua và tránh hành vi không xác định.

Một lý do các cuộc đua có thể phá vỡ các chương trình là, nếu không có các khóa hoặc các cấu trúc tương đương, trình biên dịch có thể tạo mã máy đọc và ghi bộ nhớ theo những cách khá khác so với mã C ban đầu. Ví dụ, mã máy của một luồng gọi `killed` có thể sao chép `p->killed` vào một thanh ghi và chỉ đọc giá trị được lưu trong bộ nhớ cache đó; điều này có nghĩa là luồng có thể không bao giờ thấy bất kỳ lần ghi nào vào `p->killed`. Các khóa ngăn chặn việc lưu trữ như vậy.

### 9.2 Các mẫu giống như khóa

Ở nhiều nơi, xv6 sử dụng một bộ đếm tham chiếu hoặc một cờ theo cách giống như khóa để chỉ ra rằng một đối tượng được phân bổ và không nên được giải phóng hoặc sử dụng lại. `p->state` của một tiến trình hoạt động theo cách này, cũng như các bộ đếm tham chiếu trong các cấu trúc `file`, `inode`, và `buf`. Trong khi trong mỗi trường hợp, một khóa bảo vệ cờ hoặc bộ đếm tham chiếu, chính cái sau mới ngăn đối tượng bị giải phóng sớm.

Hệ thống tệp sử dụng các bộ đếm tham chiếu `struct inode` như một loại khóa chia sẻ có thể được giữ bởi nhiều tiến trình, để tránh các khóa chết sẽ xảy ra nếu mã sử dụng các khóa thông thường. Ví dụ, vòng lặp trong `namex` (kernel/fs.c:652) khóa thư mục được đặt tên bởi mỗi thành phần tên đường dẫn lần lượt. Tuy nhiên, `namex` phải giải phóng mỗi khóa ở cuối vòng lặp, vì nếu nó giữ nhiều khóa, nó có thể bị khóa chết với chính nó nếu tên đường dẫn bao gồm một dấu chấm (ví dụ: `a/./b`). Nó cũng có thể bị khóa chết với một tra cứu đồng thời liên quan đến thư mục `..`. Như Chương 8 giải thích, giải pháp là để vòng lặp mang inode thư mục sang lần lặp tiếp theo với số lượng tham chiếu của nó được tăng lên, nhưng không bị khóa.

Một số mục dữ liệu được bảo vệ bởi các cơ chế khác nhau tại các thời điểm khác nhau, và đôi khi có thể được bảo vệ khỏi việc truy cập đồng thời một cách ngầm định bởi cấu trúc của mã xv6 thay vì bởi các khóa rõ ràng. Ví dụ, khi một trang vật lý miễn phí, nó được bảo vệ bởi `kmem.lock` (kernel/kalloc.c:24). Nếu trang sau đó được phân bổ dưới dạng một ống dẫn (kernel/pipe.c:23), nó được bảo vệ bởi một khóa khác (khóa `pi->lock` được nhúng). Nếu trang được phân bổ lại cho bộ nhớ người dùng của một tiến trình mới, nó không được bảo vệ bởi một khóa nào cả. Thay vào đó, thực tế là bộ cấp phát sẽ không cung cấp trang đó cho bất kỳ tiến trình nào khác (cho đến khi nó được giải phóng) bảo vệ nó khỏi việc truy cập đồng thời. Quyền sở hữu bộ nhớ của một tiến trình mới là phức tạp: đầu tiên cha mẹ phân bổ và thao tác nó trong `fork`, sau đó con sử dụng nó, và (sau khi con thoát) cha mẹ lại sở hữu bộ nhớ và chuyển nó cho `kfree`. Có hai bài học ở đây: một đối tượng dữ liệu có thể được bảo vệ khỏi sự đồng thời theo những cách khác nhau tại các thời điểm khác nhau trong vòng đời của nó, và sự bảo vệ có thể có dạng cấu trúc ngầm thay vì các khóa rõ ràng.

Một ví dụ cuối cùng giống như khóa là nhu cầu vô hiệu hóa các ngắt xung quanh các lệnh gọi đến `mycpu()` (kernel/proc.c:83). Việc vô hiệu hóa các ngắt làm cho mã đang gọi là nguyên tử đối với các ngắt hẹn giờ có thể buộc một chuyển đổi ngữ cảnh, và do đó di chuyển tiến trình sang một CPU khác.

### 9.3 Hoàn toàn không có khóa

Có một vài nơi mà xv6 chia sẻ dữ liệu có thể thay đổi mà không có khóa nào cả. Một là trong việc triển khai các khóa quay, mặc dù người ta có thể xem các lệnh nguyên tử RISC-V như là dựa vào các khóa được triển khai trong phần cứng. Một cái khác là biến `started` trong `main.c` (kernel/main.c:7), được sử dụng để ngăn các CPU khác chạy cho đến khi CPU 0 đã hoàn thành việc khởi tạo xv6; `volatile` đảm bảo rằng trình biên dịch thực sự tạo ra các lệnh tải và lưu trữ.

Xv6 chứa các trường hợp trong đó một CPU hoặc luồng ghi một số dữ liệu, và một CPU hoặc luồng khác đọc dữ liệu, nhưng không có khóa cụ thể nào dành riêng để bảo vệ dữ liệu đó. Ví dụ, trong `fork`, cha mẹ ghi các trang bộ nhớ người dùng của con, và con (một luồng khác, có lẽ trên một CPU khác) đọc các trang đó; không có khóa nào bảo vệ rõ ràng các trang đó. Đây không hoàn toàn là một vấn đề khóa, vì con không bắt đầu thực thi cho đến sau khi cha mẹ đã hoàn thành việc ghi. Nó là một vấn đề *thứ tự bộ nhớ* tiềm tàng (xem Chương 6), vì nếu không có một rào cản bộ nhớ, không có lý do gì để mong đợi một CPU thấy các lần ghi của một CPU khác. Tuy nhiên, vì cha mẹ giải phóng các khóa, và con lấy các khóa khi nó khởi động, các rào cản bộ nhớ trong `acquire` và `release` đảm bảo rằng CPU của con thấy các lần ghi của cha mẹ.

### 9.4 Song song

Việc khóa chủ yếu là để ngăn chặn tính song song vì lợi ích của tính đúng đắn. Bởi vì hiệu suất cũng quan trọng, các nhà thiết kế hạt nhân thường phải suy nghĩ về cách sử dụng các khóa theo cách vừa đạt được tính đúng đắn vừa cho phép tính song song. Mặc dù xv6 không được thiết kế một cách có hệ thống để có hiệu suất cao, nhưng vẫn đáng để xem xét các hoạt động nào của xv6 có thể thực thi song song, và các hoạt động nào có thể xung đột trên các khóa.

Các ống dẫn trong xv6 là một ví dụ về tính song song khá tốt. Mỗi ống dẫn có khóa riêng của nó, do đó các tiến trình khác nhau có thể đọc và ghi các ống dẫn khác nhau song song trên các CPU khác nhau. Tuy nhiên, đối với một ống dẫn nhất định, người ghi và người đọc phải đợi nhau để giải phóng khóa; chúng không thể đọc/ghi cùng một ống dẫn cùng một lúc. Cũng có trường hợp một lần đọc từ một ống dẫn trống (hoặc một lần ghi vào một ống dẫn đầy) phải chặn, nhưng điều này không phải do lược đồ khóa.

Chuyển đổi ngữ cảnh là một ví dụ phức tạp hơn. Hai luồng hạt nhân, mỗi luồng thực thi trên CPU riêng của nó, có thể gọi `yield`, `sched`, và `swtch` cùng một lúc, và các lệnh gọi sẽ thực thi song song. Các luồng mỗi luồng giữ một khóa, nhưng chúng là các khóa khác nhau, vì vậy chúng không phải đợi nhau. Tuy nhiên, một khi ở trong `scheduler`, hai CPU có thể xung đột trên các khóa trong khi tìm kiếm bảng các tiến trình để tìm một tiến trình `RUNNABLE`. Tức là, xv6 có khả năng sẽ nhận được một lợi ích hiệu suất từ nhiều CPU trong quá trình chuyển đổi ngữ cảnh, nhưng có lẽ không nhiều như nó có thể.

Một ví dụ khác là các lệnh gọi đồng thời đến `fork` từ các tiến trình khác nhau trên các CPU khác nhau. Các lệnh gọi có thể phải đợi nhau cho `pid_lock` và `kmem.lock`, và cho các khóa trên mỗi tiến trình cần thiết để tìm kiếm bảng tiến trình để tìm một tiến trình `UNUSED`. Mặt khác, hai tiến trình phân nhánh có thể sao chép các trang bộ nhớ người dùng và định dạng các trang bảng trang hoàn toàn song song.

Lược đồ khóa trong mỗi ví dụ trên hy sinh hiệu suất song song trong một số trường hợp. Trong mỗi trường hợp, có thể có được nhiều tính song song hơn bằng cách sử dụng một thiết kế phức tạp hơn. Liệu nó có đáng giá hay không phụ thuộc vào các chi tiết: các hoạt động có liên quan được gọi thường xuyên như thế nào, mã dành bao lâu với một khóa bị tranh chấp, có bao nhiêu CPU có thể đang chạy các hoạt động xung đột cùng một lúc, liệu các phần khác của mã có phải là các nút thắt cổ chai hạn chế hơn hay không. Rất khó để đoán xem một lược đồ khóa nhất định có thể gây ra các vấn đề về hiệu suất hay không, hoặc liệu một thiết kế mới có tốt hơn đáng kể hay không, vì vậy việc đo lường trên các khối lượng công việc thực tế thường được yêu cầu.

### 9.5 Bài tập

1.  Sửa đổi việc triển khai ống dẫn của xv6 để cho phép một lần đọc và một lần ghi vào cùng một ống dẫn tiến hành song song trên các CPU khác nhau.

2.  Sửa đổi `scheduler()` của xv6 để giảm sự tranh chấp khóa khi các CPU khác nhau đang tìm kiếm các tiến trình có thể chạy được cùng một lúc.

3.  Loại bỏ một số tuần tự hóa trong `fork()` của xv6.

## 10 Tóm tắt

Văn bản này đã giới thiệu các ý tưởng chính trong các hệ điều hành bằng cách nghiên cứu một hệ điều hành, xv6, từng dòng một. Một số dòng mã thể hiện bản chất của các ý tưởng chính (ví dụ: chuyển đổi ngữ cảnh, ranh giới người dùng/hạt nhân, các khóa, v.v.) và mỗi dòng là quan trọng; các dòng mã khác cung cấp một minh họa về cách triển khai một ý tưởng hệ điều hành cụ thể và có thể dễ dàng được thực hiện theo những cách khác nhau (ví dụ: một thuật toán tốt hơn để lập lịch, các cấu trúc dữ liệu trên đĩa tốt hơn để biểu diễn các tệp, ghi nhật ký tốt hơn để cho phép các giao dịch đồng thời, v.v.). Tất cả các ý tưởng đã được minh họa trong bối cảnh của một giao diện lời gọi hệ thống rất thành công, giao diện Unix, nhưng những ý tưởng đó mang sang thiết kế của các hệ điều hành khác.

## Thư mục

[1] Các lỗ hổng và phơi nhiễm phổ biến của Linux (CVE). https://cve.mitre.org/cgi-bin/cvekey.cgi?keyword=linux.

[2] Hướng dẫn sử dụng bộ lệnh RISC-V Tập I: ISA đặc tả không đặc quyền. https://drive.google.com/file/d/17GeetSnT5wW3xNuAHI95-SI1gPGd5sJ_/view?usp=drive_link, 2024.

[3] Hướng dẫn sử dụng bộ lệnh RISC-V Tập II: đặc tả đặc quyền. https://drive.google.com/file/d/1uviulnH-tScFfgrovvFCrj7Omv8tFtkp/view?usp=drive_link, 2024.

[4] Hans-J Boehm. Các luồng không thể được triển khai như một thư viện. Hội nghị ACM PLDI, 2005.

[5] Edsger Dijkstra. Các tiến trình tuần tự hợp tác. https://www.cs.utexas.edu/users/EWD/transcriptions/EWD01xx/EWD123.html, 1965.

[6] Maurice Herlihy và Nir Shavit. Nghệ thuật lập trình đa bộ xử lý, Tái bản sửa đổi. 2012.

[7] Brian W. Kernighan. Ngôn ngữ lập trình C. Prentice Hall Professional Technical Reference, tái bản lần thứ 2, 1988.

[8] Gerwin Klein, Kevin Elphinstone, Gernot Heiser, June Andronick, David Cock, Philip Derrin, Dhammika Elkaduwe, Kai Engelhardt, Rafal Kolanski, Michael Norrish, Thomas Sewell, Harvey Tuch, và Simon Winwood. Sel4: Xác minh chính thức của một hạt nhân HĐH. Trong Kỷ yếu của Hội nghị chuyên đề ACM SIGOPS lần thứ 22 về các nguyên tắc hệ điều hành, trang 207–220, 2009.

[9] Donald Knuth. Các thuật toán cơ bản. Nghệ thuật lập trình máy tính. (Tái bản lần thứ hai), tập 1. 1997.

[10] L Lamport. Một giải pháp mới cho vấn đề lập trình đồng thời của dijkstra. Communications of the ACM, 1974.

[11] John Lions. Chú giải về UNIX phiên bản thứ 6. Peer to Peer Communications, 2000.

[12] Paul E. Mckenney, Silas Boyd-wickizer, và Jonathan Walpole. Việc sử dụng RCU trong hạt nhân linux: Một thập kỷ sau, 2013.

[13] Martin Michael và Daniel Durich. NS16550A: Thiết kế và các cân nhắc ứng dụng UART. http://bitsavers.trailing-edge.com/components/national/_appNotes/AN-0491.pdf, 1987.

[14] Aleph One. Phá vỡ ngăn xếp để giải trí và kiếm lợi. http://phrack.org/issues/49/14.html#article.

[15] David Patterson và Andrew Waterman. The RISC-V Reader: an open architecture Atlas. Strawberry Canyon, 2017.

[16] Dave Presotto, Rob Pike, Ken Thompson, và Howard Trickey. Plan 9, một hệ thống phân tán. Trong Kỷ yếu của Hội nghị EurOpen mùa xuân 1991, trang 43–50, 1991.

[17] Dennis M. Ritchie và Ken Thompson. Hệ thống chia sẻ thời gian UNIX. Commun. ACM, 17(7):365–375, tháng 7 năm 1974.
