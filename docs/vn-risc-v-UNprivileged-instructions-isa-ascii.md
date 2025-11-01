Tổng quan về Kiến trúc Tập lệnh RISC-V không Đặc quyền

1. Giới thiệu về RISC-V

Phần này giới thiệu về RISC-V, một kiến trúc tập lệnh (ISA) mở và miễn phí. Việc hiểu rõ các mục tiêu và triết lý thiết kế cốt lõi của RISC-V là rất quan trọng để nắm bắt được cấu trúc mô-đun và khả năng mở rộng của nó, những yếu tố đã góp phần vào sự phổ biến nhanh chóng của kiến trúc này trong cả giới học thuật và ngành công nghiệp.

1.1. Mục tiêu và Triết lý Thiết kế

Các mục tiêu của nhóm thiết kế RISC-V bao gồm:

* Một ISA hoàn toàn mở, có sẵn miễn phí cho cả học thuật và ngành công nghiệp.
* Một ISA thực tế phù hợp cho việc triển khai phần cứng trực tiếp, không chỉ dành cho mô phỏng hay dịch nhị phân.
* Một ISA tránh "thiết kế thừa" cho một kiểu vi kiến trúc cụ thể (ví dụ: vi mã, thực thi theo thứ tự, tách rời, thực thi ngoài thứ tự) hoặc công nghệ triển khai (ví dụ: full-custom, ASIC, FPGA), nhưng cho phép triển khai hiệu quả trên bất kỳ công nghệ nào trong số này.
* Một ISA được tách thành một ISA số nguyên cơ sở nhỏ, có thể tự sử dụng làm nền tảng cho các bộ tăng tốc tùy chỉnh hoặc cho mục đích giáo dục, và các phần mở rộng tiêu chuẩn tùy chọn để hỗ trợ phát triển phần mềm đa dụng.

Những mục tiêu này đã định hình nên một kiến trúc không chỉ đơn giản và hiệu quả mà còn cực kỳ linh hoạt, cho phép tùy chỉnh và đổi mới ở mức độ chưa từng có.

1.2. Tổng quan về Tập lệnh RISC-V

RISC-V không phải là một ISA đơn lẻ mà là một họ các ISA có liên quan. Đơn vị thực thi cơ bản trong RISC-V được gọi là hart, hay luồng phần cứng (hardware thread).

 Thuật ngữ "hart" được giới thiệu để chỉ một tài nguyên thực thi trừu tượng, trái ngược với khái niệm luồng phần mềm (software thread). Sự khác biệt quan trọng giữa một luồng phần cứng (hart) và một ngữ cảnh luồng phần mềm là phần mềm chạy bên trong một môi trường thực thi không chịu trách nhiệm tạo ra sự tiến triển của mỗi hart; đó là trách nhiệm của môi trường thực thi bên ngoài.

Họ ISA RISC-V được cấu trúc xung quanh các ISA số nguyên cơ sở, được đặc trưng bởi độ rộng của thanh ghi số nguyên (XLEN) và không gian địa chỉ tương ứng. Các ISA cơ sở chính bao gồm RV32I (XLEN=32), RV64I (XLEN=64), và các biến thể RV32E và RV64E với số lượng thanh ghi ít hơn dành cho các hệ thống nhúng nhỏ. Một biến thể RV128I (XLEN=128) cũng được phác thảo cho tương lai. Trên nền tảng này, các phần mở rộng tiêu chuẩn có thể được thêm vào để cung cấp các chức năng bổ sung, chẳng hạn như:

* "M": Phép nhân và phép chia số nguyên.
* "A": Các lệnh nguyên tử để đồng bộ hóa đa bộ xử lý.
* "F" và "D": Số học dấu phẩy động đơn và đôi chính xác.
* "C": Các lệnh nén 16-bit để giảm kích thước mã.

 Bốn ISA cơ sở trong RISC-V được coi là các ISA cơ sở riêng biệt. Một câu hỏi thường gặp là tại sao không có một ISA duy nhất, và đặc biệt, tại sao RV32I không phải là một tập con nghiêm ngặt của RV64I? Một số thiết kế ISA trước đây (SPARC, MIPS) đã áp dụng chính sách siêu tập hợp nghiêm ngặt khi tăng kích thước không gian địa chỉ để hỗ trợ chạy các tệp nhị phân 32-bit hiện có trên phần cứng 64-bit mới.

