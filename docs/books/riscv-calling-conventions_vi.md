# Chương 18: Quy ước lời gọi

Chương này mô tả các tiêu chuẩn của trình biên dịch C cho các chương trình RV32 và RV64 và hai quy ước lời gọi: quy ước cho ISA cơ sở cộng với các phần mở rộng chung tiêu chuẩn (RV32G/RV64G) và quy ước soft-float cho các triển khai thiếu đơn vị dấu phẩy động (ví dụ: RV32I/RV64I).

> *Các triển khai với các phần mở rộng ISA có thể yêu cầu các quy ước lời gọi mở rộng.*

## 18.1 Kiểu dữ liệu và Căn chỉnh trong C

Bảng 18.1 tóm tắt các kiểu dữ liệu được hỗ trợ nguyên bản bởi các chương trình C của RISC-V. Trong cả hai trình biên dịch C RV32 và RV64, kiểu `int` trong C rộng 32 bit. Mặt khác, `long` và con trỏ đều có độ rộng bằng một thanh ghi số nguyên, vì vậy trong RV32, cả hai đều rộng 32 bit, trong khi ở RV64, cả hai đều rộng 64 bit. Tương đương, RV32 sử dụng mô hình số nguyên ILP32, trong khi RV64 là LP64. Trong cả RV32 và RV64, kiểu `long long` trong C là một số nguyên 64 bit, `float` là một số dấu phẩy động 32 bit theo chuẩn IEEE 754-2008, `double` là một số dấu phẩy động 64 bit theo chuẩn IEEE 754-2008 và `long double` là một số dấu phẩy động 128 bit theo chuẩn IEEE.

Các kiểu `char` và `unsigned char` trong C là các số nguyên không dấu 8 bit và được mở rộng bằng 0 (zero-extended) khi được lưu trữ trong một thanh ghi số nguyên của RISC-V. `unsigned short` là một số nguyên không dấu 16 bit và được mở rộng bằng 0 khi được lưu trữ trong một thanh ghi số nguyên của RISC-V. `signed char` là một số nguyên có dấu 8 bit và được mở rộng dấu (sign-extended) khi được lưu trữ trong một thanh ghi số nguyên của RISC-V, tức là các bit từ (XLEN-1) đến 7 đều bằng nhau. `short` là một số nguyên có dấu 16 bit và được mở rộng dấu khi được lưu trữ trong một thanh ghi.

Trong RV64, các kiểu 32 bit, chẳng hạn như `int`, được lưu trữ trong các thanh ghi số nguyên dưới dạng các phần mở rộng dấu thích hợp của các giá trị 32 bit của chúng; nghĩa là, các bit từ 63 đến 31 đều bằng nhau. Hạn chế này vẫn được áp dụng ngay cả đối với các kiểu 32 bit không dấu.

Trình biên dịch C của RV32 và RV64 và các phần mềm tương thích đều giữ cho tất cả các kiểu dữ liệu trên được căn chỉnh một cách tự nhiên khi được lưu trữ trong bộ nhớ.

### Bảng 18.1: Các kiểu dữ liệu của trình biên dịch C cho ISA RISC-V cơ sở.

| Kiểu C | Mô tả | Số byte trong RV32 | Số byte trong RV64 |
| :--- | :--- | :--- | :--- |
| `char` | Giá trị ký tự/byte | 1 | 1 |
| `short` | Số nguyên ngắn | 2 | 2 |
| `int` | Số nguyên | 4 | 4 |
| `long` | Số nguyên dài | 4 | 8 |
| `long long` | Số nguyên rất dài | 8 | 8 |
| `void*` | Con trỏ | 4 | 8 |
| `float` | Dấu phẩy động đơn | 4 | 4 |
| `double` | Dấu phẩy động đôi | 8 | 8 |
| `long double` | Dấu phẩy động mở rộng | 16 | 16 |

## 18.2 Quy ước lời gọi RVG

Quy ước gọi hàm của RISC-V truyền các đối số trong các thanh ghi nếu có thể. Tối đa tám thanh ghi số nguyên, a0-a7, và tối đa tám thanh ghi dấu phẩy động, fa0-fa7, được sử dụng cho mục đích này.

Nếu các đối số của một hàm được hình thành như các trường của một cấu trúc C, mỗi trường có căn chỉnh con trỏ, thì các thanh ghi đối số là một bản sao của tám từ con trỏ đầu tiên của cấu trúc đó. Nếu đối số i < 8 là kiểu dấu phẩy động, nó được truyền trong thanh ghi dấu phẩy động fai; nếu không, nó được truyền trong thanh ghi số nguyên ai. Tuy nhiên, các đối số dấu phẩy động là một phần của các union hoặc các trường mảng của cấu trúc được truyền trong các thanh ghi số nguyên. Ngoài ra, các đối số dấu phẩy động cho các hàm variadic (ngoại trừ những đối số được đặt tên rõ ràng trong danh sách tham số) được truyền trong các thanh ghi số nguyên.

Các đối số nhỏ hơn một từ con trỏ được truyền trong các bit có trọng số thấp nhất của các thanh ghi đối số. Tương ứng, các đối số nhỏ hơn một từ con trỏ được truyền trên stack sẽ xuất hiện ở các địa chỉ thấp hơn của một từ con trỏ, vì RISC-V có hệ thống bộ nhớ little-endian.

