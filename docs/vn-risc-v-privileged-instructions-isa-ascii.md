
Sổ tay Tập lệnh RISC-V: Tập II - Kiến trúc Đặc quyền

1.0 Giới thiệu Tổng quan về Kiến trúc Đặc quyền RISC-V

Kiến trúc Đặc quyền RISC-V định nghĩa tất cả các khía cạnh của một hệ thống RISC-V ngoài tập lệnh không đặc quyền. Điều này bao gồm các lệnh đặc quyền và các chức năng bổ sung cần thiết để chạy các hệ điều hành, quản lý hệ thống và gắn kết các thiết bị bên ngoài. Tài liệu này đóng vai trò là một đặc tả kỹ thuật, mô tả một thiết kế theo mô hình bảo vệ dựa trên các mức đặc quyền thông thường, nhằm mục đích hỗ trợ các hệ điều hành phổ biến hiện có một cách hiệu quả.

1.1 Thuật ngữ Ngăn xếp Phần mềm Đặc quyền

Kiến trúc RISC-V định nghĩa một bộ thuật ngữ và các giao diện trừu tượng để mô tả các thành phần trong ngăn xếp phần mềm đặc quyền. Việc tiêu chuẩn hóa các giao diện này cho phép phần mềm di động và có khả năng ảo hóa cao.

* ABI (Application Binary Interface - Giao diện Nhị phân Ứng dụng): Đây là giao diện giữa một ứng dụng và môi trường thực thi ứng dụng (AEE). Nó bao gồm tập lệnh mức người dùng được hỗ trợ cộng với một tập các lời gọi hệ thống (ABI calls) để tương tác với AEE. Việc trừu tượng hóa AEE cho phép cùng một ABI có thể được triển khai trên nhiều hệ điều hành khác nhau hoặc trong một môi trường giả lập.
* SBI (Supervisor Binary Interface - Giao diện Nhị phân Giám sát): Đây là giao diện giữa một hệ điều hành và môi trường thực thi giám sát (SEE). SBI bao gồm tập lệnh mức người dùng và mức giám sát, cùng với một tập các lời gọi hàm SBI. Việc sử dụng một SBI duy nhất cho phép một ảnh nhị phân của hệ điều hành có thể chạy trên bất kỳ SEE nào, từ một bộ nạp khởi động đơn giản trên phần cứng cấp thấp đến một máy ảo do hypervisor cung cấp.
* HBI (Hypervisor Binary Interface - Giao diện Nhị phân Hypervisor): Đây là giao diện giữa một hypervisor (trình ảo hóa) và môi trường thực thi hypervisor (HEE). Tương tự như các giao diện khác, HBI cô lập hypervisor khỏi các chi tiết của nền tảng phần cứng.

Tầm quan trọng chiến lược của việc tách biệt các giao diện này nằm ở khả năng phân lớp và tính di động. Bằng cách định nghĩa rõ ràng các ranh giới này, RISC-V cho phép một hệ điều hành (được viết cho một SBI) có thể chạy nguyên bản trên phần cứng hoặc bên trong một máy ảo do hypervisor cung cấp mà không cần sửa đổi. Điều này đơn giản hóa đáng kể việc ảo hóa và phát triển phần cứng mới.

1.2 Các Mức Đặc quyền (Privilege Levels)

Tại bất kỳ thời điểm nào, một luồng phần cứng RISC-V (hart) đang chạy ở một mức đặc quyền nhất định. Các mức đặc quyền được sử dụng để cung cấp sự bảo vệ giữa các thành phần khác nhau của ngăn xếp phần mềm. Hiện tại có ba mức đặc quyền được định nghĩa.

Mức	Mã hóa	Tên	Viết tắt
0	00	User/Application	U
1	01	Supervisor	S
2	10	Dành riêng	-
3	11	Machine	M

Vai trò của mỗi mức đặc quyền được phân định rõ ràng:

