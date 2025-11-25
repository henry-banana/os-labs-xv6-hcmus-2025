
# Tóm tắt sách xv6: một hệ điều hành dạy học đơn giản, giống Unix (Phương pháp 80/20)

## Giới thiệu

Cuốn sách này giải thích các khái niệm cốt lõi của hệ điều hành thông qua việc nghiên cứu mã nguồn của xv6, một hệ điều hành dạy học dựa trên Unix phiên bản 6. Tóm tắt này áp dụng nguyên tắc 80/20, tập trung vào 20% các khái niệm quan trọng nhất chiếm 80% sự hiểu biết về hệ điều hành.

## Chương 1: Giao diện Hệ điều hành

*   **Vai trò của Hệ điều hành (HĐH):** HĐH quản lý tài nguyên phần cứng, trừu tượng hóa chúng cho các ứng dụng, và cung cấp một môi trường để nhiều chương trình chạy đồng thời và tương tác.
*   **Hạt nhân (Kernel) và Lời gọi Hệ thống (System Calls):** Hạt nhân là lõi của HĐH, cung cấp các dịch vụ cho các chương trình thông qua các lời gọi hệ thống. Các tiến trình chuyển từ *không gian người dùng* sang *không gian hạt nhân* để thực hiện các lời gọi hệ thống.
*   **Các khái niệm chính của Unix:**
    *   **Tiến trình:** Một chương trình đang thực thi với không gian bộ nhớ riêng của nó.
    *   **Hệ thống tệp:** Một cấu trúc cây phân cấp của các tệp và thư mục, bắt đầu từ thư mục gốc (`/`).
    *   **Mô tả tệp (File Descriptors):** Các số nguyên nhỏ đại diện cho các tài nguyên mà một tiến trình có thể đọc hoặc ghi (ví dụ: tệp, ống dẫn, giao diện điều khiển).
    *   **Ống dẫn (Pipes):** Một cơ chế giao tiếp giữa các tiến trình, cho phép đầu ra của một tiến trình được chuyển hướng đến đầu vào của một tiến trình khác.

## Chương 2: Tổ chức Hệ điều hành

*   **Mục tiêu của HĐH:** Ghép kênh (chia sẻ tài nguyên), cô lập (bảo vệ các tiến trình khỏi nhau), và tương tác (cho phép các tiến trình giao tiếp).
*   **Chế độ Người dùng và Chế độ Giám sát:** Phần cứng cung cấp các chế độ đặc quyền để thực thi sự cô lập. Mã người dùng chạy ở chế độ người dùng, trong khi mã hạt nhân chạy ở chế độ giám sát (đặc quyền).
*   **Hạt nhân Nguyên khối (Monolithic Kernel) và Vi hạt nhân (Microkernel):**
    *   **Nguyên khối:** Toàn bộ HĐH chạy trong không gian hạt nhân. Đơn giản để xây dựng và hiệu quả, nhưng một lỗi có thể làm sập toàn bộ hệ thống. (xv6 là một hạt nhân nguyên khối).
    *   **Vi hạt nhân:** Chỉ một phần nhỏ của HĐH chạy trong không gian hạt nhân, với hầu hết các dịch vụ chạy dưới dạng các tiến trình người dùng. An toàn hơn nhưng có thể kém hiệu quả hơn.
*   **Tổng quan về Tiến trình xv6:** Mỗi tiến trình có không gian địa chỉ riêng, một luồng thực thi, và hai ngăn xếp: một ngăn xếp người dùng và một ngăn xếp hạt nhân.

## Chương 3: Bảng trang

*   **Bộ nhớ ảo:** HĐH cung cấp cho mỗi tiến trình ảo tưởng về việc có không gian bộ nhớ riêng, liền kề của nó, được gọi là *không gian địa chỉ ảo*. Phần cứng dịch các địa chỉ ảo này thành các *địa chỉ vật lý* trong RAM.
*   **Bảng trang (Page Tables):** Phần cứng sử dụng các bảng trang để dịch các địa chỉ ảo thành các địa chỉ vật lý. Các bảng trang cho phép HĐH cô lập các không gian bộ nhớ của các tiến trình và thực hiện các thủ thuật như ánh xạ cùng một bộ nhớ vào nhiều không gian địa chỉ.
*   **Phân trang theo yêu cầu và Sao chép khi ghi (Copy-on-Write):** Các kỹ thuật bộ nhớ ảo nâng cao cho phép HĐH trì hoãn việc phân bổ bộ nhớ cho đến khi nó thực sự cần thiết, và chia sẻ bộ nhớ giữa các tiến trình cho đến khi một trong số chúng sửa đổi nó. (xv6 không triển khai các tính năng này, nhưng chúng là những khái niệm quan trọng trong các HĐH hiện đại).

## Chương 4: Bẫy và Lời gọi Hệ thống

