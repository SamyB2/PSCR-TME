template <typename T>
class List {
    class Node {
        T data;
        Node * next;

        public : 
            Node(T& val, Node * n) : data(val), next(n) {}

            ~Node() {
                if (next == nullptr) return;
                next->~Node();
                delete next;
            }
    };
    Node * head;

    public :
        class iterator {
            Node * cur;

            public:
                iterator(Node * pos):cur(pos){}

                /*
                    en cpp 20, == donne la def de != 
                    soo, mieuxx def == seulement.
                */
                bool operator!= (const iterator &other)const {
                    return this->cur != other.cur;
                }

                // prefixe ++it
                // ++i = i-- ?
                iterator &operator++() {
                    cur == cur->next;
                    return *this;
                }

                // postfixe it++
                iterator operator++(int) {
                    iterator ret = *this;
                    cur = cur->next;
                    return ret;
                }

                T &operator*() const {
                    return cur->data;
                }

                T *operator->() const {
                    // permets d'acceder directement aux champs de T
                    // avec it->champs
                    return &cur->data;
                }
        };
        
        class const_iterator {
           const Node * cur;

            public:
                const_iterator(const Node * pos):cur(pos){}

                /*
                    en cpp 20, == donne la def de != 
                    soo, mieuxx def == seulement.
                */
                bool operator!= (const iterator &other)const {
                    return this->cur != other.cur;
                }

                // prefixe ++it
                const_iterator &operator++() {
                    cur == cur->next;
                    return *this;
                }

                // postfixe it++
                const_iterator operator++(int) {
                    iterator ret = *this;
                    cur = cur->next;
                    return ret;
                }

                const T &operator*() const {
                    return cur->data;
                }

                const T *operator->() const {
                    // permets d'acceder directement aux champs de T
                    // avec it->champs
                    return &cur->data;
                }
        };
        
        List(): head(nullptr){}

        ~List() {
            if (!head) return;
            head->~Node();
        }

        T& operator[](size_t index) {
            Node * i; 
            for (i = head; i && index; i = i->next, --index);
            if (index || !i) return nullptr;
            return i->data;
        }

        void push_front(const T& v) {
            head = new Node(v, head);
        }

        iterator begin() {
            return head;
        }

        iterator end() {
            return iterator(nullptr)
        }

};