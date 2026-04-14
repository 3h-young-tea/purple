#pragma once

#if	__cplusplus <= 202302l
#error	"我的瓷, 请更新编译器"
#endif

#include <bits/stdc++.h>
#include "loop.hpp"

using namespace std::literals;

namespace unda {
// https://music.douban.com/subject/35310799/	// undaword vol.1

class	word {
	std::size_t len_;
	char *s_;

public:
	word(void)
		: len_(0uz),
		s_(nullptr) {}

	word(const word &y)
		: len_(y.len_),
		s_(new char[len_ + 1uz]) {
		std::copy(y.s_, y.s_ + 1uz + y.len_, s_);
	}

	word(word &&y) noexcept
		: len_(y.len_),
		s_(y.s_) {
		y.len_ = 0uz;
		y.s_ = nullptr;
	}

	explicit word(const std::string &y)
		: len_(y.length()),
		s_(new char[len_ + 1uz]) {
		std::copy(y.begin(), y.end(), s_);
	}

	explicit word(const char *s)
		pre(s)
		: len_(std::strlen(s)),
		s_(new char[len_ + 1uz]) {
		std::copy(s, s + 1uz + len_, s_);
	}

	~word(void) noexcept {
		len_ = 0uz;
		delete[] s_;
	}

	word&	operator=(const word &y) {
		delete[] s_;

		len_ = y.len_;
		s_ = new char[len_ + 1uz];
		std::copy(y.s_, y.s_ + 1uz + y.len_, s_);

		return *this;
	}

	word&	operator=(word &&y) noexcept {
		delete[] s_;

		len_ = y.len_;
		s_ = y.s_;
		y.len_ = 0uz;
		y.s_ = nullptr;

		return *this;
	}

	word&	operator=(const std::string &y) {
		delete[] s_;

		len_ = y.length();
		s_ = new char[len_ + 1uz];
		std::copy(y.begin(), y.end(), s_);

		return *this;
	}

	word&	operator=(const char *s)
		pre(s) {
		delete[] s_;

		len_ = std::strlen(s);
		s_ = new char[len_ + 1uz];
		std::copy(s, s + 1uz + len_, s_);

		return *this;
	}

	bool	operator<(const word &y) const noexcept {
		for (std::size_t x = 0uz; s_[x] && y.s_[x]; ++x)
			if (s_[x] != y.s_[x])
				return s_[x] < y.s_[x];

		return len_ < y.len_;
	}

	bool	operator==(const word &y) const noexcept {
		if (len_ != y.len_)
			return 0;

		for (std::size_t x = 0uz; s_[x]; ++x)
			if (s_[x] != y.s_[x])
				return 0;

		return 1;
	}

	bool	operator==(const char *s) const noexcept {
		std::size_t x = 0uz;

		while (s_[x] && s[x]) {
			if (s_[x] != s[x])
				return 0;

			x++;
		}

		return s_[x] == s[x];
	}

	bool	operator!=(const word &y) const noexcept {
		if (len_ != y.len_)
			return 1;

		for (std::size_t x = 0uz; s_[x]; ++x)
			if (s_[x] != y.s_[x])
				return 1;

		return 0;
	}

	bool	operator!=(const char *s) const noexcept {
		std::size_t x = 0uz;

		while (s_[x] && s[x]) {
			if (s_[x] != s[x])
				return 1;

			x++;
		}

		return s_[x] != s[x];
	}

	word	operator+(const word &y) const {
		if (len_ == 0uz)
			return y;

		word x;
		x.len_ = len_ + y.len_;
		x.s_ = new char[x.len_ + 1uz];
		std::copy(s_, s_ + len_, x.s_);
		std::copy(y.s_, y.s_ + 1uz + y.len_, x.s_ + len_);

		return x;
	}

	word	operator+(const char *s) const {
		if (len_ == 0uz)
			return word(s);

		std::size_t s_len = std::strlen(s);
		word x;
		x.len_ = len_ + s_len;
		x.s_ = new char[x.len_ + 1uz];
		std::copy(s_, s_ + len_, x.s_);
		std::copy(s, s + 1uz + s_len, x.s_ + len_);

		return x;
	}

	word&	operator+=(const word &y) {
		if (len_ == 0uz)
			return *this = y;

		char *x = new char[len_ + y.len_ + 1uz];
		std::copy(s_, s_ + len_, x);
		std::swap(s_, x);	// "ok, 这个哥们现在换到这边, 我听你的说唱就像是在转圈" -- iron mic 2012, 艾热 vs 贝贝
		delete[] x;
		std::copy(y.s_, y.s_ + 1uz + y.len_, s_ + len_);
		len_ += y.len_;

		return *this;
	}

	word&	operator+=(const char *s) {
		if (len_ == 0uz)
			return *this = word(s);

		std::size_t s_len = std::strlen(s);
		char *x = new char[len_ + s_len + 1uz];
		std::copy(s_, s_ + len_, x);
		std::swap(s_, x);	// "ok, 这个哥们现在换到这边, 我听你的说唱就像是在转圈" -- iron mic 2012, 艾热 vs 贝贝
		delete[] x;
		std::copy(s, s + 1uz + s_len, s_ + len_);
		len_ += s_len;

		return *this;
	}

	loop<std::size_t> border(void) const noexcept 
		pre(len_) {
		loop<std::size_t> pi(len_);
		pi[0uz] = 0uz;

		for (std::size_t x = 1uz, y = 0uz; x < len_; ++x) {
			while (y && s_[y] != s_[x])
				y = pi[y - 1uz];

			pi[x] = y += s_[y] == s_[x];
		}

		return pi;
	}

	loop<std::size_t> kmp(const word &p) const noexcept
		pre(len_ && len_) {
		auto pi = p.border();
		loop<std::size_t> taocp;

		for (std::size_t x = 0uz, y = 0uz; x < len_; ++x) {
			while (y && s_[x] != p.s_[y])
				y = pi[y - 1uz];

			if ((y += s_[x] == p.s_[y]) == p.len_)
				taocp.emplace(x - y + 1uz),
				y = pi[y - 1uz];
		}

		return taocp;
	}

	std::size_t len(void) const noexcept {
		return len_;
	}

	bool	empty(void) const noexcept {
		return !len_;
	}

	const char* c_str(void) const noexcept {
		return s_;
	}

	template <class ty>
	friend ty& operator>>(ty &in, word &w) {
		int ch;

		while ((ch = in.peek()) != EOF && isspace(ch))
			in.get();

		if (!in.eof() && (ch = in.peek()) == EOF) {
			in.setstate(std::ios::failbit);
			return in;
		}

		std::size_t buf_size = 1l << 12;
		char *buf = static_cast<char*>(std::malloc(buf_size));

		std::size_t x = 0uz;
		while ((ch = in.get()) && !in.eof() && !isspace(ch)) {
			if (x == buf_size - 1uz)
				buf = static_cast<char*>(std::realloc(buf, buf_size <<= 1));

			buf[x++] = static_cast<char>(ch);
		}

		buf[x] = '\0';
		w = buf;
		std::free(buf);

		if (x == 0uz && ch && !in.eof())
			in.setstate(std::ios::failbit);

		return in;
	}
};
}
