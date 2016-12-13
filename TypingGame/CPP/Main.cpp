using namespace std;
#include <iostream>
#include <ctype.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <windows.h>
#include <Windowsx.h>
#include <commctrl.h>
#include "../Header/Main.h"
#include <tchar.h>
#include "../Header/TextProcessor.h"
#include <string>
#include <time.h>

#define _countof(array) (sizeof(array) / sizeof(array[0]))
#define ID_START_BUTTON 101
#define ID_RESET_BUTTON 102
#define ID_TIMER 103

template <typename T>
string to_string(T value) {
  ostringstream ss;
  ss << value;
  return ss.str();
}

HWND hwnd, hwnd_displayBox, hwnd_typeBox, hwnd_wpmBox, hwnd_errorBox, hwnd_startBn, hwnd_resetBn;
HINSTANCE dll, dll2;
SetWindowSubclassType SetWindowSubclassFunction;
DefSubclassProcType DefSubclassProcFunction;
const char g_szClassName[] = "Typeracer";
bool timer_start = false;
int SCREEN_WIDTH = 0, SCREEN_LENGTH = 0, timer = 0, WORDS = 0;
RECT MAIN_RECT, DISPLAY_RECT, WPM_RECT, EDIT_RECT, START_RECT, RESET_RECT, ERROR_RECT;
TextProcessor txt;
char inputBuffer[256]; // hold user input
queue<string> strQueue, trashQueue;
INPUT inputs[1];


// Main()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
  // FreeConsole();
  loadLibrary();

  WNDCLASSEX wc;
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

  if (!RegisterClassEx(&wc))
    {
      MessageBox(NULL, "Window Registration Failed!", "Error!",
                 MB_ICONEXCLAMATION | MB_OK);
      return 0;
    }

  //TODO dynamically load txt files from a subdirectory ../Textfiles
  txt.loadFile("File.txt"); // load txt file
  strQueue = txt.storage;

  initVar(); // initialize variable
  hwnd = createMainWindow(hInstance);
  hwnd_displayBox = createDisplayBox(hwnd, hInstance);
  hwnd_wpmBox = createWPMBox(hwnd, hInstance);
  hwnd_errorBox = createErrorBox(hwnd, hInstance);
  hwnd_typeBox = createEditBox(hwnd, hInstance);
  hwnd_startBn = createStartButton(hwnd, hInstance);
  hwnd_resetBn = createResetButton(hwnd, hInstance);

  ShowWindow(hwnd, nCmdShow);

  while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
      TranslateMessage(&Msg);
      DispatchMessage(&Msg);
    }
  //TODO segmentation fault when program closes
  return Msg.wParam;
}

// The Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
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
      freeLibrary();
      cout << "WM_CLOSE message" << endl;
      DestroyWindow(hwnd);
      break;
    case WM_DESTROY:
      cout << "WM_DESTROY message" << endl;
      PostQuitMessage(0);
      break;
    case WM_KEYDOWN:
      switch (wParam)
        {
        }
      cout << "(Main window) key down" << endl;
      break;
    case WM_LBUTTONDOWN:
      SetFocus(hwnd); // focus on main window
      cout << "Main window focus" << endl;
      break;
    case WM_COMMAND:
      switch (wParam)
        {
        case ID_START_BUTTON:
          cout << "Start button pressed" << endl;
          timer_start = true;
          SetTimer(hwnd_wpmBox, ID_TIMER, 1000, (TIMERPROC) &TimerProc);
          break;
        case ID_RESET_BUTTON:
          cout << "Reset button pressed" << endl;
          while (!strQueue.empty()) {
            trashQueue.push(strQueue.front());
            strQueue.pop();
          }
          while (!trashQueue.empty()) {
            strQueue.push(trashQueue.front());
            trashQueue.pop();
          }
          timer = 0;
          WORDS = 0;
          timer_start = false;
          KillTimer(hwnd_wpmBox, ID_TIMER);
          SetWindowText(hwnd_wpmBox, "0");
          break;
        }
      break;
    default:
      return DefWindowProc(hwnd, msg, wParam, lParam);
    }
  return 0;
}

