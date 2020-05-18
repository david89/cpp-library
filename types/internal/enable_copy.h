#ifndef TYPES_INTERNAL_ENABLE_COPY
#define TYPES_INTERNAL_ENABLE_COPY

namespace david {
namespace internal {

template <bool /* copy constructor */, bool /* copy assignment */>
struct enable_copy {};

template <>
struct enable_copy<false, false> {
  enable_copy() noexcept = default;
  enable_copy(const enable_copy&) = delete;
  enable_copy& operator=(const enable_copy&) = delete;
};

template <>
struct enable_copy<false, true> {
  enable_copy() noexcept = default;
  enable_copy(const enable_copy&) = delete;
  enable_copy& operator=(const enable_copy&) = default;
};

template <>
struct enable_copy<true, false> {
  enable_copy() noexcept = default;
  enable_copy(const enable_copy&) = default;
  enable_copy& operator=(const enable_copy&) = delete;
};

// By default enable_copy<false, false> has all disabled.

}  // namespace internal
}  // namespace david

#endif  // TYPES_INTERNAL_ENABLE_COPY
