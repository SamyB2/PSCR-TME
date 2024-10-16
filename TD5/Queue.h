#include <string>
#include <mutex>
#include <condition_variable>

template <typename T>
class Queue {
    T ** tab;
    const size_t alloc_size;
    size_t begin;
    size_t sz;
    std::condition_variable cond;
    mutable std::mutex m;
    // autre variante
    std::condition_variable notEmpty;
    std::condition_variable notFull;
    // switch block et !block
    bool isBloccking;


    public:
    Queue (size_t maxsize):alloc_size(maxsize),begin(0),end(0),isBloccking(true) {
        tab = new T*[maxsize];
        memset(tab, 0, maxsize * sizeof(T*));
    }

    // v1
    /* 
        -> une conodition variable et un notify_all
        faux => on risque d'avoir des gens qui prod et cons qui dorment en mm temps
        car perte de notification
        see cas buff 10
        100 prod 
        100 cons

        -- passer les 100 prod
        -- passer les 100 cons
        -- 80 dorment passent 10 et wakeup les prods derrier eux 
     */

    /*
        -> une cond avec notify_all <=> on passe vide à !vide || on plein à !plein
        => vrai, il exiet un invariant :
            si file !vide ou !plein => pas de thread bloqué sur wait
        -- preuve induction : 
            en passant plein à !plein
            --> notify_all => pas de thread sur wait
            --> si !plein pas de wait dans push
    */

   /*
    -> si on rempace les while par des if : 
        => faux, il suffit je que deux threads soient en wait sur push et qui se font notify_all
        -- called : spurious wakeup
        -> un thread réveillé car on a push
        -> il se fait doubler par un autre prod
        -> il ne teste pas full une autre fois
   */
    bool push (T * x) {
        std::unique_lock<std::mutex> l(m);
        while(full()) cond.wait(l);
        /* code */
        cond.notify_all();
        return true;
    }

    T * pop() {
        std::unique_lock<std::mutex> l(m);
        while (empty()) cond.wait(l);
        /* code */
        cond.notify_all();
        // return ...
    }

    // v2
    bool push2 (T * x) {
        std::unique_lock<std::mutex> l(m);
        while(full()) notFull.wait(l);
        /* code */
        notEmpty.notify_one();
        return true;
    }

    T * pop2() {
        std::unique_lock<std::mutex> l(m);
        while (empty()) notEmpty.wait(l);
        /* code */
        notFull.notify_one();
        // return ...
    }

    // block et !block
    bool push3 (T * x) {
        std::unique_lock<std::mutex> l(m);
        while(full() && isBloccking) notFull.wait(l);
        if (full()) return false;
        /* code */
        notEmpty.notify_one();
        return true;
    }

    T * pop3() {
        std::unique_lock<std::mutex> l(m);
        while (empty() && isBloccking) notEmpty.wait(l);
        if (empty()) return nullptr;
        /* code */
        notFull.notify_one();
        // return ...
    }

    void setBlocking(bool block) {
        std::unique_lock<std::mutex> l(m);
        isBloccking = block;
        cond.notify_all();
    }


    size_t size() const {
        std::unique_lock<std::mutex> l(m);
        return sz;
    }

    private: 
    bool full() const {
        return sz == alloc_size;
    }

    bool empty() const {
        return sz == 0;
    }

};