LRESULT CALLBACK EditProc(HWND hWnd_edit, UINT uMsg, WPARAM wParam,
                          LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
  switch (uMsg)
    {
    case WM_CREATE:
      break;
    case WM_KEYDOWN:
      switch (wParam)
        {
        case VK_RETURN:
          Edit_GetLine(hwnd_typeBox, 0, inputBuffer, 256);
          cout << inputBuffer << endl;
          cout << "(Edit Window): Return key pressed" << endl;
          break;
        case VK_SPACE:
          Edit_GetLine(hwnd_typeBox, 0, inputBuffer, 256);
          string str(inputBuffer);
          string tr = trim(str);
          if (strQueue.front().compare(tr) == 0) {
            trashQueue.push(strQueue.front());
            strQueue.pop();
            WORDS++;
            if (strQueue.front() == " ") {
              trashQueue.push(strQueue.front());
              strQueue.pop();
              cout << "space poppped" << endl;
              Edit_SetText(hwnd_typeBox, "");
              //NOTE delete random empty char after clearing edit control w/ ""
              inputs[0].type = INPUT_KEYBOARD;
              inputs[0].ki.wVk = VK_BACK;
              SendInput(_countof(inputs), inputs, sizeof(inputs));
              SetWindowText(hwnd_errorBox, "");
            }
            cout << "Correct input" << endl;
          }
          SetWindowText(hwnd_errorBox, strQueue.front().c_str());

          cout << "Spacebar pressed" << endl;
          break;
        }
      break;
    case WM_LBUTTONDOWN:
      SetFocus(hWnd_edit); // focus on edit control
      cout << "Edit window focus" << endl;
      break;
    }
  return DefSubclassProcFunction(hWnd_edit, uMsg, wParam, lParam);
}

void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
  string s;
  if (timer_start) {
    timer++;
  }
  int wpm = calcWPM(WORDS, timer);
  // cout << "wpm " << wpm << endl;
  s = to_string(wpm) + " wpm"; // convert int to c-string for display
  cout << "Timer: " << timer << endl;
  SetWindowText(hwnd_wpmBox, s.c_str());
}

void initVar() {
  if (SCREEN_LENGTH <= 0) SCREEN_LENGTH = GetSystemMetrics(SM_CXSCREEN);
  //  cout << "SCREEN_LENGTH: " << SCREEN_LENGTH << endl;
  if (SCREEN_WIDTH <= 0) SCREEN_WIDTH = GetSystemMetrics(SM_CYSCREEN);
  // cout << "SCREEN_WIDTH: " << SCREEN_WIDTH << endl;
}

HWND createMainWindow(HINSTANCE hInstance) {
  // Create Window
  int WIN_LENGTH = SCREEN_LENGTH * 0.66;
  int WIN_WIDTH = SCREEN_WIDTH * 0.66;

  HWND hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,
                             g_szClassName,
                             "Typeracer",
                             WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
                             | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
                             0, 0, WIN_LENGTH, WIN_WIDTH,
                             NULL, NULL, hInstance, NULL);
  if (hwnd == NULL){
    MessageBox(NULL, "Main window failed to load!", "Error!",
               MB_ICONEXCLAMATION | MB_OK);
    return 0;
  }

  GetWindowRect(hwnd, &MAIN_RECT); // init MAIN_RECT

  // Center window
  int xPos = (SCREEN_LENGTH - MAIN_RECT.right) / 2;
  int yPos = (SCREEN_WIDTH - MAIN_RECT.bottom) / 2;
  SetWindowPos(hwnd, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
  cout << "Main window created" << endl;
  return hwnd;
}

HWND createDisplayBox(HWND hwnd, HINSTANCE hInstance) {
  int DISPLAY_LENGTH = (MAIN_RECT.right - MAIN_RECT.left) * 0.80;
  int DISPLAY_WIDTH = 400;
  HWND  hwnd_displayBox = CreateWindowEx(WS_EX_LEFT,
                                         "STATIC",
                                         "",
                                         WS_VISIBLE | WS_CHILD | SS_LEFT,
                                         0, 0, DISPLAY_LENGTH, DISPLAY_WIDTH,
                                         hwnd, NULL, hInstance, NULL);
  if (hwnd_displayBox == NULL) {
    MessageBox(NULL, "Displaybox Creation Failed!", "Error!",
               MB_ICONEXCLAMATION | MB_OK);
    return 0;
  }
  GetWindowRect(hwnd_displayBox, &DISPLAY_RECT);
  SetWindowText(hwnd_displayBox, txt.getBuffer());
  return hwnd_displayBox;
}

HWND createWPMBox(HWND hwnd, HINSTANCE hInstance) {
  int WPM_LENGTH = 60;
  int WPM_WIDTH = 25;
  HWND hwnd_wpmBox = CreateWindowEx(WS_EX_LEFT,
                                    "STATIC",
                                    "",
                                    WS_VISIBLE | WS_CHILD | SS_LEFT,
                                    0, 0, WPM_LENGTH, WPM_WIDTH,
                                    hwnd, NULL, hInstance, NULL);
  if (hwnd_wpmBox == NULL) {
    MessageBox(NULL, "WPM box Creation Failed!", "Error!",
               MB_ICONEXCLAMATION | MB_OK);
    return 0;
  }
  SetWindowText(hwnd_wpmBox, "0 wpm");
  GetWindowRect(hwnd_wpmBox, &WPM_RECT);
  return hwnd_wpmBox;
}

