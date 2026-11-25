#include "main_window.h"
#include <windowsx.h>
#include <commctrl.h>
#include <vector>
#include <sstream>

#pragma comment(lib, "comctl32.lib")

namespace {
constexpr wchar_t kClassName[] = L"FontStudio15MainWindow";
constexpr int ID_FILE_NEW = 1001;
constexpr int ID_FILE_OPEN = 1002;
constexpr int ID_FILE_SAVE = 1003;
constexpr int ID_FILE_EXIT = 1004;
constexpr int ID_VIEW_OVERVIEW = 1101;
constexpr int ID_VIEW_EDITOR = 1102;
constexpr int ID_GLYPH_PREVIOUS = 1201;
constexpr int ID_GLYPH_NEXT = 1202;
constexpr int ID_GLYPH_PROPERTIES = 1203;
constexpr int ID_TOOLBAR = 2001;
constexpr int ID_OVERVIEW = 2002;
constexpr int ID_EDITOR = 2003;
constexpr int ID_PROPERTIES = 2004;
}

MainWindow::MainWindow(HINSTANCE instance) : instance_(instance) {}

bool MainWindow::Create()
{
    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = instance_;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
    wc.lpszClassName = kClassName;
    if (!RegisterClassExW(&wc) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
        return false;

    hwnd_ = CreateWindowExW(0, kClassName, L"FontStudio 15 - Untitled Font",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1120, 720,
        nullptr, nullptr, instance_, this);
    return hwnd_ != nullptr;
}

LRESULT CALLBACK MainWindow::WindowProc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp)
{
    MainWindow* self = reinterpret_cast<MainWindow*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    if (message == WM_NCCREATE) {
        auto* create = reinterpret_cast<CREATESTRUCTW*>(lp);
        self = static_cast<MainWindow*>(create->lpCreateParams);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
        self->hwnd_ = hwnd;
    }
    return self ? self->HandleMessage(message, wp, lp)
                : DefWindowProcW(hwnd, message, wp, lp);
}

void MainWindow::CreateMenus()
{
    HMENU bar = CreateMenu();
    HMENU file = CreatePopupMenu();
    AppendMenuW(file, MF_STRING, ID_FILE_NEW, L"&New Font...");
    AppendMenuW(file, MF_STRING, ID_FILE_OPEN, L"&Open...");
    AppendMenuW(file, MF_STRING, ID_FILE_SAVE, L"&Save");
    AppendMenuW(file, MF_SEPARATOR, 0, nullptr);
    AppendMenuW(file, MF_STRING, ID_FILE_EXIT, L"E&xit");
    AppendMenuW(bar, MF_POPUP, reinterpret_cast<UINT_PTR>(file), L"&File");

    HMENU edit = CreatePopupMenu();
    AppendMenuW(edit, MF_STRING, 0, L"&Undo");
    AppendMenuW(edit, MF_STRING, 0, L"&Redo");
    AppendMenuW(edit, MF_SEPARATOR, 0, nullptr);
    AppendMenuW(edit, MF_STRING, 0, L"Cu&t");
    AppendMenuW(edit, MF_STRING, 0, L"&Copy");
    AppendMenuW(edit, MF_STRING, 0, L"&Paste");
    AppendMenuW(bar, MF_POPUP, reinterpret_cast<UINT_PTR>(edit), L"&Edit");

    HMENU view = CreatePopupMenu();
    AppendMenuW(view, MF_STRING, ID_VIEW_OVERVIEW, L"Font &Overview");
    AppendMenuW(view, MF_STRING, ID_VIEW_EDITOR, L"Glyph &Editor");
    AppendMenuW(bar, MF_POPUP, reinterpret_cast<UINT_PTR>(view), L"&View");

    HMENU insert = CreatePopupMenu();
    AppendMenuW(insert, MF_STRING, ID_GLYPH_PROPERTIES, L"&Glyph...");
    AppendMenuW(bar, MF_POPUP, reinterpret_cast<UINT_PTR>(insert), L"&Insert");

    HMENU format = CreatePopupMenu();
    AppendMenuW(format, MF_STRING, 0, L"&Font Properties...");
    AppendMenuW(format, MF_STRING, 0, L"&Metrics...");
    AppendMenuW(bar, MF_POPUP, reinterpret_cast<UINT_PTR>(format), L"F&ormat");

    HMENU tools = CreatePopupMenu();
    AppendMenuW(tools, MF_STRING, 0, L"&Validate Font");
    AppendMenuW(tools, MF_STRING, 0, L"&Options...");
    AppendMenuW(bar, MF_POPUP, reinterpret_cast<UINT_PTR>(tools), L"&Tools");

    HMENU window = CreatePopupMenu();
    AppendMenuW(window, MF_STRING, ID_VIEW_OVERVIEW, L"Font Overview");
    AppendMenuW(window, MF_STRING, ID_VIEW_EDITOR, L"Glyph Editor");
    AppendMenuW(bar, MF_POPUP, reinterpret_cast<UINT_PTR>(window), L"&Window");

    HMENU help = CreatePopupMenu();
    AppendMenuW(help, MF_STRING, 0, L"&About FontStudio 15");
    AppendMenuW(bar, MF_POPUP, reinterpret_cast<UINT_PTR>(help), L"&Help");
    SetMenu(hwnd_, bar);
}

