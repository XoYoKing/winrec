// winrec.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
LONG g_lOldTargetProc;
HICON hicHolding = NULL;
HICON hicSighting = NULL;
HCURSOR hcsSight = NULL;

// Foward declarations of functions included in this code module:
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	hicHolding = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HOLDING));
	hicSighting = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SIGHTING));
	hcsSight = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_SIGHT));

	MSG msg;

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, NULL, (DLGPROC)About);

   return TRUE;
}

LRESULT CALLBACK TargetProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WNDPROC lpfnOldTargetProc = (WNDPROC)g_lOldTargetProc;

	switch( message )
	{
	case WM_LBUTTONDOWN:
		::SetCapture(hWnd);
		::SetCursor(hcsSight);
		::SendMessage(hWnd, STM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hicSighting);
		break;

	case WM_LBUTTONUP:
		{
			::SendMessage(hWnd, STM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hicHolding);
			::ReleaseCapture();

			POINT pt;
			HWND hWndSelected;

			::GetCursorPos(&pt);
			hWndSelected = ::WindowFromPoint(pt);

			RECT rcClient;

			::GetClientRect(hWndSelected, &rcClient);

			HDC hdcSelected = ::GetDC(hWndSelected);
			HDC hdcMem = ::CreateCompatibleDC(hdcSelected);
			HBITMAP hbmMem = ::CreateCompatibleBitmap(hdcSelected, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
			HBITMAP hbmOld = (HBITMAP)::SelectObject(hdcMem, hbmMem);

			::BitBlt(hdcMem, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, hdcSelected, 0, 0, SRCCOPY);
			::SelectObject(hdcMem, hbmOld);
			::DeleteDC(hdcMem);
			::ReleaseDC(hWndSelected, hdcSelected);

			HWND hWndRecord = ::GetDlgItem(::GetParent(hWnd), IDC_RECORD);

			::SendMessage(hWndRecord, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hbmMem);
			::DeleteObject(hbmMem);
		}
		break;
	}
	return lpfnOldTargetProc(hWnd, message, wParam, lParam);
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
			{
				HWND hwndTarget = ::GetDlgItem(hDlg, IDC_MYICON);

				g_lOldTargetProc = ::SetWindowLong(hwndTarget, GWL_WNDPROC, (LONG)TargetProc);
				return TRUE;
			}

		case WM_COMMAND:
			{
				int nID = (int)LOWORD(wParam);
				HWND hwnd = (HWND)lParam;
				HWND hwndTarget = ::GetDlgItem(hDlg, IDC_MYICON);

				if (((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL)) && (hwnd != hwndTarget)) 
				{
					PostQuitMessage(0);
					return TRUE;
				}
			}
			break;

		case WM_DESTROY:
			{
				HWND hwndTarget = ::GetDlgItem(hDlg, IDC_MYICON);

				::SetWindowLong(hwndTarget, GWL_WNDPROC, (LONG)g_lOldTargetProc);
			}
			break;
	}
    return FALSE;
}
