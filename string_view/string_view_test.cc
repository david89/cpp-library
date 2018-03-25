#include "string_view.h"

#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace dagomez {
namespace {

using ::testing::Eq;
using ::testing::Gt;
using ::testing::IsNull;
using ::testing::Lt;
using ::testing::Not;
using ::testing::SizeIs;

std::string join_strs_by_null(
    std::string a, std::string b) {
  a.push_back('\0');
  a.append(std::move(b));
  return a;
}

TEST(StringView, DefaultConstructor) {
  const string_view s;
  EXPECT_THAT(s.data(), IsNull());
  EXPECT_THAT(s.size(), Eq(0));
}

TEST(StringView, DataConstructor) {
  const char data[] = "hello";
  const string_view s(data);
  EXPECT_THAT(s.data(), Eq(data));
  EXPECT_THAT(s.size(), Eq(5));
}

TEST(StringView, DataAndLengthConstructor) {
  const char data[] = "hello";
  const string_view s(data, 3);
  EXPECT_THAT(s.data(), Eq(data));
  EXPECT_THAT(s.size(), Eq(3));
}

TEST(StringView, StringConstructor) {
  const std::string data = join_strs_by_null("hello", "world");
  const string_view s(data);
  EXPECT_THAT(s.data(), Eq(data.data()));
  EXPECT_THAT(s.size(), Eq(11));
}

TEST(StringView, AssignmentConstructor) {
  const char data[] = "hello";
  const string_view s = data;
  EXPECT_THAT(s.data(), Eq(data));
  EXPECT_THAT(s.size(), Eq(5));
}

TEST(StringView, ReferenceConstructor) {
  const string_view t = "hello";
  const string_view s(t);
  EXPECT_THAT(s.data(), Eq(t.data()));
  EXPECT_THAT(s.size(), Eq(t.size()));
}

TEST(StringView, Iterators) {
  const string_view s = "hello";
  EXPECT_THAT(*s.begin(), Eq('h'));
  EXPECT_THAT(*(s.end() - 1), Eq('o'));
  EXPECT_THAT(*s.cbegin(), Eq('h'));
  EXPECT_THAT(*(s.cend() - 1), Eq('o'));
  EXPECT_THAT(*s.rbegin(), Eq('o'));
  EXPECT_THAT(*(s.rend() - 1), Eq('h'));
  EXPECT_THAT(*s.crbegin(), Eq('o'));
  EXPECT_THAT(*(s.crend() - 1), Eq('h'));
}

TEST(StringView, SizeRelatedChecks) {
  string_view s = "hello";
  EXPECT_THAT(s, SizeIs(5));
  EXPECT_THAT(s.size(), Eq(s.length()));
  EXPECT_FALSE(s.empty());
  s = "";
  EXPECT_THAT(s, SizeIs(0));
  EXPECT_THAT(s.size(), Eq(s.length()));
  EXPECT_TRUE(s.empty());
}

TEST(StringView, AccessRelatedChecks) {
  const string_view s = "hello world!";
  EXPECT_THAT(s[0], Eq('h'));
  EXPECT_THAT(s.at(0), Eq('h'));
  EXPECT_THAT(s[s.size() - 1], Eq('!'));
  EXPECT_THAT(s.at(s.size() - 1), Eq('!'));
  EXPECT_THAT(s.front(), Eq('h'));
  EXPECT_THAT(s.back(), Eq('!'));
}

TEST(StringView, RemovePrefix) {
  const char data[] = "hello world!";
  constexpr string_view::size_type offset = 6;  /* size of "hello " */
  constexpr string_view::size_type remaining_offset = 6;  /* size of "world!" */
  string_view s = data;
  s.remove_prefix(offset);
  EXPECT_THAT(s, SizeIs(remaining_offset));
  EXPECT_THAT(s.data(), Eq(data + offset));
  s.remove_prefix(remaining_offset);
  EXPECT_THAT(s, SizeIs(0));
  EXPECT_THAT(s.data(), Eq(data + offset + remaining_offset));
}

TEST(StringView, RemoveSuffix) {
  const char data[] = "hola mundo!";
  constexpr string_view::size_type offset = 6;  /* size of "mundo!" */
  constexpr string_view::size_type remaining_offset = 5;  /* size of "¡hola " */
  string_view s = data;
  s.remove_suffix(offset);
  EXPECT_THAT(s, SizeIs(remaining_offset));
  EXPECT_THAT(s.data(), Eq(data));
  s.remove_suffix(remaining_offset);
  EXPECT_THAT(s, SizeIs(0));
  EXPECT_THAT(s.data(), Eq(data));
}

TEST(StringView, RemovePrefixAndSuffix) {
  const char data[] = "hello mundo!";
  constexpr string_view::size_type offset = 6;  /* size of "mundo!" */
  constexpr string_view::size_type remaining_offset = 6;  /* size of "hello " */
  string_view s = data;
  s.remove_suffix(offset);
  EXPECT_THAT(s, SizeIs(remaining_offset));
  EXPECT_THAT(s.data(), Eq(data));
  s.remove_prefix(remaining_offset);
  EXPECT_THAT(s, SizeIs(0));
  EXPECT_THAT(s.data(), Eq(data + remaining_offset));
}

TEST(StringView, Swap) {
  const char data_s[] = "hola";
  const char data_t[] = "mundo";
  string_view s = data_s;
  string_view t = data_t;
  s.swap(t);
  EXPECT_THAT(s.data(), Eq(data_t));
  EXPECT_THAT(s, SizeIs(5));
  EXPECT_THAT(t.data(), Eq(data_s));
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
  EXPECT_THAT(s.copy(data, 4), Eq(4));
  EXPECT_THAT(strcmp(data, "hola"), Eq(0));
  // Copy a substring.
  std::fill(std::begin(data), std::end(data), 0);
  EXPECT_THAT(s.copy(data, 4, 2), Eq(2));
  EXPECT_THAT(strcmp(data, "la"), Eq(0));
  // Copy with a count bigger than the string's length.
  std::fill(std::begin(data), std::end(data), 0);
  EXPECT_THAT(s.copy(data, 5), Eq(4));
  EXPECT_THAT(strcmp(data, "hola"), Eq(0));
  // Copy an empty string.
  std::fill(std::begin(data), std::end(data), 0);
  EXPECT_THAT(s.copy(data, 4, 4), Eq(0));
  EXPECT_THAT(strcmp(data, ""), Eq(0));
}

TEST(StringView, Substr) {
  const string_view s = "string view test";
  EXPECT_THAT(s.substr(), Eq("string view test"));
  EXPECT_THAT(s.substr(2), Eq("ring view test"));
  EXPECT_THAT(s.substr(2, 3), Eq("rin"));
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
  EXPECT_THAT(s.compare(string_view("hello")), Eq(0));
  EXPECT_THAT(s.compare("hello"), Eq(0));
}

TEST(StringView, CompareSubstringEqualTo) {
  const string_view s = "abc";
  EXPECT_THAT(s.compare(1, 2, string_view("bc")), Eq(0));
  EXPECT_THAT(s.compare(0, 2, string_view("ab")), Eq(0));
}

TEST(StringView, CompareSubstringEqualToSubstring) {
  const string_view s = "linux";
  EXPECT_THAT(s.compare(2, 2, string_view("gnu"), 1, 2), Eq(0));
  EXPECT_THAT(s.compare(0, 2, string_view("linus"), 0, 2), Eq(0));
}

TEST(StringView, FindPosTooLarge) {
  const string_view s = "pattern here";
  EXPECT_THAT(s.find(string_view("pattern"), 30), Eq(string_view::npos));
  EXPECT_THAT(s.find('p', 30), Eq(string_view::npos));
  EXPECT_THAT(s.find("pattern", 30, 7), Eq(string_view::npos));
  EXPECT_THAT(s.find("pattern", 30), Eq(string_view::npos));
}

TEST(StringView, FindPatternTooLarge) {
  const string_view s = "pattern here";
  EXPECT_THAT(s.find(string_view("pattern here and there")),
              Eq(string_view::npos));
  EXPECT_THAT(s.find(string_view("pattern here"), 1), Eq(string_view::npos));
  EXPECT_THAT(s.find("pattern here", 1), Eq(string_view::npos));
  EXPECT_THAT(s.find("pattern here", 1, 12), Eq(string_view::npos));
}

TEST(StringView, FindNotFound) {
  const string_view s = "pattern here";
  EXPECT_THAT(s.find(string_view("Pattern here")), Eq(string_view::npos));
  EXPECT_THAT(s.find('z'), Eq(string_view::npos));
  EXPECT_THAT(s.find('p', 1), Eq(string_view::npos));
  EXPECT_THAT(s.find("Pattern"), Eq(string_view::npos));
  EXPECT_THAT(s.find("attern", 2), Eq(string_view::npos));
  EXPECT_THAT(s.find("ttern", 3, 5), Eq(string_view::npos));
}

TEST(StringView, FindWithEmptyStrings) {
  const string_view empty;
  EXPECT_THAT(empty.find('a'), Eq(string_view::npos));
  EXPECT_THAT(empty.find('a', 1), Eq(string_view::npos));
  EXPECT_THAT(empty.find(""), Eq(0));
  EXPECT_THAT(empty.find("", 1), Eq(string_view::npos));

  const string_view s = "pattern";
  EXPECT_THAT(s.find(""), Eq(0));
  EXPECT_THAT(s.find("", 1), Eq(1));
}

TEST(StringView, FindSuccessful) {
  const string_view s = "pattern here pattern there, the end";
  EXPECT_THAT(s.find(string_view("pattern")), Eq(0));
  EXPECT_THAT(s.find('a'), Eq(1));
  EXPECT_THAT(s.find('a', 2), Eq(14));
  EXPECT_THAT(s.find("pattern", 1), Eq(13));
  EXPECT_THAT(s.find("end", 1, 3), Eq(s.size() - 3));
  EXPECT_THAT(s.find("end is near", 1, 3), Eq(s.size() - 3));
  EXPECT_THAT(s.find("", s.size()), Eq(s.size()));
}

TEST(StringView, RfindNotFound) {
  const string_view s = "pattern here";
  EXPECT_THAT(s.rfind(string_view("pattern herE")), Eq(string_view::npos));
  EXPECT_THAT(s.rfind('q'), Eq(string_view::npos));
  EXPECT_THAT(s.rfind('a', 0), Eq(string_view::npos));
  EXPECT_THAT(s.rfind("patterN"), Eq(string_view::npos));
  EXPECT_THAT(s.rfind("Attern", 1), Eq(string_view::npos));
  EXPECT_THAT(s.rfind("ttern", 1, 5), Eq(string_view::npos));
}

TEST(StringView, RfindWithEmptyStrings) {
  const string_view empty;
  EXPECT_THAT(empty.rfind('a'), Eq(string_view::npos));
  EXPECT_THAT(empty.rfind('a', 1), Eq(string_view::npos));
  EXPECT_THAT(empty.rfind(""), Eq(0));
  EXPECT_THAT(empty.rfind("", 1), Eq(0));

  const string_view s = "pattern";
  EXPECT_THAT(s.rfind(""), Eq(s.size()));
  EXPECT_THAT(s.rfind("", s.size() - 2), Eq(s.size() - 2));
}

TEST(StringView, RfindSuccessful) {
  const string_view s = "pattern here pattern there, the end";
  EXPECT_THAT(s.rfind(string_view("pattern")), Eq(13));
  EXPECT_THAT(s.rfind('a'), Eq(14));
  EXPECT_THAT(s.rfind('a', 2), Eq(1));
  EXPECT_THAT(s.rfind("pattern", 12), Eq(0));
  EXPECT_THAT(s.rfind("end", s.size() - 2, 3), Eq(s.size() - 3));
  EXPECT_THAT(s.rfind("end is near", s.size() - 2, 3), Eq(s.size() - 3));
}

TEST(StringView, FindFirstOfNotFound) {
  const string_view s = "pattern";
  EXPECT_THAT(s.find_first_of(string_view("xyz")), Eq(string_view::npos));
  EXPECT_THAT(s.find_first_of(string_view("n"), 7), Eq(string_view::npos));
  EXPECT_THAT(s.find_first_of('z'), Eq(string_view::npos));
  EXPECT_THAT(s.find_first_of('n', 7), Eq(string_view::npos));
  EXPECT_THAT(s.find_first_of("aer", 6, 3), Eq(string_view::npos));
  EXPECT_THAT(s.find_first_of("aer", 6), Eq(string_view::npos));
}

TEST(StringView, FindFirstOfSuccessful) {
  const string_view s = "pattern";
  EXPECT_THAT(s.find_first_of(string_view("ate")), Eq(1));
  EXPECT_THAT(s.find_first_of(string_view("e"), 2), Eq(4));
  EXPECT_THAT(s.find_first_of('a'), Eq(1));
  EXPECT_THAT(s.find_first_of('n', 2), Eq(6));
  EXPECT_THAT(s.find_first_of("rnt", 2, 2), Eq(5));
  EXPECT_THAT(s.find_first_of("rnt", 2), Eq(2));
}

TEST(StringView, FindLastOfNotFound) {
  const string_view s = "pattern";
  EXPECT_THAT(s.find_last_of(string_view("xyz")), Eq(string_view::npos));
  EXPECT_THAT(s.find_last_of(string_view("n"), 5), Eq(string_view::npos));
  EXPECT_THAT(s.find_last_of('z'), Eq(string_view::npos));
  EXPECT_THAT(s.find_last_of('n', 5), Eq(string_view::npos));
  EXPECT_THAT(s.find_last_of("tea", 1, 2), Eq(string_view::npos));
  EXPECT_THAT(s.find_last_of("te", 1), Eq(string_view::npos));

  const string_view empty;
  EXPECT_THAT(empty.find_last_of(string_view("")), Eq(string_view::npos));
}

TEST(StringView, FindLastOfSuccessful) {
  const string_view s = "pattern";
  EXPECT_THAT(s.find_last_of(string_view("ate")), Eq(4));
  EXPECT_THAT(s.find_last_of(string_view("p"), 2), Eq(0));
  EXPECT_THAT(s.find_last_of('t'), Eq(3));
  EXPECT_THAT(s.find_last_of('t', 2), Eq(2));
  EXPECT_THAT(s.find_last_of("pa", 1, 1), Eq(0));
  EXPECT_THAT(s.find_last_of("rnt", 2), Eq(2));
}

TEST(StringView, EqualOpDifferentSizes) {
  string_view a = "hello";
  string_view b = "hola";
  EXPECT_THAT(a, Not(Eq(b)));
}

}  // namespace
}  // namespace dagomez
