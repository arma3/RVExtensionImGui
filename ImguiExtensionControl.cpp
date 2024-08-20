#include "ImguiExtensionControl.hpp"

#include <array>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>

#include "ImGuiBackendArmaControl.hpp"
#include "RVExtensionUtil.hpp"

#include <dinput.h>
static ImGuiKey Dik2ImGui[256] {};

static void InitDikTranslation()
{
    std::array<std::pair<int, ImGuiKey>, 103> mapping{
        // dinput key, virtual key
        std::pair{ DIK_ESCAPE,			ImGuiKey::ImGuiKey_Escape},
        { DIK_1,				ImGuiKey::ImGuiKey_1},
        { DIK_2,				ImGuiKey::ImGuiKey_2},
        { DIK_3,				ImGuiKey::ImGuiKey_3},
        { DIK_4,				ImGuiKey::ImGuiKey_4},
        { DIK_5,				ImGuiKey::ImGuiKey_5},
        { DIK_6,				ImGuiKey::ImGuiKey_6},
        { DIK_7,				ImGuiKey::ImGuiKey_7},
        { DIK_8,				ImGuiKey::ImGuiKey_8},
        { DIK_9,				ImGuiKey::ImGuiKey_9},
        { DIK_0,				ImGuiKey::ImGuiKey_0},
        { DIK_MINUS, 			ImGuiKey::ImGuiKey_Minus},
        { DIK_EQUALS,		 	ImGuiKey::ImGuiKey_Equal},
        { DIK_BACK, 			ImGuiKey::ImGuiKey_Backspace},
        { DIK_TAB, 			    ImGuiKey::ImGuiKey_Tab},
        { DIK_Q,				ImGuiKey::ImGuiKey_Q},
        { DIK_W,				ImGuiKey::ImGuiKey_W},
        { DIK_E,				ImGuiKey::ImGuiKey_E},
        { DIK_R,				ImGuiKey::ImGuiKey_R},
        { DIK_T,				ImGuiKey::ImGuiKey_T},
        { DIK_Y,				ImGuiKey::ImGuiKey_Y},
        { DIK_U,				ImGuiKey::ImGuiKey_U},
        { DIK_I,				ImGuiKey::ImGuiKey_I},
        { DIK_O,				ImGuiKey::ImGuiKey_O},
        { DIK_P,				ImGuiKey::ImGuiKey_P},
        { DIK_LBRACKET, 		ImGuiKey::ImGuiKey_LeftBracket},
        { DIK_RBRACKET, 		ImGuiKey::ImGuiKey_RightBracket},
        { DIK_RETURN, 		    ImGuiKey::ImGuiKey_Enter},
        { DIK_LCONTROL, 		ImGuiKey::ImGuiKey_LeftCtrl},
        { DIK_A,				ImGuiKey::ImGuiKey_A},
        { DIK_S,				ImGuiKey::ImGuiKey_S},
        { DIK_D,				ImGuiKey::ImGuiKey_D},
        { DIK_F,				ImGuiKey::ImGuiKey_F},
        { DIK_G,				ImGuiKey::ImGuiKey_G},
        { DIK_H,				ImGuiKey::ImGuiKey_H},
        { DIK_J,				ImGuiKey::ImGuiKey_J},
        { DIK_K,				ImGuiKey::ImGuiKey_K},
        { DIK_L,				ImGuiKey::ImGuiKey_L},
        { DIK_SEMICOLON,		ImGuiKey::ImGuiKey_Semicolon},
        { DIK_APOSTROPHE,		ImGuiKey::ImGuiKey_Apostrophe},
        { DIK_GRAVE, 			ImGuiKey::ImGuiKey_GraveAccent},
        { DIK_LSHIFT, 		    ImGuiKey::ImGuiKey_LeftShift},
        { DIK_BACKSLASH, 		ImGuiKey::ImGuiKey_Backslash},
        { DIK_Z,				ImGuiKey::ImGuiKey_Z},
        { DIK_X,				    ImGuiKey::ImGuiKey_X},
        { DIK_C,				    ImGuiKey::ImGuiKey_C},
        { DIK_V,				    ImGuiKey::ImGuiKey_V},
        { DIK_B,				    ImGuiKey::ImGuiKey_B},
        { DIK_N,				    ImGuiKey::ImGuiKey_N},
        { DIK_M,				    ImGuiKey::ImGuiKey_M},
        { DIK_COMMA,			    ImGuiKey::ImGuiKey_Comma },
        { DIK_PERIOD, 		    ImGuiKey::ImGuiKey_Period },
        { DIK_SLASH, 			ImGuiKey::ImGuiKey_Slash },
        { DIK_RSHIFT, 		    ImGuiKey::ImGuiKey_RightShift},
        { DIK_MULTIPLY, 		    ImGuiKey::ImGuiKey_KeypadMultiply },
        { DIK_LMENU, 			ImGuiKey::ImGuiKey_LeftAlt},
        { DIK_SPACE, 			ImGuiKey::ImGuiKey_Space },
        { DIK_CAPITAL, 		    ImGuiKey::ImGuiKey_CapsLock},
        { DIK_F1,				ImGuiKey::ImGuiKey_F1},
        { DIK_F2,				ImGuiKey::ImGuiKey_F2},
        { DIK_F3,				ImGuiKey::ImGuiKey_F3},
        { DIK_F4,				ImGuiKey::ImGuiKey_F4},
        { DIK_F5,				ImGuiKey::ImGuiKey_F5},
        { DIK_F6,				ImGuiKey::ImGuiKey_F6},
        { DIK_F7,				ImGuiKey::ImGuiKey_F7},
        { DIK_F8,				ImGuiKey::ImGuiKey_F8},
        { DIK_F9,				ImGuiKey::ImGuiKey_F9},
        { DIK_F10,			    ImGuiKey::ImGuiKey_F10},
        { DIK_NUMLOCK,		    ImGuiKey::ImGuiKey_NumLock},
        { DIK_SCROLL,			ImGuiKey::ImGuiKey_ScrollLock},
        { DIK_NUMPAD7,		    ImGuiKey::ImGuiKey_Keypad7},
        { DIK_NUMPAD8,			ImGuiKey::ImGuiKey_Keypad8},
        { DIK_NUMPAD9,			ImGuiKey::ImGuiKey_Keypad9},
        { DIK_SUBTRACT,			ImGuiKey::ImGuiKey_KeypadSubtract},
        { DIK_NUMPAD4,			ImGuiKey::ImGuiKey_Keypad4},
        { DIK_NUMPAD5,			ImGuiKey::ImGuiKey_Keypad5},
        { DIK_NUMPAD6,			ImGuiKey::ImGuiKey_Keypad6},
        { DIK_ADD,				ImGuiKey::ImGuiKey_KeypadAdd},
        { DIK_NUMPAD1,			ImGuiKey::ImGuiKey_Keypad1},
        { DIK_NUMPAD2,			ImGuiKey::ImGuiKey_Keypad2},
        { DIK_NUMPAD3,			ImGuiKey::ImGuiKey_Keypad3},
        { DIK_NUMPAD0,			ImGuiKey::ImGuiKey_Keypad0},
        { DIK_DECIMAL,			ImGuiKey::ImGuiKey_KeypadDecimal},
        { DIK_F11,				ImGuiKey::ImGuiKey_F11},
        { DIK_F12,				ImGuiKey::ImGuiKey_F12},
        { DIK_F13,				ImGuiKey::ImGuiKey_F13},
        { DIK_F14,				ImGuiKey::ImGuiKey_F14},
        { DIK_F15,				ImGuiKey::ImGuiKey_F15},
        { DIK_NUMPADENTER,		ImGuiKey::ImGuiKey_KeypadEnter},
        { DIK_RCONTROL,			ImGuiKey::ImGuiKey_RightCtrl},
        { DIK_DIVIDE,			ImGuiKey::ImGuiKey_KeypadDivide},
        //{ DIK_SYSRQ, 			0},
        { DIK_RMENU,				ImGuiKey::ImGuiKey_RightAlt},
        { DIK_HOME,				ImGuiKey::ImGuiKey_Home},
        { DIK_UP,				ImGuiKey::ImGuiKey_UpArrow},
        { DIK_PRIOR,				ImGuiKey::ImGuiKey_PageUp},
        { DIK_LEFT,				ImGuiKey::ImGuiKey_LeftArrow},
        { DIK_RIGHT,				ImGuiKey::ImGuiKey_RightArrow },
        { DIK_END,				ImGuiKey::ImGuiKey_End},
        { DIK_DOWN,				ImGuiKey::ImGuiKey_DownArrow},
        { DIK_NEXT,				ImGuiKey::ImGuiKey_PageDown},
        { DIK_INSERT,			ImGuiKey::ImGuiKey_Insert},
        { DIK_DELETE,			ImGuiKey::ImGuiKey_Delete},
        //{DIK_LWIN,			    ImGuiKey::ImGuiKey_},
        //{DIK_RWIN,			    ImGuiKey::ImGuiKey_},
        //{DIK_APPS,			    ImGuiKey::ImGuiKey_},
        { DIK_PAUSE,			    ImGuiKey::ImGuiKey_Pause},
        //{ 0,					VK_CANCEL},
        //{ DIK_MUTE,			ImGuiKey::ImGuiKey_},
        //{ DIK_VOLUMEDOWN,		VK_VOLUME_DOWN},
        //{ DIK_VOLUMEUP,		VK_VOLUME_UP},
        //{ DIK_WEBHOME,		VK_BROWSER_HOME},
        //{ DIK_WEBSEARCH,		VK_BROWSER_SEARCH},
        //{ DIK_WEBFAVORITES,	VK_BROWSER_FAVORITES},
        //{ DIK_WEBREFRESH,		VK_BROWSER_REFRESH },
        //{ DIK_WEBSTOP,		VK_BROWSER_STOP},
        //{ DIK_WEBFORWARD,		VK_BROWSER_FORWARD},
        //{ DIK_WEBBACK,		VK_BROWSER_BACK},
        //{ DIK_MAIL,			VK_LAUNCH_MAIL},
        //{ DIK_MEDIASELECT,	VK_LAUNCH_MEDIA_SELECT},
    };

    for (auto& it : mapping)
    {
        Dik2ImGui[it.first] = it.second;
    }
}




