Phân Tích Quy Ước Lời Gọi Hàm trong Kiến Trúc RISC-V

Giới thiệu Tổng quan

Quy ước lời gọi hàm (calling convention) là một tập hợp các quy tắc cấp thấp quy định cách các hàm con nhận tham số từ hàm gọi và cách chúng trả về kết quả. Đây là một phần cơ bản của Giao diện Nhị phân Ứng dụng (Application Binary Interface - ABI), đóng vai trò quan trọng trong việc đảm bảo các đoạn mã được biên dịch riêng biệt (ví dụ, từ các ngôn ngữ lập trình khác nhau hoặc bởi các trình biên dịch khác nhau) có thể tương tác với nhau một cách liền mạch. Tài liệu này sẽ cung cấp một phân tích chi tiết về hai quy ước lời gọi hàm chính được chỉ định cho kiến trúc RISC-V: quy ước RVG tiêu chuẩn và quy ước Soft-Float.


--------------------------------------------------------------------------------


1. Nền tảng: Kiểu Dữ liệu và Căn chỉnh trong C

Trước khi tìm hiểu cách dữ liệu được truyền giữa các hàm, điều cần thiết là phải thiết lập một sự hiểu biết chung về kích thước và cách bố trí bộ nhớ của dữ liệu đó. Việc xác định các kiểu dữ liệu C tiêu chuẩn và các quy tắc căn chỉnh có tầm quan trọng chiến lược, tạo ra một nền tảng nhất quán cho cả hai kiến trúc RV32 và RV64, từ đó định hình cách các đối số được truyền và giá trị được trả về.

1.1. Mô hình Số nguyên và Kích thước Kiểu dữ liệu

Kiến trúc RISC-V sử dụng các mô hình số nguyên khác nhau tùy thuộc vào độ rộng của thanh ghi:

* RV32 sử dụng mô hình ILP32, nghĩa là các kiểu dữ liệu int, long và con trỏ đều có độ rộng 32 bit.
* RV64 sử dụng mô hình LP64, trong đó kiểu long và con trỏ có độ rộng 64 bit, trong khi int vẫn là 32 bit.

Bảng dưới đây tóm tắt kích thước của các kiểu dữ liệu C số nguyên và con trỏ trên cả hai kiến trúc.

Kiểu dữ liệu C	Bytes trong RV32	Bytes trong RV64
char (Giá trị ký tự/byte)	1	1
short (Số nguyên ngắn)	2	2
int (Số nguyên)	4	4
long (Số nguyên dài)	4	8
void* (Con trỏ)	4	8

Trên cả RV32 và RV64, long long là số nguyên 64-bit. Các kiểu dấu phẩy động tuân thủ tiêu chuẩn IEEE 754-2008, với float là 32-bit, double là 64-bit và long double là 128-bit.

1.2. Mở rộng Thanh ghi và Căn chỉnh Bộ nhớ

Khi các kiểu dữ liệu nhỏ hơn độ rộng thanh ghi gốc (XLEN) được lưu trữ trong một thanh ghi số nguyên, chúng sẽ được mở rộng theo các quy tắc sau:

* Mở rộng không dấu (Zero-extension): Các kiểu unsigned char (8-bit) và unsigned short (16-bit) được mở rộng bằng cách thêm các bit 0 vào các bit có trọng số cao hơn.
* Mở rộng dấu (Sign-extension): Các kiểu signed char (8-bit) và short (16-bit) được mở rộng bằng cách sao chép bit dấu (bit cao nhất) vào tất cả các bit có trọng số cao hơn.
* Quy tắc đặc biệt cho RV64: Đối với RV64, các kiểu 32-bit như int (kể cả unsigned int) cũng được mở rộng dấu để lấp đầy thanh ghi 64-bit. Tức là, các bit từ 63 đến 31 đều sẽ bằng bit 31 của giá trị gốc. Quy tắc này, dù có vẻ phản trực giác đối với các kiểu không dấu, giúp đơn giản hóa phần cứng vì các lệnh 32-bit (như addw) không cần phải lo lắng về việc dọn dẹp 32 bit cao của thanh ghi đích.

Khi được lưu trữ trong bộ nhớ, tất cả các kiểu dữ liệu C này đều được giữ "căn chỉnh tự nhiên" (naturally aligned).

Sau khi đã xác định rõ cấu trúc dữ liệu, bước tiếp theo là tìm hiểu cách dữ liệu này được truyền giữa các hàm theo quy ước lời gọi chính.


--------------------------------------------------------------------------------


2. Quy ước Lời gọi RVG (RVG Calling Convention)

Quy ước lời gọi RVG là quy ước tiêu chuẩn cho các hệ thống RISC-V có triển khai tập lệnh cơ sở cùng với các phần mở rộng chung tiêu chuẩn (được ký hiệu là "G"), đặc biệt là những hệ thống có đơn vị xử lý dấu phẩy động phần cứng.

