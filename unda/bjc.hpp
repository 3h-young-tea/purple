#pragma once

#if	__cplusplus <= 202302l
#error	"我的瓷, 请更新编译器"
#endif

#include <bits/stdc++.h>

using namespace std::literals;

namespace unda {
// https://music.douban.com/subject/35924930/	// bjc

constexpr long k_mod = 0x3b800001l;

class	bjc {
	std::size_t r_, c_;
	std::vector<std::vector<long>> a_;

public:
	bjc(std::size_t r, std::size_t c)
		: r_(r), c_(c),
		a_(r_, std::vector<long>(c_)) {}

	bjc(const bjc &rdi)
		: r_(rdi.r_), c_(rdi.c_),
		a_(rdi.a_) {}

	bjc(bjc &&rdi) noexcept
		: r_(rdi.r_), c_(rdi.c_),
		a_(std::move(rdi.a_)) {
		rdi.r_ = rdi.c_ = 0uz;
		rdi.a_.clear();
	}

	~bjc(void) {
		r_ = c_ = 0uz;
	}

	bjc&	operator=(const bjc &rdi) {
		r_ = rdi.r_;
		c_ = rdi.c_;
		a_ = rdi.a_;

		return *this;
	}

	bjc&	operator=(bjc &&rdi) noexcept {
		r_ = rdi.r_;
		c_ = rdi.c_;
		a_ = std::move(rdi.a_);
		rdi.r_ = rdi.c_ = 0uz;
		rdi.a_.clear();

		return *this;
	}

	void	resize(std::size_t r, std::size_t c) {
		r_ = r;
		c_ = c;
		a_.clear();
		a_.resize(r_);

		for (auto &x : a_)
			x.resize(c_);
	}

	std::size_t row(void) const noexcept {
		return r_;
	}

	std::size_t col(void) const noexcept {
		return c_;
	}

	std::vector<long>& operator[](std::size_t x) noexcept {
		return a_[x];
	}

	const std::vector<long>& operator[](std::size_t x) const noexcept {
		return a_[x];
	}

	bjc	operator+(const bjc &rdi) const
		pre(r_ == rdi.r_ && c_ == rdi.c_) {
		bjc rax = *this;

		for (std::size_t x = 0uz; x < r_; ++x)
		for (std::size_t y = 0uz; y < c_; ++y)
			rax[x][y] = ((rax[x][y] + rdi[x][y]) % k_mod + k_mod) % k_mod;

		return rax;
	}

	bjc&	operator+=(const bjc &rdi)
		pre(r_ == rdi.r_ && c_ == rdi.c_) {
		for (std::size_t x = 0uz; x < r_; ++x)
		for (std::size_t y = 0uz; y < c_; ++y)
			a_[x][y] = ((a_[x][y] + rdi[x][y]) % k_mod + k_mod) % k_mod;

		return *this;
	}

	bjc	operator-(const bjc &rdi) const
		pre(r_ == rdi.r_ && c_ == rdi.c_) {
		bjc rax = *this;

		for (std::size_t x = 0uz; x < r_; ++x)
		for (std::size_t y = 0uz; y < c_; ++y)
			rax[x][y] = ((rax[x][y] - rdi[x][y]) % k_mod + k_mod) % k_mod;

		return rax;
	}

	bjc&	operator-=(const bjc &rdi)
		pre(r_ == rdi.r_ && c_ == rdi.c_) {
		for (std::size_t x = 0uz; x < r_; ++x)
		for (std::size_t y = 0uz; y < c_; ++y)
			a_[x][y] = ((a_[x][y] - rdi[x][y]) % k_mod + k_mod) % k_mod;

		return *this;
	}

	bjc	operator*(const bjc &rdi) const
		pre(c_ == rdi.r_) {
		bjc rax(r_, rdi.c_);

		for (std::size_t x = 0uz; x < r_; ++x)
		for (std::size_t y = 0uz; y < rdi.c_; ++y)
		for (std::size_t z = 0uz; z < c_; ++z)
			rax[x][y] = (((rax[x][y] + a_[x][z] * rdi[z][y] % k_mod) % k_mod) + k_mod) % k_mod;

		return rax;
	}

	bjc&	operator*=(const bjc &rdi)
		pre(c_ == rdi.r_) {
		std::vector<std::vector<long>> rax(r_, std::vector<long>(rdi.c_));

		for (std::size_t x = 0uz; x < r_; ++x)
		for (std::size_t y = 0uz; y < rdi.c_; ++y)
		for (std::size_t z = 0uz; z < c_; ++z)
			rax[x][y] = (((rax[x][y] + a_[x][z] * rdi[z][y] % k_mod) % k_mod) + k_mod) % k_mod;

		c_ = rdi.c_;
		a_ = rax;
		return *this;
	}

	bjc	pow(long times) const
		pre(r_ == c_) {
		bjc rax(r_, c_), masta(*this);

		for (std::size_t x = 0uz; x < r_; ++x)
			rax[x][x] = 1z;

		while (times) {
			if (times & 1z)
				rax *= masta;

			masta *= masta;
			times >>= 1;
		}

		return rax;
	}
};
}
