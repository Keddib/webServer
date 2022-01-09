#ifndef LOCATION_001_HPP
#define LOCATION_001_HPP

#include <string>
#include <iostream>
#include <vector>


class Location
{
	private:
		std::string _prefix;
		std::string _root;
		std::vector<std::string> _indexFiles;
	public:
		Location();
		Location(const Location &);
		Location& operator=(const Location &);
		~Location();
		// getters
		const std::string& getRoot() const;
		// setters
};

#endif