Sự lựa chọn này trong RISC-V nhằm giữ cho mỗi ISA cơ sở được tối ưu hóa và đơn giản, tránh gánh nặng của việc tương thích ngược ở cấp độ phần cứng cơ bản.

1.3. Mô hình Bộ nhớ và Đồng bộ hóa

Trong các hệ thống đa lõi, mô hình nhất quán bộ nhớ định nghĩa cách các thay đổi trong bộ nhớ được thực hiện bởi một hart trở nên hữu hình đối với các hart khác. Mô hình mặc định của RISC-V là RISC-V Weak Memory Ordering (RVWMO), một mô hình thứ tự yếu cho phép hiệu suất cao bằng cách cho phép các truy cập bộ nhớ được sắp xếp lại. Một mô hình mạnh hơn, Total Store Ordering (Ztso), cũng có sẵn dưới dạng một phần mở rộng tùy chọn.

Để đảm bảo thứ tự tường minh giữa các truy cập bộ nhớ và tìm nạp lệnh, phần mềm phải sử dụng các lệnh hàng rào (fence) cụ thể. Ví dụ, lệnh FENCE.I đảm bảo rằng các thao tác ghi vào bộ nhớ lệnh trước đó sẽ được nhìn thấy bởi các lần tìm nạp lệnh tiếp theo trên cùng một hart.

Một số định nghĩa quan trọng liên quan đến tìm nạp lệnh:

* IALIGN: Ràng buộc về căn chỉnh địa chỉ lệnh, tính bằng bit. IALIGN là 32 bit trong ISA cơ sở, nhưng có thể được nới lỏng xuống 16 bit khi có các phần mở rộng như lệnh nén.
* ILEN: Độ dài lệnh tối đa được hỗ trợ bởi một triển khai, tính bằng bit. ILEN luôn là bội số của IALIGN.

RISC-V định nghĩa một số mã hóa lệnh là bất hợp lệ. Cụ thể, các lệnh có tất cả các bit bằng 0 hoặc tất cả các bit bằng 1 là bất hợp lệ.

 Việc xác định rằng bất kỳ độ dài lệnh nào chứa toàn bộ bit 0 đều không hợp lệ là một tính năng, vì nó nhanh chóng bắt lỗi các cú nhảy sai vào các vùng bộ nhớ đã được xóa về 0. Tương tự, mã hóa lệnh chứa toàn bộ bit 1 cũng được dành riêng làm lệnh bất hợp lệ, để bắt các mẫu lỗi phổ biến khác quan sát được với các thiết bị bộ nhớ không bay hơi chưa được lập trình, các bus bộ nhớ bị ngắt kết nối, hoặc các thiết bị bộ nhớ bị hỏng.

Việc hiểu các khái niệm nền tảng này là cần thiết trước khi đi sâu vào chi tiết của các tập lệnh cơ sở và các phần mở rộng tiêu chuẩn.

2. Tập lệnh Số nguyên Cơ sở RV64I

Phần này mô tả tập lệnh số nguyên cơ sở 64-bit, RV64I. Nó được xây dựng dựa trên RV32I, mở rộng các thanh ghi và không gian địa chỉ, đồng thời bổ sung các lệnh mới để thao tác hiệu quả các giá trị 32-bit trong môi trường 64-bit.

* Trạng thái Thanh ghi (4.1): RV64I mở rộng các thanh ghi số nguyên và không gian địa chỉ người dùng lên 64 bit (XLEN=64).
* Các lệnh Tính toán Số nguyên (4.2): Hầu hết các lệnh tính toán số nguyên hoạt động trên các giá trị XLEN-bit (64-bit). RV64I giới thiệu các biến thể lệnh có hậu tố 'W' (Word) để thao tác các giá trị 32-bit. Các lệnh 'W' này bỏ qua 32 bit trên của đầu vào và luôn tạo ra các giá trị 32-bit có dấu, sau đó mở rộng dấu (sign-extend) kết quả thành 64 bit.
* Lệnh Thanh ghi-Tức thời và Thanh ghi-Thanh ghi (4.2.1, 4.2.2):
  * ADDIW: Cộng một giá trị tức thời 12-bit đã mở rộng dấu vào một thanh ghi 64-bit, sau đó mở rộng dấu kết quả 32-bit thành 64 bit.
  * SLLIW, SRLIW, SRAW: Thực hiện các phép dịch trái logic, dịch phải logic và dịch phải số học trên một giá trị 32-bit (phần thấp của thanh ghi nguồn) và mở rộng dấu kết quả thành 64 bit.
  * ADDW, SUBW, SLLW, SRLW, SRAW: Các phiên bản thanh ghi-thanh ghi của các lệnh trên, hoạt động trên các giá trị 32-bit và tạo ra kết quả 64-bit được mở rộng dấu.
