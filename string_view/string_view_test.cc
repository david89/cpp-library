#include "string_view.h"

#include <exception>
#include <sstream>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace dagomez {
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

std::string join_strs_by_null(std::string a, std::string b) {
  a.push_back('\0');
  a.append(std::move(b));
  return a;
}

TEST(StringView, DefaultConstructor) {
  const string_view s;
  EXPECT_THAT(s.data(), IsNull());
  EXPECT_THAT(s, IsEmpty());
}

TEST(StringView, DataConstructor) {
  const char data[] = "hello";
  const string_view s(data);
  EXPECT_EQ(s.data(), data);
  EXPECT_THAT(s, SizeIs(5));
}

TEST(StringView, DataAndLengthConstructor) {
  const char data[] = "hello";
  const string_view s(data, 3);
  EXPECT_EQ(s.data(), data);
  EXPECT_THAT(s, SizeIs(3));
}

TEST(StringView, StringConstructor) {
  const std::string data = join_strs_by_null("hello", "world");
  const string_view s(data);
  EXPECT_EQ(s.data(), data.data());
  EXPECT_THAT(s, SizeIs(11));
}

TEST(StringView, AssignmentConstructor) {
  const char data[] = "hello";
  const string_view s = data;
  EXPECT_EQ(s.data(), data);
  EXPECT_THAT(s, SizeIs(5));
}

TEST(StringView, ReferenceConstructor) {
  const string_view t = "hello";
  const string_view s(t);
  EXPECT_EQ(s.data(), t.data());
  EXPECT_THAT(s, SizeIs(t.size()));
}

TEST(StringView, Iterators) {
  const string_view s = "hello";
  EXPECT_EQ(*s.begin(), 'h');
  EXPECT_EQ(*(s.end() - 1), 'o');
  EXPECT_EQ(*s.cbegin(), 'h');
  EXPECT_EQ(*(s.cend() - 1), 'o');
  EXPECT_EQ(*s.rbegin(), 'o');
  EXPECT_EQ(*(s.rend() - 1), 'h');
  EXPECT_EQ(*s.crbegin(), 'o');
  EXPECT_EQ(*(s.crend() - 1), 'h');
}

TEST(StringView, SizeRelatedChecks) {
  string_view s = "hello";
  EXPECT_THAT(s, SizeIs(5));
  EXPECT_THAT(s, SizeIs(s.length()));
  EXPECT_FALSE(s.empty());
  s = "";
  EXPECT_THAT(s, IsEmpty());
  EXPECT_THAT(s, SizeIs(s.length()));
  EXPECT_TRUE(s.empty());
}

TEST(StringView, AccessRelatedChecks) {
  const string_view s = "hello world!";
  EXPECT_EQ(s[0], 'h');
  EXPECT_EQ(s.at(0), 'h');
  EXPECT_EQ(s[s.size() - 1], '!');
  EXPECT_EQ(s.at(s.size() - 1), '!');
  EXPECT_EQ(s.front(), 'h');
  EXPECT_EQ(s.back(), '!');
}

TEST(StringView, AtFailsWhenOutOfRange) {
  const string_view s = "hello";
  try {
    s.at(100);
    FAIL() << "Expected assertion before";
  } catch (const std::out_of_range& e) {
    EXPECT_THAT(e.what(), HasSubstr("range"));
  } catch (...) {
    FAIL() << "Expected out_of_range exception";
  }
}

TEST(StringView, MaxSize) {
  const string_view s = "hello";
  EXPECT_THAT(s.max_size(), Gt(0));
}

TEST(StringView, RemovePrefix) {
  const char data[] = "hello world!";
  constexpr string_view::size_type offset = 6;           /* size of "hello " */
  constexpr string_view::size_type remaining_offset = 6; /* size of "world!" */
  string_view s = data;
  s.remove_prefix(offset);
  EXPECT_THAT(s, SizeIs(remaining_offset));
  EXPECT_EQ(s.data(), data + offset);
  s.remove_prefix(remaining_offset);
  EXPECT_THAT(s, IsEmpty());
  EXPECT_EQ(s.data(), data + offset + remaining_offset);
}

