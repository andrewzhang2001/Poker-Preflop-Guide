#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <random>

// Global variables
#define ID_RNG 1500
#define ID_RFI 1505
#define ID_VSRFI 1510
#define ID_VS3BET 1515
#define ID_VS4BET 1520

HBITMAP RFIChart = (HBITMAP)LoadImage(NULL, _T("rfi.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); 
HBITMAP vsRFIChart = (HBITMAP)LoadImage(NULL, _T("vsrfi.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
HBITMAP vs3betChart = (HBITMAP)LoadImage(NULL, _T("vs3bet.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
HBITMAP vs4betChart = (HBITMAP)LoadImage(NULL, _T("vs4bet.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

// The main window class name.
static TCHAR szWindowClass[] = _T("PreflopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Heads-Up Preflop Guide");

HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CleanupResources();
void generateNewRandomNumber();

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Preflop Guide"),
            NULL);

        return 1;
    }

    hInst = hInstance;

    HWND hWnd = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1920, 1080,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Preflop Guide"),
            NULL);

        return 1;
    }

    HWND RNGButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"Generate",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        5,         // x position 
        75,         // y position 
        100,        // Button width
        40,        // Button height
        hWnd,     // Parent window
        (HMENU) ID_RNG,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);

    HWND RFIButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"RFI",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        5,         // x position 
        175,         // y position 
        100,        // Button width
        40,        // Button height
        hWnd,     // Parent window
        (HMENU)ID_RFI,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);

    HWND VSRFIButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"vs RFI",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        5,         // x position 
        275,         // y position 
        100,        // Button width
        40,        // Button height
        hWnd,     // Parent window
        (HMENU)ID_VSRFI,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);

    HWND VS3betButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"vs 3-bet",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        5,         // x position 
        375,         // y position 
        100,        // Button width
        40,        // Button height
        hWnd,     // Parent window
        (HMENU)ID_VS3BET,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);

    HWND VS4betButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"vs 4-bet",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        5,         // x position 
        475,         // y position 
        100,        // Button width
        40,        // Button height
        hWnd,     // Parent window
        (HMENU)ID_VS4BET,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);


    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

TCHAR displayRNG[32];
std::random_device rd;
std::mt19937 mt(rd());
std::uniform_int_distribution<int> dist(1, 100);
int randomNumber = dist(mt);
HBITMAP currentChart = RFIChart;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_RNG:
            randomNumber = dist(mt);
            swprintf_s(displayRNG, _T("%d"), randomNumber);
            InvalidateRect(hWnd, NULL, true);
            break;
        case ID_RFI:
            currentChart = RFIChart;
            generateNewRandomNumber();
            InvalidateRect(hWnd, NULL, true);
            break;
        case ID_VSRFI:
            currentChart = vsRFIChart;
            generateNewRandomNumber();
            InvalidateRect(hWnd, NULL, true);
            break;
        case ID_VS3BET:
            currentChart = vs3betChart;
            generateNewRandomNumber();
            InvalidateRect(hWnd, NULL, true);
            break;
        case ID_VS4BET:
            currentChart = vs4betChart;
            generateNewRandomNumber();
            InvalidateRect(hWnd, NULL, true);
            break;

        }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc;
        hdc = BeginPaint(hWnd, &ps);
        HDC hdcMem = CreateCompatibleDC(hdc);
        BITMAP bm;
        HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, currentChart);
        GetObject(currentChart, sizeof(bm), &bm);
        BitBlt(hdc, 250, 75, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
        SelectObject(hdcMem, hbmOld);
        DeleteDC(hdcMem);
        TextOut(hdc,
            30, 30,
            displayRNG, _tcslen(displayRNG));

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        CleanupResources();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}
void generateNewRandomNumber() {
    randomNumber = dist(mt);
    swprintf_s(displayRNG, _T("%d"), randomNumber);
}

void CleanupResources() {
    DeleteObject(RFIChart);
    DeleteObject(vsRFIChart);
    DeleteObject(vs3betChart);
    DeleteObject(vs4betChart);
}