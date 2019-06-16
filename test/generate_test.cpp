#include  "../src/name_generator.h"
#include <cctype>
#include <fstream>
#include <iostream>
#include <set>
int main()
{
	std::set<char> support;
	std::vector<std::string> data;
	std::ifstream fstrm{ "data/imperial_male_names.txt" };

	std::string input;
	while(fstrm >> input)
	{
		for(char c : input) support.insert(tolower(c));
		data.push_back(input);
	}

	MarkovModel m{support, 3, 0.001};
	for(std::string str : data) m.observe(str);

	for(int i{ 0 }; i < 200; ++i)
	{
		std::cout << m.generate();

		if( i % 5 == 0 && i != 0) std::cout << '\n';
		else std::cout << ' ';
	}
}
