#include "CFP.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <fstream>
#include <string>

CFP::CFP(string name) {
	this->name = name;
}

void CFP::load(){

	string str;
	ifstream file(this->name);

	if(file.is_open()){

		while (getline(file, str)) {
			words.push_back(str);
		}
	}
	file.close();
}

vector<string> CFP::getCFP(){
	return words;
}

int CFP::cfpSize(){
	return words.size();
}