class ImGuiSetCurrentContext
{
    ImGuiContext* _previousCtx;
public:
    ImGuiSetCurrentContext(ImGuiContext* newCtx)
    {
        _previousCtx = ImGui::GetCurrentContext();
        ImGui::SetCurrentContext(newCtx);
    }
    ~ImGuiSetCurrentContext()
    {
        ImGui::SetCurrentContext(_previousCtx);
    }
};

ImguiExtensionControl::ImguiExtensionControl()
{
    InitDikTranslation();
    IMGUI_CHECKVERSION();
    _context = ImGui::CreateContext();
    ImGuiSetCurrentContext _c(_context);

    auto deviceDataPtr = reinterpret_cast<const RVExtensionRenderInfo* const *>(FindRVFunction("RVExtensionGData"));
    if (!deviceDataPtr)
        return;

    auto io = &ImGui::GetIO();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;         //
    //io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // This is not implemented for the ImplArma backend

    io->IniFilename = nullptr; // no save/load

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    ImGui_ImplArma_Init();
    ImGui_ImplDX11_Init((*deviceDataPtr)->d3dDevice, (*deviceDataPtr)->d3dDeviceContext);
}

ImguiExtensionControl::~ImguiExtensionControl()
{
    {
        ImGuiSetCurrentContext _c(_context);
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplArma_Shutdown();
    }

    ImGui::DestroyContext(_context);
}

