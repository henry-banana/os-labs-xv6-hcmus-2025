# Sổ tay Tập lệnh RISC-V: Quyển I - ISA không đặc quyền

**Phiên bản tài liệu 20240411-bản nháp**

**Biên tập viên: Andrew Waterman, Krste Asanović, với sự đóng góp của SiFive, Inc.**

*Tài liệu này được phát hành theo Giấy phép Quốc tế Creative Commons Ghi công 4.0.*

## Lời nói đầu

Tài liệu này, cùng với Quyển II: ISA Đặc quyền, định nghĩa kiến trúc tập lệnh (ISA) của RISC-V. RISC-V (phát âm là “risk-five”) là một ISA dựa trên máy tính tập lệnh rút gọn (RISC) mới, được phát triển ban đầu tại Đại học California, Berkeley, và hiện đang được phát triển bởi một cộng đồng toàn cầu với sự hợp tác của các thành viên của Tổ chức Quốc tế RISC-V.

Kiến trúc tập lệnh RISC-V được thiết kế cẩn thận để hỗ trợ một loạt các mục đích sử dụng, từ các bộ vi điều khiển nhỏ gọn, hiệu quả về năng lượng cho các hệ thống nhúng, đến các bộ xử lý đa lõi quy mô lớn cho các ứng dụng trung tâm dữ liệu và siêu máy tính. ISA được thiết kế để hỗ trợ các triển khai hiệu suất cao, đồng thời cung cấp một cơ sở ổn định cho sự phát triển phần mềm.

Quyển I: ISA Không đặc quyền mô tả ISA cơ sở và các phần mở rộng tiêu chuẩn cho các hoạt động tính toán không đặc quyền. Nó bao gồm các định dạng lệnh, chế độ địa chỉ và các hoạt động số học và logic cơ bản. Tài liệu này dành cho các nhà thiết kế bộ xử lý, nhà phát triển trình biên dịch và hệ điều hành, và bất kỳ ai muốn hiểu các nguyên tắc cơ bản của kiến trúc RISC-V.

Quyển II: ISA Đặc quyền, được mô tả trong một tài liệu riêng biệt, định nghĩa kiến trúc đặc quyền của RISC-V, bao gồm các thanh ghi điều khiển và trạng thái (CSR), các mức đặc quyền, và hệ thống quản lý bộ nhớ. Nó bao gồm các cơ chế cần thiết để hỗ trợ các hệ điều hành hiện đại, chẳng hạn như bộ nhớ ảo và bảo vệ.

Cả hai tập tài liệu đều được phát hành theo giấy phép Creative Commons, cho phép bất kỳ ai cũng có thể sử dụng, phân phối và tạo các tác phẩm phái sinh từ thông số kỹ thuật. Điều này thúc đẩy một hệ sinh thái mở và hợp tác, nơi các nhà nghiên cứu và các nhà công nghiệp có thể cùng nhau đổi mới và mở rộng kiến trúc RISC-V.
