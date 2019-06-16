#include <ostream>
#include <map>
#include <set>
#include <vector>

class MarkovCategorical
{
public:
	MarkovCategorical(size_t support_data_size, float prior)
		: total(support_data_size * prior), data(support_data_size, prior) {}

	void observe(size_t data_index, float count = 1.0f);
	size_t sample();

	void print(std::ostream& ostrm) const;
private:
	float total;
	std::vector<float> data;
};

class MarkovModel
{
public:
	MarkovModel(const std::set<char>& support_data,unsigned model_order,float prior, char boundary_char = '\0')
		: support(support_data), order(model_order), prior(prior), boundary_char(boundary_char), prefix(boundary_char, model_order), postfix(boundary_char)
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
