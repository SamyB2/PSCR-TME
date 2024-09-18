#include "List.h"
#include <string>
#include <iostream>
#include <cstring>

int main () {

	int tab[] = {1,2,3,4,5,6,7,8,9};
	for (int i = 0 ; i < 9 ; i++) {
		std::cout << "tab[" << i << "] = " << tab[i] << std::endl;
	}

	// for (size_t i=9; i >= 0 ; i--) {
	// 	if (tab[i] - tab[i-1] != 1) {
	// 		std::cout << "probleme !";
	// 	}
	// }

	std::string abc = "abc";
	char * str = new char [3];
	str[0] = 'a';
	str[1] = 'b';
	str[2] = 'c';
	ssize_t i = 0;

	if (! strcmp (str, abc.c_str())) {
		std::cout << "Equal !";
	}

	pr::List list;
	list.push_front(abc);
	list.push_front(abc);

	std::cout << "Liste : " << list << std::endl;
	std::cout << "Taille : " << list.size() << std::endl;

	// Affiche à l'envers
	for (i= list.size() - 1 ; i >= 0 ; i--) {
		std::cout << "elt " << i << ": " << list[i] << std::endl;
	}

	delete [] str;
	return 0;

}
