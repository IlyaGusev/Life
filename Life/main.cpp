#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include "COverlappedWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	COverlappedWindow window = COverlappedWindow();
	window.RegisterClassEx(L"Window", hInstance);
	window.Create(L"Life", hInstance);
	window.Show();
	MSG msg;
	HACCEL hAccel = ::LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	while (::GetMessage(&msg, NULL, 0, 0) != 0) {
		if (!TranslateAccelerator(window.map_window.handle, hAccel, &msg)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}