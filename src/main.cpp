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
#include <cmath>



#include <limits>

#define LEN 1000

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
std::mutex graph_mutex{};
std::mutex counter_mutex{};
int user_input_counter{ 3 };
std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> time_reference{ std::chrono::nanoseconds::zero() };
std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> now{ std::chrono::nanoseconds::zero() };
float hz{ 1 };
long long period{ static_cast<long long>((1'000'000'000 / (LEN)) * hz) };

double* data1, * data2;

double pi = std::acos(-1);

bool finish{ false };

size_t total_len{ LEN + 1 };
size_t actual_len{ (total_len - 1) };

void graph_shift()
{
	bool counter_big_zero;
	//std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	while (1 && !finish)
	{
		now = std::chrono::steady_clock::now();
#ifdef LOW_FPS
		counter_mutex.lock();
		counter_big_zero = user_input_counter > 0;
		counter_mutex.unlock();
#else
		counter_big_zero = true;
#endif

		if ((((now - time_reference).count() >= period) && hz > 0) &&  counter_big_zero)
		{
			time_reference = now;
			//std::this_thread::sleep_for(std::chrono::milliseconds(period));
			//std::lock_guard<std::mutex> guard(graph_mutex);
			//data1[actual_len] = data1[0];
			graph_mutex.lock();
			data2[actual_len] = data2[0];
			for (size_t i{ 1 }; i < actual_len; i++)
			{
				//data1[(i - 1)] = data1[i];
				data2[(i - 1)] = data2[i];
			}
			//data1[actual_len - 1] = data1[actual_len];
			data2[actual_len - 1] = data2[actual_len];
			graph_mutex.unlock();
			/*
			for (size_t i{ 0 }; i < actual_len; i++)
			{
				data1[i] = i * ((pi * 2) / actual_len);
				data2[i] = std::sin(data1[i]);
			}
			*/
		}
	}
}

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

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
	bool show_demo_window = false;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	char textText[24]{};
	strcpy_s(textText, "Counter = %d");
	const size_t len{ LEN };
	double x[(LEN + 1)];
	double y[(LEN + 1)];
	

	std::cout << period << std::endl;
	graph_mutex.lock();
	for (size_t i{ 0 }; i < actual_len; i++)
	{
		x[i] = i * ((pi * 20) / actual_len);
		y[i] = std::sin(x[i]);
	}
	graph_mutex.unlock();
	

	x[1000] = 0;
	y[1000] = 0;





	data1 = x;
	data2 = y;

	MSG msg;
	std::thread graph_shifting(graph_shift);
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
			user_input_counter--;
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







		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			
			

			ImGui::Begin("Plots");

			if (ImPlot::BeginPlot("Line Plot"))
			{
				ImPlot::SetupAxes("x", "sin(x)");
				graph_mutex.lock();
				ImPlot::PlotLine("sin(x)", x, y, (len));
				graph_mutex.unlock();
				ImPlot::EndPlot();
			}



			ImGui::End();

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
            // Display some text (you can use a format strings too)
			
			ImGui::Checkbox("Activate VSync", &VSync);
			if (VSync != VSync_changed)
			{
				glfwSwapInterval(VSync);
				VSync_changed = VSync;
			}

			ImGui::SliderFloat("Signal Speed", &hz,0.0f,5.0f);
			
			period = static_cast<long long>( ((1'000'000'000 / (LEN)) / hz) );
			

			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::MenuItem("Hello"))
				{
					printf("Menu Clicked");
				}
				ImGui::EndPopup();
			}



			


			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}


		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

		//adicionar std::conditional_variable::wait + todas as call backs de eventos resetando um contador
	}
	finish = true;
	graph_shifting.join();


	// Cleanup
	ImPlot::DestroyContext();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