* Lệnh Load và Store (4.3):
  * RV64I bổ sung các lệnh LD (Load Doubleword) và SD (Store Doubleword) để truyền các giá trị 64-bit giữa thanh ghi và bộ nhớ.
  * Các lệnh load có độ rộng nhỏ hơn (LW, LH, LB) nạp một giá trị 32-bit, 16-bit hoặc 8-bit từ bộ nhớ và mở rộng dấu nó thành 64 bit trước khi lưu vào thanh ghi đích. Các biến thể không dấu (LWU, LHU, LBU) thực hiện mở rộng zero.

Các lệnh Gợi ý (HINT) trong RV64I

Các lệnh HINT không thay đổi bất kỳ trạng thái nào có thể thấy được về mặt kiến trúc nhưng có thể được sử dụng để truyền các gợi ý về hiệu suất cho vi kiến trúc. Dưới đây là tóm tắt các lệnh HINT có sẵn trong RV64I.

Lệnh	Ràng buộc	Số điểm mã	Mục đích
LUI rd=x0			Dành cho sử dụng tiêu chuẩn trong tương lai
AUIPC rd=x0			Dành cho sử dụng tiêu chuẩn trong tương lai
ADDI rd=x0	rs1!=x0 hoặc imm!=0		Dành cho sử dụng tiêu chuẩn trong tương lai
ANDI rd=x0			Dành cho sử dụng tiêu chuẩn trong tương lai
ORI rd=x0			Dành cho sử dụng tiêu chuẩn trong tương lai
XORI rd=x0			Dành cho sử dụng tiêu chuẩn trong tương lai
ADDIW rd=x0			Dành cho sử dụng tiêu chuẩn trong tương lai
ADD rd=x0	rs1!=x0		Dành cho sử dụng tiêu chuẩn trong tương lai
ADD rd=x0	rs1=x0, rs2!=x2-x5	28	Dành cho sử dụng tiêu chuẩn trong tương lai
ADD rd=x0	rs1=x0, rs2=x2	4	NTL.P1
	rs2=x3		NTL.PALL
	rs2=x4		NTL.S1
	rs2=x5		NTL.ALL
SUB rd=x0			Dành cho sử dụng tiêu chuẩn trong tương lai
AND rd=x0			Dành cho sử dụng tiêu chuẩn trong tương lai
OR rd=x0			Dành cho sử dụng tiêu chuẩn trong tương lai
XOR rd=x0			Dành cho sử dụng tiêu chuẩn trong tương lai
SLL rd=x0			Dành cho sử dụng tiêu chuẩn trong tương lai
SRL rd=x0			Dành cho sử dụng tiêu chuẩn trong tương lai
SRA rd=x0			Dành cho sử dụng tiêu chuẩn trong tương lai
ADDW rd=x0			Dành cho sử dụng tiêu chuẩn trong tương lai
SUBW rd=x0			Dành cho sử dụng tiêu chuẩn trong tương lai
SLLW rd=x0			Dành cho sử dụng tiêu chuẩn trong tương lai
SRLW rd=x0			Dành cho sử dụng tiêu chuẩn trong tương lai
SRAW rd=x0			Dành cho sử dụng tiêu chuẩn trong tương lai
FENCE rd=rs1=x0, fm=0	pred=W, succ=0	1	PAUSE
SLTI rd=x0			Dành cho sử dụng tùy chỉnh
SLTIU rd=x0			Dành cho sử dụng tùy chỉnh
SLLI rd=x0			Dành cho sử dụng tùy chỉnh
SRLI rd=x0			Dành cho sử dụng tùy chỉnh
SRAI rd=x0			Dành cho sử dụng tùy chỉnh
SLLIW rd=x0			Dành cho sử dụng tùy chỉnh
SRLIW rd=x0			Dành cho sử dụng tùy chỉnh
SRAIW rd=x0			Dành cho sử dụng tùy chỉnh
SLT rd=x0			Dành cho sử dụng tùy chỉnh
SLTU rd=x0			Dành cho sử dụng tùy chỉnh