2.1. Truyền Đối số (Argument Passing)

Quy trình truyền đối số của hàm tuân theo một bộ quy tắc có hệ thống, ưu tiên sử dụng thanh ghi để đạt hiệu suất tối ưu.

* Ưu tiên thanh ghi: Các đối số được truyền qua thanh ghi bất cứ khi nào có thể. Có tới tám thanh ghi số nguyên (a0–a7) và tám thanh ghi dấu phẩy động (fa0–fa7) được dành riêng cho mục đích này.
* Mô hình Struct ảo: Có thể hình dung các đối số như là các trường của một cấu trúc C (struct) ảo. Nếu một đối số là kiểu dấu phẩy động, nó sẽ sử dụng một thanh ghi fa tương ứng. Ngược lại, nó sẽ sử dụng một thanh ghi a.
* Ngoại lệ: Các đối số dấu phẩy động sẽ được truyền trong thanh ghi số nguyên nếu chúng là một phần của union, là trường mảng của một struct, hoặc là đối số cho các hàm có số lượng tham số thay đổi (variadic functions), ngoại trừ những đối số được đặt tên rõ ràng trong danh sách tham số.
* Đối số nhỏ: Các đối số nhỏ hơn một từ con trỏ (pointer-word) được đặt vào các bit có trọng số thấp nhất của thanh ghi.
* Đối số lớn (gấp đôi từ con trỏ): Các đối số có kích thước gấp đôi một từ con trỏ (ví dụ, long long trong RV32) được truyền trong một cặp thanh ghi số nguyên chẵn-lẻ liền kề. Ví dụ, hàm void foo(int, long long) sẽ truyền đối số đầu tiên vào a0 và đối số thứ hai vào cặp a2/a3, bỏ qua thanh ghi a1.
* Đối số rất lớn: Các đối số có kích thước lớn hơn gấp đôi một từ con trỏ sẽ được truyền bằng tham chiếu (truyền địa chỉ của đối số).
* Sử dụng Ngăn xếp: Bất kỳ đối số nào không thể truyền qua thanh ghi sẽ được đẩy vào ngăn xếp.

2.2. Trả về Giá trị (Returning Values)

Việc trả về giá trị từ hàm cũng tuân theo các quy tắc dựa trên thanh ghi:

1. Các giá trị được trả về thông qua các thanh ghi số nguyên a0, a1 và các thanh ghi dấu phẩy động fa0, fa1.
2. Giá trị dấu phẩy động chỉ được trả về trong các thanh ghi fa nếu chúng là kiểu nguyên thủy hoặc là thành viên của một struct chỉ chứa một hoặc hai giá trị dấu phẩy động.
3. Các giá trị khác có thể vừa với hai từ con trỏ sẽ được trả về trong a0 và a1.
4. Đối với các giá trị trả về lớn hơn, cơ chế hoạt động khác đi: hàm gọi (caller) chịu trách nhiệm cấp phát bộ nhớ cho giá trị trả về và truyền một con trỏ đến vùng nhớ đó như một đối số đầu tiên ẩn cho hàm được gọi (callee).

2.3. Quản lý Ngăn xếp (Stack Management)

Quy ước lời gọi tiêu chuẩn của RISC-V đặt ra hai quy tắc chính cho việc quản lý ngăn xếp:

* Ngăn xếp phát triển hướng xuống (từ địa chỉ cao đến địa chỉ thấp).
* Con trỏ ngăn xếp (sp) luôn được giữ căn chỉnh 16-byte.

Việc hiểu rõ cơ chế truyền đối số và quản lý ngăn xếp dẫn đến một khía cạnh quan trọng khác: vai trò và trách nhiệm cụ thể của từng thanh ghi trong một lời gọi hàm.


--------------------------------------------------------------------------------


3. Phân loại và Vai trò của Thanh ghi

Một khía cạnh cốt lõi của ABI là sự phân chia trách nhiệm rõ ràng giữa thanh ghi do người gọi lưu (caller-saved) và thanh ghi do người được gọi lưu (callee-saved). Các thanh ghi do người gọi lưu (còn gọi là thanh ghi tạm thời) có thể bị hàm được gọi thay đổi mà không cần báo trước, vì vậy người gọi phải lưu lại giá trị của chúng nếu cần dùng sau này. Ngược lại, các thanh ghi do người được gọi lưu (còn gọi là thanh ghi được bảo toàn) phải được hàm được gọi giữ nguyên giá trị; nếu hàm cần sử dụng chúng, nó phải lưu giá trị cũ và khôi phục lại trước khi trả về. Sự phân chia này là nền tảng để ngăn chặn việc dữ liệu bị ghi đè một cách vô tình qua các lời gọi hàm.

3.1. Bảng Phân loại Thanh ghi theo ABI

