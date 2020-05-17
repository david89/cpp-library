#ifndef TYPES_OPTIONAL
#define TYPES_OPTIONAL

#include <exception>
#include <memory>

namespace david {
namespace internal {

template <typename T>
struct optional_rep {
  bool engaged = false;
  // TODO: this doesn't work for non-trivially default constructable objects.
  // Let's use an union here and some traits magic here.
  T obj;
};

}  // namespace internal

struct nullopt_t {
  explicit constexpr nullopt_t(int) {}
};

// TODO: Will there be multiple instances of nullopt in different translation
// units?
static const nullopt_t nullopt{0};

template <typename T>
class optional {
 public:
  // Constructors.
  optional() = default;
  optional(nullopt_t){};

  // Observers.
  explicit operator bool() const noexcept { return rep_.engaged; }
  bool has_value() const noexcept { return rep_.engaged; }

 private:
  internal::optional_rep<T> rep_;
};
}  // namespace david

#endif  // TYPES_OPTIONAL
