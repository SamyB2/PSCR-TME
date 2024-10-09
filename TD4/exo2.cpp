#include <vector>
#include <iostream>
#include <thread>   
#include <atomic>
#include <mutex>

class Compte {
    int solde;
    public:
    Compte(int solde=0) : solde(solde) {}
    void crediter(int montant) {
        solde += montant;
    }
    int getSolde() {
        return solde;
    }
};

class CompteAtomic {
    int solde;
    mutable std::mutex m;
    // on peut mettre recursive_mutex pour autoriser le meme thread a lock plusieurs fois
    public:
    CompteAtomic(int solde=0) : solde(solde) {}

    CompteAtomic(const CompteAtomic& c) : solde(c.getSolde()) {}

    void crediter(int montant) {
        std::unique_lock l(m);
        solde += montant;
    }

    int getSolde()const {
        /* m.lock();
        // int s = solde;
        // m.unlock();
        // return s; */
        std::unique_lock l(m);
        return solde;
    }

    bool debiter (int montant) {
        std::unique_lock l(m);
        // m.lock();
        bool doit = (solde >= montant);
        if (doit) {
            solde -= montant;
        }
        // m.unlock();
        return doit;
    }

    // make this func atomic !!!
    bool debiter_deprecated(int montant) {
        bool doit = (solde >= montant);
        if (doit) {
            //commutation ici possible
            solde -= montant;
        }
        return doit;
    }
};

const int JP = 10000;

/* void jackpot(Compte& c) {
//     for (int i = 0; i < JP; i++) {
//         c.crediter(1);
//     }
// } */

void jackpot(CompteAtomic& c) {
    for (int i = 0; i < JP; i++) {
        c.crediter(1);
    }
}

void losepot(CompteAtomic& c) {
    for (int i = 0; i < JP; i++) {
        c.debiter(1);
    }
}

const int NB_THREADS = 10;
int main() {
    // Compte c;
    CompteAtomic c(10);
    std::vector<std::thread> threads;
    for (int i = 0; i < NB_THREADS; i++) {
        // threads.emplace_back(jackpot, std::ref(c));
        threads.emplace_back(losepot, std::ref(c));
    }
    for (auto& t : threads) {
        t.join();
    }
    std::cout << "Solde final : " << c.getSolde() << std::endl;
    std::cout << "Solde attendu : " << JP * NB_THREADS << std::endl;
    return 0;
}