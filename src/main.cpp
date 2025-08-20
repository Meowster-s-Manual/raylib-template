#include <print>

#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

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

        if (GuiButton((Rectangle){ 24, 24, 120, 30 }, "#191#Show Message")) showMessageBox = true;

        if (showMessageBox)
        {
            int result = GuiMessageBox((Rectangle){ 85, 70, 250, 100 },
                "#191#Message Box", "Hi! This is a message!", "Nice;Cool");

            if (result >= 0) showMessageBox = false;
        }

        EndDrawing();
    }

    UnloadRenderTexture(renderTexture);
    CloseWindow();
    return 0;
}
