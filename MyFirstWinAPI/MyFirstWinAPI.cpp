#include "framework.h"
#include "MyFirstWinAPI.h"

#include <wingdi.h>
#include <cmath>

#define MAX_LOADSTRING 100

HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];            

// 함수 선언
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// 메인
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYFIRSTWINAPI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYFIRSTWINAPI));

    MSG msg;

    // 기본 메시지 루프입니다:
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


// 창 클래스 등록 (정보 등록)
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYFIRSTWINAPI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+2);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYFIRSTWINAPI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

// 창 (윈도우) 생성
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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

// hello world 출력
void PrintStr(HWND hWnd, HDC hdc)
{
    RECT rt4Text = { 100,100,500,300 };
    DrawText(hdc, L"Hello World", -1, &rt4Text, DT_CENTER | DT_WORDBREAK);
}

// 마우스 왼쪽 버튼을 눌렀을 때, +를 그려 봅시다.
void Marker(LONG x, LONG y, HWND hwnd)
{
    HDC hdc;

    hdc = GetDC(hwnd);
    MoveToEx(hdc, (int)x - 10, (int)y, (LPPOINT)NULL);
    LineTo(hdc, (int)x + 10, (int)y);
    MoveToEx(hdc, (int)x, (int)y - 10, (LPPOINT)NULL);
    LineTo(hdc, (int)x, (int)y + 10);

    ReleaseDC(hwnd, hdc);
}
void DrawMarker(HWND hWnd, LPARAM lParam)
{
    static POINT ptMouseDown[32];
    static int index;

    RECT rc;
    HRGN hrgn;
    POINTS ptTmp;

    if (index >= 32) return;

    GetClientRect(hWnd, &rc);
    hrgn = CreateRectRgn(rc.left, rc.top,
        rc.right, rc.bottom);

    ptTmp = MAKEPOINTS(lParam);
    ptMouseDown[index].x = (LONG)ptTmp.x;
    ptMouseDown[index].y = (LONG)ptTmp.y;

    // Test for a hit in the client rectangle.  

    if (PtInRegion(hrgn, ptMouseDown[index].x, ptMouseDown[index].y))
    {
        // If a hit occurs, record the mouse coords.  

        Marker(ptMouseDown[index].x, ptMouseDown[index].y, hWnd);
        index++;
    }

}

// 포물선을 그려봅시다!
void DrawParabola(HWND hWnd, HDC hdc)
{
    RECT rect;
    PAINTSTRUCT ps;
    HPEN hNewPen, hPrevPen;

    double a = -0.01;
    double b = 0;
    double c = 0;

    hNewPen = CreatePen(PS_DOT, 2, RGB(255, 255, 0)); //hNewPen 펜 생성 -> 옵션(도트), 굵기2, 색상 R:255 G:0 B:0
    hPrevPen = (HPEN)SelectObject(hdc, hNewPen); //hdc 영역에 새로운 펜을 적용후 이전에 있던 펜의 정보를 hPrevPen에 저장


    GetClientRect(hWnd, &rect);



    // Draw the parabola -> 2차 함수
    MoveToEx(hdc, rect.left, rect.bottom / 2, NULL);
    for (int x = rect.left; x <= rect.right; x++)
    {
        // 모니터 기준 좌표라서 a 부호 반대로 생각해야함
        // 원점이 (0,0)이 아니라서 식이 이상함
        double y = a * pow(x - rect.right / 2, 2) + b * (x - rect.right / 2) + c + rect.bottom / 2;
        LineTo(hdc, x, y);
    }

    //펜 정보 원상태로 바꾸기
    SelectObject(hdc, hPrevPen); //현재영역에 대한 펜을 원래 있던 펜으로 다시 적용
    DeleteObject(hNewPen); //사용이 끝난 펜 해제
}

