#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/riscv.h"    // For PGSIZE
#include "kernel/sysinfo.h"  // For struct sysinfo
#include "kernel/syscall.h"  // For SYS_ defines (SYS_getpid, ...)

int failed_tests = 0;
int total_tests = 0;

int check_u64(char* description, uint64 actual, uint64 expected) {
    printf("  -> Kiểm tra: %s\n", description);
    printf("     - Mong đợi: %ld\n", (unsigned long) expected);
    printf("     - Thực tế:   %ld\n", (unsigned long)  actual);
    if (actual == expected) {
        printf("     - Kết quả:  [OK]\n");
        return 1;
    } else {
        printf("     - Kết quả:  [FAIL]\n");
        return 0;
    }
}

// Hàm kiểm tra hai giá trị int
int check_int(char* description, int actual, int expected) {
    printf("  -> Kiểm tra: %s\n", description);
    printf("     - Mong đợi: %ld\n", (unsigned long) expected);
    printf("     - Thực tế:   %ld\n", (unsigned long) actual);
    if (actual == expected) {
        printf("     - Kết quả:  [OK]\n");
        return 1;
    } else {
        printf("     - Kết quả:  [FAIL]\n");
        return 0;
    }
}

// Hàm kiểm tra một điều kiện là đúng
int check_true(char* description, int condition) {
    printf("  -> Kiểm tra: %s\n", description);
    printf("     - Mong đợi: true (khác 0)\n");
    printf("     - Thực tế:   %s\n", condition ? "true" : "false");
    if (condition) {
        printf("     - Kết quả:  [OK]\n");
        return 1;
    } else {
        printf("     - Kết quả:  [FAIL]\n");
        return 0;
    }
}


// Hàm trợ giúp để gọi sysinfo và xử lý lỗi cơ bản
void sinfo(struct sysinfo *info) {
  if (sysinfo(info) < 0) {
    printf("   LỖI NGHIÊM TRỌNG: Lệnh gọi sysinfo() thất bại. Dừng test.\n");
    exit(1); 
  }
}

// Hàm kiểm tra A < B cho uint64
int check_less_than_u64(char* description, uint64 val_a, uint64 val_b) {
    printf("  -> Kiểm tra: %s\n", description);
    printf("     - Mong đợi: Giá trị thực tế < Giá trị ngưỡng \n");
    printf("     - Thực tế:   %ld < %ld\n", (unsigned long) val_a, (unsigned long) val_b);
    if (val_a < val_b) {
        printf("     - Kết quả:  [OK]\n");
        return 1;
    } else {
        printf("     - Kết quả:  [FAIL]\n");
        return 0;
    }
}

// Hàm kiểm tra A xấp xỉ B (trong khoảng sai số) cho uint64
int check_approx_eq_u64(char* description, uint64 actual, uint64 expected, uint64 tolerance) {
    printf("  -> Kiểm tra: %s\n", description);
    uint64 lower_bound = expected - tolerance;
    uint64 upper_bound = expected + tolerance;
    printf("     - Mong đợi:  Trong khoảng [%ld, %ld]\n", (unsigned long) lower_bound, (unsigned long) upper_bound);
    printf("     - Thực tế:   %ld\n", (unsigned long) actual);
    if (actual >= lower_bound && actual <= upper_bound) {
        printf("     - Kết quả:  [OK]\n");
        return 1;
    } else {
        printf("     - Kết quả:  [FAIL]\n");
        return 0;
    }
}

// ==================================================
// ======= sysinfo Tests ============================
// ==================================================

void test_sysinfo_bad_pointer() {
  total_tests++;
  printf("\n\nTest: sysinfo() với con trỏ không hợp lệ...\n");
  int pass = 1;

  if(!check_int("sysinfo(NULL) nên trả về -1", sysinfo(0x0), -1)) 
    pass = 0;
  
  if(!check_int("sysinfo(kernel_addr) nên trả về -1", sysinfo((struct sysinfo *)0x80000000), -1))
  pass = 0;

  if(!pass) failed_tests++;

  printf("... Kết thúc Test con trỏ không hợp lệ: %s\n", pass ? "[THÀNH CÔNG]" : "[THẤT BẠI]");
}

