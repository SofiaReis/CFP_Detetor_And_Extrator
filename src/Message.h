#ifndef SRC_MESSAGE_H_
#define SRC_MESSAGE_H_

#include <string>
#include <vector>

using namespace std;

class Message {

	// NAME FILE
	string name;

	// CFP INFOS
	string subject;
	string submission_date;
	string organization;
	vector<string> topics;

	vector<string> months;
	vector<string> countries;

	// FILES
	vector<string> lines;
	vector<string> lns;
	vector<string> msgWords;

public:
	Message(string name);
	void loadMsg();
	void loadMonths();
	vector<string> getMsgLines();
	bool fileExists(string filename);
	void splitLine(string line, string delimiter);
	void splitMsg();
	bool wordExists(string word);
	vector<string> getMsgWords();
	string convertToLowerCase(string word);
	bool iequals(const string& a, const string& b);
	size_t levenshteinDistance(string &s1, string &s2);
	bool verifyIfCFP(vector<string> dic);
	string stringMin(string &str1, string &str2);
	string stringMax(string &str1, string &str2);
	bool wExists(string word, vector<string> &strVector);
	int findWord(string word, vector<string> &strVector);
	int considerError(string &s1, string &s2, vector<string> dic);
	string getSubject();
	vector<string> getInfoData();
	vector<string> getTopics();
	string getLocal();
	string getDateEvent();
	int indTopics();
	int indDeadlines();
	bool existsDeadlines();
	bool existsTopics();
	bool checkIfStringAsDate(string str);
	bool hasMonth(string &str);
	bool existsEventDate();
	bool localEvent();
	void loadCountries();
	string detectDate(string str);
	bool checkYear(string &str);

};

#endif /* SRC_MESSAGE_H_ */