HWND createErrorBox(HWND hwnd, HINSTANCE hInstance) {
  int ERROR_LENGTH = 300;
  int ERROR_WIDTH = 20;
  HWND hwnd_errorBox = CreateWindowEx(WS_EX_LEFT,
                                      "STATIC",
                                      "",
                                      WS_VISIBLE | WS_CHILD | SS_LEFT,
                                      0, 0, ERROR_LENGTH, ERROR_WIDTH,
                                      hwnd, NULL, hInstance, NULL);
  if (hwnd_errorBox == NULL) {
    MessageBox(NULL, "Error box Creation Failed!", "Error!",
               MB_ICONEXCLAMATION | MB_OK);
    return 0;
  }
  SetWindowText(hwnd_errorBox, strQueue.front().c_str());
  GetWindowRect(hwnd_errorBox, &ERROR_RECT);
  return hwnd_errorBox;
}

HWND createEditBox(HWND hwnd, HINSTANCE hInstance) {
  // User input text box
  int EDIT_LENGTH = (MAIN_RECT.right - MAIN_RECT.left) * 0.50;
  int EDIT_WIDTH = 25;
  // cout << "EDIT_LENGTH: " << EDIT_LENGTH << endl;
  // cout << "EDIT_WIDTH: " << EDIT_WIDTH << endl;

  HWND hwnd_typeBox = CreateWindowEx(WS_EX_CLIENTEDGE,
                                     "RICHEDIT50W",
                                     NULL,
                                     WS_CHILD | WS_VISIBLE | WS_BORDER| WS_TABSTOP,
                                     0, 0, EDIT_LENGTH, EDIT_WIDTH,
                                     hwnd, NULL , hInstance, NULL);
  if (hwnd_typeBox == NULL) {
    MessageBox(NULL, "Editbox Creation Failed!", "Error!",
               MB_ICONEXCLAMATION | MB_OK);
    return 0;
  }

  GetWindowRect(hwnd_typeBox, &EDIT_RECT);
  SetWindowSubclassFunction(hwnd_typeBox, EditProc, 0, 0) ?
    cout << "Editbox subclassed successfully" << endl :
    cout << "Editbox subclassed failed" << endl;
  return hwnd_typeBox;
}

HWND createStartButton(HWND hwnd, HINSTANCE hInstance){
  int START_LENGTH = 60;
  int START_WIDTH = 25;
  HWND hwnd_startBn = CreateWindowEx(WS_EX_CLIENTEDGE,
                                     "BUTTON",
                                     "start",
                                     WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                     0, 0, START_LENGTH, START_WIDTH,
                                     hwnd, (HMENU)ID_START_BUTTON, hInstance, NULL);
  if (hwnd_startBn == NULL) {
    MessageBox(NULL, "Start button Creation Failed!", "Error!",
               MB_ICONEXCLAMATION | MB_OK);
    return 0;
  }
  GetWindowRect(hwnd_startBn, &START_RECT);
  return hwnd_startBn;
}

HWND createResetButton(HWND hwnd, HINSTANCE hInstance){
  int RESET_LENGTH = 60;
  int RESET_WIDTH = 25;
  HWND hwnd_resetBn = CreateWindowEx(WS_EX_CLIENTEDGE,
                                     "BUTTON",
                                     "reset",
                                     WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                     0, 0, RESET_LENGTH, RESET_WIDTH,
                                     hwnd, (HMENU)ID_RESET_BUTTON, hInstance, NULL);
  if (hwnd_resetBn == NULL) {
    MessageBox(NULL, "Reset button Creation Failed!", "Error!",
               MB_ICONEXCLAMATION | MB_OK);
    return 0;
  }
  GetWindowRect(hwnd_resetBn, &RESET_RECT);
  return hwnd_resetBn;
}

