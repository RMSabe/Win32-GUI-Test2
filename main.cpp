#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

#define __WNDCLASS_NAME "MY_WNDCLASS"

#define __WINDOW_NAME "Win32 GUI Test"
#define __WINDOW_X_POS 50
#define __WINDOW_Y_POS 50
#define __WINDOW_WIDTH 720
#define __WINDOW_HEIGHT 360

#define TEXT_X_POS 60
#define TEXT_Y_POS 50
#define TEXT_WIDTH 600
#define TEXT_HEIGHT 35
#define TEXT_FONT_CHARSET DEFAULT_CHARSET
#define TEXT_FONT_WIDTH 15
#define TEXT_FONT_HEIGHT TEXT_HEIGHT
#define TEXT_FONT_WEIGHT FW_SEMIBOLD

#define RUNTIME_STATUS_FIRST 1
#define RUNTIME_STATUS_FINAL 2

#define BUTTON_1 1
#define BUTTON_2 2

int runtimeStatus = -1;
LPCWSTR WNDCLASS_NAME = TEXT(__WNDCLASS_NAME);
LPCWSTR WINDOW_NAME = TEXT(__WINDOW_NAME);
HINSTANCE hRuntimeInstance;

HWND hWindow;
HWND hText;
HWND hButton1;
HWND hButton2;

void paintFirstScreen(void)
{
  SendMessage(hText, WM_SETTEXT, NULL, (LPARAM) TEXT("This is First Screen"));
  
  SendMessage(hButton1, WM_SETTEXT, NULL, (LPARAM) TEXT("Go To Second Screen"));
  SetWindowPos(hButton1, NULL, 150, 200, 200, 20, 0);
  
  ShowWindow(hButton2, SW_SHOW);
  return;
}

void paintSecondScreen(void)
{
  SendMessage(hText, WM_SETTEXT, NULL, (LPARAM) TEXT("This is Second Screen"));
  
  SendMessage(hButton1, WM_SETTEXT, NULL, (LPARAM) TEXT("Go To First Screen"));
  SetWindowPos(hButton1, NULL, 260, 200, 200, 20, 0);
  
  ShowWindow(hButton2, SW_HIDE);
  return;
}

void popMessageBox(void)
{
  MessageBox(NULL, TEXT("Here is your message box!\n:D"), TEXT("Just A Regular Message Box"), MB_OK);
  return;
}

void createElements(void)
{
  DWORD elementStyle = 0;
  LOGFONT logFont;
  HFONT hFont;
  
  elementStyle = (WS_CHILD | WS_VISIBLE | SS_CENTER);
  hText = CreateWindow(TEXT("STATIC"), NULL, elementStyle, TEXT_X_POS, TEXT_Y_POS, TEXT_WIDTH, TEXT_HEIGHT, hWindow, NULL, hRuntimeInstance, NULL);
  ZeroMemory(&logFont, sizeof(LOGFONT));
  logFont.lfCharSet = TEXT_FONT_CHARSET;
  logFont.lfWidth = TEXT_FONT_WIDTH;
  logFont.lfHeight = TEXT_FONT_HEIGHT;
  logFont.lfWeight = TEXT_FONT_WEIGHT;
  hFont = CreateFontIndirect(&logFont);
  SendMessage(hText, WM_SETFONT, (WPARAM) hFont, (LPARAM) TRUE);
  
  elementStyle = (WS_CHILD | WS_VISIBLE);
  hButton1 = CreateWindow(TEXT("BUTTON"), NULL, elementStyle, 0, 0, 200, 20, hWindow, (HMENU) BUTTON_1, hRuntimeInstance, NULL);
  hButton2 = CreateWindow(TEXT("BUTTON"), TEXT("Pop Message Box"), elementStyle, 370, 200, 200, 20, hWindow, (HMENU) BUTTON_2, hRuntimeInstance, NULL);
  
  ShowWindow(hText, SW_SHOW);
  ShowWindow(hButton1, SW_SHOW);
  ShowWindow(hButton2, SW_HIDE);
  return;
}

void destroyElements(void)
{
  DestroyWindow(hText);
  DestroyWindow(hButton1);
  DestroyWindow(hButton2);
  return;
}

void createMainWindow(void)
{
  DWORD windowStyle = (WS_CAPTION | WS_VISIBLE | WS_SYSMENU | WS_OVERLAPPED);
  RECT rectangle = {
    .left = __WINDOW_X_POS,
    .top = __WINDOW_Y_POS,
    .right = __WINDOW_X_POS + __WINDOW_WIDTH,
    .bottom = __WINDOW_Y_POS + __WINDOW_HEIGHT
  };
  AdjustWindowRect(&rectangle, windowStyle, FALSE);
  
  int xPos = rectangle.left;
  int yPos = rectangle.top;
  int width = rectangle.right - rectangle.left;
  int height = rectangle.bottom - rectangle.top;
  
  hWindow = CreateWindow(WNDCLASS_NAME, WINDOW_NAME, windowStyle, xPos, yPos, width, height, NULL, NULL, hRuntimeInstance, NULL);
  return;
}

LRESULT CALLBACK windowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  WORD arg = LOWORD(wParam);
  switch(msg)
  {
    case WM_CLOSE:
      PostQuitMessage(0);
      return 0;
      
    case WM_COMMAND:
      switch(runtimeStatus)
      {
        case RUNTIME_STATUS_FIRST:
          if(arg == BUTTON_1)
          {
            runtimeStatus = RUNTIME_STATUS_FINAL;
            paintSecondScreen();
            return 0;
          }
          else if(arg == BUTTON_2)
          {
            popMessageBox();
            return 0;
          }
          break;
          
        case RUNTIME_STATUS_FINAL:
          if(arg == BUTTON_1)
          {
            runtimeStatus = RUNTIME_STATUS_FIRST;
            paintFirstScreen();
            return 0;
          }
          break;
      }
      break;
  }
  
  return DefWindowProc(hWnd, msg, wParam, lParam);
}

void registerWndClass(void)
{
  WNDCLASS myWndClass;
  ZeroMemory(&myWndClass, sizeof(WNDCLASS));
  myWndClass.hInstance = hRuntimeInstance;
  myWndClass.lpszClassName = WNDCLASS_NAME;
  myWndClass.lpfnWndProc = windowProcedure;
  myWndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  myWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  myWndClass.hbrBackground = (HBRUSH) GetStockObject(LTGRAY_BRUSH);
  RegisterClass(&myWndClass);
  return;
}

void initialize(void)
{
  runtimeStatus = RUNTIME_STATUS_FIRST;
  registerWndClass();
  createMainWindow();
  createElements();
  paintFirstScreen();
  ShowWindow(hWindow, SW_SHOW);
  return;
}

void terminate(void)
{
  destroyElements();
  DestroyWindow(hWindow);
  UnregisterClass(WNDCLASS_NAME, hRuntimeInstance);
  return;
}

WINBOOL runtimeContinue(void)
{
  MSG msg;
  while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
  {
    if(msg.message == WM_QUIT) return FALSE;
    
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  
  return TRUE;
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
  hRuntimeInstance = hInstance;
  initialize();
  
  while(runtimeContinue()) Sleep(10);
  
  terminate();
  return 0;
}
