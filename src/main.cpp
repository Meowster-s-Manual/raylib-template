#include <print>

#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#pragma region imgui
#include <imgui.h>
#include <rlImGui.h>
// #include <imguiThemes.h>
#pragma endregion

#include <string>

// Temp Global variables
const int WINDOW_LENGTH = 800;
const int WINDOW_HEIGHT = 450;
const std::string WINDOW_TITLE = "raygui - controls test suite";
const int RENDER_TEXTURE_WIDTH = 800;
const int RENDER_TEXTURE_HEIGHT = 450;

int main(){
    std::println("Hello, from raylib-template!");
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_LENGTH, WINDOW_HEIGHT, WINDOW_TITLE.c_str());

    // Load default style for raygui
    GuiLoadStyleDefault();

    // Load a render texture for drawing
    RenderTexture2D renderTexture = LoadRenderTexture(RENDER_TEXTURE_WIDTH, RENDER_TEXTURE_HEIGHT);

    SetTargetFPS(144);

    bool showMessageBox = false;

#pragma region imgui
	rlImGuiSetup(true);

	//you can use whatever imgui theme you like!
	ImGui::StyleColorsDark();
	//imguiThemes::yellow();
	//imguiThemes::gray();
	//imguiThemes::green();
	//imguiThemes::red();
	//imguiThemes::embraceTheDarkness();


	ImGuiIO &io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.FontGlobalScale = 2;

	ImGuiStyle &style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		//style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 0.5f;
		//style.Colors[ImGuiCol_DockingEmptyBg].w = 0.f;
	}
#pragma endregion

    std::println("working directory: {}", GetWorkingDirectory());
    std::println("GetApplicationDirectory: {}", GetApplicationDirectory());
    std::println("asset path: {}", ASSETS_PATH);

    while (!WindowShouldClose())
    {
        // float dt = GetFrameTime();
        // std::println("Delta time: {}", dt);

        // Update
        //----------------------------------------------------------------------------------
        BeginTextureMode(renderTexture); // Enable drawing to texture
        ClearBackground(RAYWHITE);
        EndTextureMode(); // Disable drawing to texture

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

    #pragma region imgui
		rlImGuiBegin();
        bool open = true;
        ImGui::ShowDemoWindow(&open);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
		ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
		ImGui::DockSpaceOverViewport();
		ImGui::PopStyleColor(2);
	#pragma endregion

        if (GuiButton((Rectangle){ 24, 24, 120, 30 }, "#191#Show Message")) showMessageBox = true;

        if (showMessageBox)
        {
            int result = GuiMessageBox((Rectangle){ 85, 70, 250, 100 },
                "#191#Message Box", "Hi! This is a message!", "Nice;Cool");

            if (result >= 0) showMessageBox = false;
        }

    #pragma region imgui
		rlImGuiEnd();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	#pragma endregion

        EndDrawing();
    }

#pragma region imgui
	rlImGuiShutdown();
#pragma endregion

    UnloadRenderTexture(renderTexture);
    CloseWindow();
    return 0;
}
