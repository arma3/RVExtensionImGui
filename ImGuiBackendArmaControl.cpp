#include "ImGuiBackendArmaControl.hpp"
#include <Windows.h>

struct ImGui_ImplArma_Data
{
    INT64                       Time;
    INT64                       TicksPerSecond;

    //std::chrono::system_clock::time_point LastRender;

    ImGui_ImplArma_Data()      { memset((void*)this, 0, sizeof(*this)); }
};

static ImGui_ImplArma_Data* ImGui_ImplArma_GetBackendData()
{
    return ImGui::GetCurrentContext() ? (ImGui_ImplArma_Data*)ImGui::GetIO().BackendPlatformUserData : nullptr;
}


bool ImGui_ImplArma_Init()
{
    ImGuiIO& io = ImGui::GetIO();
    IMGUI_CHECKVERSION();
    IM_ASSERT(io.BackendPlatformUserData == nullptr && "Already initialized a platform backend!");


    // This is based on the Win32 backend
    // Instead using PerformanceCounter, std::chrono::system_clock can also be used for timing

    INT64 perf_frequency, perf_counter;
    if (!::QueryPerformanceFrequency((LARGE_INTEGER*)&perf_frequency))
        return false;
    if (!::QueryPerformanceCounter((LARGE_INTEGER*)&perf_counter))
        return false;

    // bd->LastRender = std::chrono::system_clock::now();

    ImGui_ImplArma_Data* bd = IM_NEW(ImGui_ImplArma_Data)();
    io.BackendPlatformUserData = (void*)bd;
    io.BackendPlatformName = "imgui_impl_arma";

    // I have not implemented viewports here. But technically it is possible

    //io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;    // We can create multi-viewports on the Platform side (optional)
    //io.BackendFlags |= ImGuiBackendFlags_HasMouseHoveredViewport; // We can call io.AddMouseViewportEvent() with correct data (optional)

    bd->TicksPerSecond = perf_frequency;
    bd->Time = perf_counter;


    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        //ImGui_ImplWin32_InitPlatformInterface(platform_has_own_dc);

        // For multiple-viewports support, you would need to implement all of these, likely with RVExtensionCallback, to trigger script to create additional controls via ctrlCreate

        //ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
        //platform_io.Platform_CreateWindow = ImGui_ImplWin32_CreateWindow;
        //platform_io.Platform_DestroyWindow = ImGui_ImplWin32_DestroyWindow;
        //platform_io.Platform_ShowWindow = ImGui_ImplWin32_ShowWindow;
        //platform_io.Platform_SetWindowPos = ImGui_ImplWin32_SetWindowPos;
        //platform_io.Platform_GetWindowPos = ImGui_ImplWin32_GetWindowPos;
        //platform_io.Platform_SetWindowSize = ImGui_ImplWin32_SetWindowSize;
        //platform_io.Platform_GetWindowSize = ImGui_ImplWin32_GetWindowSize;
        //platform_io.Platform_SetWindowFocus = ImGui_ImplWin32_SetWindowFocus;
        //platform_io.Platform_GetWindowFocus = ImGui_ImplWin32_GetWindowFocus;
        //platform_io.Platform_GetWindowMinimized = ImGui_ImplWin32_GetWindowMinimized;
        //platform_io.Platform_SetWindowTitle = ImGui_ImplWin32_SetWindowTitle;
        //platform_io.Platform_SetWindowAlpha = ImGui_ImplWin32_SetWindowAlpha;
        //platform_io.Platform_UpdateWindow = ImGui_ImplWin32_UpdateWindow;
        //platform_io.Platform_GetWindowDpiScale = ImGui_ImplWin32_GetWindowDpiScale; // FIXME-DPI
        //platform_io.Platform_OnChangedViewport = ImGui_ImplWin32_OnChangedViewport; // FIXME-DPI
    }
}

void ImGui_ImplArma_Shutdown()
{
    auto* bd = ImGui_ImplArma_GetBackendData();
    IM_ASSERT(bd != nullptr && "No platform backend to shutdown, or already shutdown?");
    ImGuiIO& io = ImGui::GetIO();

    ImGui::DestroyPlatformWindows(); // multi viewport support, we don't have it but that just means this does nothing

    io.BackendPlatformName = nullptr;
    io.BackendPlatformUserData = nullptr;
    //io.BackendFlags &= ~(ImGuiBackendFlags_PlatformHasViewports | ImGuiBackendFlags_HasMouseHoveredViewport);
    IM_DELETE(bd);
}

void ImGui_ImplArma_NewFrame()
{
    auto* bd = ImGui_ImplArma_GetBackendData();
    IM_ASSERT(bd != nullptr && "Context or backend not initialized? Did you call ImGui_ImplWin32_Init()?");
    ImGuiIO& io = ImGui::GetIO();


    // We update DisplaySize in ImguiExtensionControl::OnSizeChanged

    // Alternatively, we could update the size here like this
    //{
    //    auto deviceDataPtr = reinterpret_cast<const RVExtensionRenderInfo* const *>(FindRVFunction("RVExtensionGData"));
    //    if (!deviceDataPtr)
    //        return;
    //
    //    ID3D11RenderTargetView* x;
    //    (*deviceDataPtr)->d3dDeviceContext->OMGetRenderTargets(1, &x, nullptr);
    //
    //    ID3D11Texture2D *pTextureInterface = 0;
    //    ID3D11Resource *res;
    //    x->GetResource(&res);
    //    res->QueryInterface<ID3D11Texture2D>(&pTextureInterface);
    //    D3D11_TEXTURE2D_DESC desc;
    //    pTextureInterface->GetDesc(&desc);
    //
    //    io.DisplaySize = ImVec2(desc.Width, desc.Height);
    //}


    // Setup time step
    INT64 current_time = 0;
    ::QueryPerformanceCounter((LARGE_INTEGER*)&current_time);
    io.DeltaTime = (float)(current_time - bd->Time) / bd->TicksPerSecond;
    bd->Time = current_time;

    //io.DeltaTime = static_cast<std::chrono::duration<float>>(std::chrono::system_clock::now() - _lastRender).count();
    //bd->LastRender = std::chrono::system_clock::now();
}
