#ifndef TYPES_OPTIONAL
#define TYPES_OPTIONAL

#include <exception>
#include <iostream>
#include <memory>

namespace david {
namespace internal {

template <typename T>
class optional_rep {
 public:
  optional_rep() = default;

 protected:
  bool engaged_ = false;
  // TODO: this doesn't work for non-trivially default constructable objects.
  // Let's use an union here and some traits magic here.
  T obj_;
};

template <typename T,
          bool copy_constructible = std::is_copy_constructible<T>::value,
          bool trivially_copy_constructible =
              std::is_trivially_copy_constructible<T>::value>
class optional_constructors;

// Trivially copy constructible types shouldn't have a copy constructor,
// otherwise it's no longer a trivial constructor.
template <typename T>
class optional_constructors<T, true, true> : public optional_rep<T> {
 public:
};

template <typename T>
class optional_constructors<T, true, false> : public optional_rep<T> {
 public:
  optional_constructors() noexcept = default;
  optional_constructors(const optional_constructors& other) {
    this->engaged_ = other.engaged_;
    if (other.engaged_) {
      this->obj_ = other.obj_;
    }
  }
};

// Non copy constructible types cannot be instantiated.
template <typename T>
class optional_constructors<T, false, true> : public optional_rep<T> {
 public:
  optional_constructors() noexcept = default;
  optional_constructors(const optional_constructors&) = delete;
};

template <typename T>
class optional_constructors<T, false, false> : public optional_rep<T> {
 public:
  optional_constructors() noexcept = default;
  optional_constructors(const optional_constructors&) = delete;
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
class optional : public internal::optional_constructors<T> {
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
