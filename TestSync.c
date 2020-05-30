#include "TestSync.h"
#include "Sync.h"

HINSTANCE hInst;
WCHAR szTitle[] = L"TestSync";
WCHAR szMenuName[] = L"TestSync";
WCHAR szWindowClass[] = L"TestSync";

// TODO: Error handling with line numbers.
int APIENTRY
wWinMain(
	_In_	 HINSTANCE	hInstance,
	_In_opt_ HINSTANCE	hPrevInstance,
	_In_	 LPWSTR		lpCmdLine,
	_In_ 	 int		nCmdShow
)
{	
	WNDCLASSEXW wcex;
	HWND hWnd;
	HACCEL hAccelTable;
	MSG msg;

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Add custom icon or use default Visual Studio icon.
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName	= szMenuName;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, IDI_APPLICATION);
	if (!RegisterClassExW(&wcex)) {
		ErrorExit(L"RegisterClassExW");
	}

	hWnd = CreateWindowExW(WS_EX_CLIENTEDGE, szWindowClass, szTitle,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	if (!hWnd) {
		ErrorExit(L"CreateWindowExW");
	}

	if (!SetupConsole()) {
		ErrorExit(L"SetupConsole");
	}
	
	if (!SetupInput()) {
		ErrorExit(L"SetupInput");
	}
	
	ShowWindow(hWnd, nCmdShow);
	if (!UpdateWindow(hWnd)) {
		ErrorExit(L"UpdateWindow");
	}
	
	
	HMODULE sync;
	sync = LoadLibraryW(L".\\Sync.dll");
	//printf("%d\n", Test());
	
	
	hAccelTable = LoadAccelerators(hInstance, 0);
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(hWnd, NULL, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	return (int)msg.wParam;
}

LRESULT CALLBACK
WndProc(
	_In_ HWND	hWnd,
	_In_ UINT	message,
	_In_ WPARAM	wParam,
	_In_ LPARAM	lParam
)
{
	UINT dwSize;
	LPBYTE lpData;
	RAWINPUT *ri;
	
	switch (message) {
	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}
	case WM_KEYDOWN: {
		wprintf(L".\n");
		break;
	}
	default: {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	}
	return 0;
}

BOOL
SetupInput()
{
	RAWINPUTDEVICE rid[1];
	
	// Keyboard: ignore default window messages.
	rid[0].usUsagePage = 0x01;
	rid[0].usUsage	   = 0x06;
	rid[0].dwFlags	   = RIDEV_NOLEGACY;
	rid[0].hwndTarget  = 0;
	
	if (!RegisterRawInputDevices(rid, 1, sizeof(rid[0]))) {
		return FALSE;
	}
}

BOOL
SetupConsole()
{
	FILE *fConsole;

	if (!AllocConsole()) {
		return FALSE;
	}
	
	freopen_s(&fConsole, "CONOUT$", "w", stdout);
	freopen_s(&fConsole, "CONIN$", "r", stdin);
	//freopen_s(&fConsole, "CONERR$", "w", stderr); -- buggy?
	return TRUE;
}

void
ErrorExit(
	LPWSTR lpszFunction
)
{
	// Retrieve the system error message for the last error code.
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD lerr = GetLastError();
	
	FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		lerr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Print the error message to the console.
	wprintf(L"%s: %s", lpszFunction, (wchar_t *)lpMsgBuf);

	// TODO: Update the below to use standard C functions where possible.
	// Display the error message in a message box.
	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCWSTR)lpMsgBuf) +
		 lstrlen((LPCWSTR)lpszFunction) + 40) * sizeof(WCHAR));
	snwprintf((LPWSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(WCHAR),
		L"%s failed with error %d: %s",
		lpszFunction, lerr, lpMsgBuf);
	MessageBox(NULL, L"Message", L"Error", MB_OK);
	
	LocalFree(lpMsgBuf);
	ExitProcess(lerr);
}