* M-mode (Machine): Đây là mức đặc quyền cao nhất và là mức duy nhất bắt buộc phải có trên mọi nền tảng phần cứng RISC-V. Mã chạy ở M-mode được coi là đáng tin cậy vì nó có quyền truy cập cấp thấp, không bị hạn chế vào toàn bộ máy. M-mode thường được sử dụng cho firmware, bộ nạp khởi động và các môi trường thực thi an toàn.
* S-mode (Supervisor): Mức này được thiết kế để chạy các hệ điều hành thông thường như Unix. Nó cung cấp một lớp trừu tượng trên phần cứng vật lý, quản lý tài nguyên hệ thống và cung cấp các dịch vụ cho các ứng dụng thông qua các lời gọi hệ thống.
* U-mode (User/Application): Đây là mức có đặc quyền thấp nhất, dành cho các ứng dụng. U-mode bị giới hạn, không thể truy cập trực tiếp vào các tài nguyên hệ thống quan trọng và phải yêu cầu dịch vụ từ hệ điều hành thông qua ABI.

Kiến trúc này rất linh hoạt, cho phép các hệ thống chỉ triển khai các mức đặc quyền cần thiết. Một hệ thống nhúng đơn giản có thể chỉ cần M-mode, trong khi một hệ thống chạy các hệ điều hành giống Unix sẽ cần cả ba mức M, S và U để đảm bảo sự cách ly và bảo vệ cần thiết.

1.3 Chế độ Gỡ lỗi (Debug Mode)

Ngoài ba mức đặc quyền chính, các triển khai có thể bao gồm một Chế độ Gỡ lỗi (D-mode) để hỗ trợ gỡ lỗi off-chip và kiểm tra sản xuất. D-mode có thể được coi là một mức đặc quyền bổ sung với quyền truy cập thậm chí còn cao hơn cả M-mode, cho phép các công cụ gỡ lỗi bên ngoài kiểm soát và quan sát trạng thái của hart.

Để quản lý các mức đặc quyền và trạng thái hệ thống, RISC-V sử dụng một tập hợp các thanh ghi chuyên dụng được gọi là Thanh ghi Điều khiển và Trạng thái (CSR), sẽ được thảo luận chi tiết trong phần tiếp theo.

2.0 Thanh ghi Điều khiển và Trạng thái (CSRs)

Các Thanh ghi Điều khiển và Trạng thái (CSRs) là trung tâm của kiến trúc đặc quyền RISC-V. Chúng là giao diện chính để phần mềm đặc quyền tương tác, cấu hình và quan sát trạng thái của một hart. Kiến trúc RISC-V dành riêng một không gian địa chỉ 12-bit cho tối đa 4.096 CSR, được truy cập thông qua một bộ lệnh đặc quyền chuyên dụng.

2.1 Quy ước Ánh xạ Địa chỉ CSR

Theo quy ước, 4 bit cao của địa chỉ CSR (csr[11:8]) được sử dụng để mã hóa các thuộc tính mặc định về đặc quyền truy cập. Quy ước này phân chia không gian địa chỉ 12-bit thành các dải riêng biệt cho từng mức đặc quyền và loại truy cập (đọc/ghi hoặc chỉ đọc, tiêu chuẩn hoặc tùy chỉnh). Điều này giúp đơn giản hóa việc kiểm tra lỗi trong phần cứng và cung cấp một không gian địa chỉ CSR lớn và có cấu trúc.

