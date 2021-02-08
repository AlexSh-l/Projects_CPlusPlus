#include "framework.h"
#include "2048.h"
#include "windowsx.h"
#include "sstream"
#include "time.h"
#include "iostream"
#include "fstream"
#include "vector"
#include "string"

#define MAX_LOADSTRING 100

HINSTANCE hInst;                               
WCHAR szTitle[MAX_LOADSTRING];                 
WCHAR szWindowClass[MAX_LOADSTRING];            
int base[4][4] = { 0 };
bool victoryCheck = false;
bool flag = false;
bool defeatCheckDown = false;
bool defeatCheckUp = false;
bool defeatCheckLeft = false;
bool defeatCheckRight = false;
int space = 0;
LOGFONT font;
struct Cell {
    int top;
    int left;
    int right;
    int bottom;
    RECT content;
};
Cell Grid[4][4];
bool victory = true;
PAINTSTRUCT paintstruct;
char const* victoryBoxName = "Victory!";
char const* victoryBoxCaption = "You have won the game!!! Feel free to continue playing for the highest score";
char const* defeatBoxName = "You lost";
char const* defeatBoxCaption = "You have no more options";

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK ScoreBoard(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void TableGeneration(HWND hWnd);
void ScoreBoardPrint(HWND hDlg);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    font.lfCharSet = DEFAULT_CHARSET;
    font.lfPitchAndFamily = FIXED_PITCH;
    font.lfHeight = 50;
    font.lfWidth = 30;
    font.lfWeight = 10;
    font.lfEscapement = 0;
    srand(time(0));
    int i, j, i2, j2;
    do
    {
        i = 0 + rand() % 3;
        j = 0 + rand() % 3;
        i2 = 0 + rand() % 3;
        j2 = 0 + rand() % 3;
    } while (i == i2 && j == j2);
    base[i][j] = 2;
    base[i2][j2] = 2;

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY2048, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY2048));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY2048);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON2));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;          
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
    {
        TableGeneration(hWnd);
    }
        break;
    case WM_SIZE:
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_GETMINMAXINFO: {
        LPMINMAXINFO info = (LPMINMAXINFO)lParam;
        info->ptMinTrackSize.x = 520;
        info->ptMinTrackSize.y = 540;
    }
        break;
    case WM_KEYDOWN:
        switch (wParam) {
        case VK_DOWN:
            for (int j = 3; j >= 0; j--) {
                for (int i = 3; i >= 0; i--) {
                    if (base[i][j] != 0 && i < 3) {
                        int counter = 0;
                        for (int p = i; p <= 3; p++) {
                            if (base[p][j] == 0) {
                                counter++;
                            }
                        }
                        if (base[i + counter][j] == 0) {
                            base[i + counter][j] = base[i][j];
                            base[i][j] = 0;
                        }
                    }
                }
            }
            defeatCheckDown = true;
            for (int j = 3; j >= 0; j--) {
                int temp = base[3][j];
                for (int i = 2; i >= 0; i--) {
                    if (temp == base[i][j] && temp != 0) {
                        base[i + 1][j] = temp * 2;
                        defeatCheckDown = false;
                        defeatCheckUp = false;
                        defeatCheckLeft = false;
                        defeatCheckRight = false;
                        if (base[i + 1][j] == 2048)
                            victoryCheck = true;
                        for (int p = i; p >= 1; p--) {
                            base[p][j] = base[p - 1][j];
                            if (p == 1)
                                base[0][j] = 0;
                        }                        
                    }
                    temp = base[i][j];
                }                
            }
            flag = false;
            for (int i = 0; i <= 3; i++) {
                for (int j = 0; j <= 3; j++) {
                    if (base[i][j] == 0)
                        flag = true;
                }
            }
            while(flag){
                int i = 0 + rand() % 4;
                int j = 0 + rand() % 4;
                if (base[i][j] == 0 && i < 4 && j < 4) {
                    if ((1 + rand() % 10) == 1)
                        base[i][j] = 4;
                    else
                        base[i][j] = 2;
                    flag = false;
                }
            }
            InvalidateRect(hWnd, NULL, FALSE);
            if (victoryCheck && victory) {
                MessageBoxA(hWnd, victoryBoxCaption, victoryBoxName, MB_OK);
                victory = false;
            }
            else if (defeatCheckDown && defeatCheckUp && defeatCheckLeft && defeatCheckRight) {
                MessageBoxA(hWnd, defeatBoxCaption, defeatBoxName, MB_OK);
            }
            break;
        case VK_UP:
            for (int j = 3; j >= 0; j--) {
                for (int i = 0; i <= 3; i++) {
                    if (base[i][j] != 0 && i > 0) {
                        int counter = 0;
                        for (int p = i; p >= 0; p--) {
                            if (base[p][j] == 0) {
                                counter++;
                            }
                        }
                        if (base[i - counter][j] == 0) {
                            base[i - counter][j] = base[i][j];
                            base[i][j] = 0;
                        }
                    }
                }
            }
            defeatCheckUp = true;
            for (int j = 3; j >= 0; j--) {
                int temp = base[0][j];
                for (int i = 1; i <= 3; i++) {
                    if (temp == base[i][j] && temp != 0) {
                        base[i - 1][j] = temp * 2;
                        defeatCheckDown = false;
                        defeatCheckUp = false;
                        defeatCheckLeft = false;
                        defeatCheckRight = false;
                        if (base[i - 1][j] == 2048)
                            victoryCheck = true;
                        for (int p = i; p <= 2; p++) {
                            base[p][j] = base[p + 1][j];
                            if (p == 2)
                                base[3][j] = 0;
                        }
                    }
                    temp = base[i][j];
                }
            }
            space = 0;
            flag = false;
            for (int i = 0; i <= 3; i++) {
                for (int j = 0; j <= 3; j++) {
                    if (base[i][j] == 0)
                        flag = true;
                }
            }
            while (flag) {
                int i = 0 + rand() % 4;
                int j = 0 + rand() % 4;
                if (base[i][j] == 0 && i < 4 && j < 4) {
                    if ((1 + rand() % 10) == 1)
                        base[i][j] = 4;
                    else
                        base[i][j] = 2;
                    flag = false;
                }
            }
            InvalidateRect(hWnd, NULL, FALSE);
            if (victoryCheck && victory) {
                MessageBoxA(hWnd, victoryBoxCaption, victoryBoxName, MB_OK);
                victory = false;
            }
            else if (defeatCheckDown && defeatCheckUp && defeatCheckLeft && defeatCheckRight) {
                MessageBoxA(hWnd, defeatBoxCaption, defeatBoxName, MB_OK);
            }
            break;
        case VK_LEFT:
            for (int i = 0; i <= 3; i++) {            
                for (int j = 0; j <= 3; j++) {
                    if (base[i][j] != 0 && j > 0) {
                        int counter = 0;
                        for (int p = j; p >= 0; p--) {
                            if (base[i][p] == 0) {
                                counter++;
                            }
                        }
                        if (base[i][j - counter] == 0) {
                            base[i][j - counter] = base[i][j];
                            base[i][j] = 0;
                        }
                    }
                }
            }
            defeatCheckLeft = true;
            for (int i = 3; i >= 0; i--) {
                int temp = base[i][0];
                for (int j = 1; j <= 3; j++) {
                    if (temp == base[i][j] && temp != 0) {
                        base[i][j - 1] = temp * 2;
                        defeatCheckDown = false;
                        defeatCheckUp = false;
                        defeatCheckLeft = false;
                        defeatCheckRight = false;
                        if (base[i][j - 1] == 2048)
                            victoryCheck = true;
                        for (int p = j; p <= 2; p++) {
                            base[i][p] = base[i][p + 1];
                            if (p == 2)
                                base[i][3] = 0;
                        }
                    }
                    temp = base[i][j];
                }
            }            
            space = 0;
            flag = false;
            for (int i = 0; i <= 3; i++) {
                for (int j = 0; j <= 3; j++) {
                    if (base[i][j] == 0)
                        flag = true;
                }
            }
            while (flag) {
                int i = 0 + rand() % 4;
                int j = 0 + rand() % 4;
                if (base[i][j] == 0 && i < 4 && j < 4) {
                    if ((1 + rand() % 10) == 1)
                        base[i][j] = 4;
                    else
                        base[i][j] = 2;
                    flag = false;
                }
            }
            InvalidateRect(hWnd, NULL, FALSE);
            if (victoryCheck && victory) {
                MessageBoxA(hWnd, victoryBoxCaption, victoryBoxName, MB_OK);
                victory = false;
            }
            else if (defeatCheckDown && defeatCheckUp && defeatCheckLeft && defeatCheckRight) {
                MessageBoxA(hWnd, defeatBoxCaption, defeatBoxName, MB_OK);
            }
            break;
        case VK_RIGHT:
            for (int i = 0; i <= 3; i++) {
                for (int j = 3; j >= 0; j--) {
                    if (base[i][j] != 0 && j < 3) {
                        int counter = 0;
                        for (int p = j; p <= 3; p++) {
                            if (base[i][p] == 0) {
                                counter++;
                            }
                        }
                        if (base[i][j + counter] == 0) {
                            base[i][j + counter] = base[i][j];
                            base[i][j] = 0;
                        }
                    }
                }
            }
            defeatCheckRight = true;
            for (int i = 0; i <= 3; i++) {
                int temp = base[i][3];
                for (int j = 2; j >= 0; j--) {
                    if (temp == base[i][j] && temp != 0) {
                        base[i][j + 1] = temp * 2;
                        defeatCheckDown = false;
                        defeatCheckUp = false;
                        defeatCheckLeft = false;
                        defeatCheckRight = false;
                        if (base[i][j + 1] == 2048)
                            victoryCheck = true;
                        for (int p = j; p >= 1; p--) {
                            base[i][p] = base[i][p - 1];
                            if (p == 1)
                                base[i][0] = 0;
                        }
                    }
                    temp = base[i][j];
                }
            }
            space = 0;
            flag = false;
            for (int i = 0; i <= 3; i++) {
                for (int j = 0; j <= 3; j++) {
                    if (base[i][j] == 0)
                        flag = true;
                }
            }
            while (flag) {
                int i = 0 + rand() % 4;
                int j = 0 + rand() % 4;                
                if (base[i][j] == 0 && i < 4 && j < 4 && flag) {
                    if ((1 + rand() % 10) == 1)
                        base[i][j] = 4;
                    else
                        base[i][j] = 2;
                    flag = false;
                }
            }
            InvalidateRect(hWnd, NULL, FALSE);
            if (victoryCheck && victory) {
                MessageBoxA(hWnd, victoryBoxCaption, victoryBoxName, MB_OK);
                victory = false;
            }
            else if (defeatCheckDown && defeatCheckUp && defeatCheckLeft && defeatCheckRight) {
                MessageBoxA(hWnd, defeatBoxCaption, defeatBoxName, MB_OK);
            }
            break;
        case VK_SPACE:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, ScoreBoard);        
            break;
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

