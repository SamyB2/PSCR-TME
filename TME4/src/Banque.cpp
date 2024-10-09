#include "Banque.h"

#include <iostream>
#include <mutex>

using namespace std;

namespace pr {

void Banque::transfert(size_t deb, size_t cred, unsigned int val) {
	Compte & debiteur = comptes[deb];
	Compte & crediteur = comptes[cred];
	recursive_mutex & d_mu = debiteur.getMutex();
	recursive_mutex & c_mu = crediteur.getMutex();
	if (deb == cred) {
		return;
	}
	// utiliser les locks par priorité
	if (deb < cred) {
		d_mu.lock();
		c_mu.lock();
	} else {
		c_mu.lock();
		d_mu.lock();
	}
	if (debiteur.debiter(val)) {
		crediteur.crediter(val);
	}
	c_mu.unlock();
	d_mu.unlock();
}
size_t Banque::size() const {
	return comptes.size();
}
bool Banque::comptabiliser (int attendu) const {
	int bilan = 0;
	int id = 0;
	for (const auto & compte : comptes) {
		compte.getMutex().lock();
		if (compte.getSolde() < 0) {
			cout << "Compte " << id << " en négatif : " << compte.getSolde() << endl;
		}
		bilan += compte.getSolde();
		id++;
	}
	for (const auto & compte : comptes) {
		compte.getMutex().unlock();
	}
	if (bilan != attendu) {
		cout << "Bilan comptable faux : attendu " << attendu << " obtenu : " << bilan << endl;
	}
	return bilan == attendu;
}
}
