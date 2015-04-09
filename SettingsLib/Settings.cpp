#include "Settings.h"

settings::settings(std::string const & filename) {
	fname = filename;
	reload();
}

void settings::reload() {
	CheckFile();
	Settings.clear();
	ifstream cin (fname);
	string set, value;
	while (cin) {
		getline(cin, set, ' ');
		getline(cin, value);
		Settings[set] = value;
	}
	cin.close();
}

std::string const & settings::get(std::string const & name, std::string const & def = "") const {
	try {
		return Settings.at(name);
	} catch(out_of_range) {
		return def;
	}
}

void settings::set(std::string const & name, std::string const & value) {
	CheckFile();
	Settings[name] = value;
	settings::save();
}

void settings::save() {
	CheckFile();
	ofstream cout (fname);
	map <string,string>::iterator temp;
	for (temp = Settings.begin(); temp != Settings.end(); temp++) {
		cout << (*temp).first << " " << (*temp).second << endl;
	}
	cout.close();
}

void settings::reset() {
	CheckFile();
	ofstream cout (fname);
	Settings.clear();
	cout.close();
}

void settings::CheckFile() {
	if (fname == "") {
		fname = "default.cfg";
	}
}