Dải địa chỉ CSR (Hex)	Công dụng và Khả năng truy cập
0x000-0x0FF	Tiêu chuẩn, đọc/ghi
0x400-0x4FF	Tiêu chuẩn, đọc/ghi
0x800-0x8FF	Tùy chỉnh, đọc/ghi
0xC00-0xCFF	CSR Không đặc quyền/Mức Người dùng, Tiêu chuẩn, chỉ đọc
0x100-0x1FF	Tiêu chuẩn, đọc/ghi
0x500-0x5FF	Tiêu chuẩn, đọc/ghi / Tùy chỉnh, đọc/ghi
0x900-0x9FF	Tiêu chuẩn, đọc/ghi / Tùy chỉnh, đọc/ghi
0xD00-0xDFF	CSR Mức Giám sát, Tiêu chuẩn, chỉ đọc / Tùy chỉnh, chỉ đọc
0x200-0x2FF	Tiêu chuẩn, đọc/ghi
0x600-0x6FF	Tiêu chuẩn, đọc/ghi / Tùy chỉnh, đọc/ghi
0xA00-0xAFF	Tiêu chuẩn, đọc/ghi / Tùy chỉnh, đọc/ghi
0xE00-0xEFF	CSR Hypervisor và VS, Tiêu chuẩn, đọc/ghi / Tùy chỉnh, đọc/ghi
0x300-0x3FF	Tiêu chuẩn, đọc/ghi
0x700-0x7FF	Tiêu chuẩn, đọc/ghi / Tùy chỉnh, đọc/ghi
0xB00-0xBFF	Tiêu chuẩn, đọc/ghi / Tùy chỉnh, đọc/ghi
0xF00-0xFFF	CSR Mức Máy, Tiêu chuẩn, đọc/ghi / Tùy chỉnh, đọc/ghi

Việc cố gắng truy cập một CSR không tồn tại, truy cập mà không có đủ đặc quyền, hoặc ghi vào một thanh ghi chỉ đọc sẽ gây ra một ngoại lệ lệnh không hợp lệ (illegal-instruction exception).

2.2 Đặc tả các Trường CSR

Trong các thanh ghi CSR, các trường (field) riêng lẻ có thể có các hành vi cụ thể khi được đọc hoặc ghi. Phần mềm đặc quyền phải tuân thủ các quy ước này để đảm bảo tính tương thích và hoạt động chính xác.

* WPRI (Reserved Writes Preserve Values, Reads Ignore Values): Các trường này được dành riêng cho các phần mở rộng trong tương lai. Phần mềm nên bỏ qua các giá trị đọc được từ các trường này. Khi ghi vào một thanh ghi chứa trường WPRI, phần mềm phải bảo toàn giá trị hiện có của trường đó (read-modify-write) để đảm bảo tính tương thích về sau.
* WLRL (Write/Read Only Legal Values): Các trường này chỉ chấp nhận một tập hợp con các giá trị được định nghĩa là hợp lệ. Phần mềm không nên ghi các giá trị không hợp lệ vào các trường này. Việc ghi một giá trị không hợp lệ có thể dẫn đến một ngoại lệ hoặc giá trị không xác định khi đọc lại.
* WARL (Write Any Values, Reads Legal Values): Các trường này có thể được ghi bất kỳ giá trị nào, nhưng khi đọc, chúng sẽ luôn trả về một giá trị hợp lệ. Phần cứng sẽ tự động điều chỉnh hoặc chọn một giá trị hợp lệ gần nhất. Phần mềm có thể sử dụng cơ chế này để thăm dò các khả năng được hỗ trợ bằng cách ghi một giá trị và sau đó đọc lại để xem giá trị thực tế đã được thiết lập.

Các phần tiếp theo sẽ đi sâu vào các CSR và lệnh cụ thể được liên kết với từng mức đặc quyền, bắt đầu với mức có đặc quyền cao nhất: Mức Máy.

3.0 Tập lệnh Mức Máy (Machine-Level ISA)

M-mode là mức đặc quyền cao nhất và bắt buộc phải có trong mọi hệ thống RISC-V. Nó đóng vai trò là lớp phần mềm đáng tin cậy nhất, chịu trách nhiệm truy cập cấp thấp vào nền tảng phần cứng, cấu hình hệ thống và quản lý các mức đặc quyền thấp hơn. Khi hệ thống được reset, hart sẽ bắt đầu thực thi ở M-mode.

3.1 Các CSR Mức Máy Quan trọng

M-mode sử dụng một tập hợp các CSR chuyên dụng để kiểm soát và giám sát trạng thái của hart.