void ImguiExtensionControl::OnSizeChanged(unsigned int width, unsigned int height)
{
    ImGuiSetCurrentContext _c(_context);

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(width, height);
    
    _width = width;
    _height = height;
}


void ImguiExtensionControl::OnDraw(float alpha, ID3D11RenderTargetView* target)
{
    ImGuiSetCurrentContext _c(_context);

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplArma_NewFrame();
    ImGui::NewFrame();

    // Draw all the things to Imgui

    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // ImGui present

    // Rendering
    // We render onto active rendertarget in game. Which is the texture backing the UI control.
    // Note that before rendering, you must back up the context stage parameters, and restore them before releasing the lock, otherwise the game renderer might break
    // A helper class, DX11::ContextBackup, is provided for this purpose
    ImGui::Render();

    {
        // No need to do this for ImGui; it already does back up and restore the context; see the definition of the "ImGui_ImplDX11_RenderDrawData" function
        /* DX11::ContextBackup ctxBackup{deviceData->d3dDeviceContext}; */

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
}

void ImguiExtensionControl::OnDestroy(int code)
{
    ImGuiSetCurrentContext _c(_context);
    
    delete this;
}

bool ImguiExtensionControl::OnSetFocus(bool focus)
{
    ImGuiSetCurrentContext _c(_context);

    ImGui::GetIO().AddFocusEvent(focus);
    return true;
}

void ImguiExtensionControl::OnLButtonDown(float x, float y)
{
    ImGuiSetCurrentContext _c(_context);
    x *= _width; y *= _height; // percent to pixels

    ImGui::GetIO().AddMousePosEvent(x, y);
    ImGui::GetIO().AddMouseButtonEvent(ImGuiMouseButton_Left, true);
}

void ImguiExtensionControl::OnLButtonUp(float x, float y)
{
    ImGuiSetCurrentContext _c(_context);
    x *= _width; y *= _height; // percent to pixels
    ImGui::GetIO().AddMousePosEvent(x, y);
    ImGui::GetIO().AddMouseButtonEvent(ImGuiMouseButton_Left, false);
}

void ImguiExtensionControl::OnLButtonClick(float x, float y)
{
    ImGuiSetCurrentContext _c(_context);
    x *= _width; y *= _height; // percent to pixels

}

void ImguiExtensionControl::OnLButtonDblClick(float x, float y)
{
    ImGuiSetCurrentContext _c(_context);
    x *= _width; y *= _height; // percent to pixels

}

void ImguiExtensionControl::OnRButtonDown(float x, float y)
{
    ImGuiSetCurrentContext _c(_context);
    x *= _width; y *= _height; // percent to pixels
    ImGui::GetIO().AddMousePosEvent(x, y);
    ImGui::GetIO().AddMouseButtonEvent(ImGuiMouseButton_Right, true);
}

void ImguiExtensionControl::OnRButtonUp(float x, float y)
{
    ImGuiSetCurrentContext _c(_context);
    x *= _width; y *= _height; // percent to pixels
    ImGui::GetIO().AddMousePosEvent(x, y);
    ImGui::GetIO().AddMouseButtonEvent(ImGuiMouseButton_Right, false);
}

void ImguiExtensionControl::OnRButtonClick(float x, float y)
{
    ImGuiSetCurrentContext _c(_context);

}

void ImguiExtensionControl::OnMouseMove(float x, float y)
{
    ImGuiSetCurrentContext _c(_context);
    x *= _width; y *= _height; // percent to pixels
    ImGui::GetIO().AddMousePosEvent(x, y);
}

bool ImguiExtensionControl::OnMouseZChanged(float dz)
{
    ImGuiSetCurrentContext _c(_context);
    if (!ImGui::GetIO().WantCaptureMouse)
        return false;

    // If not handled, a scrollbar in a parent controls group might handle the scroll

    ImGui::GetIO().AddMouseWheelEvent(0, dz);
    return true;
}

void ImguiExtensionControl::OnMouseEnter(float x, float y)
{
    ImGuiSetCurrentContext _c(_context);
}

void ImguiExtensionControl::OnMouseExit(float x, float y)
{
    ImGuiSetCurrentContext _c(_context);
}

bool ImguiExtensionControl::OnChar(unsigned nChar, unsigned nRepCnt, unsigned nFlags)
{
    ImGuiSetCurrentContext _c(_context);
    if (!ImGui::GetIO().WantCaptureKeyboard)
        return false;

    ImGui::GetIO().AddInputCharacter(nChar);
    return true;
}


bool ImguiExtensionControl::OnKeyDown(int dikCode)
{
    ImGuiSetCurrentContext _c(_context);
    if (!ImGui::GetIO().WantCaptureKeyboard)
        return false;

    auto k = Dik2ImGui[dikCode];
    ImGui::GetIO().AddKeyEvent(k, true);
    if (k == ImGuiKey_LeftShift || k == ImGuiKey_RightShift) ImGui::GetIO().AddKeyEvent(ImGuiKey_ModShift, true);
    if (k == ImGuiKey_LeftAlt || k == ImGuiKey_RightAlt) ImGui::GetIO().AddKeyEvent(ImGuiKey_ModAlt, true);
    if (k == ImGuiKey_LeftCtrl || k == ImGuiKey_RightCtrl) ImGui::GetIO().AddKeyEvent(ImGuiKey_ModCtrl, true);
    return true;
}

bool ImguiExtensionControl::OnKeyUp(int dikCode)
{
    ImGuiSetCurrentContext _c(_context);
    if (!ImGui::GetIO().WantCaptureKeyboard)
        return false;

    auto k = Dik2ImGui[dikCode];
    ImGui::GetIO().AddKeyEvent(k, false);
    if (k == ImGuiKey_LeftShift || k == ImGuiKey_RightShift) ImGui::GetIO().AddKeyEvent(ImGuiKey_ModShift, false);
    if (k == ImGuiKey_LeftAlt || k == ImGuiKey_RightAlt) ImGui::GetIO().AddKeyEvent(ImGuiKey_ModAlt, false);
    if (k == ImGuiKey_LeftCtrl || k == ImGuiKey_RightCtrl) ImGui::GetIO().AddKeyEvent(ImGuiKey_ModCtrl, false);
    return true;
}
