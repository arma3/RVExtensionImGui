#include "WebBrowserControl.hpp"
#define NOMINMAX
#include <array>
#include <d3d11.h>

#include "RVExtensionUtil.hpp"

extern void ImguiInit();


#include <dinput.h>
static int Dik2VKeyCode[256] {};

static void InitDikTranslation()
{
    std::array<std::pair<int, int>, 120> mapping{
        // dinput key, virtual key
        std::pair
        { DIK_ESCAPE,			VK_ESCAPE},
        { DIK_1,				'1'},
        { DIK_2,				'2'},
        { DIK_3,				'3'},
        { DIK_4,				'4'},
        { DIK_5,				'5'},
        { DIK_6,				'6'},
        { DIK_7,				'7'},
        { DIK_8,				'8'},
        { DIK_9,				'9'},
        { DIK_0,				'0'},
        { DIK_MINUS, 			VK_OEM_MINUS},
        { DIK_EQUALS,		 	VK_OEM_PLUS},
        { DIK_BACK, 			VK_BACK},
        { DIK_TAB, 			VK_TAB},
        { DIK_Q,				'Q'},
        { DIK_W,				'W'},
        { DIK_E,				'E'},
        { DIK_R,				'R'},
        { DIK_T,				'T'},
        { DIK_Y,				'Y'},
        { DIK_U,				'U'},
        { DIK_I,				'I'},
        { DIK_O,				'O'},
        { DIK_P,				'P' },
        { DIK_LBRACKET, 		VK_OEM_4},
        { DIK_RBRACKET, 		VK_OEM_6},
        { DIK_RETURN, 		VK_RETURN},
        { DIK_LCONTROL, 		VK_LCONTROL},
        { DIK_A,				'A'},
        { DIK_S,				'S' },
        { DIK_D,				'D'},
        { DIK_F,				'F'},
        { DIK_G,				'G'},
        { DIK_H,				'H'},
        { DIK_J,				'J'},
        { DIK_K,				'K'},
        { DIK_L,				'L'},
        { DIK_SEMICOLON,		VK_OEM_1},
        { DIK_APOSTROPHE,		VK_OEM_7},
        { DIK_GRAVE, 			VK_OEM_3},
        { DIK_LSHIFT, 		VK_LSHIFT},
        { DIK_BACKSLASH, 		VK_OEM_5},
        { DIK_Z,				'Z'},
        {DIK_X,				'X'},
        {DIK_C,				'C'},
        {DIK_V,				'V'},
        {DIK_B,				'B'},
        {DIK_N,				'N' },
        {DIK_M,				'M' },
        {DIK_COMMA,			VK_OEM_COMMA },
        {DIK_PERIOD, 		VK_OEM_PERIOD },
        {DIK_SLASH, 			VK_OEM_2 },
        {DIK_RSHIFT, 		VK_RSHIFT},
        {DIK_MULTIPLY, 		VK_MULTIPLY },
        {DIK_LMENU, 			VK_LMENU},
        {DIK_SPACE, 			VK_SPACE },
        {DIK_CAPITAL, 		VK_CAPITAL},
        {DIK_F1,				VK_F1 },
        {DIK_F2,				VK_F2},
        {DIK_F3,				VK_F3},
        {DIK_F4,				VK_F4},
        {DIK_F5,				VK_F5},
        {DIK_F6,				VK_F6},
        {DIK_F7,				VK_F7},
        {DIK_F8,				VK_F8},
        {DIK_F9,				VK_F9},
        {DIK_F10,			VK_F10},
        {DIK_NUMLOCK,		VK_NUMLOCK},
        {DIK_SCROLL,			VK_SCROLL},
        {DIK_NUMPAD7,		VK_NUMPAD7},
        {DIK_NUMPAD8,		VK_NUMPAD8},
        {DIK_NUMPAD9,		VK_NUMPAD9},
        {DIK_SUBTRACT,		VK_SUBTRACT},
        {DIK_NUMPAD4,		VK_NUMPAD4},
        {DIK_NUMPAD5,		VK_NUMPAD5},
        {DIK_NUMPAD6,		VK_NUMPAD6},
        {DIK_ADD,			VK_ADD},
        {DIK_NUMPAD1,		VK_NUMPAD1},
        {DIK_NUMPAD2,		VK_NUMPAD2},
        {DIK_NUMPAD3,		VK_NUMPAD3},
        {DIK_NUMPAD0,		VK_NUMPAD0},
        {DIK_DECIMAL,		VK_DECIMAL},
        {DIK_F11,			VK_F11},
        {DIK_F12,			VK_F12},
        {DIK_F13,			VK_F13},
        {DIK_F14,			VK_F14},
        {DIK_F15,			VK_F15},
        {DIK_NUMPADENTER,	VK_RETURN},
        {DIK_RCONTROL,		VK_RCONTROL},
        {DIK_DIVIDE,			VK_DIVIDE},
        {DIK_SYSRQ, 			0},
        {DIK_RMENU,			VK_RMENU},
        {DIK_HOME,			VK_HOME},
        {DIK_UP,				VK_UP},
        {DIK_PRIOR,			VK_PRIOR},
        {DIK_LEFT,			VK_LEFT},
        {DIK_RIGHT,			VK_RIGHT },
        {DIK_END,			VK_END},
        {DIK_DOWN,			VK_DOWN},
        {DIK_NEXT,			VK_NEXT},
        {DIK_INSERT,			VK_INSERT},
        {DIK_DELETE,			VK_DELETE},
        {DIK_LWIN,			VK_LWIN},
        {DIK_RWIN,			VK_RWIN},
        {DIK_APPS,			VK_APPS},
        {DIK_PAUSE,			VK_PAUSE},
        {0,					VK_CANCEL},
        {DIK_MUTE,			VK_VOLUME_MUTE},
        {DIK_VOLUMEDOWN,		VK_VOLUME_DOWN},
        {DIK_VOLUMEUP,		VK_VOLUME_UP},
        {DIK_WEBHOME,		VK_BROWSER_HOME},
        {DIK_WEBSEARCH,		VK_BROWSER_SEARCH},
        {DIK_WEBFAVORITES,	VK_BROWSER_FAVORITES},
        {DIK_WEBREFRESH,		VK_BROWSER_REFRESH },
        {DIK_WEBSTOP,		VK_BROWSER_STOP},
        {DIK_WEBFORWARD,		VK_BROWSER_FORWARD},
        {DIK_WEBBACK,		VK_BROWSER_BACK},
        {DIK_MAIL,			VK_LAUNCH_MAIL},
        {DIK_MEDIASELECT,	VK_LAUNCH_MEDIA_SELECT},
    };

    for (auto& it : mapping)
    {
        Dik2VKeyCode[it.first] = it.second;
    }
}

