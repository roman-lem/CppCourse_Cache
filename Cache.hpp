#include <iostream>
#include <unordered_map>
#include <list>

template <typename T, typename KeyT = int>
struct cache_t {

	size_t sz_;
	std::list<T> LRU_list;
	std::list<T> In_list;
	std::list<T> Out_list;
	using ListIt = typename std::list<T>::iterator;
	std::unordered_map<KeyT, ListIt> hash_;

	bool full() const;

	template <typename F>
	bool lookup_update(KeyT key, F slow_get_page);
};

bool cache_t::full() const{

}

bool cache_t::lookup_update(KeyT key, F slow_get_page) {

	auto hit = hash_.find(key);
	if (hit == hash_.end()) { // not found

		if (full()) {

			hash_.erase(Out_list.back().id);
			Out_list.pop_back();
			
		}
		
		cache_.push_front(slow_get_page(key)); // fallback to slow method
		hash_[key] = cache_.begin();
		return false;
	}
	auto eltit = hit->second;
	if (eltit != cache_.begin())
	cache_.splice(cache_.begin(), cache_, eltit, std::next(eltit));
	return true;
}