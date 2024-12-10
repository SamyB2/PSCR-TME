#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <vector>
#include <forward_list>
#include <unordered_map>
#include <algorithm>
#include <assert.h>
#include "hash.hpp"

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

void print_words(HashMap<std::string,int> &h ) {
	using namespace std;
	string war = "war";
	string peace = "peace";
	cout << "war iter => " << *h.get(war) << endl;
	cout << "peace iter => " << *h.get(peace) << endl;
	cout << "toto iter => 0"  << endl;
}

void print_first_n (std::vector<std::pair<std::string,int>>& v) {
	auto b = v.begin();
	for (int i = 0; i<10; ++i) {
		std::cout << "words : " << b.base()->first
			<< ", frequence : " << b.base()->second
			<< std::endl;
		b+=1;
	}
}

std::vector<std::pair<std::string,int>> & get_entries(HashMap<std::string,int> &h, std::vector<std::pair<std::string,int>>& v) {
	using namespace std;
	for (auto b : h.buckets) {
		for (auto x : b) {
			v.push_back(pair<string,int>(x.key,x.value));
		}
	}
	return v;
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
	HashMap<string,int> h(21333);	
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);
		
		// method with HashMap
		int * res = h.get(word);
		if (res == nullptr) {
			int value = 1;
			h.put(word, value);
			nombre_lu++;
		}else (*res)++;

		//method with vector
		// if (!contains_w(v,word)) {
		// 	v.push_back(pair(word,0));
		// 	// cout << nombre_lu << " : " << word << endl;
		// 	nombre_lu++;
		// }
	}
	// print_words(v);
	size_t s1 = h.size();
	size_t s2 = h.size2();
	assert(s1 == s2);
	// print_words(h);
	vector<pair<string,int>> entries;
	sort(entries.begin(), entries.end(), [] (const pair<string,int> &p1, const pair<string,int>p2) {
		return p1.second > p2.second;
	});
	print_first_n(entries);
	cout << endl;

	// entries2 with HashMap
	vector<pair<string,int>> entries2 = h.getEntries();
	entries = get_entries(h, entries);
	sort(entries2.begin(), entries2.end(), [] (const pair<string,int> &p1, const pair<string,int>p2) {
		return p1.second > p2.second;
	});	
	print_first_n(entries2);
	input.close();


	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "Found a total of " << nombre_lu << " words." << endl;

    return 0;
}

int main2 () {
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
	unordered_map<string,int> um(20333);
	unordered_map<string,int>::iterator end_um = um.end();
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);
		unordered_map<string,int>::iterator i = um.find(word);
		if (i == end_um) {
			um.insert(pair<string,int>(word,1));
			nombre_lu++;
		}else i->second++;
	}
	input.close();

	unordered_map<int, forward_list<string>> inv_um;
	unordered_map<int, forward_list<string>>::iterator end_inv_um;
	for ( const pair<string, int>  &x : um) {
		auto i = inv_um.find(x.second);
		if (i == end_inv_um) inv_um.insert(pair<int,forward_list<string>>(x.second,forward_list<string>(1,x.first)));
		else i->second.push_front(x.first);
	}

	for (const pair<int, forward_list<string>> &x : inv_um) {
		cout << x.first << " : [ ";
		for (const string &w : x.second)
			cout << w << " ";
		cout << "]\n" << endl;
	}
	

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "Found a total of " << nombre_lu << " words." << endl;

    return 0;
}

