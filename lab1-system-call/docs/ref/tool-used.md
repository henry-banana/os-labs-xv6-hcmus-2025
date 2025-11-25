### Yêu cầu chung

Đối với khóa học này, bạn cần sử dụng **phiên bản RISC-V** của các công cụ sau:

- (v7.2 hoặc mới hơn)
- (v8.3 hoặc mới hơn)
- (RISC-V version) và (RISC-V version)

> Nếu bạn gặp khó khăn khi thiết lập, hãy tham dự giờ văn phòng hoặc đăng lên  để được hỗ trợ.
> 

---

### Hướng dẫn theo Hệ điều hành

### 🐧 Debian / Ubuntu

```bash
sudo apt-get install git build-essential gdb-multiarch qemu-system-misc gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu 
```

> Lưu ý: Có thể bạn cần sử dụng Ubuntu 24 (hoặc phiên bản mới hơn) để có QEMU đủ mới.
> 

### 🏹 Arch Linux

```bash
sudo pacman -S riscv64-linux-gnu-binutils riscv64-linux-gnu-gcc riscv64-linux-gnu-gdb qemu-emulators-full bc git base-devel
```

### 💻 Windows

- Khuyến khích: cài **Linux hoặc WSL 2** (Windows Subsystem for Linux 2).
- Gợi ý: Cài để thay cho PowerShell/Command Prompt.
    
    ```bash
    $ sudo apt-get update && sudo apt-get upgrade
    $ sudo apt-get install git build-essential gdb-multiarch qemu-system-misc gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu
    ```
    
- Đảm bảo bạn đang dùng **WSL 2** (không phải WSL 1). Kiểm tra bằng lệnh:
    
    ```bash
    wsl -l -v
    ```
    

### 📦 Sử dụng VM Linux (nếu bản thân OS không thuận tiện)

Nếu OS bạn đang dùng không phù hợp hoặc khó cài các công cụ RISC-V, bạn có thể dùng một máy ảo Linux:

1. Cài một nền tảng ảo hoá (ví dụ: — miễn phí, hỗ trợ Mac/Linux/Windows).
2. Tải file ISO Ubuntu Desktop (64-bit), ví dụ: `ubuntu-20.04.3-desktop-amd64.iso`.
3. Tạo máy ảo, boot bằng ISO, sau đó cài các công cụ RISC-V bên trong máy ảo.

### 🍏 macOS

1. Cài Developer Tools:
    
    ```bash
    xcode-select --install
    
    ```
    
2. Cài :
    
    ```bash
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    
    ```
    
3. Cài RISC-V toolchain:
    
    ```bash
    brew tap riscv/riscv
    brew install riscv-tools
    
    ```
    
    > Lưu ý: Có thể cần chỉnh sửa ~/.bashrc hoặc file shell tương ứng để thêm đường dẫn toolchain vào $PATH.
    > 
4. Cài QEMU:
    
    ```bash
    brew install qemu
    
    ```
    

---

### ✅ Kiểm tra thiết lập

Sau khi cài, hãy kiểm tra phiên bản để xác nhận cài thành công:

- `qemu-system-riscv64 --version` → nên là v7.2 hoặc mới hơn.
- `riscv64-linux-gnu-gcc --version` → (hoặc tương đương) có hỗ trợ RISC-V.

---

**🔥 Lưu ý quan trọng:**

- Đảm bảo phiên bản RISC-V (không phải bản x86 thông thường) của GCC/Binutils/QEMU.
- Nếu bạn dùng Windows, bắt buộc phải dùng **WSL 2** (không hỗ trợ WSL 1).
- Nếu toàn bộ thiết lập quá khó, bạn có thể chuyển sang dùng VM Linux như đề xuất.