TEST(StringView, RemoveSuffix) {
  const char data[] = "hola mundo!";
  constexpr string_view::size_type offset = 6;           /* size of "mundo!" */
  constexpr string_view::size_type remaining_offset = 5; /* size of "¡hola " */
  string_view s = data;
  s.remove_suffix(offset);
  EXPECT_THAT(s, SizeIs(remaining_offset));
  EXPECT_EQ(s.data(), data);
  s.remove_suffix(remaining_offset);
  EXPECT_THAT(s, IsEmpty());
  EXPECT_EQ(s.data(), data);
}

TEST(StringView, RemovePrefixAndSuffix) {
  const char data[] = "hello mundo!";
  constexpr string_view::size_type offset = 6;           /* size of "mundo!" */
  constexpr string_view::size_type remaining_offset = 6; /* size of "hello " */
  string_view s = data;
  s.remove_suffix(offset);
  EXPECT_THAT(s, SizeIs(remaining_offset));
  EXPECT_EQ(s.data(), data);
  s.remove_prefix(remaining_offset);
  EXPECT_THAT(s, IsEmpty());
  EXPECT_EQ(s.data(), data + remaining_offset);
}

TEST(StringView, Swap) {
  const char data_s[] = "hola";
  const char data_t[] = "mundo";
  string_view s = data_s;
  string_view t = data_t;
  s.swap(t);
  EXPECT_EQ(s.data(), data_t);
  EXPECT_THAT(s, SizeIs(5));
  EXPECT_EQ(t.data(), data_s);
  EXPECT_THAT(t, SizeIs(4));
  string_view empty;
  t.swap(empty);
  EXPECT_TRUE(t.empty());
}

TEST(StringView, Copy) {
  const string_view s = "hola";
  char data[5];
  // Copy the whole string.
  std::fill(std::begin(data), std::end(data), 0);
  EXPECT_EQ(s.copy(data, 4), 4);
  EXPECT_EQ(strcmp(data, "hola"), 0);
  // Copy a substring.
  std::fill(std::begin(data), std::end(data), 0);
  EXPECT_EQ(s.copy(data, 4, 2), 2);
  EXPECT_EQ(strcmp(data, "la"), 0);
  // Copy with a count bigger than the string's length.
  std::fill(std::begin(data), std::end(data), 0);
  EXPECT_EQ(s.copy(data, 5), 4);
  EXPECT_EQ(strcmp(data, "hola"), 0);
  // Copy an empty string.
  std::fill(std::begin(data), std::end(data), 0);
  EXPECT_EQ(s.copy(data, 4, 4), 0);
  EXPECT_EQ(strcmp(data, ""), 0);
}

TEST(StringView, Substr) {
  const string_view s = "string view test";
  EXPECT_EQ(s.substr(), "string view test");
  EXPECT_EQ(s.substr(2), "ring view test");
  EXPECT_EQ(s.substr(2, 3), "rin");
  EXPECT_TRUE(s.substr(16, 1).empty());
}

TEST(StringView, CompareLessThan) {
  const string_view s = "hello";
  EXPECT_THAT(s.compare(string_view("hillo")), Lt(0));
  EXPECT_THAT(s.compare(string_view("hello there")), Lt(0));

  EXPECT_THAT(s.compare("hola"), Lt(0));
  EXPECT_THAT(s.compare("hello!"), Lt(0));
}

TEST(StringView, CompareSubstringLessThan) {
  const string_view s = "hello";
  // "el" vs "il".
  EXPECT_THAT(s.compare(1, 2, string_view("il")), Lt(0));
  // "llo" vs "llo there".
  EXPECT_THAT(s.compare(2, 3, string_view("llo there")), Lt(0));

  // "hel" vs "hi".
  EXPECT_THAT(s.compare(0, 3, "hi"), Lt(0));
  // "hello" vs "hello there".
  EXPECT_THAT(s.compare(0, 5, "hello there"), Lt(0));
}

TEST(StringView, CompareSubstringLessThanSubstring) {
  const string_view s = "substring";
  // "string" vs "strong wind".
  EXPECT_THAT(s.compare(3, 6, string_view("strong wind"), 0, 6), Lt(0));
  // "ring" vs "ringo".
  EXPECT_THAT(s.compare(5, 4, string_view("ringo starr"), 0, 5), Lt(0));

  // "su" vs "ts".
  EXPECT_THAT(s.compare(0, 2, "ttss", 1, 2), Lt(0));
  // "su" vs "best subaru".
  EXPECT_THAT(s.compare(0, 2, "best subaru", 5, 6), Lt(0));
}

