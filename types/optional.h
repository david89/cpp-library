#ifndef TYPES_OPTIONAL
#define TYPES_OPTIONAL

#include <exception>
#include <iostream>
#include <memory>

#include "types/internal/enable_copy.h"

namespace david {
namespace internal {

template <typename T>
class optional_storage {
 public:
  optional_storage() = default;

 protected:
  bool engaged_ = false;
  // TODO: this doesn't work for non-trivially default constructable objects.
  // Let's use an union here and some traits magic here.
  T obj_;

  // Destroys the underlying associated object.
  // This method is only used for non-trivially destructible objects.
  void destroy() noexcept {
    if (engaged_) {
      engaged_ = false;
      obj_.~T();
    }
  }
};

template <typename T,
          bool =  // trivial copy
          std::is_trivially_copy_constructible<T>::value&&
              std::is_trivially_copy_assignable<T>::value>
class optional_base;

// Trivially copy constructible types shouldn't have a copy constructor,
// otherwise it's no longer a trivial constructor.
template <typename T>
class optional_base<T, true> : public optional_storage<T> {
 public:
  optional_base() noexcept = default;
};

template <typename T>
class optional_base<T, false> : public optional_storage<T> {
 public:
  optional_base() noexcept = default;
  optional_base(const optional_base& other) {
    this->engaged_ = other.engaged_;
    if (other.engaged_) {
      this->obj_ = other.obj_;
    }
  }
  optional_base& operator=(const optional_base& other) {
    if (other.engaged_) {
      this->obj_ = other.obj_;
    } else {
      this->destroy();
    }
  }
};

}  // namespace internal

struct nullopt_t {
  explicit constexpr nullopt_t(int) {}
};

// TODO: Will there be multiple instances of nullopt in different translation
// units?
static const nullopt_t nullopt{0};

// Interface from https://en.cppreference.com/w/cpp/utility/optional
template <typename T>
class optional
    : private internal::optional_base<T>,
      private internal::enable_copy<std::is_copy_constructible<T>::value,
                                    std::is_copy_assignable<T>::value> {
 public:
  // Constructors.
  optional() noexcept = default;
  optional(nullopt_t) noexcept {};

  // Observers.
  explicit operator bool() const noexcept { return this->engaged_; }
  bool has_value() const noexcept { return this->engaged_; }
};
}  // namespace david

#endif  // TYPES_OPTIONAL
