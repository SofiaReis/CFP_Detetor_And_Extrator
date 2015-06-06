#include <sys/types.h>
#include <sys/stat.h>
#include "Message.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <algorithm>

using namespace std;

Message::Message(string name) {
	this->name = name;
}

void Message::loadMsg(){
	string str;
	ifstream file(this->name);

	if(file.is_open()){
		while (getline(file, str)) {
			if(!str.empty()){
				lines.push_back(convertToLowerCase(str));
				lns.push_back(str);
			}
		}
	}
	file.close();
}

void Message::loadCountries(){
	string str;
	ifstream file("countries.txt");
	if(file.is_open()){
		while (getline(file, str)) {
			if(!str.empty()){
				countries.push_back(convertToLowerCase(str));
			}
		}
	}
	file.close();
}


void Message::loadMonths(){
	string str;
	ifstream file("months.txt");
	if(file.is_open()){
		while (getline(file, str)) {
			if(!str.empty()){
				months.push_back(convertToLowerCase(str));
			}
		}
	}
	file.close();
}

bool Message::fileExists(string filename){
	struct stat buf;
	if (stat(filename.c_str(), &buf) == 0) {
		//file does not exists
		return false;
	}
	else
		return true;
}

vector<string> Message::getMsgLines(){
	return this->lines;
}

void Message::splitLine(string line, string delimiter){

	size_t current = 0, found;

	while((found = line.find_first_of(delimiter, current)) != string::npos){

		if(wordExists(string(line, current, found - current))==false && string(line, current, found - current)!= " "){
			msgWords.push_back(string(line, current, found - current));
		}
		current = found + 1;
	}
	if(wordExists(string(line, current, line.size() - current))==false){
		msgWords.push_back(string(line, current, line.size() - current));
	}
}

void Message::splitMsg(){
	for(unsigned int i = 0; i < lines.size(); i++){
		string ln = lines[i].c_str();
		splitLine(ln, " .±§!@#€$%&/()=?'*+ºª´`|^~-_:;,><\"");
	}
}

bool Message::iequals(const std::string& a, const std::string& b)
    {
        size_t sz = a.length();
        if (b.length() != sz)
            return false;
        for (size_t i = 0; i < sz; ++i)
            if (tolower(a[i]) != tolower(b[i]))
                return false;
        return true;
    }

bool Message::wordExists(string word){
	for(unsigned int i = 0; i < msgWords.size(); i++){
		if(msgWords[i] == word){
			return true;
		}
	}
	return false;
}

bool Message::wExists(string word, vector<string> &strVector){
	for(unsigned int i = 0; i < strVector.size(); i++){
		if(strVector[i] == word){
			return true;
		}
	}
	return false;
}

int Message::findWord(string word, vector<string> &strVector){
	for(unsigned int i = 0; i < strVector.size(); i++){
		if(strVector[i] == word){
			return i;
		}
	}
	return -1;
}

vector<string> Message::getMsgWords(){
	return msgWords;
}

string Message::convertToLowerCase(string word){
	transform(word.begin(), word.end(), word.begin(), ::tolower);
	return word;
}

size_t Message::levenshteinDistance(string &s1, string &s2){

	const size_t m(s1.size());
	const size_t n(s2.size());

	if( m==0 ) return n;
	if( n==0 ) return m;

	size_t *costs = new size_t[n + 1];

	for( size_t k=0; k<=n; k++ ) costs[k] = k;

	size_t i = 0;
	for (string::const_iterator it1 = s1.begin(); it1 != s1.end(); ++it1, ++i )
	{
		costs[0] = i+1;
		size_t corner = i;

		size_t j = 0;
		for (string::const_iterator it2 = s2.begin(); it2 != s2.end(); ++it2, ++j )
		{
			size_t upper = costs[j+1];
			if( *it1 == *it2 )
			{
				costs[j+1] = corner;
			}
			else
			{
				size_t t(upper<corner?upper:corner);
				costs[j+1] = (costs[j]<t?costs[j]:t)+1;
			}

			corner = upper;
		}
	}

	size_t result = costs[n];
	delete [] costs;

	return result;
}

int Message::considerError(string &s1, string &s2, vector<string> dic){

	float str1S = stringMax(s1,s2).size();
	float str2S = stringMin(s1,s2).size();
	float ratio = 0;

	ratio = (str1S/str2S)*levenshteinDistance(s1,s2);

	if(levenshteinDistance(s1,s2) == 0) return 0;
	if(s1.size() == s2.size()){
		if(levenshteinDistance(s1,s2) < 3) return levenshteinDistance(s1,s2);
		else return (1/4)*(s1.size());
	}
	else{
		return ratio;
	}

}

bool Message::verifyIfCFP(vector<string> dic){

	int count = 0;
	vector<string> ind;
	float prob;
	float n = dic.size();

	for(unsigned int i = 0; i < msgWords.size(); i++){
		for(unsigned int j = 0; j < dic.size(); j++){
			if(msgWords[i] == dic[j] && levenshteinDistance(msgWords[i], dic[j]) <= considerError(msgWords[i], dic[j], dic)){
				ind.push_back(msgWords[i]);
				count++;
			}
		}
	}

	prob = count/n;
	if(prob > 0.70){
		return true;
	}
	else return false;
}

