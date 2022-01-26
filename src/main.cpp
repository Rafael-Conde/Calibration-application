#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <math.h>
#include <GUI_to_application_processing.h>
#include <thread_functions.h>
#include "FirstOrderInst.h"
#include "senoidal_entry.h"




#include <limits>



#include <GL/glew.h>
#define GLFW_EXPOSE_NATIVE_WIN32

#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <GLFW/glfw3native.h>
#include <windows.h>
#include <vector>

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static char window__name[] = "Calibracao";
extern std::mutex counter_mutex;
extern int user_input_counter;

std::mutex sampling_freq_mutex{};
int sampling_frequency{ 500 };


std::mutex num_of_points_mutex{};
int number_of_points{ 1000 };

std::mutex tau_mutex{};
double tau{ 0.01 };
std::mutex static_sensibility_mutex{};
double static_sensibility{ 1 };
extern char current_selected{ 0 };

FirstOrderInstrument first_order_instrument{};


std::mutex datas_mutex{};
double* t{ nullptr };
double* in_entry{ nullptr };
double* out_response{ nullptr };

float angular_frequency{ static_cast<float>(2*acos(-1))};
float amplitude{ 1 };


GUIToApplicationProcessing guiToAppProcess{};



/*bools for IMGUI*/

bool order_combo{ false };
bool show_eq_param_window{ false };
bool entry_window{ false };
bool plot_button{ false };
bool seno_settings_window{ false };

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

/*------------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------------*/


int main(int, char**)
{
	//std::cout << time_reference.time_since_epoch().count() << std::endl;
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);// make it possible that the 
													  // window is transparent
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// Create window with graphics context
	GLFWwindow* window = glfwCreateWindow(1280, 720, window__name, NULL, NULL);
	if (window == NULL)
		return 1;
	glfwMakeContextCurrent(window);
	bool VSync = true;
	bool VSync_changed = VSync;
	glfwSwapInterval(VSync); // Enable vsync

	//glfwHideWindow(window); // hides the window but not the console window that generated this one.

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	//ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	// Our state
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	char textText[24]{};
	strcpy_s(textText, "Counter = %d");


	

	
	MSG msg;
	// Main loop
	while (!glfwWindowShouldClose(window))
	{

		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
#ifdef LOW_FPS
		if (user_input_counter > 0)
		{
			glfwPollEvents();
			counter_mutex.lock();
			user_input_counter--;
			counter_mutex.unlock();
		}
		else
		{
			glfwWaitEvents();
		}
#else
		glfwPollEvents();
#endif
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		/* How to create a PopupContextMenu
			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::MenuItem("Hello"))
				{
					printf("Menu Clicked");
				}
				ImGui::EndPopup();
			}
			*/





		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			
			

			                        // Create a window called "Hello, world!" and append into it.
            // Display some text (you can use a format strings too)
			
			

			ImGui::Begin("Parametro de Aquisicao/Simulacao");


			if (ImGui::BeginMenu("Menu"))
			{
				ImGui::Checkbox("Activate VSync", &VSync);
				if (VSync != VSync_changed)
				{
					glfwSwapInterval(VSync);
					VSync_changed = VSync;
				}
				ImGui::EndMenu();
			}

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);


			ImGui::Separator();//--------------------------------------------------------




			ImGui::BeginGroup();
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x *0.25);
			sampling_freq_mutex.lock();
			ImGui::InputInt("Frequencia de aquisicao",&sampling_frequency);
			sampling_freq_mutex.unlock();
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.25);
			num_of_points_mutex.lock();
			ImGui::InputInt("Numero de pontos", &number_of_points);
			num_of_points_mutex.unlock();
			
			
			if (ImGui::Button("Ok"))
			{
				std::thread okButton(ok_graph_button_thread);
				okButton.detach();
			}
			ImGui::EndGroup();
			if (order_combo)
			{
				static char *comboItens[] = {"1 Ordem" ," "};
				ImGui::SameLine();
				if (ImGui::BeginCombo("Ordem do instrumento",comboItens[current_selected]))
				{
					
					show_eq_param_window = true;
					for (int n = 0; n < 1; n++)
					{
						static bool is_selected = (current_selected == n);
						if (ImGui::Selectable(comboItens[n], is_selected))
							current_selected = n;

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					
					ImGui::EndCombo();

				}
				if (show_eq_param_window)
				{
					ImGui::Separator();
					ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.25);
					static_sensibility_mutex.lock();
					ImGui::InputDouble("Sensibilidade Estatica", &static_sensibility);
					static_sensibility_mutex.unlock();
					ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.25);
					tau_mutex.lock();
					ImGui::InputDouble("Constante de tempo(Tau)", &tau);
					tau_mutex.unlock();
					


					if (ImGui::Button("Aplicar Valores"))
					{
						entry_window = true;
						//change the thread function called
						std::thread eq_param(ok_eq_param_thread);
						eq_param.detach();
					}

				}

			}

			if (entry_window)
			{
				ImGui::Separator();
				ImGui::NewLine();
				ImGui::Text("Selecione a entrada:");
				if (ImGui::Button("Seno"))
				{
					seno_settings_window = true;
				}
				ImGui::SameLine();
				ImGui::NewLine();
				if (plot_button)
				{
					ImGui::Separator();
					if (ImGui::Button("Plot"))
					{
						guiToAppProcess.prepareSimulation();
						guiToAppProcess.simulateInstrument();
						t = guiToAppProcess.getT();
						in_entry = guiToAppProcess.getInEntry();
						out_response = guiToAppProcess.getResponse();
					}
				}
				

			}

			ImGui::End();
			datas_mutex.lock();
			if (t && in_entry && out_response)
			{

				ImGui::Begin("Plot");
				if (ImPlot::BeginPlot("Teste"))
				{
					ImPlot::PlotLine("Entrada",t,in_entry, guiToAppProcess.getNumberPoints());
					ImPlot::PlotLine("Saida", t, out_response, guiToAppProcess.getNumberPoints());
					ImPlot::EndPlot();
				}
				ImGui::End();
			}
			datas_mutex.unlock();

			
			if (seno_settings_window)
			{
				ImGui::Begin("Parametros da Entrada Seno", &seno_settings_window);
				ImGui::DragFloat("Velocidade Angular", &angular_frequency, 1.0f);
				ImGui::DragFloat("Amplitude", &amplitude, 0.01f);
				if (ImGui::Button("Aplicar Seno"))
				{
					guiToAppProcess.setOmega(angular_frequency);
					guiToAppProcess.setFi(amplitude);
					guiToAppProcess.setEntryType("senoidal");
					seno_settings_window = false;
					plot_button = true;
				}


				ImGui::End();
			}


			
			
			
			
		}


		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y *clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

		//adicionar std::conditional_variable::wait + todas as call backs de eventos resetando um contador
	}


	// Cleanup
	ImPlot::DestroyContext();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}