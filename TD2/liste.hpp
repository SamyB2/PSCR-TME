template <typename T>
class Liste {
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
        Liste(): head(nullptr){}

        ~Liste() {
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

};