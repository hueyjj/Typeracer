// Window.h

#ifndef _WINDOW_H_INCLUDE_

#define _WINDOW_H_INCLUDE_

//int Edit_GetLine(HWND hwndCtl, int line, char* lpch, int cchMax);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR
                          uIdSubclass, DWORD_PTR dwRefData);
void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
typedef LRESULT (CALLBACK *SUBCLASSPROC)(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
typedef BOOL (CALLBACK* SetWindowSubclassType)(HWND, SUBCLASSPROC, UINT_PTR, DWORD_PTR);
typedef LRESULT (CALLBACK* DefSubclassProcType)(HWND, UINT, WPARAM, LPARAM);
void freeLibrary(void);
void loadLibrary(void);
void initVar(void);
HWND createMainWindow(HINSTANCE hInstance);
HWND createDisplayBox(HWND hwnd, HINSTANCE hInstance);
HWND createWPMBox(HWND hwnd, HINSTANCE hInstance);
HWND createErrorBox(HWND hwnd, HINSTANCE hInstance);
HWND createEditBox(HWND hwnd, HINSTANCE hInstance);
HWND createStartButton(HWND hwnd, HINSTANCE hInstance);
HWND createResetButton(HWND hwnd, HINSTANCE hInstance);
string GetLastErrorAsString(void);
void print(string str);
void createConsole(void);
void resizeAll(void);
void rePos(void);
int calcWPM(int words, int seconds);
string trim(const string &str);

#endif
