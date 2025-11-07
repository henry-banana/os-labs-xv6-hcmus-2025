# Sổ tay Tập lệnh RISC-V: Quyển II - Kiến trúc Đặc quyền

**Phiên bản 20240411**

## Lời nói đầu

Những người đóng góp cho tất cả các phiên bản của thông số kỹ thuật theo thứ tự bảng chữ cái (vui lòng liên hệ với ban biên tập để đề xuất sửa đổi): Krste Asanović, Peter Ashenden, Rimas Avižienis, Jacob Bachmeyer, Allen J. Baum, Jonathan Behrens, Paolo Bonzini, Ruslan Bukin, Christopher Celio, Chuanhua Chang, David Chisnall, Anthony Coulter, Palmer Dabbelt, Monte Dalrymple, Paul Donahue, Greg Favor, Dennis Ferguson, Marc Gauthier, Andy Glew, Gary Guo, Mike Frysinger, John Hauser, David Horner, Olof Johansson, David Kruckemyer, Yunsup Lee, Daniel Lustig, Andrew Lutomirski, Prashanth Mundkur, Jonathan Neuschäfer, Rishiyur Nikhil, Stefan O’Rear, Albert Ou, John Ousterhout, David Patterson, Dmitri Pavlov, Kade Phillips, Josh Scheid, Colin Schmidt, Michael Taylor, Wesley Terpstra, Matt Thomas, Tommy Thorn, Ray VanDeWalker, Megan Wachs, Steve Wallach, Andrew Waterman, Claire Wolf, và Reinoud Zandijk.

Tài liệu này được phát hành theo Giấy phép Quốc tế Creative Commons Ghi công 4.0.

Tài liệu này là một sản phẩm phái sinh của thông số kỹ thuật đặc quyền RISC-V phiên bản 1.9.1 được phát hành theo giấy phép sau: ©2010-2017 Andrew Waterman, Yunsup Lee, Rimas Avižienis, David Patterson, Krste Asanović. Giấy phép Quốc tế Creative Commons Ghi công 4.0.

## Lời mở đầu cho Phiên bản 20240326

Tài liệu này mô tả kiến trúc đặc quyền của RISC-V. Bản phát hành này, phiên bản 20240213, chứa các phiên bản sau của các mô-đun ISA RISC-V:

| Mô-đun | Phiên bản | Trạng thái |
| --- | --- | --- |
| Machine ISA | 1.13 | Bản nháp |
| Smstateen Extension | 1.0.0 | Đã phê chuẩn |
| Smcsrind/Sscsrind Extension | 1.0.0 | Đã phê chuẩn |
| Smepmp | 1.0.0 | Đã phê chuẩn |
| *Smcntrpmf | 1.0.0 | Đã phê chuẩn |
| Smrnmi Extension | 1.0.0 | Đã phê chuẩn |
| Smcdeleg | 1.13 | Bản nháp |
| Supervisor ISA | 1.0.0 | Đã phê chuẩn |
| Svade Extension | 0.1 | Bản nháp |
| Svnapot Extension | 1.0 | Đã phê chuẩn |
| Svpbmt Extension | 1.0 | Đã phê chuẩn |
| Svinval Extension | 1.0 | Đã phê chuẩn |
| Svadu Extension | 1.0 | Đã phê chuẩn |
| Sstc | 1.0 | Đã phê chuẩn |
| Sscofpmf | 1.0 | Đã phê chuẩn |
| Hypervisor ISA | 1.0 | Đã phê chuẩn |

Những thay đổi sau đã được thực hiện kể từ phiên bản 1.13, mặc dù không hoàn toàn tương thích ngược, nhưng không được dự đoán sẽ gây ra các vấn đề về tính di động của phần mềm trong thực tế:

*   Bao gồm tất cả các phần mở rộng đã được phê chuẩn cho đến tháng 3 năm 2024.

# Chương 1. Giới thiệu

Tài liệu này mô tả kiến trúc đặc quyền của RISC-V, bao gồm tất cả các khía cạnh của hệ thống RISC-V ngoài ISA không đặc quyền, bao gồm các lệnh đặc quyền cũng như các chức năng bổ sung cần thiết để chạy hệ điều hành và gắn các thiết bị bên ngoài.

> *Bình luận về các quyết định thiết kế của chúng tôi được định dạng như trong đoạn này và có thể bỏ qua nếu người đọc chỉ quan tâm đến bản thân thông số kỹ thuật.*
>
> *Chúng tôi lưu ý ngắn gọn rằng toàn bộ thiết kế cấp đặc quyền được mô tả trong tài liệu này có thể được thay thế bằng một thiết kế cấp đặc quyền hoàn toàn khác mà không thay đổi ISA không đặc quyền và có thể thậm chí không thay đổi ABI. Cụ thể, thông số kỹ thuật đặc quyền này được thiết kế để chạy các hệ điều hành phổ biến hiện có, và do đó thể hiện mô hình bảo vệ dựa trên cấp độ thông thường. Các thông số kỹ thuật đặc quyền thay thế có thể thể hiện các mô hình miền bảo vệ linh hoạt hơn khác. Để đơn giản trong cách diễn đạt, văn bản được viết như thể đây là kiến trúc đặc quyền duy nhất có thể có.*

## 1.1. Thuật ngữ về Ngăn xếp Phần mềm Đặc quyền của RISC-V

Phần này mô tả thuật ngữ chúng tôi sử dụng để mô tả các thành phần của một loạt các ngăn xếp phần mềm đặc quyền có thể có cho RISC-V.

Hình 1 cho thấy một số ngăn xếp phần mềm có thể được hỗ trợ bởi kiến trúc RISC-V. Phía bên trái cho thấy một hệ thống đơn giản chỉ hỗ trợ một ứng dụng duy nhất chạy trên môi trường thực thi ứng dụng (AEE). Ứng dụng được lập trình để chạy với một giao diện nhị phân ứng dụng (ABI) cụ thể. ABI bao gồm ISA cấp người dùng được hỗ trợ cộng với một tập hợp các lệnh gọi ABI để tương tác với AEE. ABI ẩn các chi tiết của AEE khỏi ứng dụng để cho phép linh hoạt hơn trong việc triển khai AEE. Cùng một ABI có thể được triển khai nguyên bản trên nhiều hệ điều hành máy chủ khác nhau, hoặc có thể được hỗ trợ bởi một môi trường giả lập chế độ người dùng chạy trên một máy có ISA gốc khác.

> *Quy ước đồ họa của chúng tôi biểu diễn các giao diện trừu tượng bằng các hộp đen với văn bản màu trắng, để tách chúng khỏi các phiên bản cụ thể của các thành phần triển khai các giao diện.*

Cấu hình ở giữa cho thấy một hệ điều hành (OS) thông thường có thể hỗ trợ thực thi đa chương trình của nhiều ứng dụng. Mỗi ứng dụng giao tiếp qua một ABI với HĐH, cung cấp AEE. Giống như các ứng dụng giao diện với AEE thông qua ABI, các hệ điều hành RISC-V giao diện với môi trường thực thi giám sát (SEE) thông qua giao diện nhị phân giám sát (SBI). Một SBI bao gồm ISA cấp người dùng và cấp giám sát cùng với một tập hợp các lệnh gọi hàm SBI. Sử dụng một SBI duy nhất trên tất cả các triển khai SEE cho phép một hình ảnh nhị phân HĐH duy nhất chạy trên bất kỳ SEE nào.
