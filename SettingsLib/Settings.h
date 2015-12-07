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
		// Main functions
 
		/**
		 * Construct settings store 
		 * and load data from file (if exists)
		 * \param filename Path to file with settings
		 */
		settings(std::string const & filename);
		/**
		 * Get setting value
		 * \param name Setting unique identifier
		 * \param def Default setting value
		 * \return Stored value for given name or default value
		 */
		std::string const & get(std::string const & name, 
			std::string const & def = "") const;
		/**
		 * Set or replace setting value and save changes to file
		 * \param name Setting unique identifier
		 * \param value New setting value
		 */
		void set(std::string const & name, 
			std::string const & value);
		/**
		 * Reset all settings
		 */
		void reset();		
		/**
		 * Reload all settings from file
		 */	
		void reload();
};