Các phần mở rộng tiêu chuẩn, được thảo luận tiếp theo, bổ sung các chức năng quan trọng như phép nhân, phép chia, thao tác nguyên tử và số học dấu phẩy động vào tập lệnh cơ sở này.

3. Các Phần mở rộng Tiêu chuẩn

3.1. Phần mở rộng "M": Phép nhân và Phép chia Số nguyên

Phần mở rộng "M" bổ sung các lệnh phần cứng cho phép nhân và phép chia số nguyên. Nó được tách ra khỏi tập lệnh cơ sở để các triển khai nhỏ, chi phí thấp không yêu cầu các hoạt động này có thể bỏ qua phần cứng phức tạp tương ứng.

* Phép nhân (13.1):
  * MUL: Thực hiện phép nhân XLEN-bit × XLEN-bit và trả về XLEN bit thấp của tích.
  * MULH, MULHU, MULHSU: Trả về XLEN bit cao của tích 2×XLEN-bit cho các phép nhân có dấu × có dấu, không dấu × không dấu, và có dấu × không dấu tương ứng.
  * MULW (chỉ có trong RV64): Nhân 32 bit thấp của các thanh ghi nguồn và mở rộng dấu kết quả 32-bit thành 64 bit.
* Phép chia (13.2):
  * DIV và DIVU: Thực hiện phép chia số nguyên XLEN-bit có dấu và không dấu.
  * REM và REMU: Cung cấp số dư của phép chia có dấu và không dấu tương ứng.
  * DIVW, DIVUW, REMW, REMUW (chỉ có trong RV64): Các biến thể 32-bit của các lệnh chia và lấy số dư, tạo ra kết quả 64-bit được mở rộng dấu.

Ngữ nghĩa cho các trường hợp đặc biệt của phép chia được tóm tắt dưới đây.

Điều kiện	Số bị chia	Số chia	DIVU[W]	REMU[W]	DIV[W]	REM[W]
Chia cho không	x	0	2<sup>L</sup>-1	x	-1	x
Tràn số (chỉ có dấu)	-2<sup>L-1</sup>	-1	—	—	-2<sup>L-1</sup>	0

 Chúng tôi đã xem xét việc gây ra ngoại lệ khi chia số nguyên cho không, nhưng điều này sẽ là bẫy số học duy nhất trong ISA tiêu chuẩn. Thay vào đó, việc xử lý nó bằng cách trả về một giá trị xác định cho phép các trình triển khai ngôn ngữ tránh tương tác với các trình xử lý bẫy của môi trường thực thi. Các ngôn ngữ yêu cầu một luồng điều khiển ngay lập tức thay đổi khi chia cho không chỉ cần thêm một lệnh rẽ nhánh duy nhất, thường có thể dự đoán là không được thực hiện và ít gây ảnh hưởng đến hiệu suất.

Các hoạt động này là nền tảng cho tính toán số học, nhưng đối với các hệ thống đa bộ xử lý, việc đảm bảo đồng bộ hóa đòi hỏi các nguyên tố mạnh hơn như các hoạt động nguyên tử.

3.2. Phần mở rộng "A": Các lệnh Nguyên tử

Phần mở rộng "A" cung cấp các lệnh nguyên tử, là các khối xây dựng thiết yếu để đồng bộ hóa trong các hệ thống đa lõi. Các lệnh này cho phép các thao tác đọc-sửa-ghi vào bộ nhớ mà không bị gián đoạn bởi các hart khác.

