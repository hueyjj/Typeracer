#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <commctrl.h>
using namespace std;
#include "../Header/Main.h"
//#include <tchar.h>
//#include "../Header/TextProcessor.h"
#include <string>

HWND hwnd, hwnd_typeBox;
SetWindowSubclassType SetWindowSubclassFunction;
DefSubclassProcType DefSubclassProcFunction;
const char g_szClassName[] = "Typeracer";
int SCREEN_WIDTH = 0, SCREEN_LENGTH = 0;
RECT MAIN_RECT, EDIT_RECT;

// Main()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
  // FreeConsole();
  loadLibrary();

  WNDCLASSEX wc;
  // HWND hwnd;
  MSG Msg;

  // Registering the Window Class
  wc.cbSize        = sizeof(WNDCLASSEX);
  wc.style         = 0;
  wc.lpfnWndProc   = WndProc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hInstance     = hInstance;
  wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
  wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
  wc.lpszMenuName  = NULL;
  wc.lpszClassName = g_szClassName;
  wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

  if(!RegisterClassEx(&wc))
    {
      MessageBox(NULL, "Window Registration Failed!", "Error!",
                 MB_ICONEXCLAMATION | MB_OK);
      return 0;
    }

  initVar(); // initialize variable

  createMainWindow(hInstance);
  createEditBox(hwnd);

  // Display window
  ShowWindow(hwnd, nCmdShow);
  //  UpdateWindow(hwnd); // for WM_PAINT

  // Message Loop
  while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
      TranslateMessage(&Msg);
      DispatchMessage(&Msg);
    }
  return Msg.wParam;
}

// The Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
    {
    case WM_CREATE:
      break;
    case WM_SIZE:
      //resizeAll();
      rePos();
      cout << "WM_SIZE message" << endl;
      break;
    case WM_CLOSE:
      cout << GetLastErrorAsString() << endl;
      DestroyWindow(hwnd);
      break;
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    case WM_KEYDOWN:
      switch (wParam){
        // case VK_DELETE:
        //   cout << "Del key pressed" << endl;
        //   break;
      }
      cout << "(Main window) key down" << endl;
      break;
    case WM_LBUTTONDOWN:
      SetFocus(hwnd); // focus on window
      cout << "Main window focus" << endl;
      break;
    default:
      return DefWindowProc(hwnd, msg, wParam, lParam);
    }
  return 0;
}

LRESULT CALLBACK EditProc(HWND hWnd_edit, UINT uMsg, WPARAM wParam,
                          LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
  switch (uMsg)
    {
    case WM_CREATE:
      break;
    case WM_KEYDOWN:
      switch (wParam)
        {
        case VK_RETURN:
          cout << "(Edit Window): Return key pressed" << endl;
          break;
        }
      break;
    case WM_LBUTTONDOWN:
      SetFocus(hWnd_edit); // focus on edit control
      cout << "Edit window focus" << endl;
    }
  return DefSubclassProcFunction(hWnd_edit, uMsg, wParam, lParam);
}

void initVar(){
  if (SCREEN_LENGTH <= 0) SCREEN_LENGTH = GetSystemMetrics(SM_CXSCREEN);
  cout << "SCREEN_LENGTH: " << SCREEN_LENGTH << endl;
  if (SCREEN_WIDTH <= 0) SCREEN_WIDTH = GetSystemMetrics(SM_CYSCREEN);
  cout << "SCREEN_WIDTH: " << SCREEN_WIDTH << endl;
}

