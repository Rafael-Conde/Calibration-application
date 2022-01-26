#include "senoidal_entry.h"
#include <math.h>
#include <iostream>
//#include <iostream>


senoidal::senoidal():
	entry{""}, tau{	0.0}, frequency{ 0.0 }, static_sensibility{ 0.0 }, fi{ 0.0 }, phase_angle{ 0.0 }
{

}

senoidal::senoidal(std::string entry_type, double static_sensibility, double tau, double frequency, double fi) :
	entry{ entry_type }, tau{ tau }, frequency{ frequency },  static_sensibility{ static_sensibility }, fi{ fi }, phase_angle{atan(frequency*tau)}
{
	//std::cout << "angle_phase = " << this->phase_angle << std::endl;
}

senoidal::senoidal(std::string entry_type):	
	entry{ entry_type }, tau{ 0.0 }, frequency{ 0.0 }, static_sensibility{ 0.0 }, fi{ 0.0 }, phase_angle{0.0}
{

}

std::string senoidal::get_entry_type()
{
	return this->entry_type;
}

void senoidal::set_entry_type(std::string entry_type)
{
	this->entry_type = entry_type;
}

double senoidal::get_frequency()
{
	return this->frequency;
}

void senoidal::set_frequency(double frequency)
{
	this->frequency = frequency;
	this->phase_angle = this->tau * frequency;
}

double senoidal::get_fi()
{
	return this->fi;
}

void senoidal::set_fi(double fi)
{
	this->fi = fi;
}

double senoidal::get_tau()
{
	return this->tau;
}

void senoidal::set_tau(double tau)
{
	this->tau = tau;
	this->phase_angle = this->frequency * tau;
}

double senoidal::get_static_sensibility()
{
	return this->static_sensibility;
}

void senoidal::set_static_sensibility(double static_sensibility)
{
	this->static_sensibility = static_sensibility;
}

senoidal::~senoidal()
{

}

double senoidal::particular_solution(double t)
{
	//return (  ((this->static_sensibility)*(this->fi))  /  (sqrt(1+(pow((this->tau*this->frequency), 2))  ))  ) * (sin((this->frequency*t)+this->phase_angle));
	//std::cout << "expression = " << this->tau << std::endl;
	return (((this->static_sensibility) * (this->fi)) / (sqrt(1 + ( (this->tau * this->frequency)*(this->tau * this->frequency)  )))) * (sin((this->frequency * t) + this->phase_angle));
}

void senoidal::apply_entry(double* t, double* in_entry, int number_of_points, double sampling_rate)
{
	(void)sampling_rate;
	for (int i{ 0 }; i < number_of_points; i++)
	{
		in_entry[i] = sin(this->frequency*t[i]);
	}
}