WebBrowserControl::WebBrowserControl(std::string url)
{
    InitDikTranslation();

    _browserHandle = INVALID_HTMLBROWSER;
    //if (GCWebBrowserCount++ == 0) // only when first browser opened
        SteamHTMLSurface()->Init();


    SteamAPICall_t hSteamAPICall = SteamHTMLSurface()->CreateBrowser("BIGameEngine", NULL);
    m_SteamCallResultBrowserReady.Set(hSteamAPICall, this, &WebBrowserControl::OnBrowserReady);

    //auto url = "https://arma3.com";
    SetUrl(url);
}

WebBrowserControl::~WebBrowserControl()
{
    if (_browserHandle)
        SteamHTMLSurface()->RemoveBrowser(_browserHandle);
    _browserHandle = INVALID_HTMLBROWSER;

    //if (GCWebBrowserCount-- == 1) //  only when last browser was closed
    //    SteamHTMLSurface()->Shutdown();
}

void WebBrowserControl::OnSizeChanged(unsigned int width, unsigned int height)
{
    // This is called before first Draw

    _controlWidth = width;
    _controlHeight = height;

    if (_browserHandle)
        SteamHTMLSurface()->SetSize(_browserHandle, width, height);
}


void WebBrowserControl::OnDraw(float alpha, ID3D11Texture2D* target)
{

    // If we have nothing to paint, we don't. The texture still contains the previous contents, and if the browser didn't need to repaint then we don't need to
    if (!_currentPaintData)
        return;

    //#TODO load the device context one and store it
    auto deviceDataPtr = reinterpret_cast<const RVExtensionRenderInfo* const *>(FindRVFunction("RVExtensionGData"));
    if (!deviceDataPtr)
        return;

    D3D11_MAPPED_SUBRESOURCE subResource;
    ZeroMemory(&subResource, sizeof(subResource));

    (*deviceDataPtr)->d3dDeviceContext->Map(target, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource); //#TODO error checking?

    //D3D11_TEXTURE2D_DESC desc;
    //target->GetDesc(&desc);
    //subResource.DepthPitch / subResource.RowPitch == desc.Height
    //subResource.RowPitch/4 == desc.Width (not always, DX11 tends to round it up)

    // The browser width sometimes does not match the requested width. Depending on DPI Scaling, you might request a 1024x1024 and get a 1023x1023 texture :/
    if (_browserWidth == subResource.RowPitch/4 && _browserHeight == _controlHeight)
    {
        memcpy(subResource.pData, _currentPaintData, _browserWidth * _browserHeight * 4);
    }
    else
    {
        if (_browserWidth == subResource.RowPitch/4)
        {
            // Width matches, so we can just copy and truncate/fill at end
            memcpy(subResource.pData, _currentPaintData, _browserWidth * std::min(_browserHeight, _controlHeight) * 4);

            // Remainder is just transparent
            memset(static_cast<char*>(subResource.pData) + _browserWidth * std::min(_browserHeight, _controlHeight) * 4, 0, _browserWidth * (std::max(_browserHeight, _controlHeight) - std::min(_browserHeight, _controlHeight)) * 4);
        }
        else
        {
            // We need to copy row-by-row
            // Filling right and bottom edge with transparency (Generally, browserWidth is smaller than controlWidth), or truncating right/bottom edge

            const auto rowCopyWidth = std::min(_browserWidth, subResource.RowPitch/4);
            const auto rowRemainder = std::max(0, static_cast<int>(subResource.RowPitch/4) - static_cast<int>(rowCopyWidth));

            for (unsigned int i = 0; i < std::min(_browserHeight, _controlHeight); ++i)
            {
                memcpy(
                    static_cast<char*>(subResource.pData) + subResource.RowPitch * i,
                    _currentPaintData + _browserWidth*4 * i,
                    rowCopyWidth * 4
                ); // Write as much as we have

                memset(static_cast<char*>(subResource.pData) + subResource.RowPitch * i + rowCopyWidth * 4, 0, rowRemainder * 4); // Fill right edge with transparency
            }

            // Remainder is just transparent
            if (_controlHeight > _browserHeight)
                memset(static_cast<char*>(subResource.pData) + subResource.RowPitch * _browserHeight, 0, subResource.RowPitch * (_controlHeight-_browserHeight));
        }
    }

    (*deviceDataPtr)->d3dDeviceContext->Unmap(target, 0);
    _currentPaintData = nullptr;
}

