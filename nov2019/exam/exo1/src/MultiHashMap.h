#pragma once

#include <forward_list>
#include <vector>

namespace pr {

template <typename K, typename V>
class MHashMap {

public:
	class Entry {
	public :
		const K key;
		V value;
		Entry(const K &k,const V& v):key(k),value(v){}
	};
private :

	typedef std::vector<std::forward_list<Entry> > buckets_t;
	// stockage pour la table de buckets
	buckets_t buckets;
	// nombre total d'entrées dans la table
	std::atomic<size_t> sz;
	std::vector<std::mutex> locks;

public:
	MHashMap(size_t size): buckets(size),sz(0)  {
		// le ctor buckets(size) => size cases, initialisées par défaut.
		locks.reserve(size);
		for (int i = 0; i<size; ++i)
			locks.emplace_back(std::mutex());
	}

	V* get(const K & key) {
		size_t h = std::hash<K>()(key);
		size_t target = h % buckets.size();
		{
			std::unique_lock<std::mutex> l(locks[target]);
			for (Entry & ent : buckets[target]) {
				if (ent.key==key) {
					return & ent.value;
				}
			}
		}
		return nullptr;
	}

	bool put (const K & key, const V & value) {
		size_t h = std::hash<K>()(key);
		size_t target = h % buckets.size();
		{
			std::unique_lock<std::mutex> l(locks[target]);
			for (Entry & ent : buckets[target]) {
				if (ent.key==key) {
					ent.value=value;
					return true;
				}
			}
			buckets[target].emplace_front(key,value);
		}
		sz++;
		return false;
	}

	size_t size() const { return sz ; }
};

} /* namespace pr */