void test_sysinfo_freemem() {
  total_tests++;
  printf("\n\nTest: sysinfo.freemem với sbrk()...\n");
  int pass = 1;

  struct sysinfo info;
  sinfo(&info);
  uint64 initial_free = info.freemem;
  printf("  - Bộ nhớ trống ban đầu: %ld\n", (unsigned long)initial_free);

  char *mem = sbrk(PGSIZE);
  if (!check_true("sbrk(PGSIZE) phải thành công", mem != (char*)-1)) {
    failed_tests++;
    printf("... Kết thúc Test freemem: [THẤT BẠI]\n");
    return;
  }
  
  sinfo(&info);
  uint64 free_after_alloc = info.freemem;
  if (!check_less_than_u64("Bộ nhớ trống phải giảm sau khi cấp phát", free_after_alloc, initial_free)) {
    pass = 0;
  }

  sbrk(-PGSIZE);
  sinfo(&info);
  uint64 final_free = info.freemem;
  printf("  - Bộ nhớ trống cuối cùng: %ld\n", (unsigned long)final_free);
  
  if (!check_approx_eq_u64("Bộ nhớ trống phải quay về gần giá trị ban đầu", final_free, initial_free, 100)) {
    pass = 0;
  }

  if(!pass) failed_tests++;
  printf("... Kết thúc Test freemem: %s\n", pass ? "[THÀNH CÔNG]" : "[THẤT BẠI]");
}

void test_sysinfo_nproc() {
  total_tests++;
  printf("\n\nTest: sysinfo.nproc với fork()...\n");
  int pass = 1;

  struct sysinfo info;
  sinfo(&info);
  uint64 nproc_before = info.nproc;
  printf("  - Số tiến trình ban đầu: %ld\n", (unsigned long)nproc_before);

  int pid = fork();
  if (pid < 0) {
      printf("  -> Kiểm tra: fork() phải thành công\n");
      printf("     - Kết quả: [FAIL] - fork() trả về giá trị âm.\n");
      failed_tests++;
      printf("... Kết thúc Test nproc: [THẤT BẠI]\n");
      return;
  }
  
  if (pid == 0) {
    // === TIẾN TRÌNH CON ===
    // Con chỉ kiểm tra và thoát, không in nhiều để tránh race condition
    sinfo(&info);
    if(info.nproc == nproc_before + 1) {
      exit(0);
    } else {
      exit(1);
    }
  }
  
  // === TIẾN TRÌNH CHA ===
  // Cha đợi con kết thúc rồi mới phân tích kết quả
  int status;
  wait(&status); 
  
  printf("  -> Kiểm tra: fork() đã tạo tiến trình mới thành công.\n");
  printf("     - Kết quả: [OK]\n");

  // Kiểm tra mã thoát của con
  if (!check_int("Tiến trình con phải thoát với mã 0 (báo hiệu nproc đúng)", status, 0)) {
    pass = 0;
  }

  sinfo(&info);
  if(!check_u64("nproc quay về giá trị ban đầu sau khi con thoát", info.nproc, nproc_before)) {
    pass = 0;
  }
  
  if(!pass) {
      failed_tests++;
  }
  printf("... Kết thúc Test nproc: %s\n", pass ? "[THÀNH CÔNG]" : "[THẤT BẠI]");
}

void test_sysinfo_nfile() {
  total_tests++;
  printf("\n\nTest: sysinfo.nopenfiles với open()/close()...\n");
  int pass = 1;
  
  struct sysinfo info;
  sinfo(&info);
  uint64 nfile_before = info.nopenfiles;
  printf("  - Số file đang mở ban đầu: %ld\n", (unsigned long)nfile_before);

  int fd = open("README", 0);
  if (!check_true("Mở file README phải thành công", fd >= 0)) {
    failed_tests++;
    printf("... Kết thúc Test nfile: [THẤT BẠI]\n");
    return;
  }
  
  sinfo(&info);
  if(!check_u64("nopenfiles phải tăng lên 1 sau khi open()", info.nopenfiles, nfile_before + 1))
    pass = 0;

  close(fd);
  sinfo(&info);
  
  if(!check_u64("nopenfiles phải quay về ban đầu sau khi close()", info.nopenfiles, nfile_before))
    pass = 0;
  
  if(!pass) failed_tests++;
  printf("... Kết thúc Test nfile: %s\n", pass ? "[THÀNH CÔNG]" : "[THẤT BẠI]");
}