* misa (Machine ISA Register): Thanh ghi misa báo cáo về tập lệnh (ISA) được hart hỗ trợ. Đây là một thanh ghi đọc/ghi WARL, cho phép phần mềm xác định và đôi khi sửa đổi các khả năng của hart.
  * Trường MXL: Trường này cho biết độ rộng ISA cơ sở gốc của máy (Machine XLEN). Ví dụ, MXL=1 tương ứng với RV32, MXL=2 tương ứng với RV64.
  * Trường Extensions: Đây là một bitmap 26-bit, trong đó mỗi bit tương ứng với một chữ cái trong bảng chữ cái, đại diện cho một phần mở rộng tiêu chuẩn. Ví dụ, bit 0 (A) cho biết có hỗ trợ phần mở rộng Atomic, bit 2 (C) cho phần mở rộng Compressed, và bit 12 (M) cho phần mở rộng Integer Multiply/Divide.
* Nếu misa trả về giá trị không, điều này cho biết thanh ghi chưa được triển khai và các khả năng của CPU phải được xác định thông qua một cơ chế phi tiêu chuẩn khác. Tuy nhiên, vẫn có thể xác định độ rộng cơ sở ngay cả khi misa bằng không. Bằng cách đặt giá trị tức thời 4 vào một thanh ghi và sau đó dịch trái thanh ghi đó 31 bit một lần. Nếu kết quả là không sau một lần dịch, hart là RV32. Nếu kết quả là không sau hai lần dịch, hart là RV64, nếu không thì là RV128.
* mstatus & mstatush (Machine Status Registers): Thanh ghi mstatus là thanh ghi cốt lõi để theo dõi và kiểm soát trạng thái hoạt động hiện tại của hart. Nó chứa nhiều trường quan trọng được nhóm theo chức năng:
  * Ngăn xếp Đặc quyền và Bật/Tắt Ngắt Toàn cục: Các trường MPP (Machine Previous Privilege), MPIE (Machine Previous Interrupt Enable), SPP, và SPIE tạo thành một ngăn xếp hai cấp để xử lý các bẫy lồng nhau (nested traps). Khi một bẫy xảy ra, mức đặc quyền và trạng thái bật/tắt ngắt trước đó sẽ được lưu vào các trường này, cho phép hệ thống quay trở lại trạng thái cũ một cách an toàn sau khi xử lý bẫy.
  * Đặc quyền Bộ nhớ: Các bit MPRV (Modify Privilege), MXR (Make eXecutable Readable), và SUM (Supervisor User Memory access) cung cấp các cơ chế linh hoạt để sửa đổi quyền truy cập bộ nhớ. Ví dụ, MPRV=1 cho phép M-mode thực hiện các thao tác tải và lưu với các quyền của mức đặc quyền được chỉ định trong MPP, rất hữu ích cho việc truy cập bộ nhớ của các mức đặc quyền thấp hơn mà không cần chuyển đổi ngữ cảnh phức tạp.
  * Hỗ trợ Ảo hóa: Các bit TVM (Trap Virtual Memory), TW (Timeout Wait), và TSR (Trap SRET) là các "công tắc" cho phép một hypervisor bẫy các hoạt động quan trọng của hệ điều hành khách. Ví dụ, TVM=1 sẽ gây ra một ngoại lệ khi hệ điều hành khách cố gắng truy cập thanh ghi satp (để quản lý bộ nhớ ảo), cho phép hypervisor can thiệp và duy trì các bảng trang bóng (shadow page tables).
  * Trạng thái Ngữ cảnh Mở rộng: Các trường FS, VS, và XS theo dõi trạng thái của các đơn vị dấu chấm động (Floating-point), vector và các phần mở rộng khác. Các trạng thái này (Off, Initial, Clean, Dirty) cho phép hệ điều hành tối ưu hóa việc chuyển đổi ngữ cảnh bằng cách chỉ lưu/phục hồi trạng thái của các đơn vị này khi chúng thực sự đã được sử dụng (Dirty).