void MainWindow::CreateToolbar()
{
    HWND toolbar = CreateWindowExW(0, TOOLBARCLASSNAMEW, nullptr,
        WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS | CCS_TOP,
        0, 0, 0, 0, hwnd_, reinterpret_cast<HMENU>(ID_TOOLBAR), instance_, nullptr);
    if (toolbar) {
        SendMessageW(toolbar, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);
        TBBUTTON buttons[3]{};
        buttons[0].iBitmap = I_IMAGENONE; buttons[0].idCommand = ID_FILE_NEW;
        buttons[0].fsState = TBSTATE_ENABLED; buttons[0].fsStyle = BTNS_BUTTON;
        buttons[1].iBitmap = I_IMAGENONE; buttons[1].idCommand = ID_FILE_OPEN;
        buttons[1].fsState = TBSTATE_ENABLED; buttons[1].fsStyle = BTNS_BUTTON;
        buttons[2].iBitmap = I_IMAGENONE; buttons[2].idCommand = ID_FILE_SAVE;
        buttons[2].fsState = TBSTATE_ENABLED; buttons[2].fsStyle = BTNS_BUTTON;
        SendMessageW(toolbar, TB_ADDBUTTONS, 3, reinterpret_cast<LPARAM>(buttons));
        SendMessageW(toolbar, TB_AUTOSIZE, 0, 0);
    }
}

void MainWindow::CreateChildPanels()
{
    overview_ = CreateWindowExW(WS_EX_CLIENTEDGE, L"STATIC", L"",
        WS_CHILD | WS_VISIBLE | SS_NOTIFY, 0, 0, 100, 100,
        hwnd_, reinterpret_cast<HMENU>(ID_OVERVIEW), instance_, nullptr);
    editor_ = CreateWindowExW(WS_EX_CLIENTEDGE, L"STATIC", L"",
        WS_CHILD | WS_VISIBLE | SS_NOTIFY, 0, 0, 100, 100,
        hwnd_, reinterpret_cast<HMENU>(ID_EDITOR), instance_, nullptr);
    properties_ = CreateWindowExW(WS_EX_CLIENTEDGE, L"STATIC",
        L"Glyph Properties\r\n\r\nCharacter: A\r\nUnicode: U+0041\r\n\r\nMetrics\r\nAdvance Width: 600\r\nLeft Side Bearing: 0",
        WS_CHILD | WS_VISIBLE | SS_LEFT, 0, 0, 100, 100,
        hwnd_, reinterpret_cast<HMENU>(ID_PROPERTIES), instance_, nullptr);
    uiFont_ = static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT));
    SendMessageW(properties_, WM_SETFONT, reinterpret_cast<WPARAM>(uiFont_), TRUE);
}

