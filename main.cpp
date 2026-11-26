#include "main_window.h"
#include <windows.h>
#include <commctrl.h>

#pragma comment(lib, "comctl32.lib")

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int showCommand)
{
    INITCOMMONCONTROLSEX controls{ sizeof(controls), ICC_BAR_CLASSES };
    InitCommonControlsEx(&controls);
    MainWindow app(instance);
    if (!app.Create())
        return 1;

    ShowWindow(app.Handle(), showCommand);
    UpdateWindow(app.Handle());

    MSG message{};
    while (GetMessageW(&message, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    return static_cast<int>(message.wParam);
}
