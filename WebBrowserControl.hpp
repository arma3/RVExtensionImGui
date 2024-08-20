#pragma once
#pragma once
#include <cstdint>
#include <string_view>
#include <vector>

// https://partner.steamgames.com/doc/sdk
// Download version 1.57 and place "sdk" folder into lib

#if __has_include("lib/sdk/public/steam/isteamhtmlsurface.h")
#include "lib/sdk/public/steam/isteamhtmlsurface.h"
#endif

#ifdef ISTEAMHTMLSURFACE_H
#pragma comment(lib, "lib/sdk/redistributable_bin/win64/steam_api64.lib")

struct ID3D11Texture2D;

class ISteamHTMLSurface;

class WebBrowserControl
{
    uint32_t _controlWidth = 1024;
    uint32_t _controlHeight = 1024;

    uint32_t _browserWidth = 0;
    uint32_t _browserHeight = 0;

    HHTMLBrowser _browserHandle;
    std::vector<uint32_t> _bufferData;
    const char* _currentPaintData = nullptr;

    std::string _url;

    bool _focused = false;
    bool _underCursor = false;

public:
    WebBrowserControl(std::string url);
    ~WebBrowserControl();

    void OnSizeChanged(unsigned int width, unsigned int height);

    void OnDraw(float alpha, ID3D11Texture2D* target);
    void OnDestroy(int code);
    bool OnSetFocus(bool focus);

    void OnLButtonDown(float x, float y);
    void OnLButtonUp(float x, float y);
    void OnLButtonClick(float x, float y);
    void OnLButtonDblClick(float x, float y);

    void OnRButtonDown(float x, float y);
    void OnRButtonUp(float x, float y);
    void OnRButtonClick(float x, float y);

    void OnMouseMove(float x, float y);
    bool OnMouseZChanged(float dz);
    void OnMouseEnter(float x, float y);
    void OnMouseExit(float x, float y);

    bool OnChar(unsigned nChar, unsigned nRepCnt, unsigned nFlags);
    bool OnKeyDown(int dikCode);
    bool OnKeyUp(int dikCode);


    // Steam

    STEAM_CALLBACK( WebBrowserControl, OnStartRequest, HTML_StartRequest_t ) const; // REQUIRED
    STEAM_CALLBACK( WebBrowserControl, OnJSAlert, HTML_JSAlert_t ); // REQUIRED
    STEAM_CALLBACK( WebBrowserControl, OnJSConfirm, HTML_JSConfirm_t ); // REQUIRED
    STEAM_CALLBACK( WebBrowserControl, OnUploadLocalFile, HTML_FileOpenDialog_t ); // REQUIRED

    STEAM_CALLBACK( WebBrowserControl, OnNeedsPaint, HTML_NeedsPaint_t );
    STEAM_CALLBACK( WebBrowserControl, OnCloseBrowser, HTML_CloseBrowser_t );
    STEAM_CALLBACK( WebBrowserControl, OnFinishedRequest, HTML_FinishedRequest_t );
    STEAM_CALLBACK( WebBrowserControl, OnBrowserRestarted, HTML_BrowserRestarted_t );


    void OnBrowserReady( HTML_BrowserReady_t *pBrowserReady, bool bIOFailure );
    CCallResult< WebBrowserControl, HTML_BrowserReady_t > m_SteamCallResultBrowserReady;

    // API

    void SetUrl(std::string_view url);
    void ExecuteJavascript(const std::string& code);
    void OpenDeveloperTools();
    void JSDialogResponse(bool resp);
};

#endif // ISTEAMHTMLSURFACE_H