* Xác định Thứ tự (14.1): Mỗi lệnh nguyên tử có hai bit, aq (acquire) và rl (release), để áp đặt các ràng buộc thứ tự bộ nhớ.
  * Bit aq: Đảm bảo rằng không có thao tác bộ nhớ nào sau lệnh nguyên tử này (theo thứ tự chương trình) có thể được quan sát là xảy ra trước nó.
  * Bit rl: Đảm bảo rằng tất cả các thao tác bộ nhớ trước lệnh nguyên tử này (theo thứ tự chương trình) được quan sát là đã xảy ra trước nó.
  * Khi cả aq và rl được đặt, hoạt động này có tính nhất quán tuần tự.
* Phần mở rộng "Zalrsc" - Load-Reserved/Store-Conditional (14.2): Cặp lệnh LR và SC được sử dụng để thực hiện các hoạt động nguyên tử phức tạp.
  * LR.W/LR.D: Tải một giá trị từ bộ nhớ và đăng ký một "tập hợp dành riêng" (reservation set) trên địa chỉ đó.
  * SC.W/SC.D: Cố gắng ghi một giá trị trở lại địa chỉ đó. Thao tác chỉ thành công (trả về 0) nếu tập hợp dành riêng vẫn còn hợp lệ (tức là không có hart nào khác ghi vào địa chỉ đó kể từ lệnh LR). Nếu không, nó sẽ thất bại (trả về giá trị khác không).
* Đảm bảo Thành công Cuối cùng của SC (14.3): Đặc tả đảm bảo rằng một vòng lặp LR/SC bị ràng buộc (tối đa 16 lệnh, chỉ chứa chuỗi LR/SC và mã thử lại) cuối cùng sẽ thành công, miễn là không có hart hoặc thiết bị nào khác liên tục ghi vào tập hợp dành riêng.
* Phần mở rộng "Zaamo" - Thao tác Bộ nhớ Nguyên tử (14.4): Các lệnh AMO thực hiện một thao tác đọc-sửa-ghi hoàn chỉnh trong một lệnh duy nhất. Chúng bao gồm AMOSWAP (hoán đổi), AMOADD (cộng), AMOXOR, AMOAND, AMOOR, AMOMIN[U], và AMOMAX[U]. Lệnh sẽ tải giá trị cũ từ bộ nhớ vào thanh ghi đích, thực hiện thao tác với một giá trị từ thanh ghi nguồn thứ hai, và lưu kết quả trở lại bộ nhớ.

Dưới đây là các ví dụ về cách sử dụng các lệnh nguyên tử để triển khai các hàm đồng bộ hóa phổ biến.

Ví dụ 2: Mã mẫu cho hàm compare-and-swap sử dụng LR/SC

# a0 giữ địa chỉ của vị trí bộ nhớ
# a1 giữ giá trị mong đợi
# a2 giữ giá trị mong muốn
# a0 giữ giá trị trả về, 0 nếu thành công, !0 nếu ngược lại
cas:
    lr.w t0, (a0)        # Tải giá trị ban đầu.
    bne t0, a1, fail     # Không khớp, vì vậy thất bại.
    sc.w t0, a2, (a0)    # Thử cập nhật.
    bnez t0, cas         # Thử lại nếu store-conditional thất bại.
    li a0, 0             # Đặt giá trị trả về là thành công.
    jr ra                # Trở về.
fail:
    li a0, 1             # Đặt giá trị trả về là thất bại.
    jr ra                # Trở về.


Ví dụ 3: Mã mẫu cho loại trừ tương hỗ (mutual exclusion)

# a0 chứa địa chỉ của khóa.
    li           t0, 1        # Khởi tạo giá trị hoán đổi.
again:
    lw           t1, (a0)     # Kiểm tra xem khóa có bị giữ không.
    bnez         t1, again    # Thử lại nếu bị giữ.
    amoswap.w.aq t1, t0, (a0) # Cố gắng chiếm khóa.
    bnez         t1, again    # Thử lại nếu bị giữ.
    # ...
    # Vùng tranh chấp.
    # ...
    amoswap.w.rl x0, x0, (a0) # Giải phóng khóa bằng cách lưu 0.


Ngoài việc xử lý đồng bộ hóa, các phần mở rộng khác cung cấp hỗ trợ cho các loại dữ liệu khác nhau, chẳng hạn như số dấu phẩy động.

3.3. Các Phần mở rộng "F", "D", và "Q": Số học Dấu phẩy động