* mtvec (Machine Trap-Vector Base-Address Register): Thanh ghi này cấu hình địa chỉ cơ sở của trình xử lý bẫy cho M-mode.
  * Chế độ Direct: Tất cả các bẫy (ngoại lệ và ngắt) đều chuyển điều khiển đến cùng một địa chỉ cơ sở.
  * Chế độ Vectored: Các ngắt chuyển điều khiển đến một địa chỉ được tính bằng cách cộng địa chỉ cơ sở với mã nguyên nhân của ngắt, cho phép xử lý ngắt nhanh hơn bằng cách nhảy trực tiếp đến trình xử lý cụ thể.
* mepc, mcause, mtval (Machine Trap Handling CSRs): Bộ ba CSR này hoạt động cùng nhau để cung cấp thông tin chi tiết về một bẫy đã xảy ra.
  * mepc (Machine Exception Program Counter): Lưu trữ địa chỉ của lệnh đã gây ra bẫy hoặc bị ngắt. Đây là địa chỉ mà hệ thống sẽ quay trở lại sau khi xử lý xong bẫy.
  * mcause (Machine Cause Register): Lưu một mã số cho biết nguyên nhân của bẫy. Bit cao nhất cho biết đó là một ngắt (interrupt) hay một ngoại lệ (exception), và các bit còn lại xác định nguyên nhân cụ thể (ví dụ: lỗi trang, lệnh không hợp lệ).
  * mtval (Machine Trap Value Register): Cung cấp thông tin bổ sung về bẫy. Ví dụ, đối với một lỗi trang (page fault) hoặc lỗi truy cập bộ nhớ, mtval sẽ chứa địa chỉ ảo đã gây ra lỗi.

3.2 Lệnh Đặc quyền Chế độ Máy

M-mode có một tập hợp các lệnh đặc quyền để quản lý hệ thống.

* ECALL và EBREAK: ECALL được sử dụng để thực hiện một cuộc gọi đến môi trường thực thi (ví dụ: từ U-mode gọi hệ điều hành), gây ra một ngoại lệ Environment Call. EBREAK được sử dụng bởi các trình gỡ lỗi để tạo một điểm ngắt, gây ra một ngoại lệ Breakpoint.
* MRET: Lệnh này được sử dụng để quay trở về từ một trình xử lý bẫy ở M-mode. Nó phục hồi trạng thái đặc quyền và PC từ các thanh ghi mstatus và mepc.
* WFI (Wait for Interrupt): Lệnh này cho phép hart tạm dừng thực thi cho đến khi có một ngắt đang chờ xử lý. Đây là một cơ chế hiệu quả để tiết kiệm năng lượng khi không có công việc gì để làm.

3.3 Bảo vệ Bộ nhớ Vật lý (PMP - Physical Memory Protection)

Cơ chế Bảo vệ Bộ nhớ Vật lý (PMP) là một tính năng tùy chọn cho phép M-mode định cấu hình các quyền truy cập (đọc, ghi, thực thi) cho các vùng bộ nhớ vật lý. Bằng cách sử dụng các thanh ghi pmpcfg và pmpaddr, M-mode có thể thiết lập tối đa 64 vùng nhớ với các quyền khác nhau. Các quy tắc PMP này được áp dụng cho các truy cập từ các mức đặc quyền thấp hơn (S và U). Tuy nhiên, các quy tắc này chỉ được thực thi đối với các truy cập ở M-mode nếu mục PMP tương ứng được khóa (bit L được đặt). Điều này cung cấp một lớp bảo vệ phần cứng mạnh mẽ để cách ly các thành phần phần mềm và bảo vệ các tài nguyên hệ thống quan trọng khỏi các truy cập trái phép.

Trong khi M-mode cung cấp khả năng kiểm soát toàn bộ hệ thống, các hệ điều hành hiện đại yêu cầu một mức đặc quyền ít hơn để chạy an toàn và hiệu quả. Mức này được gọi là S-mode, sẽ được khám phá trong phần tiếp theo.

4.0 Tập lệnh Mức Giám sát (Supervisor-Level ISA)

