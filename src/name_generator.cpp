#include "name_generator.h"
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

void MarkovCategorical::observe(size_t data_index, float count)
{
	data.at(data_index) += count;
	total += count;
}

size_t MarkovCategorical::sample()
{
	static std::default_random_engine rand_eng(time(NULL));
	static std::uniform_real_distribution<float> uniform_dist{0, total};
	if(uniform_dist.max() != total)
	{
		uniform_dist = std::uniform_real_distribution<float>{0, total};
	}

	float rand{ uniform_dist( rand_eng ) };
	for(decltype(data.size()) i = 0; i < data.size(); ++i)
	{
		if(data.at(i) >= rand) return i;
		rand -= data.at(i);
	}

	throw std::logic_error("MarkovCategorical::sample()");
}

void MarkovCategorical::print(std::ostream& ostrm) const
{
	auto begin = data.begin();
	unsigned i{ 0 };
	while( begin != data.end())
	{
		ostrm << ' ' << i++ << '=' << *begin++;
	}
}

void MarkovModel::observe(std::string sequence, float count)
{
	sequence = prefix + sequence + postfix;
	for(decltype(sequence.size()) i{ order }; i < sequence.size(); ++i)
	{
		std::string context{ sequence.substr(i - order,order) };
		char event = sequence.at(i);
		categorical(context).observe(event, count);
	}
}

MarkovCategorical& MarkovModel::categorical(std::string context)
{
	if( context.size() != order)throw std::logic_error("MarkovModel::categorical(context) context.size() != order");

	auto iter = data.find(context);
	if(iter == data.end())
	{
		MarkovCategorical cat{ support.size(), prior};
		iter = data.insert( {context, cat} ).first;
	}
	return iter->second;
}

void MarkovModel::print(std::ostream& ostrm) const
{
	auto begin = data.begin();
	while( begin != data.end())
	{
		ostrm << begin->first << "| ";
		begin->second.print(ostrm);
		ostrm << '\n';
	}
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
