#include "GUI_to_application_processing.h"
#include <iostream>
#include "FirstOrderInst.h"
#include "senoidal_entry.h"
#include <mutex>

GUIToApplicationProcessing::GUIToApplicationProcessing() : number_of_points{ 0 },
in_entry{ nullptr }, currentAllocated{ -1 }, sampling_rate{ 0 }, instrument_order{ -1 },
inst{ nullptr }, response{ nullptr }, static_sensibility{ 0.0 }, tau{ 0.0 }, t{ nullptr }, 
entry_t{ nullptr }, fi{ 0.0 }, omega{0.0}
{
	//std::cout << "Constructor called" << std::endl;
}

GUIToApplicationProcessing::~GUIToApplicationProcessing()
{
	delete[] this->in_entry;
	delete[] this->response;
	delete[] this->t;
}


void GUIToApplicationProcessing::reallocNumPoints(const int &number_of_points)
{
	double* temp{ new double[number_of_points] };
	double* resp_temp{ new double[number_of_points] };
	double* t_temp{ new double[number_of_points] };
	for (size_t i{ 0 }; i < this->number_of_points; i++)
	{
		temp[i] = this->in_entry[i];
		resp_temp[i] = this->response[i];
		t_temp[i] = this->t[i];
	}
	delete[] this->in_entry;
	delete[] this->response;
	delete[] this->t;
	this->currentAllocated = number_of_points;
	this->in_entry = temp;
	this->response = resp_temp;
	this->t = t_temp;
	temp = nullptr;
	resp_temp = nullptr;
	t_temp = nullptr;
}

void GUIToApplicationProcessing::setNumberPoints(const int &number_of_points)
{
	if (number_of_points < 0)
		return;
	extern std::mutex datas_mutex;
	extern double* t;
	extern double* in_entry;
	extern double* out_response;

	datas_mutex.lock();
	t = nullptr;
	in_entry = nullptr;
	out_response = nullptr;
	datas_mutex.unlock();
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
			this->t        = new double[number_of_points];
			this->currentAllocated = number_of_points;
		}
	}
	//std::cout << "Number of points set" << std::endl;
}

void GUIToApplicationProcessing::setSamplingRate(const int &samplingRate)
{
	this->sampling_rate = samplingRate;
}

void GUIToApplicationProcessing::setInst(Instrument* inst)
{
	if (this->inst)
	{
		delete this->inst;
		this->inst = inst;
	}
	else
	{
		this->inst = inst;
	}
	
	// chage this function so it actually change function pointers 
	//that perform tha calculations on the data
}

void GUIToApplicationProcessing::setEntryType(std::string entry_type)
{
	this->entry_type = entry_type;
}

int GUIToApplicationProcessing::getNumberPoints()
{
	return this->number_of_points;
}

void GUIToApplicationProcessing::setInstrument_order(char instrument_order)
{
	this->instrument_order = instrument_order;
}



double* GUIToApplicationProcessing::getT()
{
	return this->t;
}

double* GUIToApplicationProcessing::getInEntry()
{
	return this->in_entry;
}

double* GUIToApplicationProcessing::getResponse()
{
	return this->response;
}

void GUIToApplicationProcessing::setFi(double fi)
{
	this->fi = fi;
}

void GUIToApplicationProcessing::setOmega(double omega)
{
	this->omega = omega;
}

void GUIToApplicationProcessing::simulateInstrument()
{
	this->inst->respond();
}

void GUIToApplicationProcessing::prepareSimulation()
{
	if (this->inst)
	{
		delete this->inst;
		this->inst = nullptr;
	}
	if (this->entry_t)
	{
		delete this->entry_t;
		this->entry_t = nullptr;
	}
	switch (this->instrument_order)
	{
	case 1:
		if (this->entry_type == "senoidal")
		{
			senoidal* temp_senoidal = new senoidal(this->entry_type, this->static_sensibility, this->tau, this->omega, this->fi);
			this->entry_t = temp_senoidal;
			FirstOrderInstrument* temp_foi = new FirstOrderInstrument(this->in_entry, this->response, this->t, this->number_of_points, this->static_sensibility, this->tau, this->sampling_rate, temp_senoidal);
			this->inst = temp_foi;
			temp_foi = nullptr;
			temp_senoidal = nullptr;
		}
		break;
	}
}

void GUIToApplicationProcessing::setStaticSensibility(const int& static_sensibility)
{
	this->static_sensibility = static_sensibility;
}

void GUIToApplicationProcessing::setTau(const double& tau)
{
	this->tau = tau;
}

bool GUIToApplicationProcessing::isNumberOfPointSet()
{
	return (this->number_of_points)>0;
}

bool GUIToApplicationProcessing::isSamplingRateSet()
{
	return (this->sampling_rate) > 0;
}

bool GUIToApplicationProcessing::isInstrumentOrderSet()
{
	return (this->instrument_order) > -1;
}


