#pragma once

#if	__cplusplus <= 202302l
#error	"我的瓷, 请更新编译器"
#endif

#include <bits/stdc++.h>
#include "beatz.hpp"
#include "loop.hpp"

using namespace std::literals;

namespace unda {
// https://music.douban.com/subject/30305164/	// dirty moss - interact

template <class ty>
class	interact {
public:
	robj_t<interact<ty>> pre_;
	sobj_t<interact<ty>> nxt_;
	uobj_t<ty> val_;

	template <class...args>
	interact(args&&...arg)
		: val_(make_uobj<ty>(std::forward<args>(arg)...)) {}

	~interact(void) noexcept {}

	template <class ref_ty, class...args>
	void	touch(const ref_ty &ref_this, args&&...arg)
		pre(&*ref_this == this) {
		sobj_t<interact<ty>> x = make_sobj<interact<ty>>(std::forward<args>(arg)...);
		x->pre_ = ref_this;
		x->nxt_ = std::move(nxt_);

		if (x->nxt_)
			x->nxt_->pre_ = x;

		nxt_ = x;
	}

	void	rm(void) noexcept
		pre(pre_) {
		if (nxt_)
			nxt_->pre_ = pre_;

		pre_->nxt_ = std::move(nxt_);
	}

	bool	has_ring(void) const noexcept
		pre(!pre_) {
		std::map<robj_t<interact<ty>>, bool> book;	// 我爱红黑树, 我爱红黑树, 我爱红黑树, never use idiot hash

		for (robj_t<interact<ty>> x = nxt_; x; x = x->nxt_) {
			if (book[x])
				return 1;

			book[x] = 1;
		}

		return 0;
	}

	loop<robj_t<interact<ty>>> cast_loop(void) const noexcept
		pre(!pre_ && !has_ring()) {
		loop<robj_t<interact<ty>>> taocp;

		for (robj_t<interact<ty>> x = nxt_; x; x = x->nxt_)
			taocp.emplace(x);

		return taocp;
	}
};
}
