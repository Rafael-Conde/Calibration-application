#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <cstring>

//to enable the members that are only available to the windows vista and foward
// members of the struct NOTIFYICONDATA
//#define NTDDI_VERSION NTDDI_VISTA
//#define _WIN32_WINNT _WIN32_WINNT_VISTA

/*#define CREATE_MENU_MESSAGE_ (WM_USER+1)
#define ID_TRAY_EXIT_CONTEXT_MENU_ITEM  3000
#define ID_TRAY_APP_ICON                5000

#define TRAY_ICON1 "icon.ico"
#define TRAY_ICON2 "icon.ico"
#define TRAY_WINAPI 1
#include "tray.h"


//static struct tray tray;

static void toggle_cb(struct tray_menu* item) {
	printf("toggle cb\n");
	item->checked = !item->checked;
	tray_update(&tray);
}

static void hello_cb(struct tray_menu* item) {
	(void)item;
	printf("hello cb\n");
	if (strcmp(tray.icon, TRAY_ICON1) == 0) {
		tray.icon = TRAY_ICON2;
	}
	else {
		tray.icon = TRAY_ICON1;
	}
	tray_update(&tray);
}

static void quit_cb(struct tray_menu* item) {
	(void)item;
	printf("quit cb\n");
	tray_exit();
}

static void submenu_cb(struct tray_menu* item) {
	(void)item;
	printf("submenu: clicked on %s\n", item->text);
	tray_update(&tray);
}

// Test tray init
static struct tray tray = {TRAY_ICON1,(struct tray_menu[]){
			{.text = "Hello", .cb = hello_cb},
			{.text = "Checked", .checked = 1, .cb = toggle_cb},
			{.text = "Disabled", .disabled = 1},
			{.text = "-"},
			{.text = "SubMenu",
			 .submenu =
				 (struct tray_menu[]){
					 {.text = "FIRST", .checked = 1, .cb = submenu_cb},
					 {.text = "SECOND",
					  .submenu =
						  (struct tray_menu[]){
							  {.text = "THIRD",
							   .submenu =
								   (struct tray_menu[]){
									   {.text = "7", .cb = submenu_cb},
									   {.text = "-"},
									   {.text = "8", .cb = submenu_cb},
									   {.text = NULL}}},
							  {.text = "FOUR",
							   .submenu =
								   (struct tray_menu[]){
									   {.text = "5", .cb = submenu_cb},
									   {.text = "6", .cb = submenu_cb},
									   {.text = NULL}}},
							  {.text = NULL}}},
					 {.text = NULL}}},
			{.text = "-"},
			{.text = "Quit", .cb = quit_cb},
			{.text = NULL}},
};*/


//#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GL/glew.h>
//#endif

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

static char window__name[] = "CommandHelper";

//useless toUnicode Method
/*static std::vector<unsigned int> toUnicode(const char* text)
{
	size_t size = strlen(text);
	std::vector<unsigned int> inUnicode(size + 1);
	for (size_t i{ 0 }; text[i] != '\0'; i++)
	{
		inUnicode.push_back((unsigned short)text[i]);
	}
	inUnicode.push_back((unsigned short)'\0');
	return inUnicode;
}*/


static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

// createSystemTrayStruct method
/*NOTIFYICONDATA createSystemTrayStruct(HWND window_handle, UINT flags, DWORD dwflags)
{
	NOTIFYICONDATA iconData = {};

	/*
	ULONGLONG ullVersion = GetDllVersion(_T("Shell32.dll"));

	if (ullVersion >= MAKEDLLVERULL(6, 0, 0, 0))
		iconData.cbSize = sizeof(NOTIFYICONDATA);

	else if (ullVersion >= MAKEDLLVERULL(5, 0, 0, 0))
		iconData.cbSize = NOTIFYICONDATA_V2_SIZE;

	else iconData.cbSize = NOTIFYICONDATA_V1_SIZE; 

iconData.cbSize = sizeof(NOTIFYICONDATA);
iconData.hWnd = window_handle;
iconData.uID = ID_TRAY_APP_ICON;
iconData.uFlags = flags;// verify the flags to be set
iconData.uVersion = NOTIFYICON_VERSION_4;
char text[] = "Guilherme nao passa de um Gayzao!!!";
for (size_t i{ 0 }; text[i] != '\0'; i++)
{
	iconData.szInfo[i] = (unsigned short)text[i];
}
iconData.szInfo[strlen(text) + 1] = (unsigned short)'\0';
// iconData.szInfo
//iconData.szInfoTitle, "!Delay jão - Tofu Japonês!");
char textTitle[] = "*Atencao! - Guilherme e gay!*";
for (size_t i{ 0 }; textTitle[i] != '\0'; i++)
{
	iconData.szInfoTitle[i] = (unsigned short)textTitle[i];
}
iconData.szInfoTitle[strlen(textTitle) + 1] = (unsigned short)'\0';
//char textTip[] = "Delay e jao e o tofu e japones";
char textTip[] = "Guilherme nao passa de um Gayzao!!!";
for (size_t i{ 0 }; textTip[i] != '\0'; i++)
{
	iconData.szTip[i] = (unsigned short)textTip[i];
}
iconData.szTip[strlen(textTip) + 1] = (unsigned short)'\0';
iconData.hBalloonIcon = NULL;
iconData.uCallbackMessage = CREATE_MENU_MESSAGE_;
iconData.dwInfoFlags = dwflags;//set the warnning ballon message
//iconData.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
iconData.hIcon = (HICON)LoadImage(NULL, TEXT("green_man.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);


return iconData;
}*/


