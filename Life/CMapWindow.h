#include "CMap.h"
#include "resource.h"

class CMapWindow {
public:
	CMapWindow() : handle(0), map(200, 200), gen(0), cells(0){}

	static bool RegisterClassEx(wchar_t*, HINSTANCE);
	bool Create(wchar_t*, HINSTANCE, HWND, int, int, int, int);
	void Show();
	LONG SetWindowLong(HWND, int, LONG);
	HWND handle;
	HWND gen;
	HWND cells;
	CMap map;
protected:
	void OnDestroy();
	void OnPaint();
	void OnTimer();
	void OnCreate();
	void OnClose();
	void OnLButtonDown();
	void OnRButtonDown();
private:
	LPARAM lparam;
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};
