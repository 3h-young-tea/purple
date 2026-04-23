#pragma once

#if	__cplusplus <= 202302l
#error	"我的瓷, 请更新编译器"
#endif

#include <bits/stdc++.h>

using namespace std::literals;

namespace unda {
// https://music.douban.com/subject/36178648/	// undabeatz vol.1
// https://music.douban.com/subject/36776810/	// undabeatz vol.2
// https://music.douban.com/subject/37923198/	// undabeatz vol.3
// https://music.douban.com/subject/37935894/	// undabeatz vol.4

template <class ty>
class	uobj_t {	// ultra object
	ty *x_;

	uobj_t(const uobj_t<ty>&) = delete;
	void operator=(const uobj_t<ty>&) = delete;

	void	pop(void) noexcept {
		if (x_ == nullptr)
			return;

		delete x_;
		x_ = nullptr;
	}

public:
	uobj_t(void) noexcept
		: x_(nullptr) {}

	uobj_t(uobj_t<ty> &&y) noexcept
		: x_(y.x_) {
		y.x_ = nullptr;
	}

	explicit uobj_t(ty *y) noexcept
		: x_(y) {}

	template <class...args>
	uobj_t(args&&...val)
		: x_(new ty(std::forward<args>(val)...)) {}

	~uobj_t(void) noexcept {
		pop();
	}

	uobj_t<ty>& operator=(uobj_t<ty> &&y) noexcept {
		if (this == &y)
			return *this;

		pop();
		x_ = y.x_;
		y.x_ = nullptr;

		return *this;
	}

	uobj_t<ty>& operator=(nullptr_t) noexcept {
		pop();

		return *this;
	}

	bool	operator<(const uobj_t<ty> &y) const noexcept {
		return x_ < y.x_;
	}

	explicit operator bool(void) const noexcept {
		return x_ != nullptr;
	}

	ty*	operator->(void) noexcept
		pre(x_) {
		return x_;
	}

	const ty* operator->(void) const noexcept
		pre(x_) {
		return x_;
	}

	ty&	operator*(void) noexcept
		pre(x_) {
		return *x_;
	}

	const ty& operator*(void) const noexcept
		pre(x_) {
		return *x_;
	}
};

template <class typ>
uobj_t<typ> make_uobj(void) {
	uobj_t<typ> x(new typ());
	return x;
}

template <class typ, class...args>
uobj_t<typ> make_uobj(args&&...val) {
	uobj_t<typ> x(new typ(std::forward<args>(val)...));
	return x;
}

template <class ty>
class	robj_t;

template <class ty>
class	sobj_t {	// super object
	ty *x_;
	std::atomic<long> *tot_;

	friend class robj_t<ty>;

	void	pop(ty *x, std::atomic<long> *tot) const noexcept {
		if (x == nullptr)
			return;

		if (tot->fetch_sub(1z, std::memory_order::acq_rel) == 1z) {
			delete x;
			delete tot;
		}
	}

public:
	sobj_t(void) noexcept
		: x_(nullptr), tot_(nullptr) {}

	sobj_t(const sobj_t<ty> &y) noexcept
		: x_(y.x_), tot_(y.tot_) {
		if (x_)
			tot_->fetch_add(1z, std::memory_order::relaxed);
	}

	sobj_t(const robj_t<ty> &y) noexcept
		: x_(y.x_), tot_(y.tot_) {
		if (x_)
			tot_->fetch_add(1z, std::memory_order::relaxed);
	}

	sobj_t(sobj_t<ty> &&y) noexcept
		: x_(y.x_), tot_(y.tot_) {
		y.x_ = nullptr;
		y.tot_ = nullptr;
	}

	explicit sobj_t(ty *y) noexcept
		: x_(y), tot_(new std::atomic<long>(1z)) {}

	template <class...args>
	sobj_t(args&&...val)
		: x_(new ty(std::forward<args>(val)...)),
		tot_(new std::atomic<long>(1z)) {}

	~sobj_t(void) noexcept {
		pop(x_, tot_);
	}

	sobj_t<ty>& operator=(const sobj_t<ty> &y) noexcept {
		if (this == &y)
			return *this;

		auto tmp_x = x_;
		auto tmp_tot = tot_;
		x_ = y.x_;
		tot_ = y.tot_;
		
		if (x_)
			tot_->fetch_add(1z, std::memory_order::relaxed);

		pop(tmp_x, tmp_tot);

		return *this;
	}