TEST(StringView, CompareGreaterThan) {
  const string_view s = "world";
  EXPECT_THAT(s.compare(string_view("hello")), Gt(0));
  EXPECT_THAT(s.compare(string_view("wo")), Gt(0));

  EXPECT_THAT(s.compare("wonderland"), Gt(0));
  EXPECT_THAT(s.compare(""), Gt(0));
}

TEST(StringView, CompareSubstringGreaterThan) {
  const string_view s = "what a wonderful hello world";
  // " a " vs "   ".
  EXPECT_THAT(s.compare(4, 3, string_view("   ")), Gt(0));
  // "wonderful" vs "wonder".
  EXPECT_THAT(s.compare(7, 9, string_view("wonder")), Gt(0));

  // "hat" vs "has".
  EXPECT_THAT(s.compare(1, 3, "has"), Gt(0));
  // "world" vs "w".
  EXPECT_THAT(s.compare(s.size() - 5, 5, "w"), Gt(0));
}

TEST(StringView, CompareSubstringGreaterThanSubstring) {
  const string_view s = "https://youtu.be/dQw4w9WgXcQ";
  // "youtu.be" vs "youtu.ae".
  EXPECT_THAT(s.compare(8, 8, string_view(" https://youtu.ae"), 1, 16), Gt(0));
  // "https" vs "http".
  EXPECT_THAT(s.compare(0, 5, string_view("the http"), 4, 4), Gt(0));

  // "ht" vs "e".
  EXPECT_THAT(s.compare(0, 2, "hello", 1, 1), Gt(0));
  // "tt" vs "t".
  EXPECT_THAT(s.compare(1, 2, "star", 1, 1), Gt(0));
}

TEST(StringView, CompareNullStrings) {
  const std::string data_s = join_strs_by_null("hello", "world");
  const string_view s(data_s);
  const std::string data_t = join_strs_by_null("hello", "friend");
  EXPECT_THAT(s.compare(string_view(data_t)), Gt(0));
}

TEST(StringView, CompareEqualTo) {
  const string_view s = "hello";
  EXPECT_EQ(s.compare(string_view("hello")), 0);
  EXPECT_EQ(s.compare("hello"), 0);
}

TEST(StringView, CompareSubstringEqualTo) {
  const string_view s = "abc";
  EXPECT_EQ(s.compare(1, 2, string_view("bc")), 0);
  EXPECT_EQ(s.compare(0, 2, string_view("ab")), 0);
}

TEST(StringView, CompareSubstringEqualToSubstring) {
  const string_view s = "linux";
  EXPECT_EQ(s.compare(2, 2, string_view("gnu"), 1, 2), 0);
  EXPECT_EQ(s.compare(0, 2, string_view("linus"), 0, 2), 0);
}

TEST(StringView, StartsWith) {
  const string_view s = "some text";
  EXPECT_TRUE(s.starts_with(string_view("some")));
  EXPECT_TRUE(s.starts_with('s'));
  EXPECT_TRUE(s.starts_with("some"));
  EXPECT_FALSE(s.starts_with(string_view("not")));
  EXPECT_FALSE(s.starts_with('t'));
  EXPECT_FALSE(s.starts_with("not"));
  EXPECT_FALSE(s.starts_with("some text that is too large"));
}

TEST(StringView, EndsWith) {
  const string_view s = "some text";
  EXPECT_TRUE(s.ends_with(string_view("text")));
  EXPECT_TRUE(s.ends_with('t'));
  EXPECT_TRUE(s.ends_with("text"));
  EXPECT_FALSE(s.ends_with(string_view("not")));
  EXPECT_FALSE(s.ends_with('s'));
  EXPECT_FALSE(s.ends_with("not"));
  EXPECT_FALSE(s.ends_with("too large some text"));
}

TEST(StringView, FindPosTooLarge) {
  const string_view s = "pattern here";
  EXPECT_EQ(s.find(string_view("pattern"), 30), string_view::npos);
  EXPECT_EQ(s.find('p', 30), string_view::npos);
  EXPECT_EQ(s.find("pattern", 30, 7), string_view::npos);
  EXPECT_EQ(s.find("pattern", 30), string_view::npos);
}