void createMainWindow(HINSTANCE hInstance){
  // Create Window
  int WIN_LENGTH = SCREEN_LENGTH * 0.66;
  int WIN_WIDTH = SCREEN_WIDTH * 0.66;

  hwnd = CreateWindowEx( WS_EX_CLIENTEDGE,
                         g_szClassName,
                         "Typeracer",
                         WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
                         0, 0, WIN_LENGTH, WIN_WIDTH,
                         NULL, NULL, hInstance, NULL);


  GetWindowRect(hwnd, &MAIN_RECT); // init MAIN_RECT

  // Center window
  int xPos = (SCREEN_LENGTH - MAIN_RECT.right) / 2;
  int yPos = (SCREEN_WIDTH - MAIN_RECT.bottom) / 2;
  SetWindowPos(hwnd, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

  if (hwnd == NULL){
    MessageBox(NULL, "Main window failed to load!", "Error!",
               MB_ICONEXCLAMATION | MB_OK);
    return;
  }
  cout << "Main window created" << endl;
}

void createEditBox(HWND hwnd){
  // User input text box
  //WS_EX_PALETTEWINDOW
  int EDIT_LENGTH = (MAIN_RECT.right - MAIN_RECT.left) * 0.50;
  int EDIT_WIDTH = 25;
  cout << "EDIT_LENGTH: " << EDIT_LENGTH << endl;
  cout << "EDIT_WIDTH: " << EDIT_WIDTH << endl;

  hwnd_typeBox = CreateWindowEx(WS_EX_CLIENTEDGE,
                                "EDIT",
                                NULL,
                                WS_CHILD | WS_VISIBLE | WS_BORDER| ES_LEFT,
                                0, 0, EDIT_LENGTH, EDIT_WIDTH,
                                hwnd, NULL , NULL, NULL);
  GetWindowRect(hwnd_typeBox, &EDIT_RECT);

  if(hwnd_typeBox == NULL){
    MessageBox(NULL, "WindowSubclass Creation Failed!", "Error!",
               MB_ICONEXCLAMATION | MB_OK);
    return;
  }

  //ShowWindow(hwnd, SW_SHOW);
  SetWindowSubclassFunction(hwnd_typeBox, EditProc, 0, 0) ?
    cout << "Editbox subclassed successfully" << endl :
    cout << "Editbox subclassed failed" << endl;

  //UpdateWindow(hwnd);
  //SetFocus(hwnd_typeBox);
  //  ShowWindow(hwnd, SW_SHOW);
  //  UpdateWindow(hwnd);
}

//TODO: Free library and its functions
void loadLibrary(){
  HINSTANCE dll = LoadLibrary("C:\\Windows\\System32\\ComCtl32.dll");

  if (dll != NULL){
    cout << "DLL loaded successfully" << endl;
  }
  else{
    cout << "DLL failed to load" << endl;
  }

  SetWindowSubclassFunction = (SetWindowSubclassType)GetProcAddress(dll, "SetWindowSubclass");

  if(SetWindowSubclassFunction != NULL){
    cout << "SetWindowSubclass function loaded successfully" << endl;
  }
  else{
    cout << "SetWindowSubclass function failed to load" << endl;
  }

  DefSubclassProcFunction = (DefSubclassProcType)GetProcAddress(dll, "DefSubclassProc");

  if(DefSubclassProcFunction != NULL){
    cout << "DefSubclassProc function loaded successfully" << endl;
  }
  else{
    cout << "DefSubclassProc function failed to load" << endl;
  }
}

//Returns the last Win32 error, in string format. Returns an empty string if there is no error.
std::string GetLastErrorAsString(){
  //Get the error message, if any.
  DWORD errorMessageID = ::GetLastError();
  if(errorMessageID == 0)
    return std::string(); //No error message has been recorded

  LPSTR messageBuffer = nullptr;
  size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                               NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

  std::string message(messageBuffer, size);

  //Free the buffer.
  LocalFree(messageBuffer);

  return message;
}

void resizeAll(){
  //Resize main window

  //Resize edit window
}

void rePos(){
  GetWindowRect(hwnd, &MAIN_RECT); // set new window rect
  GetWindowRect(hwnd_typeBox, &EDIT_RECT);
  // Reposition main window

  // Reposition edit window
  int main_midpoint = (MAIN_RECT.right - MAIN_RECT.left) / 2;
  int edit_midpoint = (EDIT_RECT.right - EDIT_RECT.left) / 2;
  long editXPos = main_midpoint - edit_midpoint;
  long editYPos = (MAIN_RECT.bottom - MAIN_RECT.top) * 0.80;
  // cout << "MAIN_RECT.right: " << MAIN_RECT.right << endl;
  // cout << "MAIN_RECT.left: " << MAIN_RECT.left << endl;
  //cout << "MAIN_RECT.bottom: " << MAIN_RECT.bottom << endl;
  //cout << "MAIN_RECT.top: " << MAIN_RECT.top << endl;
  // cout << "EDIT_RECT.right: " << EDIT_RECT.right << endl;
  // cout << "EDIT_RECT.left: " << EDIT_RECT.left << endl;
  //cout << "EDIT_RECT.top: " << EDIT_RECT.top << endl;
  //cout << "EDIT_RECT.bottom: " << EDIT_RECT.bottom << endl;
  // cout << "main_midpointer: " << main_midpoint << endl;
  // cout << "edit_midpoint: " << edit_midpoint << endl;
  // cout << "(main window) xPos: " << MAIN_RECT.left << endl;
  // cout << "(main window) yPos: " << MAIN_RECT.top << endl;
  // cout << "(main window) length: " << MAIN_RECT.right << endl;
  // cout << "(main window) width: " << MAIN_RECT.bottom << endl;
  // cout << "editXPos: " << editXPos << endl;
  // cout << "editYPos: " << editYPos << endl;
  SetWindowPos(hwnd_typeBox, 0, editXPos, editXPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

}
