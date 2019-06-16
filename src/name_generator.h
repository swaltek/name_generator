#include <ostream>
#include <map>
#include <set>
#include <vector>

#include <iostream>

class MarkovCategorical
{
public:
	MarkovCategorical(std::set<char> support, float prior)
		: total(support.size() * prior)
	{
		for(char c : support)
		{
			data[c] = prior;
		}
	}

	void observe(char c, float count = 1.0f);
	size_t sample();

	void print(std::ostream& ostrm) const;
private:
	float total;
	std::map<char, float> data;
};

class MarkovModel
{
public:
	MarkovModel(const std::set<char>& support_data,unsigned model_order,float data_prior, char word_boundary_char = '_')
		: support(support_data), order(model_order), prior(data_prior), boundary_char(word_boundary_char), prefix(model_order, word_boundary_char), postfix(word_boundary_char)
	{
		support.insert(boundary_char);
	}

	void observe(std::string sequence, float count = 1);

	void print(std::ostream& ostrm) const;
private:
	MarkovCategorical& categorical(std::string context);

	const unsigned order; //how many prevouse characters will influence selction of next character
	const float prior; //base chance of any character being selected

	const char boundary_char;
	const std::string prefix;
	const char postfix;

	std::set<char> support; //all character used in model
	std::map<std::string, MarkovCategorical> data;
};
std::ostream& operator<<(std::ostream& ostrm, const MarkovCategorical& mc);
std::ostream& operator<<(std::ostream& ostrm, const MarkovModel& mm);

/*
class NameGenerator
{
};
*/
