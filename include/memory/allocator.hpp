#include <algorithm>
#include <cstddef>
#include <iostream>
#include <new>
#include <unistd.h>

static constexpr int kFreeMemDivisor = 20;

namespace delta_std {
template <typename T> class allocator {
public:
  using ValueType = T;

  using Pointer = T *;

  using ConstPointer = const T *;

  Pointer allocate(std::size_t n) {
    if (n > std::numeric_limits<std::size_t>::max() / sizeof(ValueType)) {
      std::cerr << "allocation size overflows size_t\n";
      throw std::bad_alloc();
    }

    if (n * sizeof(ValueType) > getAvailableMemory()) {
      std::cerr << "allocation exceeds memory limit.\n";
      throw std::bad_alloc();
    }

    std::size_t align_size = alignof(ValueType);

    void *memory =
        ::operator new(n * sizeof(ValueType), std::align_val_t(align_size));

    return static_cast<Pointer>(memory);
  }

  // Deallocate needs to maintain symmetry with the align_val_t 
  void deallocate(ValueType *ptr, std::size_t n) {}

private:
  unsigned long long getAvailableMemory() {
    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGESIZE);

    unsigned long long total_bytes =
        static_cast<unsigned long long>(pages * page_size);

    return total_bytes / kFreeMemDivisor;
  }
};
} // namespace delta_std