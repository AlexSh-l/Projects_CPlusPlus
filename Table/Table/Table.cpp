// Table.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Table.h"
#include "windowsx.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

LOGFONT font;
struct Cell {
    int top;
    int left;
    int right;
    int bottom;
    RECT content;
};
Cell Grid[3][3];
char const* textContent[3][3] = { { "Text", "Sample text", "String text" }, { "string", "Ipsum", "dolor"}, { "sit", "amet", "trtrtrtr" } };

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TABLE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TABLE));

    MSG msg;

    font.lfCharSet = DEFAULT_CHARSET;
    font.lfPitchAndFamily = FIXED_PITCH;
    font.lfHeight = 50;
    font.lfWidth = 30;
    font.lfWeight = 10;
    font.lfEscapement = 0;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TABLE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TABLE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {    
    case WM_PAINT:
    {
        RECT clientRect;
        GetClientRect(hWnd, &clientRect);
        int clientHeight = clientRect.bottom - clientRect.top;
        int clientWidth = clientRect.right - clientRect.left;
        HDC currDC = GetDC(hWnd);
        HDC virtDC = CreateCompatibleDC(currDC);
        HBITMAP bitmap = CreateCompatibleBitmap(currDC, clientWidth, clientHeight);
        SelectBitmap(virtDC, bitmap);
        HPEN newPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HPEN pen = SelectPen(virtDC, newPen);

        LOGBRUSH brush;
        brush.lbStyle = BS_SOLID;
        brush.lbColor = RGB(255, 255, 255);
        HBRUSH hBrush;
        hBrush = CreateBrushIndirect(&brush);
        FillRect(virtDC, &clientRect, hBrush);
        DeleteObject(hBrush);

        HFONT hFont = CreateFontIndirect(&font);
        SelectObject(virtDC, hFont);
        SetTextColor(virtDC, RGB(0, 0, 0));
        SetBkColor(virtDC, RGB(255, 255, 255));

        Rectangle(virtDC, 0, 0, clientWidth, clientHeight);
        int cellWidth = clientWidth / 3;
        int cellHeight = clientHeight / 3;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                Grid[i][j].left = j * cellWidth;
                Grid[i][j].right = (j + 1) * cellWidth;
                Grid[i][j].top = i * cellHeight;
                Grid[i][j].bottom = (i + 1) * cellHeight;
                Grid[i][j].content.left = Grid[i][j].left + cellWidth / 100;
                Grid[i][j].content.top = Grid[i][j].top + cellHeight / 100;
                Grid[i][j].content.right = Grid[i][j].right - cellWidth / 100;
                Grid[i][j].content.bottom = Grid[i][j].bottom - cellHeight / 100;
            }
        }
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                Rectangle(virtDC, Grid[i][j].left, Grid[i][j].top, Grid[i][j].right, Grid[i][j].bottom);
                DrawTextA(virtDC, textContent[i][j], -1, (LPRECT)&Grid[i][j].content, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
            }
        }
        SetStretchBltMode(virtDC, COLORONCOLOR);
        BitBlt(currDC, 0, 0, clientWidth, clientHeight, virtDC, 0, 0, SRCCOPY);
        DeleteObject(hFont);
        DeleteObject(bitmap);
        DeletePen(SelectPen(virtDC, pen));
        DeleteDC(virtDC);
        ReleaseDC(hWnd, currDC);
    }
        break;
    case WM_SIZE:
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
