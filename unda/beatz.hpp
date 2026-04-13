#pragma once

#if	__cplusplus <= 202302l
#error	"我的瓷, 请更新编译器"
#endif

#include <bits/stdc++.h>

using namespace std::literals;

namespace unda {
//	https://music.douban.com/subject/36178648/
//	https://music.douban.com/subject/36776810/
//	https://music.douban.com/subject/37923198/
//	https://music.douban.com/subject/37935894/

template <class ty>
class	uobj_t {	// ultra object
	ty *x_;

	uobj_t(const uobj_t<ty>&) = delete;
	void operator=(const uobj_t<ty>&) = delete;

public:
	uobj_t(void) noexcept
		: x_(nullptr) {}

	uobj_t(uobj_t<ty> &&y) noexcept
		: x_(y.x_) {
		y.x_ = nullptr;
	}

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

	bool	operator<(const uobj_t<ty> &y) const noexcept {
		return x_ < y.x_;
	}

	void	push(const ty &val) {
		pop();
		x_ = new ty(val);
	}

	void	pop(void) noexcept {
		if (x_ == nullptr)
			return;

		delete x_;
		x_ = nullptr;
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

	template <class typ>
	friend uobj_t<typ> make_uobj(void) {
		uobj_t<typ> x;
		x.x_ = new typ();
		return x;
	}

	template <class typ, class...args>
	friend uobj_t<typ> make_uobj(args&&...val) {
		uobj_t<typ> x;
		x.x_ = new typ(std::forward<args&&>(val)...);
		return x;
	}
};

template <class typ>
uobj_t<typ> make_uobj(void);

template <class typ, class...args>
uobj_t<typ> make_uobj(args&&...);

template <class ty>
class	robj_t;

template <class ty>
class	sobj_t {	// super object
	ty *x_;
	std::atomic<std::size_t> *tot_;

	friend class robj_t<ty>;

public:
	sobj_t(void) noexcept
		: x_(nullptr), tot_(nullptr) {}

	sobj_t(const sobj_t<ty> &y) noexcept
		: x_(y.x_), tot_(y.tot_) {
		if (x_)
			tot_->fetch_add(1uz, std::memory_order::relaxed);
	}

	sobj_t(const robj_t<ty> &y) noexcept
		: x_(y.x_), tot_(y.tot_) {
		if (x_)
			tot_->fetch_add(1uz, std::memory_order::relaxed);
	}

	sobj_t(sobj_t<ty> &&y) noexcept
		: x_(y.x_), tot_(y.tot_) {
		y.x_ = nullptr;
		y.tot_ = nullptr;
	}

	~sobj_t(void) noexcept {
		pop();
	}

	sobj_t<ty>& operator=(const sobj_t<ty> &y) noexcept {
		if (this == &y)
			return *this;

		pop();
		x_ = y.x_;
		tot_ = y.tot_;

		if (x_)
			tot_->fetch_add(1uz, std::memory_order::relaxed);

		return *this;
	}

	sobj_t<ty>& operator=(const robj_t<ty> &y) noexcept {
		pop();
		x_ = y.x_;
		tot_ = y.tot_;

		if (x_)
			tot_->fetch_add(1uz, std::memory_order::relaxed);

		return *this;
	}

	sobj_t<ty>& operator=(sobj_t<ty> &&y) noexcept {
		if (this == &y)
			return *this;

		pop();
		x_ = y.x_;
		tot_ = y.tot_;
		y.x_ = nullptr;
		y.tot_ = nullptr;

		return *this;
	}

	bool	operator<(const sobj_t<ty> &y) const noexcept {
		return x_ < y.x_;
	}

	bool	operator<(const robj_t<ty> &y) const noexcept {
		return x_ < y.x_;
	}

	void	pop(void) noexcept {
		if (x_ == nullptr)
			return;

		if (tot_->fetch_sub(1uz, std::memory_order::acq_rel) == 1) {
			delete x_;
			delete tot_;
		}

		x_ = nullptr;
		tot_ = nullptr;
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

	template <class typ>
	friend sobj_t<typ> make_sobj(void) {
		sobj_t<typ> x;
		x.x_ = new typ();
		x.tot_ = new std::atomic<std::size_t>(1uz);
		return x;
	}

	template <class typ, class...args>
	friend sobj_t<typ> make_sobj(args&&...val) {
		sobj_t<typ> x;
		x.x_ = new typ(std::forward<args>(val)...);
		x.tot_ = new std::atomic<std::size_t>(1uz);
		return x;
	}
};

template <class typ>
sobj_t<typ> make_sobj(void);

template <class typ, class...args>
sobj_t<typ> make_sobj(args&&...);

/*	这个 rare object 的语义是, 我拥有这个东西, 但是我拒绝参与引用计数
 *	简单来说, 我拥有这个东西, 那我就不应该是观察者, i`m da masta
 *	所以每次使用前不用检查 if (ptr), 直接用就行了
 *	主要用途之一是你可以用于记录双链表的前一个顶点, 如果前一个顶点不存在就说明你把代码写炸了 (例外就是, 头顶点没有前一个顶点)
 */

template <class ty>
class	robj_t {	// rare object
	ty *x_;
	std::atomic<std::size_t> *tot_;

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
