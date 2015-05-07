#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
using namespace std;

class settings {
    private:
		map<string, string> Settings;
		string fname;
		void save();
		void CheckFile();
    public: 		
		settings(std::string const & filename);
		
		std::string const & get(std::string const & name, 
			std::string const & def = "") const;
		
		void set(std::string const & name, 
			std::string const & value);
		
		void reset();		
		
		void reload();
 
};