Các phần mở rộng "F", "D", và "Q" cung cấp hỗ trợ cho số học dấu phẩy động tuân thủ tiêu chuẩn IEEE 754-2008, tương ứng cho các độ chính xác đơn (32-bit), đôi (64-bit), và bốn (128-bit). Các phần mở rộng này giới thiệu một tệp thanh ghi dấu phẩy động riêng biệt và một thanh ghi trạng thái và điều khiển (FCSR).

* "F" - Độ chính xác Đơn (Ch. 20):
  * Giới thiệu 32 thanh ghi dấu phẩy động (f0-f31), mỗi thanh ghi có thể chứa một giá trị đơn chính xác.
  * Giới thiệu thanh ghi fcsr để điều khiển chế độ làm tròn và lưu trữ các cờ ngoại lệ (ví dụ: tràn, chia cho không).
  * Cung cấp các lệnh tải và lưu (FLW, FSW), lệnh tính toán (cộng, trừ, nhân, chia, căn bậc hai), lệnh chuyển đổi sang/từ số nguyên, lệnh so sánh và một lệnh phân loại (FCLASS.S) để xác định loại của một số dấu phẩy động (ví dụ: số 0, số vô cực, NaN).
* "D" - Độ chính xác Đôi (Ch. 21):
  * Xây dựng dựa trên phần mở rộng "F". Trên các triển khai 64-bit, các thanh ghi f0-f31 được mở rộng để chứa các giá trị đôi chính xác.
  * Bổ sung các lệnh tải và lưu (FLD, FSD) và các phiên bản đôi chính xác của các lệnh tính toán, chuyển đổi, so sánh và phân loại.
* "Q" - Độ chính xác Bốn (Ch. 22):
  * Xây dựng dựa trên phần mở rộng "D".
  * Bổ sung các lệnh tải/lưu (FLQ, FSQ) và các lệnh tính toán, chuyển đổi, so sánh, phân loại tương ứng cho các giá trị 128-bit.

Trường fmt trong mã hóa lệnh được sử dụng để chỉ định độ chính xác của toán hạng, như được hiển thị trong bảng dưới đây.

Trường fmt	Ký hiệu	Ý nghĩa
00	S	32-bit đơn chính xác
01	D	64-bit đôi chính xác
10	H	16-bit nửa chính xác
11	Q	128-bit bốn chính xác

Trong nhiều ứng dụng, đặc biệt là trong các hệ thống nhúng, kích thước mã là một yếu tố quan trọng. Phần mở rộng lệnh nén giải quyết vấn đề này.

3.4. Phần mở rộng "C": Các lệnh Nén

Phần mở rộng "C" được thiết kế để giảm kích thước mã, một yếu tố quan trọng đối với hiệu suất bộ nhớ cache và chi phí bộ nhớ trong các hệ thống nhúng. Nó cung cấp các mã hóa 16-bit cho các lệnh 32-bit phổ biến.

* Tổng quan: Các lệnh 16-bit (RVC) và 32-bit (RVI) có thể được trộn lẫn tự do trong một luồng lệnh. Bộ xử lý có thể giải mã cả hai loại lệnh mà không cần chuyển chế độ.
* Định dạng Lệnh: Các định dạng lệnh RVC (ví dụ: CR, CI, CSS) được thiết kế cẩn thận để đơn giản hóa việc giải mã. Các trường thanh ghi được đặt ở các vị trí cố định khi có thể, và các giá trị tức thời được mã hóa để giảm độ phức tạp của phần cứng.
* Các loại Lệnh: Phần mở rộng "C" cung cấp phiên bản nén của các lệnh thường được sử dụng nhất, bao gồm:
  * Lệnh Load và Store: Các lệnh tải và lưu dựa trên con trỏ ngăn xếp (x2) hoặc một trong 8 thanh ghi cơ sở thường dùng (s0-s1, a0-a5).
  * Lệnh Chuyển điều khiển: Bao gồm các lệnh nhảy không điều kiện (C.J), nhảy và liên kết (C.JAL), và các lệnh rẽ nhánh có điều kiện so sánh với 0 (C.BEQZ, C.BNEZ).
  * Lệnh Tính toán Số nguyên: Bao gồm các thao tác thanh ghi-tức thời (C.ADDI, C.LI, C.LUI) và thanh ghi-thanh ghi (C.MV, C.ADD). Nhiều lệnh trong số này chỉ có thể truy cập 8 thanh ghi thường dùng.
