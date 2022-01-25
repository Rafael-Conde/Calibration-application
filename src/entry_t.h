#pragma once
#include <string>

class entry
{
protected:
	std::string entry_type;

public:
	
	virtual double particular_solution(double t) = 0;
	virtual void apply_entry(double* t, double* in_entry, int number_of_points, double sampling_rate) = 0;
	entry(std::string entry_type);
	virtual ~entry();
};