TEST(StringView, FindPatternTooLarge) {
  const string_view s = "pattern here";
  EXPECT_THAT(s.find(string_view("pattern here and there")),
              Eq(string_view::npos));
  EXPECT_EQ(s.find(string_view("pattern here"), 1), string_view::npos);
  EXPECT_EQ(s.find("pattern here", 1), string_view::npos);
  EXPECT_EQ(s.find("pattern here", 1, 12), string_view::npos);
}

TEST(StringView, FindNotFound) {
  const string_view s = "pattern here";
  EXPECT_EQ(s.find(string_view("Pattern here")), string_view::npos);
  EXPECT_EQ(s.find('z'), string_view::npos);
  EXPECT_EQ(s.find('p', 1), string_view::npos);
  EXPECT_EQ(s.find("Pattern"), string_view::npos);
  EXPECT_EQ(s.find("attern", 2), string_view::npos);
  EXPECT_EQ(s.find("ttern", 3, 5), string_view::npos);
}

TEST(StringView, FindWithEmptyStrings) {
  const string_view empty;
  EXPECT_EQ(empty.find('a'), string_view::npos);
  EXPECT_EQ(empty.find('a', 1), string_view::npos);
  EXPECT_EQ(empty.find(""), 0);
  EXPECT_EQ(empty.find("", 1), string_view::npos);

  const string_view s = "pattern";
  EXPECT_EQ(s.find(""), 0);
  EXPECT_EQ(s.find("", 1), 1);
}

TEST(StringView, FindSuccessful) {
  const string_view s = "pattern here pattern there, the end";
  EXPECT_EQ(s.find(string_view("pattern")), 0);
  EXPECT_EQ(s.find('a'), 1);
  EXPECT_EQ(s.find('a', 2), 14);
  EXPECT_EQ(s.find("pattern", 1), 13);
  EXPECT_EQ(s.find("end", 1, 3), s.size() - 3);
  EXPECT_EQ(s.find("end is near", 1, 3), s.size() - 3);
  EXPECT_EQ(s.find("", s.size()), s.size());
}

TEST(StringView, RfindNotFound) {
  const string_view s = "pattern here";
  EXPECT_EQ(s.rfind(string_view("pattern herE")), string_view::npos);
  EXPECT_EQ(s.rfind('q'), string_view::npos);
  EXPECT_EQ(s.rfind('a', 0), string_view::npos);
  EXPECT_EQ(s.rfind("patterN"), string_view::npos);
  EXPECT_EQ(s.rfind("Attern", 1), string_view::npos);
  EXPECT_EQ(s.rfind("ttern", 1, 5), string_view::npos);
}

TEST(StringView, RfindWithEmptyStrings) {
  const string_view empty;
  EXPECT_EQ(empty.rfind('a'), string_view::npos);
  EXPECT_EQ(empty.rfind('a', 1), string_view::npos);
  EXPECT_EQ(empty.rfind(""), 0);
  EXPECT_EQ(empty.rfind("", 1), 0);

  const string_view s = "pattern";
  EXPECT_EQ(s.rfind(""), s.size());
  EXPECT_EQ(s.rfind("", s.size() - 2), s.size() - 2);
}

TEST(StringView, RfindSuccessful) {
  const string_view s = "pattern here pattern there, the end";
  EXPECT_EQ(s.rfind(string_view("pattern")), 13);
  EXPECT_EQ(s.rfind('a'), 14);
  EXPECT_EQ(s.rfind('a', 2), 1);
  EXPECT_EQ(s.rfind("pattern", 12), 0);
  EXPECT_EQ(s.rfind("end", s.size() - 2, 3), s.size() - 3);
  EXPECT_EQ(s.rfind("end is near", s.size() - 2, 3), s.size() - 3);
}

TEST(StringView, FindFirstOfNotFound) {
  const string_view s = "pattern";
  EXPECT_EQ(s.find_first_of(string_view("xyz")), string_view::npos);
  EXPECT_EQ(s.find_first_of(string_view("n"), 7), string_view::npos);
  EXPECT_EQ(s.find_first_of('z'), string_view::npos);
  EXPECT_EQ(s.find_first_of('n', 7), string_view::npos);
  EXPECT_EQ(s.find_first_of("aer", 6, 3), string_view::npos);
  EXPECT_EQ(s.find_first_of("aer", 6), string_view::npos);
}

