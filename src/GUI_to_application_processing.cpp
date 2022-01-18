#include "GUI_to_application_processing.h"
#include <iostream>

GUIToApplicationProcessing::GUIToApplicationProcessing() : number_of_points{ 0 },
points{ nullptr }, currentAllocated{ -1 }, sampling_Rate{ 0 }, instrument_order{ -1 }
{
	//std::cout << "Constructor called" << std::endl;
}

GUIToApplicationProcessing::~GUIToApplicationProcessing()
{
	delete[] this->points;
}


void GUIToApplicationProcessing::reallocNumPoints(const int &number_of_points)
{
	double* temp{ new double[number_of_points] };
	for (size_t i{ 0 }; i < this->number_of_points; i++)
	{
		temp[i] = (this->points)[i];
	}
	delete[] this->points;
	this->currentAllocated = number_of_points;
	this->points = temp;
	temp = nullptr;
}

void GUIToApplicationProcessing::setNumberPoints(const int &number_of_points)
{
	if (number_of_points < 0)
		return;
	
	std::cout << this->points << std::endl;
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
		if (this->points != nullptr)
		{
			this->reallocNumPoints(number_of_points);

		}
		else
		{

			this->points = new double[number_of_points];
			this->currentAllocated = number_of_points;
		}
	}
	//std::cout << "Number of points set" << std::endl;
}

void GUIToApplicationProcessing::setSamplingRate(const int &samplingRate)
{
	this->sampling_Rate = samplingRate;
}

void GUIToApplicationProcessing::setInstOrder(const char &instrument_order)
{
	this->instrument_order = instrument_order;
	// chage this function so it actually change function pointers 
	//that perform tha calculations on the data
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