void WebBrowserControl::OnDestroy(int code)
{
    delete this;
}

bool WebBrowserControl::OnSetFocus(bool focus)
{
    _focused = focus;
    SteamHTMLSurface()->SetKeyFocus(_browserHandle, focus);
    return true;
}

void WebBrowserControl::OnLButtonDown(float x, float y)
{
    if (!_underCursor)
        return;

    OnMouseMove(x, y); // Probably not needed, doing it anyway
    x *= _browserWidth; y *= _browserHeight; // percent to pixels

    SteamHTMLSurface()->MouseDown(_browserHandle, ISteamHTMLSurface::eHTMLMouseButton_Left); 
}

void WebBrowserControl::OnLButtonUp(float x, float y)
{
    if (!_underCursor)
        return;

    OnMouseMove(x, y); // Probably not needed, doing it anyway
    x *= _browserWidth; y *= _browserHeight; // percent to pixels

    SteamHTMLSurface()->MouseUp(_browserHandle, ISteamHTMLSurface::eHTMLMouseButton_Left); 
}

void WebBrowserControl::OnLButtonClick(float x, float y)
{
}

void WebBrowserControl::OnLButtonDblClick(float x, float y)
{
    if (!_underCursor)
        return;

    OnMouseMove(x, y); // Probably not needed, doing it anyway
    x *= _browserWidth; y *= _browserHeight; // percent to pixels

    SteamHTMLSurface()->MouseDoubleClick(_browserHandle, ISteamHTMLSurface::eHTMLMouseButton_Left); 
}