Mức đặc quyền Giám sát (S-mode) được thiết kế đặc biệt để chạy các hệ điều hành phức tạp như các hệ điều hành họ Unix. Nó cung cấp một lớp trừu tượng trên phần cứng vật lý, hỗ trợ các khái niệm cốt lõi như bộ nhớ ảo, ngắt, và xử lý ngoại lệ mà không cần cấp cho hệ điều hành toàn quyền kiểm soát hệ thống như ở M-mode. Điều này cho phép một lớp phần mềm cấp thấp hơn (chạy ở M-mode) quản lý và ảo hóa hệ điều hành.

4.1 Các CSR Mức Giám sát

S-mode có một tập hợp các CSR riêng, phần lớn là các phiên bản có giới hạn của các CSR M-mode tương ứng. Điều này cho phép hệ điều hành quản lý trạng thái của chính nó và các ứng dụng mà nó chạy, trong khi vẫn bị giới hạn bởi các chính sách do M-mode thiết lập.

* sstatus (Supervisor Status Register): Thanh ghi sstatus là một cái nhìn bị hạn chế của mstatus. Nó chứa các trường mà một hệ điều hành cần để quản lý ngữ cảnh của nó và của các ứng dụng người dùng, chẳng hạn như SPP (Supervisor Previous Privilege), SIE (Supervisor Interrupt Enable), SPIE, SUM (Supervisor User Memory access), và MXR. Hệ điều hành có thể đọc/ghi các trường này, nhưng không thể truy cập các trường chỉ dành cho M-mode trong mstatus.
* sip & sie (Supervisor Interrupt Registers): Tương tự, sip (Supervisor Interrupt Pending) và sie (Supervisor Interrupt Enable) là các phiên bản S-mode của mip và mie. Chúng chỉ hiển thị và cho phép kiểm soát các ngắt đã được M-mode ủy quyền (delegate) một cách rõ ràng cho S-mode. Điều này cho phép M-mode giữ lại quyền kiểm soát đối với các ngắt quan trọng của hệ thống (như ngắt timer của máy) trong khi ủy quyền các ngắt khác (như ngắt từ thiết bị ngoại vi) cho hệ điều hành.
* sepc, scause, stval: Đây là bộ ba thanh ghi xử lý bẫy cho S-mode. Chúng hoạt động giống hệt như các thanh ghi mepc, mcause, và mtval của M-mode nhưng chỉ dành cho các bẫy được xử lý trong S-mode. Khi một bẫy được ủy quyền cho S-mode, các thanh ghi này sẽ được phần cứng tự động cập nhật, cung cấp cho trình xử lý bẫy của hệ điều hành thông tin cần thiết về lỗi.
* satp (Supervisor Address Translation and Protection Register): Đây là một trong những CSR quan trọng nhất của S-mode. Nó kiểm soát hệ thống bộ nhớ ảo dựa trên trang.
  * Trường MODE: Bật hoặc tắt bộ nhớ ảo và chọn thuật toán dịch địa chỉ (ví dụ: Sv32 cho hệ thống 32-bit).
  * Trường ASID (Address Space Identifier): Cho phép gắn thẻ các mục trong bộ đệm dịch địa chỉ (TLB) với một ID không gian địa chỉ, giúp tránh phải xóa toàn bộ TLB khi chuyển đổi giữa các tiến trình.
  * Trường PPN (Physical Page Number): Chứa địa chỉ vật lý của bảng trang gốc (root page table).

4.2 Lệnh SFENCE.VMA

Lệnh SFENCE.VMA (Supervisor FENCE, Virtual Memory All) là một lệnh đặc quyền quan trọng. Khi hệ điều hành thay đổi các bảng trang (ví dụ, khi ánh xạ hoặc hủy ánh xạ một trang bộ nhớ), các thay đổi này có thể không được phần cứng nhìn thấy ngay lập tức vì phần cứng có thể đã lưu trữ các bản dịch địa chỉ cũ trong bộ đệm dịch địa chỉ (TLB). Lệnh SFENCE.VMA được sử dụng để đồng bộ hóa các thay đổi trong bộ nhớ với phần cứng, đảm bảo rằng tất cả các bản dịch địa chỉ sau lệnh này sẽ sử dụng thông tin bảng trang mới nhất.

