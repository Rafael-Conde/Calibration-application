#pragma once
#include "Instrument.h"

class GUIToApplicationProcessing
{
	int number_of_points;
	double* t;
	double* in_entry;
	double* response;
	int currentAllocated;
	int sampling_Rate;
	char instrument_order;
	Instrument* inst;
	double static_sensibility;
	double tau;

	void reallocNumPoints(const int& number_of_points);
	

public:
	GUIToApplicationProcessing();
	~GUIToApplicationProcessing();

	// setters
	void setNumberPoints(const int& number_of_points);
	void setSamplingRate(const int& samplingRate);
	void setInst(Instrument* inst);
	void setinstrument_order(char instrument_order);
	void setStaticSensibility(const int& static_sensibility);
	void setTau(const int& tau);
	void set_buffers(double* t, double* in_entry, double* out_response);
	//getters

	//other methods
	bool isNumberOfPointSet();
	bool isSamplingRateSet();
	bool isInstrumentOrderSet();



};