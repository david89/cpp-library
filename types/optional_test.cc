#include "types/optional.h"

#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace david {
namespace {

TEST(Optional, DefaultConstructor) {
  optional<int> s;
  EXPECT_FALSE(s);
  EXPECT_TRUE(std::is_trivially_copy_constructible<optional<int>>::value);
}

TEST(Optional, CopyConstructibleCapabilities) {
  optional<int> s;
  EXPECT_TRUE(std::is_trivially_copy_constructible<optional<int>>::value);
  EXPECT_TRUE(std::is_copy_constructible<optional<int>>::value);
}

TEST(Optional, NulloptConstructor) {
  optional<int> s(nullopt);
  EXPECT_FALSE(s);
}

TEST(Optional, IsTriviallyCopyConstructible) {
  optional<int> a;
  optional<int> b(a);
  EXPECT_FALSE(b);
}

TEST(Optional, IsTriviallyCopyAssignable) {
  optional<int> a;
  optional<int> b = a;
  EXPECT_FALSE(b);
}

TEST(Optional, IsNonTriviallyCopyConstructible) {
  optional<std::string> a;
  optional<std::string> b(a);
  EXPECT_FALSE(b);
}

TEST(Optional, IsNonTriviallyCopyAssignable) {
  optional<std::string> a;
  optional<std::string> b = a;
  EXPECT_FALSE(b);
}

TEST(Optional, IsNonCopyConstructible) {
  optional<std::unique_ptr<int>> a;
  // This line doesn't compile as expected.
  // optional<std::unique_ptr<int>> b(a);
}

TEST(Optional, IsNonCopyAssignable) {
  optional<std::unique_ptr<int>> a;
  // This line doesn't compile as expected.
  // optional<std::unique_ptr<int>> b = a;
}

TEST(Optional, AssignedWithNullopt) {
  optional<int> a = nullopt;
  EXPECT_FALSE(a);
}

TEST(Optional, IsTriviallyMoveConstructible) {
  optional<std::unique_ptr<int>> a;
  optional<std::unique_ptr<int>> b(std::move(a));
  EXPECT_FALSE(b);
}

}  // namespace
}  // namespace david