void WebBrowserControl::OnRButtonDown(float x, float y)
{
    if (!_underCursor)
        return;

    OnMouseMove(x, y); // Probably not needed, doing it anyway
    x *= _browserWidth; y *= _browserHeight; // percent to pixels

    SteamHTMLSurface()->MouseDown(_browserHandle, ISteamHTMLSurface::eHTMLMouseButton_Right); 
}

void WebBrowserControl::OnRButtonUp(float x, float y)
{
    if (!_underCursor)
        return;

    OnMouseMove(x, y); // Probably not needed, doing it anyway
    x *= _browserWidth; y *= _browserHeight; // percent to pixels

    SteamHTMLSurface()->MouseUp(_browserHandle, ISteamHTMLSurface::eHTMLMouseButton_Right); 
}

void WebBrowserControl::OnRButtonClick(float x, float y)
{
}

void WebBrowserControl::OnMouseMove(float x, float y)
{
    x *= _browserWidth; y *= _browserHeight; // percent to pixels
    SteamHTMLSurface()->MouseMove(_browserHandle, x, y); 
}

bool WebBrowserControl::OnMouseZChanged(float dz)
{
    if (!_underCursor)
        return false;

    // Calculate that into pixels, lets just say one turny-bit is 5% of screen height
    dz *= _browserHeight * 0.05f; //#TODO scroll speed scale factor

    SteamHTMLSurface()->MouseWheel(_browserHandle, dz); 
    return true;
}

void WebBrowserControl::OnMouseEnter(float x, float y)
{
    _underCursor = true;
}

void WebBrowserControl::OnMouseExit(float x, float y)
{
    _underCursor = false;
}

static bool IsVkDown(int vk)
{
    return (::GetKeyState(vk) & 0x8000) != 0;
}

static ISteamHTMLSurface::EHTMLKeyModifiers GetKeyMod()
{
    int kMod = ISteamHTMLSurface::k_eHTMLKeyModifier_None;
    if (IsVkDown(VK_CONTROL))
        kMod |= ISteamHTMLSurface::k_eHTMLKeyModifier_CtrlDown;
    if (IsVkDown(VK_MENU))
        kMod |= ISteamHTMLSurface::k_eHTMLKeyModifier_AltDown;
    if (IsVkDown(VK_SHIFT))
        kMod |= ISteamHTMLSurface::k_eHTMLKeyModifier_ShiftDown;

    return static_cast<ISteamHTMLSurface::EHTMLKeyModifiers>(kMod);
}

bool WebBrowserControl::OnChar(unsigned nChar, unsigned nRepCnt, unsigned nFlags)
{
    if (!_underCursor)
        return false;

    SteamHTMLSurface()->KeyChar(_browserHandle, nChar, GetKeyMod());

    return true;
}


