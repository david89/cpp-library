#include "string_view.h"

namespace dagomez {

string_view::const_reference string_view::at(size_type pos) const {
  if (pos > len_) {
    throw std::out_of_range("Out of range");
  }

  return this->operator[](pos);
}

string_view::size_type string_view::copy(string_view::pointer dest,
                                         string_view::size_type count,
                                         string_view::size_type pos) const {
  if (pos > len_) {
    throw std::out_of_range("Out of range");
  }

  const size_type rcount = std::min(count, len_ - pos);
  traits_type::copy(dest, data_ + pos, rcount);
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

string_view::size_type
string_view::find(string_view s, string_view::size_type pos) const noexcept {
  if (empty() && s.empty() && pos == 0) {
    return 0;
  }
  if (pos > len_ || s.len_ > (len_ - pos)) {
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
  if (s.empty()) {
    return std::min(pos, len_);
  }
  if (s.len_ > len_) {
    return npos;
  }
  pos = std::min(pos, len_ - s.len_);
  while (pos != npos) {
    if (memcmp(data_ + pos, s.data_, s.len_) == 0) {
      return pos;
    }

    pos--;
  }

  return npos;
}

string_view::size_type
string_view::find_first_of(string_view s, string_view::size_type pos) const
    noexcept {
  while (pos < len_) {
    if (traits_type::find(s.data_, s.len_, data_[pos]) != nullptr) {
      return pos;
    }

    pos++;
  }

  return npos;
}

string_view::size_type
string_view::find_last_of(string_view s, string_view::size_type pos) const
    noexcept {
  if (empty()) {
    return npos;
  }

  pos = std::min(pos, len_ - 1);
  while (pos != npos) {
    if (traits_type::find(s.data_, s.len_, data_[pos]) != nullptr) {
      return pos;
    }

    pos--;
  }

  return npos;
}

string_view::size_type
string_view::find_first_not_of(string_view s, string_view::size_type pos) const
    noexcept {
  while (pos < len_) {
    if (traits_type::find(s.data_, s.len_, data_[pos]) == nullptr) {
      return pos;
    }

    pos++;
  }

  return npos;
}

string_view::size_type
string_view::find_last_not_of(string_view s, string_view::size_type pos) const
    noexcept {
  if (empty()) {
    return npos;
  }

  pos = std::min(pos, len_ - 1);
  while (pos != npos) {
    if (traits_type::find(s.data_, s.len_, data_[pos]) == nullptr) {
      return pos;
    }

    pos--;
  }

  return npos;
}

constexpr string_view::size_type string_view::kMaxSize;

} // namespace dagomez
