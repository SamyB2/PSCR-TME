#pragma once

#include <vector>
#include <iostream>
#include <string>

namespace pr {

class concat {
	// TODO : attributs stockant ref ou pointeurs vers les constituants v1,v2
	std::vector<std::string> &v1;
	std::vector<std::string> &v2;
public:
	concat(std::vector<std::string> & v1, std::vector<std::string> & v2):v1(v1),
		v2(v2) {}

	class iterator {
		std::vector<std::string>::iterator actual;
		int fin;
		std::vector<std::string> &v1;
		std::vector<std::string> &v2;
	public:
		iterator(std::vector<std::string>& v1, std::vector<std::string>& v2, std::vector<std::string>::iterator act): v1(v1), v2(v2),fin(0),actual(act) {}
		// TODO : contrat itérateur
		std::string & operator*() {
			return *actual;
		}
		iterator & operator++() {
			++actual;
			if (actual == v1.end() && !fin) {
				actual = v2.begin();
				fin = 1;
			}
			return *this;
		}
		bool operator!=(const iterator & other) const {
			return other.actual != actual;
		}
	};

	iterator begin() {
		return iterator(v1, v2, v1.begin());
	}
	iterator end() {
		return iterator(v1, v2, v2.end());
	}
};

}
