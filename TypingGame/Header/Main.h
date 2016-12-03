// Window.h

#ifndef _WINDOW_H_INCLUDE_

#define _WINDOW_H_INCLUDE_

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
typedef LRESULT (CALLBACK *SUBCLASSPROC)(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
typedef BOOL (CALLBACK* SetWindowSubclassType)(HWND, SUBCLASSPROC, UINT_PTR, DWORD_PTR);
typedef LRESULT (CALLBACK* DefSubclassProcType)(HWND, UINT, WPARAM, LPARAM);
void loadLibrary(void);
void initVar(void);
void createMainWindow(HINSTANCE hInstance);
void createEditBox(HWND hwnd);
string GetLastErrorAsString(void);
void print(string str);
void createConsole(void);
void resizeAll(void);
void rePos(void);

#endif