void TableGeneration(HWND hWnd) {
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    int clientHeight = clientRect.bottom - clientRect.top;
    int clientWidth = clientRect.right - clientRect.left;
    HDC currDC = BeginPaint(hWnd, &paintstruct);
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
    int cellWidth = clientWidth / 4;
    int cellHeight = clientHeight / 4;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
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
    char temp;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            Rectangle(virtDC, Grid[i][j].left, Grid[i][j].top, Grid[i][j].right, Grid[i][j].bottom);
            std::stringstream s;
            s << std::scientific << base[i][j];
            temp = base[i][j];
            if (base[i][j] != 0)
                DrawTextA(virtDC, s.str().c_str(), -1, (LPRECT)&Grid[i][j].content, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
        }
    }
    SetStretchBltMode(virtDC, COLORONCOLOR);
    BitBlt(currDC, 0, 0, clientWidth, clientHeight, virtDC, 0, 0, SRCCOPY);
    DeleteObject(hFont);
    DeleteObject(bitmap);
    DeletePen(SelectPen(virtDC, pen));
    DeleteDC(virtDC);
    EndPaint(hWnd, &paintstruct);
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void ScoreBoardPrint(HWND hDlg) {
    std::vector<int> intVector;
    std::vector<std::string*> strVector;
    std::ifstream readFile;
    readFile.open("Scoreboard.txt");
    std::string* s = new std::string();
    readFile >> *s;
    int i = 0;
    while (!readFile.eof()) {

        strVector.push_back(s);
        SetDlgItemTextA(hDlg, IDC_STATIC1 + i, (LPCSTR)s->c_str());
        i++;
        s = new std::string();
        readFile >> *s;
        SetDlgItemTextA(hDlg, IDC_STATIC1 + i, (LPCSTR)s->c_str());
        i++;
        intVector.push_back(std::stol(*s));
        s = new std::string();
        readFile >> *s;
    }
    readFile.close();
    delete s;
}