// 격자 출력
void DrawGrid(HWND hWnd, HDC hdc)
{
    RECT rect;
    GetClientRect(hWnd, &rect);

    PAINTSTRUCT ps;
    HPEN hBoldPen, hGridPen, hPrevPen;
    hBoldPen = CreatePen(PS_DOT, 2, RGB(255, 255, 0));
    hGridPen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));

    hPrevPen = (HPEN)SelectObject(hdc, hBoldPen);


    MoveToEx(hdc, rect.left, rect.bottom / 2, NULL);
    LineTo(hdc, rect.right, rect.bottom / 2);
    MoveToEx(hdc, rect.right / 2, rect.top, NULL);
    LineTo(hdc, rect.right / 2, rect.bottom);

    hPrevPen = (HPEN)SelectObject(hdc, hGridPen);
    int i = 0;
    while (true)
    {
        if (rect.bottom / 2 - i < 0) break;
        MoveToEx(hdc, rect.left, rect.bottom / 2 + i, NULL);
        LineTo(hdc, rect.right, rect.bottom / 2 + i);
        MoveToEx(hdc, rect.left, rect.bottom / 2 - i, NULL);
        LineTo(hdc, rect.right, rect.bottom / 2 - i);
        i += 100;
    }
    i = 0;
    while (true)
    {
        if (rect.right / 2 - i < 0) break;
        MoveToEx(hdc, rect.right / 2 + i, rect.top, NULL);
        LineTo(hdc, rect.right / 2 + i, rect.bottom);
        MoveToEx(hdc, rect.right / 2 - i, rect.top, NULL);
        LineTo(hdc, rect.right / 2 - i, rect.bottom);
        i += 100;
    }
    
    //펜 정보 원상태로 바꾸기
    SelectObject(hdc, hPrevPen);
    DeleteObject(hBoldPen);
    DeleteObject(hGridPen);
}

// 칸 선택
void GridMarker(LONG x, LONG y, HWND hwnd)
{
    HDC hdc;
    hdc = GetDC(hwnd);

    RECT winRect;
    GetClientRect(hwnd, &winRect);
    int padding;
    padding = 100 - (winRect.right/2) % 100;

    COLORREF color = GetPixel(hdc, x, y);
    int r = GetRValue(color);
    int g = GetGValue(color);
    int b = GetBValue(color);

    HPEN hNewPen, hPrevPen;
    if (r == 255 && g == 255 && b == 255)
    {
        hNewPen = CreatePen(PS_DOT, 2, RGB(100, 100, 100));
    }
    else hNewPen = CreatePen(PS_DOT, 2, RGB(255, 255, 255));

    hPrevPen = (HPEN)SelectObject(hdc, hNewPen);

    RECT smallRect;
    smallRect.left = x - (x % 100) - padding;
    smallRect.right = smallRect.left + 100 ;
    smallRect.top = y - (y % 100) - padding;
    smallRect.bottom = smallRect.top + 100 ;

    for (int i = smallRect.left; i <= smallRect.right; i++)
    {
        MoveToEx(hdc, i, smallRect.top, (LPPOINT)NULL);
        LineTo(hdc, i, smallRect.bottom);
    }
  
    SelectObject(hdc, hPrevPen); 
    DeleteObject(hNewPen); 

    DrawGrid(hwnd, hdc);


    ReleaseDC(hwnd, hdc);
}
void DrawGridMarker(HWND hWnd, LPARAM lParam)
{
    static POINT ptMouseDown[32];
    static int index;

    RECT rc;
    HRGN hrgn;
    POINTS ptTmp;

    if (index >= 32) return;

    GetClientRect(hWnd, &rc);
    hrgn = CreateRectRgn(rc.left, rc.top,
        rc.right, rc.bottom);

    ptTmp = MAKEPOINTS(lParam);
    ptMouseDown[index].x = (LONG)ptTmp.x;
    ptMouseDown[index].y = (LONG)ptTmp.y;

 

    if (PtInRegion(hrgn, ptMouseDown[index].x, ptMouseDown[index].y))
    {

        GridMarker(ptMouseDown[index].x, ptMouseDown[index].y, hWnd);
        index++;
    }

}

// 메시지 처리
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
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
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
           
            PrintStr(hWnd, hdc);
            DrawGrid(hWnd, hdc);
            //DrawParabola(hWnd, hdc);
            

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_LBUTTONDOWN:

        //DrawMarker(hWnd, lParam);
        DrawGridMarker(hWnd, lParam);

        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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
