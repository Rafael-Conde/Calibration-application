#include "GUI_to_application_processing.h"
#include <iostream>

GUIToApplicationProcessing::GUIToApplicationProcessing() : number_of_points{ 0 },
in_entry{ nullptr }, currentAllocated{ -1 }, sampling_Rate{ 0 }, instrument_order{ -1 },
inst{ nullptr }, response{ nullptr }, static_sensibility{ 0.0 }, tau{ 0.0 }, t{nullptr}
{
	//std::cout << "Constructor called" << std::endl;
}

GUIToApplicationProcessing::~GUIToApplicationProcessing()
{
	delete[] this->in_entry;
}


void GUIToApplicationProcessing::reallocNumPoints(const int &number_of_points)
{
	double* temp{ new double[number_of_points] };
	double* resp_temp{ new double[number_of_points] };
	for (size_t i{ 0 }; i < this->number_of_points; i++)
	{
		temp[i] = this->in_entry[i];
		resp_temp[i] = this->response[i];
	}
	delete[] this->in_entry;
	delete[] this->response;
	this->currentAllocated = number_of_points;
	this->in_entry = temp;
	this->response = resp_temp;
	temp = nullptr;
	resp_temp = nullptr;
}

void GUIToApplicationProcessing::setNumberPoints(const int &number_of_points)
{
	if (number_of_points < 0)
		return;
	
	//std::cout << this->points << std::endl;
	this->number_of_points = number_of_points;
	if (number_of_points <= this->currentAllocated)
	{
		if ((this->currentAllocated - number_of_points)>= 1'000)
		{
			this->reallocNumPoints(number_of_points);

			//std::cout << "Number of points realloced2" << std::endl;
		}			
	}
	else
	{
		if (this->in_entry != nullptr)
		{
			this->reallocNumPoints(number_of_points);

		}
		else
		{

			this->in_entry = new double[number_of_points];
			this->response = new double[number_of_points];
			this->currentAllocated = number_of_points;
		}
	}
	//std::cout << "Number of points set" << std::endl;
}

void GUIToApplicationProcessing::setSamplingRate(const int &samplingRate)
{
	this->sampling_Rate = samplingRate;
}

void GUIToApplicationProcessing::setInst(Instrument* inst)
{
	this->inst = inst;
	// chage this function so it actually change function pointers 
	//that perform tha calculations on the data
}

void GUIToApplicationProcessing::setStaticSensibility(const int& static_sensibility)
{
	this->static_sensibility = static_sensibility;
}

void GUIToApplicationProcessing::setTau(const int& tau)
{
	this->tau = tau;
}

bool GUIToApplicationProcessing::isNumberOfPointSet()
{
	return (this->number_of_points)>0;
}

bool GUIToApplicationProcessing::isSamplingRateSet()
{
	return (this->sampling_Rate) > 0;
}

bool GUIToApplicationProcessing::isInstrumentOrderSet()
{
	return (this->instrument_order) > -1;
}

void GUIToApplicationProcessing::set_buffers(double* t, double* in_entry, double* out_response)
{
	this->t = t;
	this->in_entry = in_entry;
	this->response = out_response;
}
