#include <Instrument.h>

class FirstOrderInstrument : public Instrument
{
protected:
	double static_sensibility;
	double tau;
	double* in_entry;
	double* out_response;
	int number_of_points;
	int sampling_rate;
	double* t;
public:
	virtual void respond() override;
	void clear_pointers();
	/*-----------------------setters and getters----------------------------*/
	// mover boa parte desses métodos para a classe abstrata Instrument
	void set_t(double* t);
	double* get_t();
	void set_static_sensibility(double static_sensibility);
	double get_static_sensibility();
	void set_static_sensibility(int sampling_rate);
	int get_sampling_rate();
	void set_tau(double tau);
	double get_tau();
	void set_in_entry(double* in_entry);
	double* get_in_entry();
	void set_out_response(double* out_response);
	double* get_out_response();
	void set_number_of_points(int number_of_points);
	int get_number_of_points();
	void set_entry_t(entry* entry_t);
	entry* get_entry_t();
	/*---------------------------------------------------*/
	FirstOrderInstrument();
	FirstOrderInstrument(double* in_entry,double* out_response, double* t, int number_of_points, int sampling_rate, entry* entry_t);
	FirstOrderInstrument(double static_sensibility, double tau, entry* entry_t);
	FirstOrderInstrument(double* in_entry, double* out_response, double* t, int number_of_points, double static_sensibility, double tau, int sampling_rate, entry* entry_t);
	virtual ~FirstOrderInstrument();
};