#include "name_generator.h"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

void MarkovCategorical::observe(char c, float count)
{
	data[c] += count;
	total += count;
}

char MarkovCategorical::sample() const
{
	static std::default_random_engine rand_eng(time(NULL));
	static std::uniform_real_distribution<float> uniform_dist{0, total};
	if(uniform_dist.max() != total)
	{
		uniform_dist = std::uniform_real_distribution<float>{0, total};
	}

	float rand{ uniform_dist( rand_eng ) };
	for(auto begin{ data.begin()}; begin != data.end(); ++begin)
	{
		if(begin->second >= rand) return begin->first;
		rand -= begin->second;
	}

	throw std::logic_error("MarkovCategorical::sample()");
}

void MarkovCategorical::print(std::ostream& ostrm) const
{
	auto begin = data.begin();
	while( begin != data.end())
	{
		if(begin->second > (total/data.size())) //only print values that have a substantial value
		{
			ostrm << ' ' << begin->first  << '=' << begin->second;
		}
		++begin;
	}
}

std::string MarkovModel::generate() const
{
	std::string sequence = prefix;
	do
	{
		sequence.push_back(sample(sequence));
	}
	while(sequence.back() != boundary_char);

	//remove prefix and postfix
	sequence.erase(sequence.begin(), sequence.begin() + order);
	sequence.erase(sequence.end() - 1, sequence.end());
	return sequence;
}

void MarkovModel::observe(std::string sequence, float count)
{
	sequence = prefix + sequence + postfix;
	for(decltype(sequence.size()) i{ order }; i < sequence.size(); ++i)
	{
		std::string context{ sequence.substr(i - order,order) };
		char event = sequence.at(i);
		//adds not only 012 but 12 and 2 where 0,1,2, is indexes of sequence
		//allows model to not be stuck when generating a sequence of 3 that was not observed
		for(unsigned i{ 0 }; i < order; ++i)
		{
			std::string observed_context =  context.substr(i);
			categorical(observed_context).observe( event, count);
		}
	}
}

const MarkovCategorical& MarkovModel::find_categorical(std::string context) const
{
	if(context.size() > order)
	{
		//trim start
		context = context.substr(context.size() - order);
	}
	else if(context.size() < order)
	{
		//add to start
		context = std::string(order - context.size(), boundary_char) + context;
	}
	
	auto iter = data.find(context);
	while(iter == data.end())
	{
		context = context.substr(1);
		iter = data.find(context);
	}
	return iter->second;
}

MarkovCategorical& MarkovModel::categorical(std::string context)
{
	if( context.size() > order)throw std::logic_error("MarkovModel::categorical(context) context.size() != order");

	auto iter = data.find(context);
	if(iter == data.end())
	{
		MarkovCategorical cat{ support, prior};
		iter = data.insert( {context, cat} ).first;
	}
	return iter->second;
}

void MarkovModel::print(std::ostream& ostrm) const
{
	auto begin = data.begin();
	while( begin != data.end())
	{
		ostrm << begin->first << "| " << begin->second << '\n';
		++begin;
	}
}

char MarkovModel::sample(std::string context) const
{
	return find_categorical(context).sample();
}

std::ostream& operator<<(std::ostream& ostrm, const MarkovCategorical& mc)
{
	mc.print(ostrm);
	return ostrm;
}

std::ostream& operator<<(std::ostream& ostrm, const MarkovModel& mm)
{
	mm.print(ostrm);
	return ostrm;
}