TEST(StringView, FindFirstOfSuccessful) {
  const string_view s = "pattern";
  EXPECT_EQ(s.find_first_of(string_view("ate")), 1);
  EXPECT_EQ(s.find_first_of(string_view("e"), 2), 4);
  EXPECT_EQ(s.find_first_of('a'), 1);
  EXPECT_EQ(s.find_first_of('n', 2), 6);
  EXPECT_EQ(s.find_first_of("rnt", 2, 2), 5);
  EXPECT_EQ(s.find_first_of("rnt", 2), 2);
}

TEST(StringView, FindLastOfNotFound) {
  const string_view s = "pattern";
  EXPECT_EQ(s.find_last_of(string_view("xyz")), string_view::npos);
  EXPECT_EQ(s.find_last_of(string_view("n"), 5), string_view::npos);
  EXPECT_EQ(s.find_last_of('z'), string_view::npos);
  EXPECT_EQ(s.find_last_of('n', 5), string_view::npos);
  EXPECT_EQ(s.find_last_of("tea", 1, 2), string_view::npos);
  EXPECT_EQ(s.find_last_of("te", 1), string_view::npos);

  const string_view empty;
  EXPECT_EQ(empty.find_last_of(string_view("")), string_view::npos);
}

TEST(StringView, FindLastOfSuccessful) {
  const string_view s = "pattern";
  EXPECT_EQ(s.find_last_of(string_view("ate")), 4);
  EXPECT_EQ(s.find_last_of(string_view("p"), 2), 0);
  EXPECT_EQ(s.find_last_of('t'), 3);
  EXPECT_EQ(s.find_last_of('t', 2), 2);
  EXPECT_EQ(s.find_last_of("pa", 1, 1), 0);
  EXPECT_EQ(s.find_last_of("rnt", 2), 2);
}

TEST(StringView, FindFirstNotOfNotFound) {
  const string_view s = "pattern";
  EXPECT_THAT(s.find_first_not_of(string_view("patern")),
              Eq(string_view::npos));
  EXPECT_THAT(s.find_first_not_of(string_view("tern"), 2),
              Eq(string_view::npos));
  EXPECT_EQ(s.find_first_not_of('n', 6), string_view::npos);
  EXPECT_EQ(s.find_first_not_of("paternz", 1, 6), string_view::npos);
  EXPECT_EQ(s.find_first_not_of("rn", 5), string_view::npos);

  const string_view empty;
  EXPECT_EQ(empty.find_first_not_of(string_view("")), string_view::npos);
}

TEST(StringView, FindFirstNotOfSuccessful) {
  const string_view s = "pattern";
  EXPECT_EQ(s.find_first_not_of(string_view("ate")), 0);
  EXPECT_EQ(s.find_first_not_of(string_view("t"), 2), 4);
  EXPECT_EQ(s.find_first_not_of('t'), 0);
  EXPECT_EQ(s.find_first_not_of('t', 2), 4);
  EXPECT_EQ(s.find_first_not_of("pa", 1, 1), 1);
  EXPECT_EQ(s.find_first_not_of("rnt", 2), 4);
}

TEST(StringView, FindLastNotOfNotFound) {
  const string_view s = "pattern";
  EXPECT_EQ(s.find_last_not_of(string_view("patern")), string_view::npos);
  EXPECT_EQ(s.find_last_not_of(string_view("pat"), 2), string_view::npos);
  EXPECT_EQ(s.find_last_not_of('p', 0), string_view::npos);
  EXPECT_EQ(s.find_last_not_of("paternz", 6, 6), string_view::npos);
  EXPECT_EQ(s.find_last_not_of("pa", 1), string_view::npos);

  const string_view empty;
  EXPECT_EQ(empty.find_last_not_of(string_view("")), string_view::npos);
}

TEST(StringView, FindLastNotOfSuccessful) {
  const string_view s = "pattern";
  EXPECT_EQ(s.find_last_not_of(string_view("enr")), 3);
  EXPECT_EQ(s.find_last_not_of(string_view("rn"), 6), 4);
  EXPECT_EQ(s.find_last_not_of('t'), 6);
  EXPECT_EQ(s.find_last_not_of('t', 2), 1);
  EXPECT_EQ(s.find_last_not_of("rn", 6, 1), 6);
  EXPECT_EQ(s.find_last_not_of("rnt", 2), 1);
}

