#include "GUI_to_application_processing.h"
#include <mutex>
//#include <iostream>


extern GUIToApplicationProcessing guiToAppProcess;



void ok_graph_button_thread()
{
	extern bool order_combo;

	extern std::mutex sampling_freq_mutex;
	extern int sampling_frequency;


	extern std::mutex num_of_points_mutex;
	extern int number_of_points;
	num_of_points_mutex.lock();
	guiToAppProcess.setNumberPoints(number_of_points);
	num_of_points_mutex.unlock();
	sampling_freq_mutex.lock();
	guiToAppProcess.setSamplingRate(sampling_frequency);
	sampling_freq_mutex.unlock();
	order_combo = true;
}

void ok_eq_param_thread()
{
	extern std::mutex tau_mutex;
	extern double tau;
	extern std::mutex static_sensibility_mutex;
	extern double static_sensibility;
	extern char current_selected;
	tau_mutex.lock();
	guiToAppProcess.setTau(tau);
	tau_mutex.unlock();
	static_sensibility_mutex.lock();
	guiToAppProcess.setStaticSensibility(static_sensibility);
	static_sensibility_mutex.unlock();
	switch (current_selected)
	{
	case 0:
		guiToAppProcess.setInstrument_order(1);
		break;
	default:
		break;
	}
	//std::cout << "Everything is setup!" << std::endl;
}