void loadLibrary() {
  dll2 = LoadLibrary("Msftedit.dll");
  if (dll2 != NULL) {
    cout << "DLL2 loaded successfully" << endl;
  } else {
    cout << "DLL2 failed to load" << endl;
  }

  dll = LoadLibrary("ComCtl32.dll");

  if (dll != NULL) {
    cout << "DLL loaded successfully" << endl;
  } else {
    cout << "DLL failed to load" << endl;
  }

  SetWindowSubclassFunction = (SetWindowSubclassType)GetProcAddress(dll, "SetWindowSubclass");

  if (SetWindowSubclassFunction != NULL) {
    cout << "SetWindowSubclass function loaded successfully" << endl;
  } else {
    cout << "SetWindowSubclass function failed to load" << endl;
  }

  DefSubclassProcFunction = (DefSubclassProcType)GetProcAddress(dll, "DefSubclassProc");

  if (DefSubclassProcFunction != NULL) {
    cout << "DefSubclassProc function loaded successfully" << endl;
  } else {
    cout << "DefSubclassProc function failed to load" << endl;
  }

}

void freeLibrary() {
  FreeLibrary(dll);
  FreeLibrary(dll2);
  dll = dll2 = NULL;
  cout << "freeLibrary() called" << endl;
}

//Returns the last Win32 error, in string format. Returns an empty string if there is no error.
std::string GetLastErrorAsString() {
  //Get the error message, if any.
  DWORD errorMessageID = ::GetLastError();
  if(errorMessageID == 0)
    return std::string(); //No error message has been recorded

  LPSTR messageBuffer = nullptr;
  size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                               FORMAT_MESSAGE_FROM_SYSTEM |
                               FORMAT_MESSAGE_IGNORE_INSERTS,
                               NULL, errorMessageID,
                               MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                               (LPSTR)&messageBuffer, 0, NULL);

  std::string message(messageBuffer, size);

  //Free the buffer.
  LocalFree(messageBuffer);

  return message;
}

void resizeAll() {
  //Resize main window

  //Resize edit window
}

void rePos() {
  // Reposition main window
  GetWindowRect(hwnd, &MAIN_RECT);
  int main_midpoint = (MAIN_RECT.right - MAIN_RECT.left) / 2;
  // Reposition display window

  int display_midpoint = (DISPLAY_RECT.right - DISPLAY_RECT.left) / 2;
  int displayXPos = main_midpoint - display_midpoint;
  int displayYPos = (MAIN_RECT.bottom - MAIN_RECT.top) * .05;
  int displayLength = DISPLAY_RECT.right - DISPLAY_RECT.left;
  SetWindowPos(hwnd_displayBox, 0, displayXPos, displayYPos,
               0, 0, SWP_NOZORDER | SWP_NOSIZE);
  GetWindowRect(hwnd_displayBox, &DISPLAY_RECT);

  // Reposition wpm window
  int wpmXPos = displayLength + displayXPos;
  int wpmYPos = displayYPos;
  SetWindowPos(hwnd_wpmBox, 0, wpmXPos + 10, wpmYPos,
               0, 0, SWP_NOZORDER | SWP_NOSIZE);
  GetWindowRect(hwnd_wpmBox, &WPM_RECT);

  // Reposition edit window
  int edit_midpoint = (EDIT_RECT.right - EDIT_RECT.left) / 2;
  long editXPos = main_midpoint - edit_midpoint;
  long editYPos = (MAIN_RECT.bottom - MAIN_RECT.top) * 0.80;
  SetWindowPos(hwnd_typeBox, 0, editXPos, editYPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
  GetWindowRect(hwnd_typeBox, &EDIT_RECT);

  // Reposition error window
  int errorXPos = editXPos;
  int errorYPos = editYPos - 30;
  SetWindowPos(hwnd_errorBox, 0, errorXPos, errorYPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
  GetWindowRect(hwnd_errorBox, &ERROR_RECT);

  // Reposition start button
  int startBnLength = START_RECT.right - START_RECT.left;
  int startBnXPos = editXPos + EDIT_RECT.right - EDIT_RECT.left;
  int startBnYPos = editYPos;
  SetWindowPos(hwnd_startBn, 0, startBnXPos+10, startBnYPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
  GetWindowRect(hwnd_startBn, &START_RECT);

  // Reposition stop button
  int resetBnXPos = startBnXPos + startBnLength;
  int resetBnYPos = editYPos;
  SetWindowPos(hwnd_resetBn, 0, resetBnXPos+20, resetBnYPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
  GetWindowRect(hwnd_resetBn, &RESET_RECT);
}

int calcWPM(int words, int seconds) {
  cout << "words " << words << " seconds " << seconds << endl;
  double min = (double)seconds / (double)60;
  cout << "min " << min << endl;
  double wpm = (double)words / min;
  return wpm;
}

string trim(const string &str) {
  size_t pos1 = str.find_first_not_of(' ');
  if (string::npos == pos1) return str;
  size_t pos2 = str.find_last_not_of(' ');
  return str.substr(pos1, pos2 - pos1);
}