TEST(StringView, EqualOpDifferentSizes) {
  const string_view a = "hello";
  // We exercise both the !(a == b) and the a != b paths.
  EXPECT_THAT(a, Not(Eq(string_view("hola"))));
  EXPECT_THAT(a, Ne(string_view("hola")));
}

TEST(StringView, EqualOpSameDataDifferentSizes) {
  const string_view a = "hello";
  EXPECT_THAT(a, Not(Eq(string_view(a.data(), a.size() - 1))));
  EXPECT_THAT(a, Ne(string_view(a.data(), a.size() - 1)));
}

TEST(StringView, EqualOpSameSizeDifferentData) {
  const string_view a = "hello";
  EXPECT_THAT(a, Not(Eq(string_view("hellp"))));
  EXPECT_THAT(a, Ne(string_view("hellp")));
}

TEST(StringView, EqualOp) {
  const string_view a = "hello";
  EXPECT_EQ(a, string_view("hello world", 5));
  EXPECT_EQ(a, string_view("hello"));
  EXPECT_THAT(a, Not(Ne(string_view("hello world", 5))));
  EXPECT_THAT(a, Not(Ne(string_view("hello"))));
}

TEST(StringView, LessThan) {
  const string_view a = "hello";
  // We exercise both the a < b and the !(b < a) paths.
  EXPECT_THAT(a, Lt(string_view("hellp")));
  EXPECT_THAT(a, Lt(string_view("hello world")));
  EXPECT_THAT(string_view("hellp"), Not(Lt(a)));
  EXPECT_THAT(string_view("hello"), Not(Lt(a)));
}

TEST(StringView, GreaterThan) {
  const string_view a = "hellp";
  // We exercise both the a > b and the !(b > a) paths.
  EXPECT_THAT(a, Gt(string_view("hello")));
  EXPECT_THAT(a, Gt(string_view("hell")));
  EXPECT_THAT(string_view("hello"), Not(Gt(a)));
  EXPECT_THAT(string_view("hell"), Not(Gt(a)));
}

TEST(StringView, LessOrEqualThan) {
  const string_view a = "hello";
  EXPECT_THAT(a, Le(string_view("hellp")));
  EXPECT_THAT(a, Le(string_view("hello world")));
  EXPECT_THAT(a, Le(string_view("hello")));
  EXPECT_THAT(string_view("hellp"), Gt(a));
  EXPECT_THAT(string_view("hello world"), Gt(a));
}

TEST(StringView, GreaterOrEqualThan) {
  const string_view a = "hello";
  EXPECT_THAT(a, Ge(string_view("hell")));
  EXPECT_THAT(a, Ge(string_view("a")));
  EXPECT_THAT(a, Ge(string_view("hello")));
  EXPECT_THAT(string_view("hell"), Lt(a));
  EXPECT_THAT(string_view("a"), Lt(a));
}

TEST(StringView, StringOutput) {
  const string_view a = "hello world";
  std::ostringstream oss;
  oss << a;
  EXPECT_EQ(oss.str(), "hello world");

  oss.str("");
  oss << std::setw(20) << std::left << a;
  EXPECT_EQ(oss.str(), "hello world         ");

  oss.str("");
  oss << std::setw(20) << std::right << a;
  EXPECT_EQ(oss.str(), "         hello world");

  oss.str("");
  oss << std::setw(20) << std::setfill('*') << std::left << a;
  EXPECT_EQ(oss.str(), "hello world*********");

  oss.str("");
  oss << std::setw(20) << std::setfill('*') << std::right << a;
  EXPECT_EQ(oss.str(), "*********hello world");

  oss.str("");
  oss << std::setw(20) << std::setfill('*') << std::right << a << a;
  EXPECT_EQ(oss.str(), "*********hello worldhello world");
}

TEST(StringView, Hash) {
  EXPECT_EQ(std::hash<string_view>{}("hello world"),
            std::hash<std::string>{}("hello world"));
}

TEST(StringView, Literals) {
  EXPECT_EQ(string_view("hello world"), "hello world"_sv);
}

}  // namespace
}  // namespace dagomez
