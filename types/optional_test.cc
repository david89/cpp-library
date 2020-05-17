#include "types/optional.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace david {
namespace {

using ::testing::Eq;
using ::testing::Ge;
using ::testing::Gt;
using ::testing::HasSubstr;
using ::testing::IsEmpty;
using ::testing::IsNull;
using ::testing::Le;
using ::testing::Lt;
using ::testing::Ne;
using ::testing::Not;
using ::testing::SizeIs;

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

}  // namespace
}  // namespace david
