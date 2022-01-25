#pragma once
#include "Instrument.h"
#include "entry_t.h"

class GUIToApplicationProcessing
{
	int number_of_points;
	entry* entry_t;
	double* t;
	double* in_entry;
	double* response;
	int currentAllocated;
	int sampling_rate;
	char instrument_order;
	std::string entry_type;
	Instrument* inst;
	/*Instrument parameters*/
	double static_sensibility;
	double tau;
	/*temporary senoidal entry parameters*/
	double omega;
	double fi;
	void reallocNumPoints(const int& number_of_points);
	

public:
	GUIToApplicationProcessing();
	~GUIToApplicationProcessing();

	// setters
	void setNumberPoints(const int& number_of_points);
	void setSamplingRate(const int& samplingRate);
	void setInst(Instrument* inst);
	void setInstrument_order(char instrument_order);
	void setStaticSensibility(const int& static_sensibility);
	void setTau(const int& tau);
	void setFi(double fi);
	void setOmega(double omega);
	//void set_buffers(double* t, double* in_entry, double* out_response);
	//getters
	int getNumberPoints();
	double* getT();
	double* getInEntry();
	double* getResponse();

	//other methods
	void prepareSimulation();
	void simulateInstrument();
	bool isNumberOfPointSet();
	bool isSamplingRateSet();
	bool isInstrumentOrderSet();



};