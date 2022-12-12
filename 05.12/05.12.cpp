#include <windows.h>
#include <tchar.h>
#include "resource.h"

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

HHOOK hHook = NULL;
TCHAR str[20];


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

LRESULT CALLBACK MouseProc(int idCode, WPARAM wParam, LPARAM lParam)
{
	/*
		Если первый параметр равен HC_ACTION - сообщение будет удалено из очереди, т.е. наша хук-функция должна его обработать.
		HC_NOREMOVE - сообщение не будет удалено из очереди и его нужно передать функции CallNextHookEx без дальнейшей обработки.
		wParam - идентификатор сообщения
		lParam - указатель на структуру MOUSEHOOKSTRUCT
		typedef struct {
		POINT pt; - координаты курсора мыши в экранных координатах
		HWND hwnd; - дескриптор окна, которому пришло сообщение
		UINT wHitTestCode; - определяет в какой области окна был курсор мыши
		ULONG_PTR dwExtraInfo; - информация, которая зависит от сообщения.
		} MOUSEHOOKSTRUCT, *PMOUSEHOOKSTRUCT;
	*/

	POINT pt;
	GetCursorPos(&pt);
	WindowFromPoint(pt);

	wsprintf(str, TEXT("%d"), pt);

	MOUSEHOOKSTRUCT* ms = (MOUSEHOOKSTRUCT*)lParam;
	if (idCode == HC_ACTION) {
		if (wParam == WM_MOUSEMOVE) {
			SetWindowText(ms->hwnd, str);
		}
		return 0;
	}

	return CallNextHookEx(hHook, idCode, wParam, lParam);
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		hHook = SetWindowsHookEx(WH_MOUSE, MouseProc, NULL, GetCurrentThreadId());
		break;

	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;
	}
	return FALSE;
}