* Lệnh C.EBREAK và C.NOP: Các phiên bản 16-bit của lệnh ngắt điểm và lệnh không thao tác.

 Nói một cách chính xác, C.JALR không hoàn toàn mở rộng thành một lệnh RVI cơ sở vì giá trị được thêm vào PC để tạo địa chỉ liên kết là 2 thay vì 4 như trong ISA cơ sở, nhưng việc hỗ trợ cả hai offset 2 và 4 byte chỉ là một thay đổi rất nhỏ đối với vi kiến trúc cơ sở.

Từ việc tối ưu hóa kích thước mã, chúng ta chuyển sang một mô hình tính toán hoàn toàn khác: song song dữ liệu, được hỗ trợ bởi phần mở rộng vector.

3.5. Phần mở rộng "V": Các Thao tác Vector

Phần mở rộng vector "V" là một phần mở rộng tiêu chuẩn mạnh mẽ hỗ trợ các hoạt động xử lý vector (SIMD - Single Instruction, Multiple Data). Nó rất quan trọng đối với các khối lượng công việc hiệu năng cao như tính toán khoa học, đa phương tiện và học máy, cho phép một lệnh duy nhất thực hiện cùng một thao tác trên nhiều phần tử dữ liệu.

* Mô hình Lập trình (31.3):
  * Thanh ghi Vector: Giới thiệu 32 thanh ghi vector (v0-v31). Độ dài của các thanh ghi này (VLEN) là do triển khai xác định.
  * Các CSR quan trọng: Một tập hợp các thanh ghi điều khiển và trạng thái (CSR) được sử dụng để cấu hình đơn vị vector:
    * vtype: Cấu hình kiểu vector, bao gồm SEW (Độ rộng phần tử được chọn), LMUL (Bội số nhóm thanh ghi), vta (agnostic đuôi), và vma (agnostic mặt nạ).
    * vl: Lưu số lượng phần tử sẽ được xử lý bởi các lệnh vector tiếp theo.
    * vlenb: Một CSR chỉ đọc chứa giá trị VLEN/8 (độ dài thanh ghi vector tính bằng byte).
* Cấu hình Vector (31.6): Các lệnh vsetvli và vsetivli là trung tâm của việc lập trình vector. Chúng được sử dụng ở đầu mỗi vòng lặp vector để cấu hình đơn vị vector (thiết lập vtype và vl) dựa trên số lượng phần tử cần xử lý. Điều này cho phép cùng một mã nhị phân chạy hiệu quả trên các triển khai phần cứng với các giá trị VLEN khác nhau.
* Các loại Lệnh Vector: Phần mở rộng "V" bao gồm một bộ lệnh phong phú, được phân loại thành:
  * Tải và Lưu trữ Vector (31.7): Hỗ trợ các mẫu truy cập bộ nhớ khác nhau, bao gồm sải đơn vị (unit-stride), sải (strided) và chỉ mục (indexed).
  * Số học Vector (31.10): Các lệnh cho cả số nguyên và dấu phẩy động, bao gồm các phép toán cộng, trừ, nhân, chia, v.v.
  * Các lệnh Mặt nạ (31.15): Cho phép thực thi có điều kiện trên từng phần tử, nơi một thao tác chỉ được áp dụng cho các phần tử có bit mặt nạ tương ứng được đặt.
  * Các lệnh Hoán vị (31.16): Cung cấp các cách linh hoạt để sắp xếp lại các phần tử trong và giữa các thanh ghi vector, ví dụ như vrgather và vslideup.

Dưới đây là một ví dụ về kỹ thuật "stripmining" sử dụng vsetvli để xử lý một mảng có độ dài bất kỳ.