string Message::stringMin(string &str1, string &str2){

	if(str1.size() < str2.size()) return str1;
	else return str2;
}

string Message::stringMax(string &str1, string &str2){

	if(str1.size() > str2.size()) return str1;
	else return str2;
}

string Message::getSubject(){

	for(unsigned int i = 0; i < lns.size(); i++){
		if(lns[i].find("Subject: ") != string::npos){
			return lns[i].substr(8, lns[i].size()-8);
		}
	}
}


vector<string> Message::getInfoData(){

	int date = 0;
	int found = 0;
	vector<string> dates;

	for(unsigned int i = 0; i < lines.size(); i++){
		if(existsDeadlines() == true && indDeadlines() != -1){
			date = indDeadlines()+1;
			break;
		}
	}

	for(unsigned int i = date; i < date+20; i++){
		if((lines[i].find("-") != string::npos && hasMonth(lines[i]) == true) ||
				(lines[i].find("/") != string::npos && checkYear(lines[i]) == true)
				|| (checkYear(lines[i]) == true && hasMonth(lines[i]) == true)
				|| (lines[i].find(":") != string::npos && hasMonth(lines[i]) == true)){
			dates.push_back(lines[i]);
		}
	}

	return dates;

}

bool Message::checkYear(string &str)
{
	for(unsigned int i = 2000; i < 200; i++){
		if(str.find(i) != string::npos){
			return true;
		}
	}
	return false;
}

bool Message::hasMonth(string &str){

	for(unsigned int i = 0; i< months.size(); i++){
		if(str.find(months[i]) != string::npos){
			return true;
		}
	}
	return false;
}

bool Message::existsDeadlines(){
	for(unsigned int i = 0; i < lines.size(); i++){
		if(lines[i].find("submission") != string::npos || lines[i].find("important dates") != string::npos){
			return true;
		}
	}
	return false;
}

bool Message::existsEventDate(){

	for(unsigned int i = 0; i < lines.size(); i++){
		for(unsigned int j = 0; j < months.size(); j++){
			if(lines[i].find(months[j]) != string::npos){
				return true;
			}
		}
	}
	return false;
}

bool Message::existsTopics(){
	for(unsigned int i = 0; i < lines.size(); i++){
			if(lines[i].find("topics of interest") != string::npos || (lines[i].find("topics") != string::npos && lines[i+2].find("- ")!= string::npos) ||
					(lines[i].find("topics") != string::npos && lines[i+2].find(") ")!= string::npos)){
				return true;
			}
		}
		return false;
}

bool Message::localEvent(){

	for(unsigned int i = 0; i < lines.size(); i++){
		for(unsigned int j = 0; j < countries.size(); j++){
			if(lines[i].find(countries[j]) != string::npos){
				return true;
			}
		}
	}
	return false;
}

string Message::getLocal(){

	string str;

	for(unsigned int i = 0; i < lines.size(); i++){
		for(unsigned int j = 0; j < countries.size(); j++){
			for(unsigned int k = 0; k < months.size(); k++){
				if((lines[i].find(countries[j]) != string::npos && lines[i].find(months[k]) != string::npos) ||
						(lines[i].find(countries[j]) != string::npos && lines[i].find(countries[j+1]) != string::npos)){
					if(lines[i].find(countries[j]) != string::npos){
						str = countries[j];
					}
					else if(lines[i].find(countries[j]) != string::npos && lines[i].find(countries[j+1]) != string::npos)
					{
						str = countries[j] + " " + countries[j+1];
					}
					return str;
				}
			}
		}
	}
}

int Message::indTopics(){
	for(unsigned int i = 0; i < lines.size(); i++){
		if(lines[i].find("topics of interest") != string::npos || lines[i].find("topics") != string::npos){
			return i;
		}
	}
	return -1;
}

int Message::indDeadlines(){

	string s;
	for(unsigned int i = 0; i < lines.size(); i++){
		s = lines[i+2];
			if((lines[i].find("submission") != string::npos || lines[i].find("important dates") != string::npos) && hasMonth(s) == true){
				return i;
			}
		}
		return -1;
}

vector<string> Message::getTopics(){

	int j = 0;
	int found = 0;

	for(unsigned int i = 0; i < lines.size(); i++){
		if(existsTopics() == true && indTopics() != -1){
			j = indTopics()+1;
		}
	}

	for(unsigned int i = j; i < j+30; i++){

		if(lines[i].find("submission") != string::npos || lines[i].find("important dates") != string::npos){
			found = 1;
		}
		if((lines[i].find(" -") != string::npos || lines[i].find(") ") != string::npos || lines[i].find("- ") != string::npos ) && found!=1){
			topics.push_back(lns[i]);
		}
	}

	return topics;
}


