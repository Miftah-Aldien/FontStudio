#pragma once
#include <windows.h>
#include <string>

class MainWindow
{
public:
    explicit MainWindow(HINSTANCE instance);
    bool Create();
    HWND Handle() const { return hwnd_; }

private:
    static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
    LRESULT HandleMessage(UINT, WPARAM, LPARAM);
    void CreateMenus();
    void CreateToolbar();
    void CreateChildPanels();
    void LayoutChildren(int width, int height);
    void PaintOverview(HDC dc, const RECT& area);
    void PaintEditor(HDC dc, const RECT& area);

    HINSTANCE instance_;
    HWND hwnd_ = nullptr;
    HWND overview_ = nullptr;
    HWND editor_ = nullptr;
    HWND properties_ = nullptr;
    HFONT uiFont_ = nullptr;
    int selectedGlyph_ = 65;
};
