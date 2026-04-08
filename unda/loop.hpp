#pragma once

#if	__cplusplus <= 202302l
#error	"我的瓷, 请更新编译器"
#endif

#include <bits/stdc++.h>

using namespace std::literals;

namespace unda {
//	https://music.douban.com/subject/35924930/
//	https://music.douban.com/subject/36241994/

template <class ty>
class	loop {
	std::size_t size_;
	ty *f_, *g_;

	void	check_it(void) {
		if (g_ < f_ + size_ - 1)
			return;

		if (size_ == 0) {
			size_ = 32;
			f_ = new ty[size_];
			g_ = f_;
			return;
		}

		ty *p = new ty[size_ <<= 1];

		for (std::size_t x = 0; x < size(); ++x)
			new (p + x) ty(std::move(f_[x]));

		g_ = p + size();
		delete[] f_;
		f_ = p;
	}

public:
	loop(void)
		: size_(32),
		f_(new ty[size_]), g_(f_) {}

	loop(std::size_t len)
		: size_(len * 2),
		f_(new ty[size_]), g_(f_ + len) {
	}

	loop(const loop &y)
		: size_(y.size_),
		f_(new ty[size_]), g_(f_ + y.size()) {
		for (std::size_t x = 0; x < size(); ++x)
			new (f_ + x) ty(y.f_[x]);
	}

	loop(loop &&y)
		: size_(y.size_),
		f_(y.f_), g_(y.g_) {
		y.size_ = 0;
		y.f_ = y.g_ = nullptr;
	}

	~loop(void) noexcept {
		for (std::size_t x = 0; x < size(); ++x)
			f_[x].~ty();

		delete[] f_;
	}

	loop<ty>& operator=(const loop<ty> &y) {
		delete[] f_;

		size_ = y.size_;
		f_ = new ty[size_], g_ = f_ + y.size();

		for (std::size_t x = 0; x < size(); ++x)
			new (f_ + x) ty(y.f_[x]);

		return *this;
	}

	loop<ty>& operator=(loop<ty> &&y) noexcept {
		delete[] f_;

		size_ = y.size_;
		f_ = y.f_;
		g_ = y.g_;
		y.size_ = 0;
		y.f_ = y.g_ = nullptr;

		return *this;
	}

	void	push(const ty &x) {
		check_it();
		new (g_++) ty(x);
	}

	template <class...arg>
	void	emplace(arg&&...args) {
		check_it();
		new (g_++) ty(std::forward<arg>(args)...);
	}

	void	pop(void)
		pre(f_ < g_) {
		(--g_)->~ty();
	}

	void	rm(std::size_t p)
		pre(p < size()) {
		for (std::size_t x = p; x + 1 < size(); ++x)
			f_[x] = std::move(f_[x + 1]);

		(--g_)->~ty();
	}

	ty&	operator[](std::size_t x) {
		return f_[x];
	}

	const ty& operator[](std::size_t x) const {
		return f_[x];
	}

	std::size_t size(void) const noexcept {
		return g_ - f_;
	}

	bool empty(void) const noexcept {
		return g_ <= f_;
	}

	ty*	begin(void) const {
		return f_;
	}

	ty*	end(void) const {
		return g_;
	}
};
}
