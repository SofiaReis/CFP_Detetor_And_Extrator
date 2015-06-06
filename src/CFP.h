#ifndef SRC_CFP_H_
#define SRC_CFP_H_

#include <string>
#include <vector>

using namespace std;

class CFP {
	string name;
	vector<string> words;
public:
	CFP(string name);
	void load();
	vector<string> getCFP();
	int cfpSize();
};

#endif /* SRC_CFP_H_ */