// === trace Tests ===

void test_trace_visual() {
  printf("\n== Bắt đầu Test TRACE (Kiểm tra bằng mắt) ==\n");
  printf("Lưu ý: Các test sau đây yêu cầu bạn tự kiểm tra output.\n\n");

  // 1. Test syscall trace đơn giản
  printf("Test 1: Trace syscall 'getpid' (syscall %ld).\n", (unsigned long)SYS_getpid);
  printf("  -> BẠN NÊN THẤY 1 dòng trace cho 'getpid':\n");
  trace(1 << SYS_getpid);
  getpid();
  trace(0);

  // 2. Test trace mask
  printf("\nTest 2: Trace 'read' (%ld) & 'write' (%ld). KHÔNG trace 'open' (%ld).\n", (unsigned long)SYS_read, (unsigned long)SYS_write, (unsigned long)SYS_open);
  printf("  -> BẠN NÊN THẤY 'write' và 'read', NHƯNG KHÔNG THẤY 'open':\n");
  
  trace((1 << SYS_read) | (1 << SYS_write));
  int fd = open("README", 0); 
  if (fd < 0) {
    printf("Lỗi: không mở được README cho test trace.\n");
  } else {
    char buf[10];
    read(fd, buf, 10);    
    close(fd);
  }
  write(1, "  (Test syscall write)\n", 23); 
  trace(0);

  // 3. Test kế thừa mask qua fork() và trace 'exit'
  printf("\nTest 3: Trace 'fork' (%ld), 'exit' (%ld), 'wait' (%ld).\n", (unsigned long)SYS_fork, (unsigned long)SYS_exit, (unsigned long)SYS_wait);
  printf("  -> BẠN NÊN THẤY 'fork', 'exit' (từ con), và 'wait' (từ cha):\n");

  trace((1 << SYS_fork) | (1 << SYS_exit) | (1 << SYS_wait) | (1 << SYS_getpid));

  int pid = fork(); 
  if (pid == 0) {
    printf("  [Con PID %ld] Đang chạy và sẽ thoát...\n", (unsigned long)getpid());
    exit(0); 
  }
  
  wait(0); 
  printf("  [Cha] Tiến trình con đã thoát.\n");
  trace(0);

  printf("\n== Kết thúc Test TRACE ==\n");
}

// === Main test runner ===

int
main(int argc, char *argv[])
{
  total_tests = 0;
  failed_tests = 0;

  printf("=== BẮT ĐẦU BỘ TEST TOÀN DIỆN CHO LAB 1 ===\n\n");
  
  printf("--- Phần 1: Test sysinfo tự động ---\n");
  test_sysinfo_bad_pointer();
  test_sysinfo_freemem();
  test_sysinfo_nproc();
  test_sysinfo_nfile();
  
  printf("\n--- Phần 2: Test trace bằng mắt ---\n");
  test_trace_visual();
  
  printf("\n--- TỔNG KẾT ---\n");
  printf("Số test tự động đã chạy: %ld\n", (unsigned long)total_tests);
  printf("Số test tự động thất bại: %ld\n", (unsigned long)failed_tests);
  
  if (failed_tests > 0) {
    printf("\nKẾT QUẢ: CÓ TEST TỰ ĐỘNG THẤT BẠI.\n");
  } else {
    printf("\nKẾT QUẢ: TẤT CẢ TEST TỰ ĐỘNG ĐÃ THÀNH CÔNG.\n");
    printf("(Vui lòng kiểm tra kỹ output của phần test trace ở trên!)\n");
  }
  
  exit(0);
}