#include "../src/name_generator.h"
#include <iostream>
#include <vector>

int main()
{
	std::set<char> support = { 'a', 'b', 'c', 'd', 'e' };
	std::map<char, int> count;
	MarkovCategorical mcat{ support, 1 };

	for(int s = 0; s < 1000; ++s)
	{
		++count[ mcat.sample() ];
	}

	for(auto i  : count) std::cout << ' ' << i.first << '=' << i.second;
	std::cout << '\n';
}
