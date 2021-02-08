// Sprite.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Sprite.h"
#include "windows.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

int xCoord = 0;
int yCoord = 0;
const int spriteSize = 10;
const int space = 20;
const int knockback = 30;
HBITMAP hBitmap;
bool clickChecker = false;
const CHAR* const image = "sprite.bmp";
int wheelSpace;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.
    HWND hWnd;
    MSG msg;
    
    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SPRITE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPRITE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW-1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPRITE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
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
    case WM_CREATE:
        hBitmap = (HBITMAP)LoadImageA(NULL, (LPCSTR)image, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        break;
    case WM_PAINT:
        {
        BITMAP bitmap;
        GetObject(hBitmap, sizeof(bitmap), &bitmap);
        HDC currDC = GetDC(hWnd);
        HDC virtDC = CreateCompatibleDC(currDC);
        HBITMAP oldBmp = (HBITMAP)SelectObject(virtDC, hBitmap);
        RECT clientRect;
        GetClientRect(hWnd, &clientRect);
        int clientWidth = clientRect.right - clientRect.left;
        int clientHeight = clientRect.bottom - clientRect.top;
        if (xCoord < 0) {
            xCoord = space + knockback;
        }
        else if (xCoord + clientWidth / spriteSize > clientWidth) {
            xCoord = clientWidth - space - knockback - clientWidth / spriteSize;
        }
        if (yCoord < 0) {
            yCoord = space + knockback;
        }
        else if (yCoord + clientHeight / spriteSize > clientHeight) {
            yCoord = clientHeight - space - knockback - clientHeight / spriteSize;
        }
        SetStretchBltMode(currDC, HALFTONE);
        StretchBlt(currDC, xCoord, yCoord, clientWidth / spriteSize, clientHeight / spriteSize, virtDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCAND);
        SelectObject(virtDC, oldBmp);
        DeleteDC(virtDC);
        ReleaseDC(hWnd, currDC);
        }
        break;
    case WM_KEYDOWN:
        switch (wParam) {
        case VK_DOWN:
            yCoord += space;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_UP:
            yCoord -= space;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_LEFT:
            xCoord -= space;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_RIGHT:
            xCoord += space;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        break;
    case WM_MOUSEWHEEL:
        if (wParam & MK_SHIFT) {
            wheelSpace += GET_WHEEL_DELTA_WPARAM(wParam);
            for (; wheelSpace < 0; wheelSpace += WHEEL_DELTA) {
                xCoord -= space;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            for (; wheelSpace > 0; wheelSpace -= WHEEL_DELTA) {
                xCoord += space;
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
        else {
            wheelSpace += GET_WHEEL_DELTA_WPARAM(wParam);
            for (; wheelSpace < 0; wheelSpace += WHEEL_DELTA) {
                yCoord += space;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            for (; wheelSpace > 0; wheelSpace -= WHEEL_DELTA) {
                yCoord -= space;
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
        break;
    case WM_LBUTTONDOWN:
        xCoord = LOWORD(lParam);
        yCoord = HIWORD(lParam);
        clickChecker = true;
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_LBUTTONUP:
        clickChecker = false;
        break;
    case WM_MOUSEMOVE:
        if (clickChecker) {
            xCoord = LOWORD(lParam);
            yCoord = HIWORD(lParam);
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;    
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}