*   **Bẫy (Traps):** Các sự kiện khiến CPU chuyển quyền điều khiển sang hạt nhân. Có ba loại bẫy: lời gọi hệ thống, ngoại lệ (ví dụ: chia cho 0), và ngắt thiết bị.
*   **Xử lý Bẫy:** Khi một bẫy xảy ra, phần cứng lưu trạng thái của tiến trình hiện tại và chuyển quyền điều khiển cho một trình xử lý bẫy trong hạt nhân. Hạt nhân xử lý sự kiện và sau đó tiếp tục tiến trình bị ngắt.
*   **Lời gọi Hệ thống trong xv6:** Các tiến trình người dùng gọi các lời gọi hệ thống bằng cách sử dụng lệnh `ecall`, lệnh này bẫy vào hạt nhân. Hạt nhân thực thi hàm lời gọi hệ thống tương ứng và trả về một kết quả cho tiến trình người dùng.

## Chương 5: Ngắt và Trình điều khiển Thiết bị

*   **Trình điều khiển Thiết bị:** Mã trong hạt nhân quản lý một thiết bị phần cứng cụ thể. Các trình điều khiển xử lý các ngắt từ các thiết bị và cung cấp một giao diện trừu tượng cho phần còn lại của hạt nhân.
*   **Ngắt (Interrupts):** Các tín hiệu từ các thiết bị phần cứng cho biết rằng chúng cần sự chú ý. Các ngắt là một loại bẫy, và được xử lý bởi các trình xử lý ngắt trong hạt nhân.
*   **Đồng thời trong các Trình điều khiển:** Các trình điều khiển phải được thiết kế cẩn thận để xử lý sự đồng thời, vì chúng có thể bị ngắt bởi các ngắt từ các thiết bị mà chúng đang quản lý.

## Chương 6: Khóa

*   **Đồng thời và Tranh chấp (Races):** Khi nhiều luồng thực thi truy cập dữ liệu được chia sẻ đồng thời, có thể xảy ra các tranh chấp, dẫn đến kết quả không chính xác. *Các khóa* được sử dụng để ngăn chặn các tranh chấp bằng cách đảm bảo *loại trừ lẫn nhau* (chỉ một luồng tại một thời điểm có thể giữ một khóa).
*   **Khóa quay (Spinlocks) và Khóa ngủ (Sleep-locks):**
    *   **Khóa quay:** Các khóa trong đó một luồng chờ trong một vòng lặp bận rộn ("quay") cho đến khi khóa có sẵn. Hiệu quả cho các phần quan trọng ngắn.
    *   **Khóa ngủ:** Các khóa trong đó một luồng nhường CPU và đi ngủ cho đến khi khóa có sẵn. Tốt hơn cho các phần quan trọng dài.
*   **Khóa chết (Deadlock):** Một tình huống trong đó hai hoặc nhiều luồng đang chờ nhau để giải phóng các khóa, dẫn đến không có tiến trình nào có thể tiến triển. Các khóa chết có thể được tránh bằng cách thiết lập một thứ tự khóa toàn cục.

## Chương 7: Lập lịch

*   **Lập lịch (Scheduling):** HĐH quyết định tiến trình nào sẽ chạy trên CPU tại bất kỳ thời điểm nào. Mục tiêu là chia sẻ CPU một cách công bằng và hiệu quả giữa tất cả các tiến trình.
*   **Chuyển đổi Ngữ cảnh (Context Switching):** Hành động lưu trạng thái của một tiến trình và khôi phục trạng thái của một tiến trình khác để chuyển CPU từ một tiến trình này sang một tiến trình khác.
*   **Ngủ và Đánh thức (Sleep and Wakeup):** Một cơ chế cho phép một tiến trình đợi một sự kiện xảy ra. Một tiến trình có thể gọi `sleep` để nhường CPU, và một tiến trình khác có thể gọi `wakeup` để đánh thức nó khi sự kiện đã xảy ra.

## Chương 8: Hệ thống tệp

*   **Lớp Hệ thống tệp:** Hệ thống tệp xv6 được tổ chức thành nhiều lớp, bao gồm lớp đĩa, lớp bộ đệm cache, lớp ghi nhật ký, lớp inode, lớp thư mục, và lớp tên đường dẫn.
*   **Ghi nhật ký (Logging):** Để đảm bảo tính nhất quán của hệ thống tệp khi có sự cố, xv6 sử dụng một hệ thống ghi nhật ký. Các thay đổi đối với hệ thống tệp trước tiên được ghi vào một nhật ký trên đĩa. Sau khi các thay đổi đã được cam kết vào nhật ký, chúng được ghi vào các cấu trúc dữ liệu của hệ thống tệp. Điều này đảm bảo rằng các hoạt động của hệ thống tệp là nguyên tử đối với các sự cố.
*   **Inode:** Mỗi tệp và thư mục được biểu diễn bằng một inode, chứa siêu dữ liệu về tệp (ví dụ: loại, kích thước, số khối dữ liệu).

## Chương 9: Đồng thời được xem xét lại

Chương này xem xét lại các thách thức của sự đồng thời trong xv6 và thảo luận về các mẫu khóa và các kỹ thuật giống như khóa được sử dụng để giải quyết chúng. Nó nhấn mạnh sự phức tạp của việc lập trình đồng thời và sự cần thiết phải suy nghĩ cẩn thận về các bất biến và các tranh chấp tiềm ẩn.

## Chương 10: Tóm tắt

Cuốn sách kết thúc bằng cách tóm tắt các ý tưởng chính được trình bày và nhấn mạnh rằng mặc dù xv6 là một hệ điều hành đơn giản, các khái niệm mà nó minh họa là nền tảng cho tất cả các hệ điều hành hiện đại.
