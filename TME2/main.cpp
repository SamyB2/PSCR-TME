#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <vector>

bool contains_w (std::vector<std::pair<std::string, int>>& vec, const std::string word) {
	for (std::pair<std::string, int> &x : vec) {
		if (word == x.first) {
			x.second++;
			return true;
		}
	}
	return false;
}

void print_words (std::vector<std::pair<std::string,int>>& vec) {
	using namespace std;
	for (pair<string,int> &x : vec) {
		if (x.first == "war" || x.first == "peace" || x.first == "toto") {
			cout << x.first << " iter => " << x.second << endl;
		}
	}
}

int main () {
	using namespace std;
	using namespace std::chrono;

	ifstream input = ifstream("WarAndPeace.txt");

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;

	size_t nombre_lu = 0;
	// prochain mot lu
	string word;
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re( R"([^a-zA-Z])");
	std::vector<pair<string,int>> v;
	
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);
		if (!contains_w(v,word)) {
			v.push_back(pair(word,0));
			// cout << nombre_lu << " : " << word << endl;
			nombre_lu++;
		}
	}
	print_words(v);
	input.close();

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "Found a total of " << nombre_lu << " words." << endl;

    return 0;
}


