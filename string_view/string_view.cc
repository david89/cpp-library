#include "string_view.h"

namespace dagomez {

string_view::size_type string_view::copy(string_view::pointer dest,
                                         string_view::size_type count,
                                         string_view::size_type pos) const {
  if (pos > len_) {
    throw std::out_of_range("Out of range");
  }

  const size_type rcount = std::min(count, len_ - pos);
  if (rcount > 0) {
    std::copy(data_ + pos, data_ + pos + rcount, dest);
  }
  return rcount;
}

string_view
string_view::substr(string_view::size_type pos,
                    string_view::size_type count) const {
  if (pos > len_) {
    throw std::out_of_range("Out of range");
  }

  const size_type rcount = std::min(count, len_ - pos);
  if (rcount > 0) {
    return string_view(data_ + pos, rcount);
  }
  return string_view();
}

int string_view::compare(string_view s) const noexcept {
  const size_type rlen = std::min(len_, s.len_);
  const int comparison = traits_type::compare(data_, s.data_, rlen);
  if (comparison != 0) {
    return comparison;
  }
  if (len_ != s.len_) {
    return len_ < s.len_ ? -1 : 1;
  }
  return 0;
}

string_view::size_type
string_view::find(string_view s, string_view::size_type pos) const noexcept {
  if (empty() && s.empty() && pos == 0) {
    return 0;
  }
  if (pos >= len_ || s.len_ > (len_ - pos)) {
    return npos;
  }
  // We cannot just use strstr, because we may have non-null terminated strings,
  // so we have to implement our own version.
  while (pos + s.len_ <= len_) {
    if (memcmp(data_ + pos, s.data_, s.len_) == 0) {
      return pos;
    }

    pos++;
  }

  return npos;
}

string_view::size_type
string_view::rfind(string_view s, string_view::size_type pos) const noexcept {
  return npos;
}

constexpr string_view::size_type string_view::kMaxSize;

} // namespace dagomez