	sobj_t<ty>& operator=(const robj_t<ty> &y) noexcept {
		auto tmp_x = x_;
		auto tmp_tot = tot_;
		x_ = y.x_;
		tot_ = y.tot_;

		if (x_)
			tot_->fetch_add(1z, std::memory_order::relaxed);

		pop(tmp_x, tmp_tot);

		return *this;
	}

	sobj_t<ty>& operator=(sobj_t<ty> &&y) noexcept {
		if (this == &y)
			return *this;

		auto tmp_x = x_;
		auto tmp_tot = tot_;
		x_ = y.x_;
		tot_ = y.tot_;
		y.x_ = nullptr;
		y.tot_ = nullptr;
		pop(tmp_x, tmp_tot);

		return *this;
	}

	sobj_t<ty>& operator=(nullptr_t) noexcept {
		pop(x_, tot_);

		return *this;
	}

	bool	operator<(const sobj_t<ty> &y) const noexcept {
		return x_ < y.x_;
	}

	bool	operator<(const robj_t<ty> &y) const noexcept {
		return x_ < y.x_;
	}

	explicit operator bool(void) const noexcept {
		return x_ != nullptr;
	}

	ty*	operator->(void) noexcept
		pre(x_) {
		return x_;
	}

	const ty* operator->(void) const noexcept
		pre(x_) {
		return x_;
	}

	ty&	operator*(void) noexcept
		pre(x_) {
		return *x_;
	}

	const ty& operator*(void) const noexcept
		pre(x_) {
		return *x_;
	}
};

template <class typ>
sobj_t<typ> make_sobj(void) {
	sobj_t<typ> x(new typ());
	return x;
}

template <class typ, class...args>
sobj_t<typ> make_sobj(args&&...val) {
	sobj_t<typ> x(new typ(std::forward<args>(val)...));
	return x;
}

template <class ty>
class	robj_t {	// rare object
	ty *x_;
	std::atomic<long> *tot_;

	friend class sobj_t<ty>;

public:
	robj_t(void) noexcept
		: x_(nullptr), tot_(nullptr) {}

	robj_t(const robj_t<ty> &y) noexcept
		: x_(y.x_), tot_(y.tot_) {}

	robj_t(const sobj_t<ty> &y) noexcept
		: x_(y.x_), tot_(y.tot_) {}

	robj_t(robj_t<ty> &&y) noexcept
		: x_(y.x_), tot_(y.tot_) {
		y.x_ = nullptr;
		y.tot_ = nullptr;
	}

	~robj_t(void) noexcept {
		x_ = nullptr;
		tot_ = nullptr;
	}

	robj_t<ty>& operator=(const robj_t<ty> &y) noexcept {
		if (this == &y)
			return *this;

		x_ = y.x_;
		tot_ = y.tot_;

		return *this;
	}

	robj_t<ty>& operator=(const sobj_t<ty> &y) noexcept {
		x_ = y.x_;
		tot_ = y.tot_;

		return *this;
	}

	robj_t<ty>& operator=(robj_t<ty> &&y) noexcept {
		if (this == &y)
			return *this;

		x_ = y.x_;
		tot_ = y.tot_;
		y.x_ = nullptr;
		y.tot_ = nullptr;

		return *this;
	}

	robj_t<ty>& operator=(nullptr_t) noexcept {
		x_ = nullptr;
		tot_ = nullptr;

		return *this;
	}

	bool	operator<(const robj_t<ty> &y) const noexcept {
		return x_ < y.x_;
	}

	bool	operator<(const sobj_t<ty> &y) const noexcept {
		return x_ < y.x_;
	}

	explicit operator bool(void) const noexcept {
		return x_ != nullptr;
	}

	ty*	operator->(void) noexcept
		pre(x_) {	// pre(x_ && std::is_within_lifetime(x_))
		return x_;
	}

	const ty* operator->(void) const noexcept
		pre(x_) {	// pre(x_ && std::is_within_lifetime(x_))
		return x_;
	}

	ty&	operator*(void) noexcept
		pre(x_) {	// pre(x_ && std::is_within_lifetime(x_))
		return *x_;
	}

	const ty& operator*(void) const noexcept
		pre(x_) {	// pre(x_ && std::is_within_lifetime(x_))
		return *x_;
	}
};
}
