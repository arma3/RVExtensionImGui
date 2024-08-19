#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include "CtxBackup.hpp"
#include "RVExtensionUtil.hpp"

bool isInit = false;
ImGuiIO* io;
RVExtensionGLockProc* GetGPULock;

//! Utility to handle GPU Lock and never forget to release it
struct GPULockScope
{
    RVExtensionGraphicsLockGuard* _guard;
    GPULockScope()
    {
        _guard = GetGPULock();
    }
    ~GPULockScope()
    {
        _guard->ReleaseLock();
    }
};

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
bool WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    //#TODO if for some reason our EventHandler goes away and we are not drawing (Mission end for example). We need to disable our hook here. Otherwise a Invisible non-rendering UI might block mouse/key input.

    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    if (io->WantCaptureMouse)
    {
        switch (msg)
        {
        case WM_MOUSEMOVE:
        case WM_LBUTTONDOWN: // MouseMove works, but all the buttons seem to not work, Arma still reads mouse input, Probably need to utilize a dialog or disableUserInput here
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
            return true; // Swallow message
        }
    }

    if (io->WantCaptureKeyboard)
    {
        switch (msg)
        {
        case WM_INPUT:
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_CHAR:
        case WM_IME_CHAR:
        case WM_IME_COMPOSITION:
            return true; // Swallow message
        }
    }

    // To block game inputs you can block
    // Mouse: WM_MOUSEMOVE WM_LBUTTONDOWN WM_RBUTTONDOWN WM_MBUTTONDOWN WM_LBUTTONUP WM_RBUTTONUP WM_MBUTTONUP
    // Keyboard: WM_INPUT WM_KEYDOWN WM_KEYUP WM_SYSKEYDOWN WM_SYSKEYUP WM_CHAR WM_IME_CHAR WM_IME_COMPOSITION

    return false;
}

static const RVExtensionRenderInfo* deviceData = nullptr;
void ImguiInit()
{
    if (isInit)
        return;

    auto deviceDataPtr = reinterpret_cast<const RVExtensionRenderInfo* const *>(FindRVFunction("RVExtensionGData"));
    GetGPULock = reinterpret_cast<RVExtensionGLockProc*>(FindRVFunction("RVExtensionGLock"));
    auto setWindowHook = reinterpret_cast<RVExtensionGSetWHkProc*>(FindRVFunction("RVExtensionGSetWHk"));
    if (!deviceDataPtr || !GetGPULock || !setWindowHook)
        return;

    deviceData = *deviceDataPtr;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io->ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;   // Conflicts with game and will flicker alot
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;         //
    //io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // This does NOT work in Arma 3. You loose all input and the game freezes


    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    auto hwnd = GetActiveWindow(); // Get Arma's main window associated with this thread, which will be main thread

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(deviceData->d3dDevice, deviceData->d3dDeviceContext);
    setWindowHook(WndProc);

    // Load TTF fonts? optionally from inside a pbo

    isInit = true;
}

// imgui state
bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void ImguiRenderTick()
{
    if (!isInit)
        ImguiInit();


    // Imgui render


    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();


    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    // Rendering
    // We render onto active rendertarget in game. Which (inside draw3D eventhandler) will be the UI target. The game will clear it for us so we just render ontop of it
    // Using https://community.bistudio.com/wiki/User_Interface_Event_Handlers#onDraw would have the current UI target set to the UIOnTexture, so drawing from within there could draw into a texture
    // Note that before rendering, you must back up the context stage parameters, and restore them before releasing the lock, otherwise the game renderer might break
    // A helper class, DX11::ContextBackup, is provided for this purpose
    ImGui::Render();

    {
        GPULockScope guard;

        // No need to do this for ImGui; it already does back up and restore the context; see the definition of the "ImGui_ImplDX11_RenderDrawData" function
        /* DX11::ContextBackup ctxBackup{deviceData->d3dDeviceContext}; */

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    //#TODO! Forward this to ingame script, to block user input.
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
}

//#TODO call this?
void ImguiCleanup()
{
    if (!isInit)
        return;

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    auto setWindowHook = reinterpret_cast<RVExtensionGSetWHkProc*>(FindRVFunction("RVExtensionGSetWHk"));
    setWindowHook(nullptr);
}