void MainWindow::LayoutChildren(int width, int height)
{
    HWND toolbar = GetDlgItem(hwnd_, ID_TOOLBAR);
    RECT tr{};
    int top = 0;
    if (toolbar && GetWindowRect(toolbar, &tr))
        top = tr.bottom - tr.top;
    const int margin = 8;
    const int leftWidth = 290;
    const int rightWidth = 205;
    int contentHeight = height - top - margin * 2;
    MoveWindow(overview_, margin, top + margin, leftWidth, contentHeight, TRUE);
    MoveWindow(editor_, leftWidth + margin * 2, top + margin,
        width - leftWidth - rightWidth - margin * 4, contentHeight, TRUE);
    MoveWindow(properties_, width - rightWidth - margin, top + margin,
        rightWidth, contentHeight, TRUE);
}

void MainWindow::PaintOverview(HDC dc, const RECT& area)
{
    FillRect(dc, &area, GetSysColorBrush(COLOR_WINDOW));
    SetBkMode(dc, TRANSPARENT);
    RECT title = area;
    title.bottom = title.top + 30;
    DrawTextW(dc, L"  Font Overview     |     Basic Latin", -1, &title,
        DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    const int cellW = 42, cellH = 48;
    const int startX = area.left + 8, startY = area.top + 36;
    for (int i = 0; i < 96; ++i) {
        int x = startX + (i % 6) * cellW;
        int y = startY + (i / 6) * cellH;
        RECT cell{ x, y, x + cellW - 3, y + cellH - 3 };
        HBRUSH brush = CreateSolidBrush((i + 32 == selectedGlyph_)
            ? RGB(205, 225, 250) : RGB(248, 248, 248));
        FillRect(dc, &cell, brush);
        DeleteObject(brush);
        FrameRect(dc, &cell, GetSysColorBrush(COLOR_3DSHADOW));
        wchar_t glyph[2] = { static_cast<wchar_t>(i + 32), 0 };
        RECT glyphRect = cell;
        glyphRect.top += 3;
        DrawTextW(dc, glyph, -1, &glyphRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        wchar_t code[8]{};
        wsprintfW(code, L"%04X", i + 32);
        RECT codeRect = cell;
        codeRect.top = cell.bottom - 15;
        DrawTextW(dc, code, -1, &codeRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
}

void MainWindow::PaintEditor(HDC dc, const RECT& area)
{
    FillRect(dc, &area, GetSysColorBrush(COLOR_WINDOW));
    SetBkMode(dc, TRANSPARENT);
    RECT heading = area;
    heading.bottom = heading.top + 28;
    DrawTextW(dc, L"  Glyph Editor     |     Selected character", -1, &heading,
        DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    RECT canvas{ area.left + 18, area.top + 42, area.right - 18, area.bottom - 24 };
    HBRUSH white = CreateSolidBrush(RGB(255,255,255));
    FillRect(dc, &canvas, white);
    DeleteObject(white);
    HPEN guide = CreatePen(PS_DOT, 1, RGB(185, 195, 205));
    HPEN oldPen = static_cast<HPEN>(SelectObject(dc, guide));
    int baseline = canvas.bottom - 75;
    int xHeight = baseline - 145;
    int capHeight = baseline - 205;
    int ascender = baseline - 245;
    int descender = baseline + 45;
    MoveToEx(dc, canvas.left, baseline, nullptr); LineTo(dc, canvas.right, baseline);
    MoveToEx(dc, canvas.left, xHeight, nullptr); LineTo(dc, canvas.right, xHeight);
    MoveToEx(dc, canvas.left, capHeight, nullptr); LineTo(dc, canvas.right, capHeight);
    MoveToEx(dc, canvas.left, ascender, nullptr); LineTo(dc, canvas.right, ascender);
    MoveToEx(dc, canvas.left, descender, nullptr); LineTo(dc, canvas.right, descender);
    SelectObject(dc, oldPen); DeleteObject(guide);

    const wchar_t* labels[] = { L"Ascender", L"Cap Height", L"x-Height", L"Baseline", L"Descender" };
    int ys[] = { ascender, capHeight, xHeight, baseline, descender };
    for (int i = 0; i < 5; ++i) {
        RECT label{ canvas.left + 5, ys[i] - 17, canvas.left + 90, ys[i] };
        DrawTextW(dc, labels[i], -1, &label, DT_LEFT | DT_BOTTOM | DT_SINGLELINE);
    }

    HFONT large = CreateFontW(210, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY, DEFAULT_PITCH, L"Times New Roman");
    HFONT oldFont = static_cast<HFONT>(SelectObject(dc, large));
    RECT glyphRect{ canvas.left + 100, canvas.top + 15, canvas.right - 40, baseline + 30 };
    wchar_t glyph[2] = { static_cast<wchar_t>(selectedGlyph_), 0 };
    DrawTextW(dc, glyph, -1, &glyphRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    SelectObject(dc, oldFont); DeleteObject(large);
}

LRESULT MainWindow::HandleMessage(UINT message, WPARAM wp, LPARAM lp)
{
    switch (message) {
    case WM_CREATE:
        CreateMenus();
        CreateToolbar();
        CreateChildPanels();
        return 0;
    case WM_SIZE:
        LayoutChildren(LOWORD(lp), HIWORD(lp));
        InvalidateRect(overview_, nullptr, TRUE);
        InvalidateRect(editor_, nullptr, TRUE);
        return 0;
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case ID_FILE_EXIT: DestroyWindow(hwnd_); return 0;
        case ID_GLYPH_PREVIOUS: if (selectedGlyph_ > 32) --selectedGlyph_; break;
        case ID_GLYPH_NEXT: if (selectedGlyph_ < 126) ++selectedGlyph_; break;
        case ID_FILE_NEW:
            MessageBoxW(hwnd_, L"New Font wizard will be added in a later milestone.",
                L"FontStudio 15", MB_OK | MB_ICONINFORMATION); return 0;
        case ID_FILE_OPEN:
        case ID_FILE_SAVE:
            MessageBoxW(hwnd_, L"Font file operations are planned for a later milestone.",
                L"FontStudio 15", MB_OK | MB_ICONINFORMATION); return 0;
        default: break;
        }
        InvalidateRect(overview_, nullptr, TRUE);
        InvalidateRect(editor_, nullptr, TRUE);
        return 0;
    case WM_LBUTTONDOWN: {
        POINT p{ GET_X_LPARAM(lp), GET_Y_LPARAM(lp) };
        RECT r{}; GetWindowRect(overview_, &r);
        POINT origin{ r.left, r.top }; ScreenToClient(hwnd_, &origin);
        p.x -= origin.x; p.y -= origin.y;
        if (p.x >= 8 && p.y >= 36) {
            int col = (p.x - 8) / 42, row = (p.y - 36) / 48;
            if (col >= 0 && col < 6 && row >= 0 && row < 16) {
                int value = 32 + row * 6 + col;
                if (value <= 127) selectedGlyph_ = value;
                InvalidateRect(overview_, nullptr, TRUE);
                InvalidateRect(editor_, nullptr, TRUE);
            }
        }
        return 0;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps{};
        HDC dc = BeginPaint(hwnd_, &ps);
        EndPaint(hwnd_, &ps);
        return 0;
    }
    case WM_CTLCOLORSTATIC: {
        HWND child = reinterpret_cast<HWND>(lp);
        if (child == overview_ || child == editor_) {
            HDC dc = reinterpret_cast<HDC>(wp);
            RECT r{}; GetClientRect(child, &r);
            if (child == overview_) PaintOverview(dc, r);
            else PaintEditor(dc, r);
            return reinterpret_cast<LRESULT>(GetSysColorBrush(COLOR_WINDOW));
        }
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd_, message, wp, lp);
}
