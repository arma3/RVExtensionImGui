#pragma once
#include <chrono>

#include "imgui_internal.h"


class ImguiExtensionControl
{
    ImGuiContext* _context;
    bool show_demo_window = true;

    uint32_t _width = 0;
    uint32_t _height = 0;

public:
    ImguiExtensionControl();
    ~ImguiExtensionControl();

    void OnSizeChanged(unsigned int width, unsigned int height);

    void OnDraw(float alpha);
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
};

