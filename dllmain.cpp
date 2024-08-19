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

	auto newControl = new ImguiExtensionControl();
	iface->context = newControl;

	iface->OnDraw = [](void* context, float alpha) { return static_cast<ImguiExtensionControl*>(context)->OnDraw(alpha); };
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

	return true;
}