4.3 Hệ thống Bộ nhớ Ảo Sv32

Sv32 là cơ chế bộ nhớ ảo dựa trên trang 32-bit được định nghĩa cho RISC-V. Nó cung cấp một không gian địa chỉ ảo 32-bit cho mỗi tiến trình.

* Dịch Địa chỉ: Một địa chỉ ảo 32-bit được dịch thành một địa chỉ vật lý 34-bit thông qua một cây bảng trang hai cấp. 20 bit cao của địa chỉ ảo (Virtual Page Number - VPN) được sử dụng để lập chỉ mục vào các bảng trang, trong khi 12 bit thấp (page offset) được giữ nguyên và nối vào địa chỉ trang vật lý kết quả. Ngoài ra, bất kỳ mức nào của PTE cũng có thể là một PTE lá, do đó ngoài các trang 4 KiB, Sv32 còn hỗ trợ các siêu trang (megapages) 4 MiB.
* Định dạng Mục Bảng Trang (PTE - Page Table Entry): Mỗi mục trong bảng trang chứa thông tin về một trang bộ nhớ. Các bit quan trọng trong một PTE Sv32 bao gồm:
  * V (Valid): Bit này cho biết liệu PTE có hợp lệ hay không. Nếu không, một lỗi trang sẽ xảy ra.
  * R/W/X (Permissions): Các bit này xác định quyền truy cập vào trang: đọc (Read), ghi (Write), và thực thi (Execute).
  * U (User): Bit này cho biết liệu trang có thể được truy cập từ U-mode hay không.
  * A (Accessed): Bit này được phần cứng tự động bật khi trang được truy cập. Hệ điều hành sử dụng nó để quyết định trang nào sẽ được đưa ra bộ nhớ phụ.
  * D (Dirty): Bit này được phần cứng tự động bật khi trang được ghi. Nó cho biết trang đã bị sửa đổi và cần được ghi lại vào bộ nhớ phụ trước khi được tái sử dụng.

Để hỗ trợ ảo hóa hiệu quả, RISC-V cung cấp một phần mở rộng Hypervisor chuyên dụng, sẽ được trình bày chi tiết trong phần cuối cùng.

5.0 Mở rộng "H" hỗ trợ Hypervisor

Phần mở rộng Hypervisor ("H") được thiết kế để ảo hóa kiến trúc S-mode, cho phép các hệ điều hành khách (guest OS) chạy hiệu quả và an toàn trên một hypervisor. Thay vì ảo hóa cổ điển bằng cách chạy hệ điều hành khách ở U-mode (gây ra nhiều bẫy tốn kém), phần mở rộng "H" giới thiệu các chế độ và cơ chế phần cứng mới để giảm thiểu sự can thiệp của hypervisor.

5.1 Các Chế độ Đặc quyền và Ảo hóa

Phần mở rộng này giới thiệu một trạng thái ảo hóa, được biểu thị bằng biến V.

* Khi V=0 (Không ảo hóa): Hart hoạt động ở các chế độ thông thường.
  * HS-mode (Hypervisor-extended Supervisor mode): Đây là S-mode được mở rộng với các đặc quyền của hypervisor. Hypervisor chạy ở chế độ này.
  * U-mode: Chế độ người dùng thông thường, chạy các ứng dụng trên hypervisor.
* Khi V=1 (Ảo hóa): Hart đang thực thi bên trong một máy ảo.
  * VS-mode (Virtual Supervisor mode): Hệ điều hành khách chạy ở chế độ này. Nó có các đặc quyền tương tự như S-mode thông thường nhưng được kiểm soát bởi hypervisor.
  * VU-mode (Virtual User mode): Các ứng dụng của khách chạy ở chế độ này, bên trên hệ điều hành khách.

5.2 Các CSR Hypervisor và Giám sát Ảo

