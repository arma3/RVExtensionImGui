#include <algorithm>
#include <string>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <vector>
#include <iostream>
#include <string_view>

#include "ImguiExtensionControl.hpp"
#include "RVExtensionUtil.hpp"
#include "WebBrowserControl.hpp"

#ifndef _WIN32
#include <signal.h>
#define debugBreak raise(SIGTRAP)
#else
#define debugBreak __debugbreak()
#endif

extern void ImguiRenderTick();

void CALL_CONVENTION RVExtension(char *output, unsigned int outputSize, const char *function)
{
	if (std::string_view(function) == "rendr")
	{
		ImguiRenderTick();
	}

	output[outputSize - 1] = 0;
}

void CALL_CONVENTION RVExtensionVersion(char *output, unsigned int outputSize)
{
	std::strncpy(output, "Test-Extension v1.0", outputSize - 1);
}

bool CALL_CONVENTION RVExtensionRequestUI(const char* uiClass, CExtensionControlInterface* iface)
{
	if (iface->size != sizeof(CExtensionControlInterface)) // Engine version missmatches, we could handle backwards/forwards compatibility here by checking what size it is
		return false;

	auto className = std::string_view(uiClass);

	if (className == "imgui")
	{
		auto newControl = new ImguiExtensionControl();
		iface->flags = CExtensionControlInterface::Flags::None; // Target is RenderTargetView
		iface->context = newControl;

		iface->OnDraw = [](void* context, float alpha, void* target) { return static_cast<ImguiExtensionControl*>(context)->OnDraw(alpha, static_cast<ID3D11RenderTargetView*>(target)); };
		iface->OnDestroy = [](void* context, int code) { return static_cast<ImguiExtensionControl*>(context)->OnDestroy(code); };

		iface->OnSizeChanged = [](void* context, unsigned int width, unsigned int height) { return static_cast<ImguiExtensionControl*>(context)->OnSizeChanged(width, height); };
		iface->OnSetFocus = [](void* context, bool focus) { return static_cast<ImguiExtensionControl*>(context)->OnSetFocus(focus); };

		iface->OnLButtonDown = [](void* context, float x, float y) { return static_cast<ImguiExtensionControl*>(context)->OnLButtonDown(x, y); };
		iface->OnLButtonUp = [](void* context, float x, float y) { return static_cast<ImguiExtensionControl*>(context)->OnLButtonUp(x, y); };
		iface->OnLButtonClick = [](void* context, float x, float y) { return static_cast<ImguiExtensionControl*>(context)->OnLButtonClick(x, y); };
		iface->OnLButtonDblClick = [](void* context, float x, float y) { return static_cast<ImguiExtensionControl*>(context)->OnLButtonDblClick(x, y); };

		iface->OnRButtonDown = [](void* context, float x, float y) { return static_cast<ImguiExtensionControl*>(context)->OnRButtonDown(x, y); };
		iface->OnRButtonUp = [](void* context, float x, float y) { return static_cast<ImguiExtensionControl*>(context)->OnRButtonUp(x, y); };
		iface->OnRButtonClick = [](void* context, float x, float y) { return static_cast<ImguiExtensionControl*>(context)->OnRButtonClick(x, y); };

		iface->OnMouseMove = [](void* context, float x, float y) { return static_cast<ImguiExtensionControl*>(context)->OnMouseMove(x, y); };
		iface->OnMouseZChanged = [](void* context, float dz) { return static_cast<ImguiExtensionControl*>(context)->OnMouseZChanged(dz); };

		iface->OnMouseEnter = [](void* context, float x, float y) { return static_cast<ImguiExtensionControl*>(context)->OnMouseEnter(x, y); };
		iface->OnMouseExit = [](void* context, float x, float y) { return static_cast<ImguiExtensionControl*>(context)->OnMouseExit(x, y); };

		iface->OnKeyDown = [](void* context, int dikCode) { return static_cast<ImguiExtensionControl*>(context)->OnKeyDown(dikCode); };
		iface->OnKeyUp = [](void* context, int dikCode) { return static_cast<ImguiExtensionControl*>(context)->OnKeyUp(dikCode); };
		iface->OnChar = [](void* context, unsigned nChar, unsigned nRepCnt, unsigned nFlags) { return static_cast<ImguiExtensionControl*>(context)->OnChar(nChar, nRepCnt, nFlags); };
	}
	else if (className.substr(0, 10) == "webbrowser") // webbrowser_urlhere
	{
		auto url = className.substr(11);

#ifdef ISTEAMHTMLSURFACE_H

		auto newControl = new WebBrowserControl(std::string(url));
		iface->flags = static_cast<CExtensionControlInterface::Flags>(CExtensionControlInterface::Flags::DynamicTexture | CExtensionControlInterface::Flags::TextureFormatBGRA); // Target is Texure2D
		iface->context = newControl;

		iface->OnDraw = [](void* context, float alpha, void* target) { return static_cast<WebBrowserControl*>(context)->OnDraw(alpha, static_cast<ID3D11Texture2D*>(target)); };
		iface->OnDestroy = [](void* context, int code) { return static_cast<WebBrowserControl*>(context)->OnDestroy(code); };

		iface->OnSizeChanged = [](void* context, unsigned int width, unsigned int height) { return static_cast<WebBrowserControl*>(context)->OnSizeChanged(width, height); };
		iface->OnSetFocus = [](void* context, bool focus) { return static_cast<WebBrowserControl*>(context)->OnSetFocus(focus); };

		iface->OnLButtonDown = [](void* context, float x, float y) { return static_cast<WebBrowserControl*>(context)->OnLButtonDown(x, y); };
		iface->OnLButtonUp = [](void* context, float x, float y) { return static_cast<WebBrowserControl*>(context)->OnLButtonUp(x, y); };
		iface->OnLButtonClick = [](void* context, float x, float y) { return static_cast<WebBrowserControl*>(context)->OnLButtonClick(x, y); };
		iface->OnLButtonDblClick = [](void* context, float x, float y) { return static_cast<WebBrowserControl*>(context)->OnLButtonDblClick(x, y); };

		iface->OnRButtonDown = [](void* context, float x, float y) { return static_cast<WebBrowserControl*>(context)->OnRButtonDown(x, y); };
		iface->OnRButtonUp = [](void* context, float x, float y) { return static_cast<WebBrowserControl*>(context)->OnRButtonUp(x, y); };
		iface->OnRButtonClick = [](void* context, float x, float y) { return static_cast<WebBrowserControl*>(context)->OnRButtonClick(x, y); };

		iface->OnMouseMove = [](void* context, float x, float y) { return static_cast<WebBrowserControl*>(context)->OnMouseMove(x, y); };
		iface->OnMouseZChanged = [](void* context, float dz) { return static_cast<WebBrowserControl*>(context)->OnMouseZChanged(dz); };

		iface->OnMouseEnter = [](void* context, float x, float y) { return static_cast<WebBrowserControl*>(context)->OnMouseEnter(x, y); };
		iface->OnMouseExit = [](void* context, float x, float y) { return static_cast<WebBrowserControl*>(context)->OnMouseExit(x, y); };

		iface->OnKeyDown = [](void* context, int dikCode) { return static_cast<WebBrowserControl*>(context)->OnKeyDown(dikCode); };
		iface->OnKeyUp = [](void* context, int dikCode) { return static_cast<WebBrowserControl*>(context)->OnKeyUp(dikCode); };
		iface->OnChar = [](void* context, unsigned nChar, unsigned nRepCnt, unsigned nFlags) { return static_cast<WebBrowserControl*>(context)->OnChar(nChar, nRepCnt, nFlags); };

#endif // ISTEAMHTMLSURFACE_H
	}


	return true;
}