#include "types/optional.h"

#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace david {
namespace {

TEST(Optional, DefaultConstructor) {
  optional<int> s;
  EXPECT_FALSE(s.has_value());
  EXPECT_FALSE(bool(s));
}

TEST(Optional, NulloptConstructor) {
  optional<int> s(nullopt);
  EXPECT_FALSE(s.has_value());
  EXPECT_FALSE(bool(s));
}

TEST(Optional, IsTriviallyCopyConstructible) {
  optional<int> a;
  optional<int> b(a);
  EXPECT_FALSE(b.has_value());
  EXPECT_FALSE(bool(b));
}

TEST(Optional, IsNonTriviallyCopyConstructible) {
  optional<std::string> a;
  optional<std::string> b(a);
  EXPECT_FALSE(b.has_value());
  EXPECT_FALSE(bool(b));
}

TEST(Optional, IsNonCopyConstructible) {
  struct A {
    A() = default;
    A(A&&) {}
  };
  optional<A> a;
  // This line doesn't compile as expected.
  // optional<A> b(a);
}

}  // namespace
}  // namespace david