Để quản lý môi trường ảo hóa, phần mở rộng "H" giới thiệu hai bộ CSR mới.

* Các CSR Hypervisor: Đây là các CSR mới được hypervisor (chạy ở HS-mode) sử dụng để cấu hình và quản lý các máy ảo. Các CSR chính bao gồm:
  * hstatus: Tương tự như mstatus, nhưng dành cho việc kiểm soát trạng thái của máy ảo.
  * hedeleg và hideleg: Cho phép hypervisor ủy quyền các ngoại lệ và ngắt cho hệ điều hành khách xử lý trực tiếp, thay vì bẫy vào hypervisor.
  * hgatp: Cấu hình giai đoạn thứ hai của việc dịch địa chỉ (sẽ được giải thích bên dưới).
* Các CSR Giám sát Ảo (VS CSRs): Đây là các bản sao ảo hóa của các CSR S-mode. Ví dụ bao gồm vsstatus, vsepc, và vsatp. Khi V=1 (đang ở trong một máy ảo), việc truy cập các CSR S-mode thông thường (ví dụ: sstatus) sẽ được phần cứng tự động chuyển hướng đến các VS CSR tương ứng (ví dụ: vsstatus). Điều này tạo ra một môi trường trong suốt cho hệ điều hành khách, cho phép nó chạy mà không cần sửa đổi, như thể nó đang chạy trực tiếp trên phần cứng.

5.3 Dịch Địa chỉ Hai Giai đoạn

Một trong những tính năng cốt lõi của phần mở rộng "H" là cơ chế dịch địa chỉ hai giai đoạn để cung cấp sự cách ly bộ nhớ mạnh mẽ giữa các máy ảo và giữa máy ảo với hypervisor.

* Giai đoạn 1 (VS-stage): Được kiểm soát bởi hệ điều hành khách thông qua thanh ghi vsatp. Giai đoạn này dịch một địa chỉ ảo của khách (Guest Virtual Address) thành một địa chỉ vật lý của khách (Guest Physical Address). Quá trình này hoàn toàn giống với quá trình dịch địa chỉ S-mode thông thường, cho phép hệ điều hành khách quản lý không gian địa chỉ của riêng nó.
* Giai đoạn 2 (G-stage): Được kiểm soát bởi hypervisor thông qua thanh ghi hgatp. Giai đoạn này dịch địa chỉ vật lý của khách thu được từ giai đoạn 1 thành một địa chỉ vật lý của giám sát (Supervisor Physical Address), tức là địa chỉ vật lý thực của máy.

Cơ chế hai giai đoạn này là nền tảng cho việc ảo hóa bộ nhớ. Hypervisor có toàn quyền kiểm soát ánh xạ từ không gian vật lý "giả" của khách sang không gian vật lý thực của máy, cho phép nó phân bổ, di chuyển và bảo vệ bộ nhớ của các máy ảo một cách hiệu quả và an toàn.

Kết luận

Kiến trúc đặc quyền RISC-V được xây dựng theo từng lớp một cách có chủ ý, thể hiện tính mô-đun và linh hoạt vốn có của triết lý RISC-V. Bắt đầu từ M-mode cơ bản cung cấp quyền kiểm soát phần cứng tuyệt đối, kiến trúc này bổ sung S-mode để hỗ trợ các hệ điều hành hiện đại với bộ nhớ ảo và bảo vệ. Cuối cùng, phần mở rộng Hypervisor xây dựng trên nền tảng đó để cung cấp khả năng ảo hóa hiệu suất cao. Mỗi lớp đều định nghĩa một giao diện rõ ràng với lớp bên dưới, cho phép các triển khai khác nhau lựa chọn các tính năng cần thiết trong khi vẫn duy trì khả năng tương thích phần mềm. Cách tiếp cận theo lớp này không chỉ giúp đơn giản hóa thiết kế mà còn tạo điều kiện cho một hệ sinh thái phần mềm mạnh mẽ và có thể mở rộng trong tương lai.
