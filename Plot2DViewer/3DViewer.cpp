#include <windows.h>
#include <windowsx.h>
#include "Data.h"
#include "Scene3D.h"
#include "AffineTransform.h"
#include "Model3D.h"

LRESULT _stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);						// �������� ������� ���������
int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)		// �������� ���������
{
	// ������ ������������ ����� �������� ��������� - �������� ����: ������� ����������� ������� ����� wc, ����� �������� ���� hWnd
	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;						// ��� ������� ���������, ������������ �� ������ �������
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;						// ������������� ����������, ���������� ����� ������ ���������� ��� ������� ������
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(6);
	wc.lpszMenuName = 0;							// ���� � ������� ������ �����������
	wc.lpszClassName = (LPCSTR)"MainWindowClass";	// ��� �������� ������, ������������ ��� �������� ����������� ����
	RegisterClass(&wc);								// ����������� ������ wc

	HWND hWnd = CreateWindow(						// hWnd - ����������, ���������������� ����; ������� �������� ���� ��������� ���������� hWnd ��������� ���������
		(LPCSTR)"MainWindowClass",					// ��� �������� ������
		(LPCSTR)"3D Viewer",						// ��������� ����
		WS_OVERLAPPEDWINDOW,						// ����� ����
		200,200,400,400,							// ���������� �� ������ ������ �������� ���� ����, ��� ������ � ������
		nullptr,nullptr,hInstance,nullptr);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	// ������ ������������ ����� �������� ��������� - �������� ���� ��������� ��������� ���������, ������� ������� ��������� � ��������� �� ��������������� �����
	MSG msg;
	while(GetMessage(&msg,nullptr,0,0))				// ������� GetMessage �������� �� ������� ��������� � ������� ��� � ��������� msg
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);						// ������� DispatchMessage ��������� ������� � ������������� ������ ������� ���������
	}

	return 0;
}

// � �������� ������ ����������� ������ ���� ���������� ���������� - �������� ������ ������ Scene3D
// ��� ���������� �������� �������������� ����������� ��������� � �������, ������������� � ���� ������
Scene3D scene(L, R, B, T);

LRESULT _stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)	// ������� ��������� ��������� � ������������ ��� ���������, ������������ ����
{
	switch(msg)
	{
	case WM_PAINT:
	{
		HDC dc = GetDC(hWnd);
		scene.Clear(dc); // ����� �������������� � ������ Camera2D ������, ����������� �� ������� ������� ������� ���� hWnd
		scene.Render(dc);
		ReleaseDC(hWnd,dc);
		return DefWindowProc(hWnd,msg,wParam,lParam);
	}
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_LEFT:
		{
			scene.Apply(Translation(-2, 0, 0));
			break;
		}
		case VK_RIGHT:
		{
			scene.Apply(Translation(2, 0, 0));
			break;
		}
		case VK_UP:
		{
			scene.Apply(Translation(0, 2, 0));
			break;
		}
		case VK_DOWN:
		{
			scene.Apply(Translation(0, -2, 0));
			break;
		}
		case VK_CONTROL:
		{
			POINT P;
			GetCursorPos(&P);
			ScreenToClient(hWnd, &P);
			scene.StartDraggingCamera(P.x, P.y);
			break;
		}
		case VK_TAB:
		{
			scene.Apply(RotationX(0.17));
			break;
		}
		}
		InvalidateRect(hWnd, nullptr, false);
		return 0;
	}
	case WM_SIZE:
	{
		HDC dc = GetDC(hWnd);
		scene.SetResolution(dc);
		ReleaseDC(hWnd, dc);
		InvalidateRect(hWnd,nullptr,false);
		return 0;
	}

	case WM_LBUTTONDOWN:
	{
		scene.StartDragging(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}

	case WM_MOUSEMOVE:
	{
		if (scene.IsGragging())
		{
			scene.Drag(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			InvalidateRect(hWnd, nullptr, false);
		}
		else 
			if (scene.IsGraggingCamera())
			{
				POINT P;
				GetCursorPos(&P);
				ScreenToClient(hWnd, &P);

				scene.MoveCamera(P.x, P.y);
				scene.UpdateCamera();
				scene.UpdateProject();

				InvalidateRect(hWnd, nullptr, false);
			}
		return 0;
	}
	case WM_LBUTTONUP:
	{
		scene.StopDraging();
		return 0;
	}

	case WM_MOUSEWHEEL:
	{
		if (scene.IsGraggingCamera())
		{
			double k = GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? 5 : -5;
			scene.Distance(k);

			scene.UpdateCamera();
			scene.UpdateProject();
			InvalidateRect(hWnd, nullptr, false);
		}
		else
		{
			double k = GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? 1.1 : 0.9;
			POINT P;
			GetCursorPos(&P);
			ScreenToClient(hWnd, &P);
			scene.Size(k, P.x, P.y);
			InvalidateRect(hWnd, nullptr, false);
		}
		
		return 0;
	}
	case WM_KEYUP:
	{
		switch (wParam)
		{
		case VK_CONTROL:
		{
			scene.StopDraggingCanera();
			break;
		}
		}
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	default:
	{
		return DefWindowProc(hWnd,msg,wParam,lParam);
	}
	}
	return 0;
}
