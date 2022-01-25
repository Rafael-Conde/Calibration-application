#include <string>
#include "entry_t.h"

class senoidal : public entry
{
	double fi;
	double phase_angle;
	double static_sensibility;
	double tau;
	double frequency;



public:
	senoidal();
	senoidal(std::string entry_type, double static_sensibility, double tau, double frequency, double fi);
	senoidal(std::string entry_type);

	/*--------------------------Getters and Setters--------------------------*/

	std::string get_entry_type();
	void set_entry_type(std::string entry_type);
	double get_frequency();
	void set_frequency(double frequency);
	double get_fi();
	void set_fi(double fi);
	double get_tau();
	void set_tau(double tau);
	double get_static_sensibility();
	void set_static_sensibility(double static_sensibility);


	/*-----------------------------------------------------------------------*/

	virtual ~senoidal();
	virtual double particular_solution(double t) override;
	virtual void apply_entry(double* t, double* in_entry, int number_of_points, double sampling_rate) override;
};