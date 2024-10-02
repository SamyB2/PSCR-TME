template <typename iterator>
size_t count (iterator begin, iterator end) {
	size_t nb = 0;
	for (;begin != end; ++begin, ++nb);
	return nb;
}

template <typename iterator, typename T>
size_t count_if_equal (iterator begin, iterator end, const T & target) {
	size_t nb = 0;
	for (;begin != end; ++begin) {
		if (*begin == target) ++nb;
	}
	return nb;
}

template <typename K, typename V>
class HashMap {
    struct Entry {
        const K key;
        V value;

        Entry(const K k, V v): key(k), value(v) {}
    };
	public:
    typedef std::vector<std::forward_list<Entry>> buckets_t;
	buckets_t buckets;
		class iterator {
			typename buckets_t::iterator end_vit;
			typename buckets_t::iterator vit;
			typename std::forward_list<Entry>::iterator lit;
			public:
			iterator(typename buckets_t::iterator end_vit, typename buckets_t::iterator vit,
				typename std::forward_list<Entry>::iterator lit):
					end_vit(end_vit),
					vit(vit), lit(lit){}

			iterator & operator++() {
				++lit;
				if (vit->end() == lit) {
					++vit;
					for(;vit != end_vit && vit->empty(); ++vit) ;
					if (vit != end_vit) lit = vit->begin();
				}
				return *this;
			}

			bool operator!=(const iterator &other) const {
				return vit != other.vit || lit != other.lit;
			}

			Entry & operator*() {
				return *lit;
			}

		};
		HashMap(size_t cap): buckets(cap) {
			buckets.reserve(cap);
			for (int i = 0; i<cap; ++i) {
				buckets.emplace_back();
			}
		}

		V* get (const K& key) {
			using namespace std;
			size_t h = std::hash<K>()(key);
			size_t index = h % buckets.size();
			for (auto & x : buckets[index]) {
				if (x.key == key) return &(x.value);
			}
			return nullptr;
		}

		bool put (const K &key, V &value) {
			using namespace std;
			size_t h = std::hash<K>()(key);
			size_t index = h % buckets.size();
			auto res = get(key);
			if (res == nullptr) {
				buckets[index].emplace_front(key, value);
				return false;
			}
			for (auto& x : buckets[index]) {
				if (x.key == key) {
					x.value = value;
					break;
				}
			}
			return true;
		}

        size_t size() const {
            size_t s = 0;
            for (auto &b : buckets) {
                for (auto &e : b) {
					++s;
				}
            }
            return s;
        }

	 	size_t size2() const {
			size_t s = 0;
			for (auto &b : buckets) {
				s += count(b.begin(), b.end());
			}
			return s;
		}   

		iterator begin() {
			return iterator(buckets.end(), buckets.begin(),
				(*buckets.begin()).begin());
		}

		iterator end() {
			return iterator(buckets.end(), buckets.end(), 
				buckets.begin()->end());
		}

		std::vector<std::pair<K,V>> getEntries () {
			std::vector<std::pair<K,V>> entries;
			for (auto it = begin(), _end = end(); it != _end; ++it) {
				entries.emplace_back((*it).key, (*it).value);
			}
			return entries;
		}

};