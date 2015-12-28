#include <Windows.h>
#include <windowsx.h>
#include "CMapWindow.h"
#include "resource.h"


class COverlappedWindow {
public:
	COverlappedWindow() : handle(0){}
	static bool RegisterClassEx(wchar_t*, HINSTANCE);
	bool Create(wchar_t*, HINSTANCE);
	void Show();
	LONG SetWindowLong(HWND, int, LONG);
	CMapWindow map_window;

protected:
	void OnDestroy();
	void OnCreate();
	void OnSize();
	void OnClose();
	void LoadWorld();
	void NewWorld();
private:
	int GetSize();
	HWND hwnd_cells_text;
	HWND hwnd_gen_text;
	HWND hwnd_help_text;
	HWND hwnd_size_edit;
	HWND hwnd_size_text;
	HWND handle;
	LPARAM lparam;
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

};
