template <typename K, typename V>
class HashMap {
    struct Entry {
        const K key;
        V value;

        Entry(const K k, V v): key(k), value(v) {}
    };
    typedef std::vector<std::forward_list<Entry>> buckets_t;
	buckets_t buckets;
	public:
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
                s += b.size();
            }
            return s;
        }
        

};