INT_PTR CALLBACK ScoreBoard(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG: 
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL || LOWORD(wParam) == IDCLOSE)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDC_BUTTON1) {
            LPSTR str = new char[10];
            GetDlgItemTextA(hDlg, IDC_EDIT1, str, 10);
            std::vector<int> intVector;
            std::vector<std::string*> strVector;
            std::ifstream readFile;
            readFile.open("Scoreboard.txt");
            std::string* s = new std::string();
            readFile >> *s;
            int i = 0;
            while (!readFile.eof()) {
                strVector.push_back(s);
                s = new std::string();
                readFile >> *s;
                intVector.push_back(std::stol(*s));
                s = new std::string();
                readFile >> *s;
            }
            readFile.close();
            int sum = 0;
            for (int k = 0; k <= 3; k++) {
                for (int p = 0; p <= 3; p++) {
                    sum += base[k][p];
                }
            }
            std::ofstream file;
            file.open("Scoreboard.txt");
            for (int k = 0; k < intVector.size(); k++) {
                if (intVector[k] < sum) {                   
                    file << str << "\n";
                    file << sum << "\n";
                    for (int p = k; p <= 3; p++) {
                        file << *strVector[p] << "\n";
                        if (p == 3)
                            file << intVector[p];
                        else
                            file << intVector[p] << "\n";
                    }
                    break;
                }
                else {
                    file << *strVector[k] << "\n";
                    if (k == 4)
                        file << intVector[k];
                    else
                        file << intVector[k] << "\n";
                }
            }

            file.close();
            delete str;
            readFile.open("Scoreboard.txt");
            s = new std::string();
            readFile >> *s;
            i = 0;
            while (!readFile.eof()) {
                SetDlgItemTextA(hDlg, IDC_STATIC1 + i, (LPCSTR)s->c_str());
                i++;
                s = new std::string();
                readFile >> *s;
                SetDlgItemTextA(hDlg, IDC_STATIC1 + i, (LPCSTR)s->c_str());
                i++;
                s = new std::string();
                readFile >> *s;
            }
            readFile.close();
            delete s;
        }
        break;  
    case WM_PAINT:
        ScoreBoardPrint(hDlg);
        break;
    }
    return (INT_PTR)FALSE;
}