//showPopupMenu method
/*BOOL ShowPopupMenu(HWND hWnd, POINT* curpos, int wDefaultItem)
{
	//ADD MENU ITEMS.------------------------------------------------------------------

	HMENU hPop = CreatePopupMenu();


	std::vector<unsigned int> about_vec(toUnicode("About..."));
	std::vector<unsigned int> exit_vec(toUnicode("Exit"));
	const unsigned int* about{ &about_vec[0] };
	const unsigned int* exit{ &exit_vec[0] };
	InsertMenu(hPop, 0, MF_BYPOSITION | MF_STRING, 0x1, (LPCWSTR)about);

	InsertMenu(hPop, 1, MF_BYPOSITION | MF_STRING, 0x2, (LPCWSTR)exit);

	//CAN DO WITHOUT STUFF.------------------------------------------------------------

	SetMenuDefaultItem(hPop, 0x1, FALSE);

	SetFocus(hWnd);

	SendMessage(hWnd, WM_INITMENUPOPUP, (WPARAM)hPop, 0);



	//SHOW POPUP MENU.-----------------------------------------------------------------

	{

		//GET CURSOR POSITION TO CREATE POPUP THERE.-------------------------------------

		POINT pt;

		if (!curpos) {

			GetCursorPos(&pt);

			curpos = &pt;

		}



		{

			//DISPLAT MENU AND WAIT FOR SELECTION.-----------------------------------------

			WORD cmd = TrackPopupMenu(hPop, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, curpos->x, curpos->y, 0, hWnd, NULL);



			//SEND MESSAGE MAPPED TO SELECTED ITEM.----------------------------------------

			SendMessage(hWnd, WM_COMMAND, cmd, 0);

		}

	}
	return true;
}*/


//window_procedure method
/*LRESULT CALLBACK window_procedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	printf("WndProc called\n uMsg = %ud\n", uMsg);
	switch (uMsg) {
	case ID_TRAY_APP_ICON:
		//case WM_CREATE:
		HMENU menu;
		menu = CreatePopupMenu();

		AppendMenu(menu, MF_STRING, ID_TRAY_EXIT_CONTEXT_MENU_ITEM, TEXT("Exit"));

		break;
		return 0;
	default: printf("message failed\n");
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}*/



int main(int, char**)
{
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
	
	
	
	
	/*
	if (tray_init(&tray) < 0) {
		printf("failed to create tray\n");
		return 1;
	}
	tray_loop(0);
	*/




	/*HWND window_handle = glfwGetWin32Window(window);

	HINSTANCE hInst = GetModuleHandle(NULL);

	//REGISTER WINDOW.--------------------------------------------------------------------------
	WNDCLASSEX wclx;
	memset(&wclx, 0, sizeof(wclx));
	wclx.cbSize = sizeof(wclx);
	//wclx.style = CS_HREDRAW | CS_VREDRAW;
	//wclx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//wclx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wclx.lpfnWndProc = &window_procedure;
	wclx.lpszClassName = (LPCWSTR)window__name;
	//wclx.cbClsExtra = 0;
	//wclx.cbWndExtra = 0;
	wclx.hInstance = hInst;
	//wclx.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wclx.hIcon        = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_TRAYICON ) );
	//wclx.hIconSm      = LoadSmallIcon( hInstance, IDI_TRAYICON );
	//wclx.hCursor = NULL;
	//wclx.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE;
	//wclx.lpszMenuName = NULL;
	// wclx.lpszClassName = className;
	ATOM register_value = RegisterClassEx(&wclx);

	if (register_value)
	{
		printf("window Successfully registered!\n");
		printf("Register value = %u\n", register_value);
	}
	else
	{
		printf("Fail to register the window\n\n");
		printf("%ul", GetLastError());
		printf("\n\n"); \
			//return -1;
	}

	NOTIFYICONDATA niData = createSystemTrayStruct(window_handle,
		(NIF_INFO | NIF_ICON | NIF_REALTIME | NIF_TIP | NIF_SHOWTIP | NIF_MESSAGE),
		(NIIF_INFO | NIIF_LARGE_ICON));
	if (Shell_NotifyIcon(NIM_ADD, &niData))
	{
		Shell_NotifyIcon(NIM_SETVERSION, &niData);
		printf("Successfully created the Notification area Icon, check it out!\n");
	}
	else
	{
		printf("For some reason we failed to create the Notification area icon. :(");
		return -1;
	}




	*/

	//glfwHideWindow(window); // hides the window but not the console window that generated this one.

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
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

	MSG msg;

	// Main loop
	while (!glfwWindowShouldClose(window))
	{

		/*
		GetMessage(&msg, NULL, 0, 0);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		*/

		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();


		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float f = 0.0f;
			static int counter = 0;


			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);
			ImGui::Checkbox("Activate VSync", &VSync);
			if (VSync != VSync_changed)
			{
				glfwSwapInterval(VSync);
				VSync_changed = VSync;
			}
			static const int mouseButtonValue{ 1 };
			//if (ImGui::IsMouseReleased(mouseButtonValue))
			//{
				//printf("Mouse button %d release\n", mouseButtonValue);
			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::MenuItem("Hello"))
				{
					printf("Menu Clicked");
				}
				ImGui::EndPopup();
			}
			//}



			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))
			{
				if (counter == 0)
					strcpy_s(textText, "O Delay e %d vezes jao");
				counter++;
			}// Buttons return true when clicked (most widgets return true when edited/activated)

			ImGui::SameLine();
			ImGui::Text(textText, counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Delay e jao e Tofu e Japones!!! Lah");
			if (ImGui::Button("Tofu e Japones!"))
				show_another_window = false;
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
	}
	
	
	
	// Cleanup
	//Shell_NotifyIcon(NIM_DELETE, &niData);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
