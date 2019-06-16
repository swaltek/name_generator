#include "../src/name_generator.h"
#include <iostream>
#include <vector>

int main()
{
	MarkovCategorical mcat{ 5, 0.001 };

	std::vector<int> count( 5 );
	for(int s = 0; s < 1000; ++s)
	{
		++count.at( mcat.sample() );
	}

	std::cout << mcat;
	std::cout << '\n';

	int index{ 0 };
	for(float f : count) std::cout << ' ' << index << '=' << f;
	std::cout << '\n';
}