Bảng sau đây chi tiết hóa vai trò của từng thanh ghi số nguyên và dấu phẩy động theo quy ước lời gọi RVG.

Thanh ghi	Tên ABI	Mô tả	Bên lưu
Mục đích Đặc biệt			
x0	zero	Luôn là số không	—
x1	ra	Địa chỉ trả về	Người gọi
x2	sp	Con trỏ ngăn xếp	Người được gọi
x3	gp	Con trỏ toàn cục (trỏ tới vùng dữ liệu toàn cục)	—
x4	tp	Con trỏ luồng (Thread Pointer)	—
Đối số & Giá trị trả về			
x10–11	a0–1	Đối số hàm/Giá trị trả về	Người gọi
x12–17	a2–7	Đối số hàm	Người gọi
f10–11	fa0–1	Đối số FP/Giá trị trả về FP	Người gọi
f12–17	fa2–7	Đối số FP	Người gọi
Thanh ghi Tạm thời (Caller-Saved)			
x5–7	t0–2	Tạm thời	Người gọi
x28–31	t3–6	Tạm thời	Người gọi
f0–7	ft0–7	Tạm thời FP	Người gọi
f28–31	ft8–11	Tạm thời FP	Người gọi
Thanh ghi Được bảo toàn (Callee-Saved)			
x8	s0/fp	Thanh ghi được lưu/Con trỏ khung	Người được gọi
x9	s1	Thanh ghi được lưu	Người được gọi
x18–27	s2–11	Thanh ghi được lưu	Người được gọi
f8–9	fs0–1	Thanh ghi FP được lưu	Người được gọi
f18–27	fs2–11	Thanh ghi FP được lưu	Người được gọi

Trong khi quy ước RVG được thiết kế cho phần cứng đầy đủ tính năng, một quy ước thay thế được tạo ra cho các hệ thống không có phần cứng dấu phẩy động chuyên dụng.


--------------------------------------------------------------------------------


4. Quy ước Lời gọi Soft-Float (The Soft-Float Calling Convention)

Quy ước lời gọi Soft-Float được thiết kế đặc biệt cho các hệ thống RISC-V không có đơn vị xử lý dấu phẩy động phần cứng (như các biến thể RV32I và RV64I). Do đó, quy ước này hoàn toàn không sử dụng bất kỳ thanh ghi hay lệnh dấu phẩy động nào.

4.1. Cơ chế Hoạt động (Mechanism)

Cơ chế của quy ước Soft-Float giữ lại nhiều điểm tương đồng với quy ước RVG nhưng có một khác biệt cơ bản trong cách xử lý các giá trị dấu phẩy động.

1. Việc truyền và trả về các đối số số nguyên, cùng với kỷ luật quản lý ngăn xếp, hoàn toàn giống hệt với quy ước RVG.
2. Điểm khác biệt cốt lõi là: các đối số và giá trị trả về dấu phẩy động được truyền trong các thanh ghi số nguyên, tuân theo các quy tắc tương tự như đối với các đối số số nguyên có cùng kích thước.
3. Các ví dụ sau minh họa rõ quy tắc này cho hàm double foo(int, double, long double):
  * Trên RV32, đối số int đầu tiên được truyền trong a0. Đối số double (64-bit) thứ hai, vì có kích thước gấp đôi từ con trỏ (32-bit), được truyền trong cặp thanh ghi chẵn-lẻ a2 và a3, bỏ qua a1 theo đúng quy tắc đã nêu ở mục 2.1. Đối số long double (128-bit) thứ ba được truyền bằng tham chiếu qua a4. Kết quả double được trả về trong cặp a0 và a1.
  * Trên RV64, đối số int được truyền trong a0, đối số double được truyền trong a1, và đối số long double được truyền trong cặp a2-a3. Kết quả double được trả về trong a0.
4. Các chế độ làm tròn động và các cờ ngoại lệ được truy cập thông qua các hàm thư viện được cung cấp bởi header fenv.h của C99.

Những quy ước được định nghĩa rõ ràng này là nền tảng cho một hệ sinh thái phần mềm mạnh mẽ.


--------------------------------------------------------------------------------


5. Kết luận

Kiến trúc RISC-V cung cấp hai quy ước lời gọi hàm riêng biệt nhưng được định nghĩa rõ ràng để đáp ứng các khả năng phần cứng khác nhau. Quy ước RVG tiêu chuẩn tận dụng các thanh ghi dấu phẩy động để đạt hiệu suất cao trên các hệ thống được trang bị đầy đủ, trong khi quy ước Soft-Float đảm bảo khả năng tương thích trên các hệ thống chỉ có tập lệnh số nguyên cơ bản. Việc tuân thủ nghiêm ngặt các quy ước này là yếu tố quan trọng để xây dựng một hệ sinh thái phần mềm mạnh mẽ và có khả năng tương tác cao cho RISC-V.