bool WebBrowserControl::OnKeyDown(int dikCode)
{
    if (!_underCursor)
        return false;

    if (dikCode == DIK_ESCAPE)
    {
        return false; // Let parent handle it
    }

    int vk = Dik2VKeyCode[dikCode];
    SteamHTMLSurface()->KeyDown(_browserHandle, vk, GetKeyMod(), true);

    if (vk == VK_LSHIFT || vk == VK_RSHIFT) SteamHTMLSurface()->KeyDown(_browserHandle, VK_SHIFT, GetKeyMod(), true);
    if (vk == VK_LMENU || vk == VK_RMENU) SteamHTMLSurface()->KeyDown(_browserHandle, VK_MENU, GetKeyMod(), true);
    if (vk == VK_LCONTROL || vk == VK_LCONTROL) SteamHTMLSurface()->KeyDown(_browserHandle, VK_CONTROL, GetKeyMod(), true);
    return true;
}

bool WebBrowserControl::OnKeyUp(int dikCode)
{
    if (!_underCursor)
        return false;

    int vk = Dik2VKeyCode[dikCode];
    SteamHTMLSurface()->KeyUp(_browserHandle, vk, GetKeyMod());

    if (vk == VK_LSHIFT || vk == VK_RSHIFT) SteamHTMLSurface()->KeyUp(_browserHandle, VK_SHIFT, GetKeyMod());
    if (vk == VK_LMENU || vk == VK_RMENU) SteamHTMLSurface()->KeyUp(_browserHandle, VK_MENU, GetKeyMod());
    if (vk == VK_LCONTROL || vk == VK_LCONTROL) SteamHTMLSurface()->KeyUp(_browserHandle, VK_CONTROL, GetKeyMod());
    return true;
}

void WebBrowserControl::OnStartRequest(HTML_StartRequest_t *pParam) const
{
    if (pParam->unBrowserHandle != _browserHandle) //#TODO macro this away
        return; // We received the update for a different browser instance

    //#TODO whitelist/safety filtering?

    // MUST call AllowStartRequest once for every OnStartRequest callback!
    SteamHTMLSurface()->AllowStartRequest(_browserHandle, true);
}

void WebBrowserControl::OnJSAlert(HTML_JSAlert_t *pParam)
{
    if (pParam->unBrowserHandle != _browserHandle)
        return; // We received the update for a different browser instance

    /*
    if (script)
    {
        // Trigger a script callback
        // Script can display a message box
        // Script either sends back result from message box or just uses this for communication
        // But you must always call JSDialogResponse

        // It is handled by Script, let it do that

        SteamHTMLSurface()->JSDialogResponse(_browserHandle, result);
        // else the script must manually use UICtrlWebBrowserAction command to set response
        return;
    }
    */

    // We cannot forward it, just say no
    SteamHTMLSurface()->JSDialogResponse(_browserHandle, false);
}

void WebBrowserControl::OnJSConfirm(HTML_JSConfirm_t *pParam)
{
    if (pParam->unBrowserHandle != _browserHandle)
        return; // We received the update for a different browser instance

    /*
    if (script)
    {
        // Trigger a script callback
        // Script can display a message box
        // Script either sends back result from message box or just uses this for communication
        // But you must always call JSDialogResponse

        // It is handled by Script, let it do that

        SteamHTMLSurface()->JSDialogResponse(_browserHandle, result);
        // else the script must manually use UICtrlWebBrowserAction command to set response
        return;
    }
    */

    // We cannot forward it, just say no
    SteamHTMLSurface()->JSDialogResponse(_browserHandle, false);
}

void WebBrowserControl::OnUploadLocalFile(HTML_FileOpenDialog_t *pParam)
{
    if (pParam->unBrowserHandle != _browserHandle)
        return; // We received the update for a different browser instance

    // MUST call FileLoadDialogResponse once for every OnLocalFileBrowse callback!

    // Most applications do NOT want to allow the web browser to upload local file
    // content from the customer's hard drive to the remote web server! That would
    // be a pretty big security hole, unless you carefully vetted every file path.
    SteamHTMLSurface()->FileLoadDialogResponse(_browserHandle, NULL);

    // But if you did want to allow it, you would do something like this:
    // ... show modal file selection dialog box ...
    // ... verify that the selected files are safe to upload ...
    // std::vector< const char * > vecUTF8FilenamesArray;
    // ... populate vecUTF8FilenamesArray ...
    // vecUTF8FilenamesArray.push_back(NULL);
    // SteamHTMLSurface()->FileLoadDialogResponse(_browserHandle, &vecUTF8FilenamesArray[0]);
}