Khi các đối số nguyên thủy có kích thước gấp đôi kích thước từ con trỏ được truyền trên stack, chúng được căn chỉnh tự nhiên. Khi chúng được truyền trong các thanh ghi số nguyên, chúng nằm trong một cặp thanh ghi chẵn-lẻ được căn chỉnh, với thanh ghi chẵn chứa các bit có trọng số thấp nhất. Ví dụ, trong RV32, hàm `void foo(int, long long)` được truyền đối số đầu tiên vào `a0` và đối số thứ hai vào `a2` và `a3`. Không có gì được truyền vào `a1`.

Các đối số có kích thước lớn hơn hai lần kích thước của một từ con trỏ được truyền theo tham chiếu.

Phần của cấu trúc khái niệm không được truyền qua các thanh ghi đối số sẽ được truyền trên stack. Con trỏ stack sp trỏ đến đối số đầu tiên không được truyền trong một thanh ghi.

Các giá trị được trả về từ các hàm trong các thanh ghi số nguyên a0 và a1, và các thanh ghi dấu phẩy động fa0 và fa1. Các giá trị dấu phẩy động chỉ được trả về trong các thanh ghi dấu phẩy động nếu chúng là kiểu dữ liệu nguyên thủy hoặc là thành viên của một cấu trúc chỉ bao gồm một hoặc hai giá trị dấu phẩy động. Các giá trị trả về khác mà vừa với hai từ con trỏ sẽ được trả về trong a0 và a1. Các giá trị trả về lớn hơn được truyền hoàn toàn trong bộ nhớ; bên gọi hàm (caller) cấp phát vùng bộ nhớ này và truyền một con trỏ tới nó như một tham số ngầm định đầu tiên cho bên được gọi (callee).

Trong quy ước gọi hàm tiêu chuẩn của RISC-V, ngăn xếp phát triển xuống dưới và con trỏ ngăn xếp luôn được căn chỉnh 16 byte.

Ngoài các thanh ghi đối số và giá trị trả về, bảy thanh ghi số nguyên t0-t6 và mười hai thanh ghi dấu phẩy động fto-ft11 là các thanh ghi tạm thời không được bảo toàn qua các lần gọi hàm và phải được bên gọi lưu lại nếu được sử dụng sau này. Mười hai thanh ghi số nguyên s0-s11 và mười hai thanh ghi dấu phẩy động fs0-fs11 được bảo toàn qua các lần gọi hàm và phải được bên được gọi lưu lại nếu được sử dụng. Bảng 18.2 chỉ ra vai trò của từng thanh ghi số nguyên và dấu phẩy động trong quy ước gọi hàm.

### Bảng 18.2: Cách sử dụng thanh ghi trong quy ước lời gọi của RISC-V.

| Thanh ghi | Tên ABI | Mô tả | Bên lưu |
| :--- | :--- | :--- | :--- |
| x0 | zero | Giá trị 0 cứng | | 
| x1 | ra | Địa chỉ trả về | Bên gọi |
| x2 | sp | Con trỏ ngăn xếp | Bên được gọi |
| x3 | gp | Con trỏ toàn cục | | 
| x4 | tp | Con trỏ luồng | | 
| x5-7 | to-2 | Thanh ghi tạm | Bên gọi |
| x8 | s0/fp | Thanh ghi đã lưu/con trỏ khung | Bên được gọi |
| x9 | s1 | Thanh ghi đã lưu | Bên được gọi |
| x10-11 | a0-1 | Đối số hàm/giá trị trả về | Bên gọi |
| x12-17 | a2-7 | Đối số hàm | Bên gọi |
| x18-27 | s2-11 | Các thanh ghi đã lưu | Bên được gọi |
| x28-31 | t3-6 | Thanh ghi tạm | Bên gọi |
| f0-7 | ft0-7 | Thanh ghi tạm FP | Bên gọi |
| f8-9 | fs0-1 | Thanh ghi FP đã lưu | Bên được gọi |
| f10-11 | fa0-1 | Đối số FP/giá trị trả về | Bên gọi |
| f12-17 | fa2-7 | Đối số FP | Bên gọi |
| f18-27 | fs2-11 | Thanh ghi FP đã lưu | Bên được gọi |
| f28-31 | ft8-11 | Thanh ghi tạm FP | Bên gọi |

## 18.3 Quy ước lời gọi Soft-Float

Quy ước gọi dấu phẩy động mềm được sử dụng trên các triển khai RV32 và RV64 thiếu phần cứng dấu phẩy động. Nó tránh mọi việc sử dụng các lệnh trong các phần mở rộng tiêu chuẩn F, D và Q, và do đó tránh các thanh ghi f.

Các đối số nguyên được truyền và trả về theo cùng một cách như quy ước RVG, và nguyên tắc ngăn xếp cũng tương tự. Các đối số dấu phẩy động được truyền và trả về trong các thanh ghi số nguyên, sử dụng các quy tắc dành cho đối số nguyên cùng kích thước. Ví dụ, trong RV32, hàm `double foo(int, double, long double)` được truyền đối số đầu tiên vào a0, đối số thứ hai vào a2 và a3, và đối số thứ ba bằng tham chiếu qua a4; kết quả của nó được trả về trong a0 và a1. Trong RV64, các đối số được truyền vào a0, a1 và cặp a2-a3, và kết quả được trả về trong a0.

Chế độ làm tròn động và các cờ ngoại lệ được tích lũy được truy cập thông qua các chương trình con được cung cấp bởi tệp tiêu đề C99 fenv.h.