#include "Message.h"
#include <iostream>
#include <iomanip>
#include "CFP.h"

using namespace std;

int main(){

	string n;

	cout << endl << endl;
	cout << "   ---------------------------------------------------------" << endl;
	cout << "   |                     CALL FOR PAPERS                   |" << endl;
	cout << "   ---------------------------------------------------------" << endl;
	cout << "    Choose a number of the message you want to analyse: ";
	cin >> n;
	cout << endl;

	string msg_name = "msg" + n + ".txt";

	string cfpFile = "cfp.txt";
	vector<string> dictionary;
	CFP cfp = CFP(cfpFile);
	cfp.load();
	dictionary = cfp.getCFP();

	Message msg = Message(msg_name);
	msg.loadMsg();
	msg.loadMonths();
	msg.loadCountries();
	msg.splitMsg();

	if(msg.verifyIfCFP(dictionary) == true){

		cout << "   ---------------------------------------------------------------------" << endl;
		cout << "   |                CALL FOR PAPERS - INFORMATION RESUM                |" << endl;
		cout << "   ---------------------------------------------------------------------" << endl << endl;

		cout << "      SUBJECT: " << endl;
		cout << "      --------" << endl;
		cout << "     " << msg.getSubject() << endl << endl;

		if(msg.localEvent()){
			cout << "      WHERE: " << endl;
			cout << "      ------" << endl;
			cout << "      " << msg.getLocal() << endl << endl;
		}

		if(msg.existsDeadlines()){
			cout << "      DEADLINES: " << endl;
			cout << "      ----------" << endl;
			vector<string> dates = msg.getInfoData();
			for(unsigned int i = 0; i<dates.size();i++){
				cout << "      " << dates[i] << endl;
			}
		}

		if(msg.existsTopics()){
			vector<string> tpcs = msg.getTopics();
			cout << endl << "      TOPICS: " << endl;
			cout << "      -------" << endl;

			for(unsigned int i = 0; i<tpcs.size();i++){
				cout << "      " << tpcs[i] << endl;
			}
		}
	}
	else if(msg.fileExists(msg_name) == true) {
		cout << "    ERROR: SORRY! THE CHOOSEN MESSAGE DOES NOT EXISTS!" << endl;
	}
	else if(msg.fileExists(msg_name) == false){
		cout << "    CHOOSEN MESSAGE IT'S NOT A CALL FOR PAPERS! SORRY." << endl;
	}

	return 0;
}









