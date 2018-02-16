#ifndef LIBRARY_STRING_VIEW
#define LIBRARY_STRING_VIEW

#include <cassert>
#include <cstddef>

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <string>

#include <iostream>

namespace dagomez {

class string_view {
public:
  // Types.
  using traits_type = std::char_traits<char>;
  using value_type = char;
  using pointer = char*;
  using const_pointer = const char*;
  using reference = char&;
  using const_reference = const char&;
  using const_iterator = const char*;
  using iterator = const_iterator;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using reverse_iterator = const_reverse_iterator;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  static constexpr size_type npos = static_cast<size_type>(-1);

  // Construction and assignment.
  constexpr string_view() noexcept : data_(nullptr), len_(0) {}
  constexpr string_view(const string_view&) noexcept = default;
  string_view& operator=(const string_view&) noexcept = default;
  constexpr string_view(const_pointer str)
      : data_(str), len_(internal_strlen(str)) {}
  constexpr string_view(const_pointer str, size_type len)
      : data_(str), len_(len) {}
  constexpr string_view(const std::string& str)
      : data_(str.data()), len_(str.size()) {}

  // Destruction.
  ~string_view() = default;

  // Iterator support.
  constexpr const_iterator begin() const noexcept { return data_; }
  constexpr const_iterator end() const noexcept { return data_ + len_; }
  constexpr const_iterator cbegin() const noexcept { return begin(); }
  constexpr const_iterator cend() const noexcept { return end(); }
  const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }
  const_reverse_iterator crbegin() const noexcept { return rbegin(); }
  const_reverse_iterator crend() const noexcept { return rend(); }

  // Capacity.
  constexpr size_type size() const noexcept {
    return len_;
  }
  constexpr size_type length() const noexcept {
    return size();
  }
  constexpr size_type max_size() const noexcept;
  constexpr bool empty() const noexcept {
    return len_ == 0;
  }

  // Element access
  constexpr const_reference operator[](size_type pos) const { return at(pos); }
  constexpr const_reference at(size_type pos) const { return data_[pos]; }
  constexpr const_reference front() const { return at(0); }
  constexpr const_reference back() const { return at(len_ - 1); }
  constexpr const_pointer data() const noexcept { return data_; }

  // Modifiers.
  void remove_prefix(size_type n) {
    assert(n <= len_);
    data_ = data_ + n;
    len_ -= n;
  }
  void remove_suffix(size_type n) {
    assert(n <= len_);
    len_ -= n;
  }
  void swap(string_view& s) noexcept {
    string_view other = *this;
    *this = s;
    s = other;
  }

  // Copies the substring [pos, pos + rcount) to the character string pointed to
  // by dest, where rcount is the smaller of count and size() - pos.
  size_type copy(pointer dest, size_type count, size_type pos = 0) const;
  // Returns a view of the substring [pos, pos + rcount), where rcount is the
  // smaller of count and size() - pos.
  string_view substr(size_type pos = 0, size_type count = npos) const;
  // Compares two character sequences.
  // TODO(dagomez): it cannot be marked as constexpr due to memcmp not being
  // constexpr.
  int compare(string_view s) const noexcept;
  // Compare substring(pos1, count1) with s.
  constexpr int compare(size_type pos1, size_type count1, string_view s) const {
    return substr(pos1, count1).compare(s);
  }
  // Compare substring(pos1, count1) with s.substring(pos2, count2).
  constexpr int compare(size_type pos1, size_type count1, string_view s,
                        size_type pos2, size_type count2) const {
    return substr(pos1, count1).compare(s.substr(pos2, count2));
  }
  constexpr int compare(const_pointer s) const {
    return compare(string_view(s));
  }
  constexpr int compare(size_type pos1, size_type count1,
                        const_pointer s) const {
    return substr(pos1, count1).compare(string_view(s));
  }
  constexpr int compare(size_type pos1, size_type count1, const_pointer s,
                        size_type count2) const {
    return substr(pos1, count1).compare(string_view(s, count2));
  }
  size_type find(string_view s, size_type pos = 0) const noexcept;
  size_type find(value_type c, size_type pos = 0) const noexcept {
    return find(string_view(&c, 1), pos);
  }
  constexpr size_type find(const_pointer s, size_type pos, size_type n) const {
    return find(string_view(s, n), pos);
  }
  constexpr size_type find(const_pointer s, size_type pos = 0) const {
    return find(string_view(s), pos);
  }
  size_type rfind(string_view s, size_type pos = npos) const noexcept;

private:
  constexpr static size_type internal_strlen(const_pointer str) {
    return str ? strlen(str) : 0;
  }

  static constexpr size_type kMaxSize = std::numeric_limits<size_type>::max();
  const_pointer data_;
  size_type len_;
};

constexpr inline bool operator==(string_view a, string_view b) noexcept {
  if (a.size() != b.size()) return false;
  if (a.size() <= 0) return true;
  // If they point to the same underlying data and have the same length, then
  // they are the same. Avoid doing a byte by byte comparison.
  if (a.data() == b.data()) return a.size() == b.size();
  return string_view::traits_type::compare(a.data(), b.data(), a.size()) == 0;
}

constexpr inline bool operator!=(string_view a, string_view b) noexcept {
  return !(a == b);
}

}  // dagomez

#endif  // LIBRARY_STRING_VIEW