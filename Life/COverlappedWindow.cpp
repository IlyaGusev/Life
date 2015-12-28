#include "COverlappedWindow.h"
#include "resource.h"

#define IDC_STATIC1 9000
#define IDC_STATIC2 9001
#define IDC_STATIC3 9002
#define IDC_STATIC4 9004
#define IDC_EDIT1 9005

bool COverlappedWindow::RegisterClassEx(wchar_t* name, HINSTANCE hInstance){
	WNDCLASSEX tag;
	tag.cbSize = sizeof(WNDCLASSEX);
	tag.style = CS_HREDRAW | CS_VREDRAW;
	tag.lpfnWndProc = windowProc;
	tag.cbClsExtra = 0;
	tag.cbWndExtra = 0;
	tag.hIcon = static_cast<HICON>(::LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON2), 1, 32, 32, NULL));
	tag.hCursor = LoadCursor(NULL, IDC_ARROW);
	tag.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	tag.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	tag.lpszClassName = L"OverlappedWindow";
	tag.hInstance = hInstance;
	tag.hIconSm = 0;
	return ::RegisterClassEx(&tag) != 0;
}

bool COverlappedWindow::Create(wchar_t* name, HINSTANCE hInstance){
	handle = CreateWindowEx(0, L"OverlappedWindow", name, WS_OVERLAPPEDWINDOW&~WS_THICKFRAME, 50, 50, 1000, 800, 0, 0, hInstance, this);
	::LoadMenu(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1));
	return true;
}
void COverlappedWindow::Show(){
	ShowWindow(handle, SW_SHOWNORMAL);
	UpdateWindow(handle);
}

LRESULT __stdcall COverlappedWindow::windowProc(HWND hnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NCCREATE) {
		COverlappedWindow* that = reinterpret_cast<COverlappedWindow*>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
		::SetWindowLong(hnd, GWL_USERDATA, reinterpret_cast<LONG>(that));
		return ::DefWindowProc(hnd, message, wParam, lParam);
	}
	COverlappedWindow* that = reinterpret_cast<COverlappedWindow*>(::GetWindowLong(hnd, GWL_USERDATA));
	switch (message){
		case WM_CREATE:
			that->handle = hnd;
			that->OnCreate();
			return ::DefWindowProc(hnd, message, wParam, lParam);
		case WM_SIZE:
			that->lparam = lParam;
			that->OnSize();
			return 0;
		case WM_CLOSE:
			that->OnClose();
			return 0;
		case WM_DESTROY:
			that->OnDestroy();
			return 0;
		case WM_COMMAND:
			switch (LOWORD(wParam)){
				case ID_NEW:
					that->NewWorld();
					return 0;
				case ID_LOAD:
					that->LoadWorld();
					return 0;
				case ID_EXIT:
					that->OnClose();
					return 0;
			}
			return 0;
		default:
			return ::DefWindowProc(hnd, message, wParam, lParam);
	}
}

void COverlappedWindow::OnCreate(){
	this->map_window = CMapWindow();
	RECT rec;
	GetClientRect(handle, &rec);
	this->map_window.RegisterClassEx(L"Map", GetModuleHandle(NULL));
	this->map_window.Create(L"Map", GetModuleHandle(NULL), this->handle, 0, 0, rec.right*0.8, rec.bottom);

	this->hwnd_gen_text = CreateWindowEx(0, L"STATIC", L"Generation",  WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, this->handle, 
		reinterpret_cast<HMENU>(IDC_STATIC1), reinterpret_cast<HINSTANCE>(::GetWindowLong(this->handle, GWL_HINSTANCE)), NULL);
	this->hwnd_cells_text = CreateWindowEx(0, L"STATIC",L"Active cells", WS_CHILD | WS_VISIBLE, 0, 0, 0, 20, this->handle,
		reinterpret_cast<HMENU>(IDC_STATIC2), reinterpret_cast<HINSTANCE>(::GetWindowLong(this->handle, GWL_HINSTANCE)), NULL);
	this->hwnd_help_text = CreateWindowEx(0, L"STATIC", L"Help text", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, this->handle,
		reinterpret_cast<HMENU>(IDC_STATIC3), reinterpret_cast<HINSTANCE>(::GetWindowLong(this->handle, GWL_HINSTANCE)),NULL);
	this->hwnd_size_text = CreateWindowEx(0, L"STATIC", L"Size text", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, this->handle,
		reinterpret_cast<HMENU>(IDC_STATIC4), reinterpret_cast<HINSTANCE>(::GetWindowLong(this->handle, GWL_HINSTANCE)), NULL);
	this->hwnd_size_edit = ::CreateWindowEx(0, L"EDIT", NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, this->handle, 
		reinterpret_cast<HMENU>(IDC_EDIT1), reinterpret_cast<HINSTANCE>(::GetWindowLong(this->handle, GWL_HINSTANCE)),  NULL); 

	this->map_window.gen = this->hwnd_gen_text;
	this->map_window.cells = this->hwnd_cells_text;

	::SendMessage(this->hwnd_gen_text, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(L"Generation: 0"));
	::SendMessage(this->hwnd_cells_text, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(L"Active cells: 0"));
	::SendMessage(this->hwnd_help_text, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(L"Help:\nSpace - pause/play\nLMB - add cell\nRMB - delete cell"));
	::SendMessage(this->hwnd_size_text, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(L"Size: "));
	::SendMessage(this->hwnd_size_edit, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(L"200"));
}

