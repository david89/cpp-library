#include "string_view.h"

#include <ios>

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

int string_view::compare(string_view s) const noexcept {
  const size_t rlen = std::min(len_, s.len_);
  const int comparison = traits_type::compare(data_, s.data_, rlen);
  if (comparison != 0) return comparison;
  if (len_ == s.len_) return 0;
  return len_ < s.len_ ? -1 : 1;
}

bool string_view::starts_with(string_view s) const noexcept {
  return len_ >= s.len_ && substr(0, s.len_) == s;
}

bool string_view::starts_with(string_view::value_type c) const noexcept {
  return !empty() && traits_type::eq(front(), c);
};

bool string_view::starts_with(string_view::const_pointer s) const {
  return starts_with(string_view(s));
}

bool string_view::ends_with(string_view s) const noexcept {
  return len_ >= s.len_ && substr(len_ - s.len_, npos) == s;
}

bool string_view::ends_with(value_type c) const noexcept {
  return !empty() && traits_type::eq(back(), c);
}

bool string_view::ends_with(const_pointer s) const {
  return ends_with(string_view(s));
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

std::ostream& operator<<(std::ostream& os, string_view s) {
  std::ostream::sentry sentry{os};
  if (!sentry) return os;

  // Ideas from:
  // https://stackoverflow.com/questions/39653508/implementation-of-string-view-formatted-stream-ouput

  std::string padding;
  if (s.size() < os.width()) {
    // TODO(dagomez): This is not efficient, but let's get this working first.
    padding = std::string(os.width() - s.size(), os.fill());
  }

  bool align_left = os.flags() & std::ios_base::left;
  //github.com/andrewseidl/githook-clang-formatool align_left = os.flags() & std::ios_base::left;
  if (!padding.empty() && !align_left) {
    os.write(padding.data(), padding.size());
  }
  os.write(s.data(), s.size());
  if (!padding.empty() && align_left) {
    os.write(padding.data(), padding.size());
  }

  os.width(0);
  return os;
}

} // namespace dagomez