# Ví dụ: Tải các giá trị 16-bit, nhân mở rộng lên 32b, dịch phải kết quả 32b
# 3 bit, lưu các giá trị 32b.
# Khi bắt đầu:
#  a0 giữ tổng số phần tử cần xử lý
#  a1 giữ địa chỉ của mảng nguồn
#  a2 giữ địa chỉ của mảng đích
loop:
    vsetvli a3, a0, e16, m4, ta, ma  # vtype = vector số nguyên 16-bit;
                                    # cũng cập nhật a3 với vl (# phần tử trong lần lặp này)
    vle16.v v4, (a1)        # Lấy vector 16b
    slli t1, a3, 1          # Nhân # phần tử lần lặp này với 2 byte/phần tử nguồn
    add a1, a1, t1          # Tăng con trỏ nguồn
    vwmul.vx v8, v4, x10    # Nhân mở rộng thành 32b trong <v8--v15>
    vsrl.vi v8, v8, 3       # Dịch phải 3 bit
    vse32.v v8, (a2)        # Lưu vector 32b
    slli t1, a3, 2          # Nhân # phần tử lần lặp này với 4 byte/phần tử đích
    add a2, a2, t1          # Tăng con trỏ đích
    sub a0, a0, a3          # Giảm số lượng phần tử
    bnez a0, loop           # Còn phần tử nào không?


Ngoài các phần mở rộng tính toán, việc đảm bảo an ninh thông tin cũng đòi hỏi sự hỗ trợ từ phần cứng, dẫn đến sự ra đời của các phần mở rộng mật mã.

3.6. Các Phần mở rộng Mật mã

Các phần mở rộng mật mã cung cấp khả năng tăng tốc phần cứng cho các thuật toán và nguyên tố mật mã phổ biến. Chúng được chia thành hai nhóm chính: các lệnh vô hướng (scalar) cho các triển khai chi phí thấp và các lệnh vector cho các ứng dụng đòi hỏi thông lượng cao.

* Mật mã Vô hướng (Ch. 32):
  * Các phần mở rộng này được nhóm lại dưới tiền tố "Zk" (ví dụ: Zkn, Zkr, Zkt).
  * Chúng bao gồm các lệnh chuyên dụng để tăng tốc các vòng lặp cốt lõi của các thuật toán tiêu chuẩn như AES (ví dụ: aes64es cho mã hóa, aes64ds cho giải mã) và SHA-2.
  * Phần mở rộng "Zkr" định nghĩa một giao diện tiêu chuẩn cho Nguồn Entropy (Entropy Source) thông qua CSR seed. Giao diện này cho phép phần mềm truy cập vào nguồn nhiễu ngẫu nhiên phần cứng, một thành phần cơ bản để tạo khóa an toàn.
  * Phần mở rộng "Zkt" (Data Independent Execution Latency) cung cấp một "hợp đồng ISA" cho các nhà phát triển phần mềm. Khi một triển khai hỗ trợ Zkt, nó đảm bảo rằng một tập hợp con các lệnh (bao gồm tất cả các lệnh mật mã) sẽ thực thi trong một khoảng thời gian không phụ thuộc vào giá trị dữ liệu mà chúng đang xử lý. Điều này rất quan trọng để giảm thiểu các cuộc tấn công kênh bên thời gian (timing side-channel attacks).
* Mật mã Vector (Ch. 33):
  * Các lệnh này tận dụng kiến trúc vector để xử lý nhiều khối dữ liệu song song, mang lại thông lượng rất cao cho các hoạt động mật mã hàng loạt.
  * Chúng sử dụng khái niệm Nhóm Phần tử Rộng (EGW - Element Group Width) để đảm bảo tính di động của phần mềm trên các triển khai có VLEN khác nhau.
  * Các phần mở rộng được nhóm theo bộ thuật toán, ví dụ: Zvkn cho bộ NIST (AES, SHA-2) và Zvks cho bộ ShangMi của Trung Quốc (SM3, SM4). Các biến thể bổ sung hỗ trợ các chế độ hoạt động như GCM.

Kiến trúc tập lệnh RISC-V, với ISA cơ sở tinh gọn, mô hình phần mở rộng tiêu chuẩn phong phú, và khả năng tùy chỉnh linh hoạt, thể hiện một cách tiếp cận hiện đại và có khả năng mở rộng để thiết kế bộ xử lý. Nó cung cấp một nền tảng vững chắc cho cả máy tính đa dụng và các giải pháp tăng tốc chuyên dụng, thúc đẩy sự đổi mới trong cả phần cứng và phần mềm.
