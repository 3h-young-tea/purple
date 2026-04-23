#pragma once

#if	__cplusplus <= 202302l
#error	"我的瓷, 请更新编译器"
#endif

#include <bits/stdc++.h>

using namespace std::literals;

namespace fs {	// shout out 2 [_fusu](https://codeforces.com/profile/_Fusu) who taught me pointaz
template <class ty>
class	interact {
public:
	std::weak_ptr<interact<ty>> pre_;
	std::shared_ptr<interact<ty>> nxt_;
	std::unique_ptr<ty> val_;

	template <class...args>
	interact(args&&...arg)
		: val_(std::make_unique<ty>(std::forward<args>(arg)...)) {}

	template <class...args>
	void	touch(const std::shared_ptr<interact<ty>> &ref_this, args&&...arg)
		pre(ref_this.get() == this) {
		auto x = std::make_shared<interact<ty>>(std::forward<args>(arg)...);
		x->pre_ = ref_this;
		x->nxt_ = std::move(nxt_);

		if (x->nxt_)
			x->nxt_->pre_ = x;

		nxt_ = x;
	}

	template <class...args>
	void	touch(const std::weak_ptr<interact<ty>> &ref_this, args&&...arg)
		pre(ref_this.lock().get() == this) {
		auto x = std::make_shared<interact<ty>>(std::forward<args>(arg)...);
		x->pre_ = ref_this.lock();
		x->nxt_ = std::move(nxt_);

		if (x->nxt_)
			x->nxt_->pre_ = x;

		nxt_ = x;
	}

	void	rm(void) noexcept
		pre(pre_.lock()) {
		if (nxt_)
			nxt_->pre_ = pre_;

		pre_.lock()->nxt_ = std::move(nxt_);
	}

	bool	has_ring(void) const noexcept
		pre(!pre_.lock()) {
		std::map<interact<ty>*, bool> book;	// 我爱红黑树, i will never use idiot hash

		for (std::weak_ptr<interact<ty>> x = nxt_; x.lock(); x = x.lock()->nxt_) {
			if (book[x.lock().get()])
				return 1;

			book[x.lock().get()] = 1;
		}

		return 0;
	}

	std::vector<std::weak_ptr<interact<ty>>> cast_vector(void) const noexcept
		pre(!pre_.lock()) {
		std::vector<std::weak_ptr<interact<ty>>> rax;
		
		for (std::weak_ptr<interact<ty>> x = nxt_; x.lock(); x = x.lock()->nxt_)
			rax.emplace_back(x);

		return rax;
	}

	std::expected<std::weak_ptr<interact<ty>>, nullptr_t> contains(const ty& t) const noexcept
		pre(!pre_.lock()) {
		for (std::weak_ptr<interact<ty>> x = nxt_; x.lock(); x = x.lock()->nxt_)
			if (*x.lock()->val_ == t)
				return x;

		return std::unexpected<nullptr_t>(nullptr);
	}
};
}