void WebBrowserControl::OnNeedsPaint(HTML_NeedsPaint_t *pParam)
{
    if (pParam->unBrowserHandle != _browserHandle)
        return; // We received the update for a different browser instance

    {
        _browserWidth = pParam->unWide;
        _browserHeight = pParam->unTall;

        _bufferData.resize(pParam->unWide * pParam->unTall);
        memcpy(_bufferData.data(), pParam->pBGRA, pParam->unWide * pParam->unTall * 4);

        // pParam->pBGRA invalidates on next call to SteamAPI_RunCallbacks, we have to keep it so sadly we'll waste some memory here
        _currentPaintData = reinterpret_cast<char*>(_bufferData.data()); 
    }
}

void WebBrowserControl::OnCloseBrowser(HTML_CloseBrowser_t *pParam)
{
    if (pParam->unBrowserHandle != _browserHandle)
        return; // We received the update for a different browser instance

    //#TODO we need to tell script to close the UI?

    _browserHandle = INVALID_HTMLBROWSER;
}

void WebBrowserControl::OnFinishedRequest(HTML_FinishedRequest_t *pParam)
{
    if (pParam->unBrowserHandle != _browserHandle)
        return; // We received the update for a different browser instance

}

void WebBrowserControl::OnBrowserRestarted(HTML_BrowserRestarted_t *pParam)
{
    if (pParam->unOldBrowserHandle == _browserHandle)
    {
        HTML_BrowserReady_t ready;
        ready.unBrowserHandle = pParam->unBrowserHandle;;
        OnBrowserReady(&ready, false);
    }
}

void WebBrowserControl::OnBrowserReady(HTML_BrowserReady_t *pBrowserReady, bool bIOFailure)
{
    if (bIOFailure)
        return;

    _browserHandle = pBrowserReady->unBrowserHandle;
    SteamHTMLSurface()->SetSize(_browserHandle, _controlWidth, _controlHeight);
    SteamHTMLSurface()->SetDPIScalingFactor(_browserHandle, 1.0f);
    SteamHTMLSurface()->SetPageScaleFactor(_browserHandle, 1.2f, 0, 0);
    //SteamHTMLSurface()->LoadURL(_browserHandle, "http://steamcommunity.com/", NULL);
    //SteamHTMLSurface()->LoadURL(_browserHandle, "https://www.youtube.com/watch_popup?v=xvFZjo5PgG0&autoplay=1&loop=1", NULL);
    SteamHTMLSurface()->LoadURL(_browserHandle, _url.data(), NULL);
    //SteamHTMLSurface()->SetBackgroundMode(_browserHandle, true);
    //SteamHTMLSurface()->OpenDeveloperTools(_browserHandle);
}



void WebBrowserControl::SetUrl(std::string_view url)
{
    _url = url;

    if (_browserHandle)
    {
        // Browser already open
        SteamHTMLSurface()->LoadURL(_browserHandle, _url.data(), NULL);
    }
    // else browser will open soon, and we will find _url once it does
}

void WebBrowserControl::ExecuteJavascript(const std::string& code)
{
    if (!_browserHandle)
        return; // Browser not yet open

    SteamHTMLSurface()->ExecuteJavascript(_browserHandle, code.data());
}

void WebBrowserControl::OpenDeveloperTools()
{
    if (!_browserHandle)
        return; // Browser not yet open

    SteamHTMLSurface()->OpenDeveloperTools(_browserHandle);
}

void WebBrowserControl::JSDialogResponse(bool resp)
{
    if (!_browserHandle)
        return; // Browser not yet open

    SteamHTMLSurface()->JSDialogResponse(_browserHandle, resp);
}
