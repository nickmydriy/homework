#include "settings.h"
#include <string.h>
using namespace std;

class SettingsTests {	
private :
	string testresult[6];
	string Compare(string a, string b) {
		if (a != b) {
			return "failed"; 
		} else return "OK";
	}
	void Test() {	
		string sett[7] = {"1dfd", "2wewer","weeDGdG234","werxcgew","wefxewfwzfx","ferfxewazfewf","ewFDDrewfea"};
		string param[7] = {"23fdgrecfrfr", "weweewweEEFFD","00000","322","32423vd32e","xefweafsexg","wefergcezwewxefderfewewfg"};
		string def = "dwfrsdgdfqxrdg";
		try {
			settings *nsett = new settings("stest.cfg");
			testresult[0] = "OK";
		} catch (exception) {
			testresult[0] = "Settings create failed!";
		}	
		settings *nsett = new settings("stest.cfg");
		try {
			for (int i = 0; i < 7; i++) {
				nsett->set(sett[i], param[i]);
			}
			testresult[1] = "OK";
		} catch (exception) {
			testresult[1] = "Settings set failed!";
		}
		try {
			for (int i = 0; i < 7; i++) {
				string res = Compare(param[i], nsett->get(sett[i], def));
				testresult[2] = res;
			}
		} catch (exception) {
			testresult[2] = "error";
		}

		nsett->reload();

		for (int i = 0; i < 7; i++) {
			string res = Compare(param[i], nsett->get(sett[i], def));
			testresult[3] = res;
		}

		nsett->reset();

		for (int i = 0; i < 7; i++) {
			string res = Compare(def, nsett->get(sett[i], def));
			testresult[4] = res;
		}

		delete nsett;
		nsett = new settings("stest.cfg");

		for (int i = 0; i < 7; i++) {
			string res = Compare(def, nsett->get(sett[i], def));
			testresult[4] = res;
		}

		for (int i = 0; i < 7; i++) {
			nsett->set(sett[i], param[i]);
		}

		delete nsett;
		nsett = new settings("stest.cfg");

		for (int i = 0; i < 7; i++) {
			string res = Compare(param[i], nsett->get(sett[i], def));
			testresult[5] = res;
		}
	}
public :
	SettingsTests() {		
	}
	void runTest() {
		Test();
		if (testresult[0] != "OK") {
			cout << testresult[0];
		} else {
			if (testresult[1] != "OK") {
				cout << testresult[1];
			} else {
				string testmessage[6] = {
					"settings create", 
					"settings set",
					"settings get",
					"settings reload",
					"settings reset",
					"settings save"
				};
				for (int i = 0; i < 6; i++) {
					cout << "Testing " << testmessage[i] << " : " << testresult[i] << endl;
				}
			}
		}
	}
};

int main() {
	SettingsTests *tests = new SettingsTests();
	tests->runTest();
}


