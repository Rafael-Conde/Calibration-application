#include "FirstOrderInst.h"
#include <math.h>
#include <iostream>
//para calcular a resposta do seno assistir a gravação da aula do dia 29/11/2021 em 1h25min26s

inline double homogene_solution(double C1, double expoent)
{
	return C1 * exp(expoent);
}

void FirstOrderInstrument::respond()
{
	if ((this->in_entry) && (this->out_response) && (this->t))
	{
		double step{ 1.0 / sampling_rate };
		for (int i{0};i<this->number_of_points;i++)
		{
			t[i] = step * i;
		}
		this->entry_t->apply_entry(this->t, this->in_entry, this->number_of_points, this->sampling_rate);
		for (int i{ 0 }; i < this->number_of_points; i++)
		{
			out_response[i] = homogene_solution(0.0, (-(this->t[i] / this->tau))) + this->entry_t->particular_solution(t[i]);
			//std::cout<<t[i] <<" - "<<i<< "\n";
		}
	}
	else
	{
		std::cout << "one or two of the pointers are a nullptr so the calculation can't proceed." << std::endl;
	}
}

void FirstOrderInstrument::clear_pointers()
{
	this->in_entry = nullptr;
	this->out_response = nullptr;
}

void FirstOrderInstrument::set_t(double* t)
{
	this->t = t;
}

double* FirstOrderInstrument::get_t()
{
	return this->t;
}

void FirstOrderInstrument::set_static_sensibility(double static_sensibility)
{
	this->static_sensibility = static_sensibility;
}

double FirstOrderInstrument::get_static_sensibility()
{
	return this->static_sensibility;
}

void FirstOrderInstrument::set_static_sensibility(int sampling_rate)
{
	this->sampling_rate = sampling_rate;
}

int FirstOrderInstrument::get_sampling_rate()
{
	return this->sampling_rate;
}

void FirstOrderInstrument::set_tau(double tau)
{
	this->tau = tau;
}

double FirstOrderInstrument::get_tau()
{
	return this->tau;
}

void FirstOrderInstrument::set_in_entry(double* in_entry)
{
	this->in_entry = in_entry;
}

double* FirstOrderInstrument::get_in_entry()
{
	return this->in_entry;
}

void FirstOrderInstrument::set_out_response(double* out_response)
{
	this->out_response = out_response;
}

double* FirstOrderInstrument::get_out_response()
{
	return this->out_response;
}

void FirstOrderInstrument::set_number_of_points(int number_of_points)
{
	this->number_of_points = number_of_points;
}

int FirstOrderInstrument::get_number_of_points()
{
	return this->number_of_points;
}

void FirstOrderInstrument::set_entry_t(entry* entry_t)
{
	this->entry_t = entry_t;
}

entry* FirstOrderInstrument::get_entry_t()
{
	return this->entry_t;
}

FirstOrderInstrument::FirstOrderInstrument():
	Instrument{ 1 , nullptr }, in_entry{ nullptr }, out_response{ nullptr }, number_of_points{ -1 }, tau{ 0.01 }, static_sensibility{ 1 }, t{ nullptr }, sampling_rate{500}
{

}

FirstOrderInstrument::FirstOrderInstrument(double* in_entry, double* out_response, double* t, int number_of_points, int sampling_rate, entry* entry_t) :
	Instrument{ 1 , entry_t }, in_entry{ in_entry }, out_response{ out_response }, number_of_points{ number_of_points }, tau{ 0.01 }, static_sensibility{ 1 }, sampling_rate{sampling_rate}, t{ t }
{

}

FirstOrderInstrument::FirstOrderInstrument(double static_sensibility, double tau, entry* entry_t):
	Instrument{ 1, entry_t }, in_entry{ nullptr }, out_response{ nullptr }, number_of_points{ -1 }, tau{ tau }, static_sensibility{ static_sensibility }, sampling_rate{ 500 }, t{nullptr}
{

}

FirstOrderInstrument::FirstOrderInstrument(double* in_entry, double* out_response, double* t, int number_of_points, double static_sensibility, double tau, int sampling_rate, entry* entry_t):
	Instrument{ 1 , entry_t }, in_entry{ in_entry }, out_response{ out_response }, number_of_points{ number_of_points }, tau{ tau }, static_sensibility{ static_sensibility }, sampling_rate{ sampling_rate }, t{t}
{

}

FirstOrderInstrument::~FirstOrderInstrument()
{

}
