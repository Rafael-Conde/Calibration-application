#pragma once

class GUIToApplicationProcessing
{
	int number_of_points;
	double* points;
	int currentAllocated;
	int sampling_Rate;
	char instrument_order;
	void reallocNumPoints(const int& number_of_points);

public:
	GUIToApplicationProcessing();
	~GUIToApplicationProcessing();

	// setters
	void setNumberPoints(const int& number_of_points);
	void setSamplingRate(const int& samplingRate);
	void setInstOrder(const char& instrument_order);
	//getters

	//other methods
	bool isNumberOfPointSet();
	bool isSamplingRateSet();
	bool isInstrumentOrderSet();



};