void COverlappedWindow::OnSize(){
	RECT rec;
	GetClientRect(handle, &rec);
	SetWindowPos(this->map_window.handle, HWND_TOP, 0, 0, rec.right*0.8, rec.bottom, SWP_DRAWFRAME);
	SetWindowPos(this->hwnd_gen_text, HWND_TOP, rec.right*0.8 + 10, 20, rec.right*0.2 - 20, 20, SWP_DRAWFRAME);
	SetWindowPos(this->hwnd_cells_text, HWND_TOP, rec.right*0.8 + 10, 60, rec.right*0.2 - 20, 20, SWP_DRAWFRAME);
	SetWindowPos(this->hwnd_help_text, HWND_TOP, rec.right*0.8 + 10, 150, rec.right*0.2 - 20, 80, SWP_DRAWFRAME);
	SetWindowPos(this->hwnd_size_text, HWND_TOP, rec.right*0.8 + 10, 100, 50, 20, SWP_DRAWFRAME);
	SetWindowPos(this->hwnd_size_edit, HWND_TOP, rec.right*0.8 +65, 100, rec.right*0.2 - 20, 80, SWP_DRAWFRAME);
}

int COverlappedWindow::GetSize(){
	DWORD len = ::SendMessage(this->hwnd_size_edit, WM_GETTEXTLENGTH, 0, 0);
	wchar_t* buffer = new wchar_t[len];
	::SendMessage(this->hwnd_size_edit, WM_GETTEXT, (WPARAM)len + 1, (LPARAM)buffer);

	int size = _wtof(buffer);
	return size;
}

void COverlappedWindow::NewWorld(){
	int size = GetSize();
	map_window.map = CMap(size, size);
	map_window.map.paused = true;
}

void COverlappedWindow::LoadWorld(){
	OPENFILENAME ofn = { 0 };
	wchar_t szDirect[260];
	wchar_t szFileName[260];
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szDirect;
	*(ofn.lpstrFile) = 0;
	ofn.nMaxFile = sizeof(szDirect);
	ofn.lpstrFilter = NULL;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = szFileName;
	*(ofn.lpstrFileTitle) = 0;
	ofn.nMaxFileTitle = sizeof(szFileName);
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_EXPLORER;
	GetOpenFileName(&ofn);

	std::wstring wide(ofn.lpstrFile);
	std::string str(wide.begin(), wide.end());

	int size = GetSize();
	CMap map = CMap(size, size);
	map.paused = true;
	if (map.LoadPattern(str) != 1){
		map_window.map = map;
	}
}

void COverlappedWindow::OnClose(){
	wchar_t path[256] = L"";
	int msgBox = MessageBox(handle, L"Do you want to exit?", L"Exit", MB_YESNO | MB_ICONQUESTION);
	switch (msgBox) {
		case IDYES:
			break;
		case IDNO:
			return;
	}
	::DestroyWindow(handle);
}

void COverlappedWindow::OnDestroy(){
	KillTimer(handle, 1);
	PostQuitMessage(WM_QUIT);
}
