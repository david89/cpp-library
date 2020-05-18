#ifndef TYPES_INTERNAL_ENABLE_COPY_MOVE
#define TYPES_INTERNAL_ENABLE_COPY_MOVE

namespace david {
namespace internal {

// Unfortunately we cannot keep enable_copy and enable_move as two different
// entities because of cases like this:
// enable_copy<false, false> {
//   disable copy constructor
// };
//
// enable_move<true, true> {
//   enable move constructor
// };
//
// class optional : private enable_copy<...>,
//                  private enable_move<...> {
//   ...
// };
//
// In this case disabling the copy constructor automatically disables the move
// constructor, even if it's enabled in enable_move.

template <bool /* copy constructor */, bool /* copy assignment */,
          bool /* move constructor */, bool /* move assignment */>
struct enable_copy_move {};

template <>
struct enable_copy_move<false, false, false, false> {
  enable_copy_move() noexcept = default;
  enable_copy_move(const enable_copy_move&) = delete;
  enable_copy_move& operator=(const enable_copy_move&) = delete;
  enable_copy_move(enable_copy_move&&) = delete;
  enable_copy_move& operator=(enable_copy_move&&) = delete;
};

template <>
struct enable_copy_move<false, false, false, true> {
  enable_copy_move() noexcept = default;
  enable_copy_move(const enable_copy_move&) = delete;
  enable_copy_move& operator=(const enable_copy_move&) = delete;
  enable_copy_move(enable_copy_move&&) = delete;
  enable_copy_move& operator=(enable_copy_move&&) = default;
};

template <>
struct enable_copy_move<false, false, true, false> {
  enable_copy_move() noexcept = default;
  enable_copy_move(const enable_copy_move&) = delete;
  enable_copy_move& operator=(const enable_copy_move&) = delete;
  enable_copy_move(enable_copy_move&&) = default;
  enable_copy_move& operator=(enable_copy_move&&) = delete;
};

template <>
struct enable_copy_move<false, false, true, true> {
  enable_copy_move() noexcept = default;
  enable_copy_move(const enable_copy_move&) = delete;
  enable_copy_move& operator=(const enable_copy_move&) = delete;
  enable_copy_move(enable_copy_move&&) = default;
  enable_copy_move& operator=(enable_copy_move&&) = default;
};

template <>
struct enable_copy_move<false, true, false, false> {
  enable_copy_move() noexcept = default;
  enable_copy_move(const enable_copy_move&) = delete;
  enable_copy_move& operator=(const enable_copy_move&) = default;
  enable_copy_move(enable_copy_move&&) = delete;
  enable_copy_move& operator=(enable_copy_move&&) = delete;
};

template <>
struct enable_copy_move<false, true, false, true> {
  enable_copy_move() noexcept = default;
  enable_copy_move(const enable_copy_move&) = delete;
  enable_copy_move& operator=(const enable_copy_move&) = default;
  enable_copy_move(enable_copy_move&&) = delete;
  enable_copy_move& operator=(enable_copy_move&&) = default;
};

template <>
struct enable_copy_move<false, true, true, false> {
  enable_copy_move() noexcept = default;
  enable_copy_move(const enable_copy_move&) = delete;
  enable_copy_move& operator=(const enable_copy_move&) = default;
  enable_copy_move(enable_copy_move&&) = default;
  enable_copy_move& operator=(enable_copy_move&&) = delete;
};

template <>
struct enable_copy_move<false, true, true, true> {
  enable_copy_move() noexcept = default;
  enable_copy_move(const enable_copy_move&) = delete;
  enable_copy_move& operator=(const enable_copy_move&) = default;
  enable_copy_move(enable_copy_move&&) = default;
  enable_copy_move& operator=(enable_copy_move&&) = default;
};

// --
template <>
struct enable_copy_move<true, false, false, false> {
  enable_copy_move() noexcept = default;
  enable_copy_move(const enable_copy_move&) = default;
  enable_copy_move& operator=(const enable_copy_move&) = delete;
  enable_copy_move(enable_copy_move&&) = delete;
  enable_copy_move& operator=(enable_copy_move&&) = delete;
};

template <>
struct enable_copy_move<true, false, false, true> {
  enable_copy_move() noexcept = default;
  enable_copy_move(const enable_copy_move&) = default;
  enable_copy_move& operator=(const enable_copy_move&) = delete;
  enable_copy_move(enable_copy_move&&) = delete;
  enable_copy_move& operator=(enable_copy_move&&) = default;
};

template <>
struct enable_copy_move<true, false, true, false> {
  enable_copy_move() noexcept = default;
  enable_copy_move(const enable_copy_move&) = default;
  enable_copy_move& operator=(const enable_copy_move&) = delete;
  enable_copy_move(enable_copy_move&&) = default;
  enable_copy_move& operator=(enable_copy_move&&) = delete;
};

template <>
struct enable_copy_move<true, false, true, true> {
  enable_copy_move() noexcept = default;
  enable_copy_move(const enable_copy_move&) = default;
  enable_copy_move& operator=(const enable_copy_move&) = delete;
  enable_copy_move(enable_copy_move&&) = default;
  enable_copy_move& operator=(enable_copy_move&&) = default;
};

template <>
struct enable_copy_move<true, true, false, false> {
  enable_copy_move() noexcept = default;
  enable_copy_move(const enable_copy_move&) = default;
  enable_copy_move& operator=(const enable_copy_move&) = default;
  enable_copy_move(enable_copy_move&&) = delete;
  enable_copy_move& operator=(enable_copy_move&&) = delete;
};

template <>
struct enable_copy_move<true, true, false, true> {
  enable_copy_move() noexcept = default;
  enable_copy_move(const enable_copy_move&) = default;
  enable_copy_move& operator=(const enable_copy_move&) = default;
  enable_copy_move(enable_copy_move&&) = delete;
  enable_copy_move& operator=(enable_copy_move&&) = default;
};

template <>
struct enable_copy_move<true, true, true, false> {
  enable_copy_move() noexcept = default;
  enable_copy_move(const enable_copy_move&) = default;
  enable_copy_move& operator=(const enable_copy_move&) = default;
  enable_copy_move(enable_copy_move&&) = default;
  enable_copy_move& operator=(enable_copy_move&&) = delete;
};

template <>
struct enable_copy_move<true, true, true, true> {
  enable_copy_move() noexcept = default;
  enable_copy_move(const enable_copy_move&) = default;
  enable_copy_move& operator=(const enable_copy_move&) = default;
  enable_copy_move(enable_copy_move&&) = default;
  enable_copy_move& operator=(enable_copy_move&&) = default;
};

}  // namespace internal
}  // namespace david

#endif  // TYPES_INTERNAL_ENABLE_COPY_MOVE
