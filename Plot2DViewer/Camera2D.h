#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include <windows.h>

class Camera2D
{
private:
	bool isDragging;
	int previousX, previousY;
	double posX, posY, X0, Y0;					// Позиция графического курсора в мировых координатах (для функций MoveTo и LineTo)
protected:
	double L, R, B, T, pixel_w;					// Мировые координаты границ рабочей области окна
	int W, H;							// Разрешение рабочей области окна
	int WorldToScreenX(double X)		// Переход от мировых координат к экранным (для абсциссы)
	{
		return (int)((X-L)/(R-L)*W);
	}
	int WorldToScreenY(double Y)		// Переход от мировых координат к экранным (для ординаты)
	{
		return (int)((T-Y)/(T-B)*H);
	}
	double ScreenToWorldX(int X)		// Переход от экранных координат к мировым (для абсциссы)
	{
		return L + (R-L)*(X+0.5)/W;
	}
	double ScreenToWorldY(int Y)		// Переход от экранных координат к мировым (для ординаты)
	{
		return T - (T-B)*(Y+0.5)/H;
	}
public:
	Camera2D(double L, double R, double B, double T) : L(L), R(R), B(B), T(T) {}

	void Clear(HDC dc) // Очистка рабочей области окна
	{
		//SelectObject(dc, CreatePen(PS_SOLID, 0, RGB(255, 255, 255)));
		//SelectObject(dc, CreateSolidBrush(RGB(255, 255, 255)));
		Rectangle(dc, 0, 0, W, H);
	}
	void SetResolution(HDC dc)
	{
		RECT rect;
		GetClientRect(WindowFromDC(dc), &rect);
		W = rect.right + 1;
		H = rect.bottom + 1;
		double tmp1 = (B + T) / 2, tmp2 = (R - L) / 2 * H / W;
		B = tmp1 - tmp2;
		T = tmp1 + tmp2;
		pixel_w = ScreenToWorldX(2) - ScreenToWorldX(1);
		// Данная процедура вызывается при изменении размеров окна
		// В ней задаются значения величин W, H, а также настраиваются значения параметров L, R, B, T таким образом, чтобы обеспечить одинаковые масштабы по координатным осям
	}
	void MoveTo(double X, double Y)
	{
		posX = X;
		posY = Y;
		// Перемещение графического курсора (posX, posY)
		// действуем в мировых координатах
	}
	void LineTo(HDC dc, double X, double Y)
	{
		SelectObject(dc, CreatePen(PS_SOLID, 0, RGB(0, 0, 0)));
		::MoveToEx(dc, WorldToScreenX(posX), WorldToScreenY(posY), NULL);
		::LineTo(dc, WorldToScreenX(X), WorldToScreenY(Y));
		MoveTo(X, Y);
		// Отрисовка линии из текущей позиции графического курсора в точку (X, Y) и его перемещение в эту точку
		// действуем в мировых координатах
	}
	void Axes(HDC dc)
	{
		MoveTo(0, T);
		LineTo(dc, 0, B);

		MoveTo(L, 0);
		LineTo(dc, R, 0);

		// Отрисовка координатных осей
	}
	
	//----------------------------------------------------------------

	void StartDragging(int X, int Y)
	{
		isDragging = true;
		previousX = X;
		previousY = Y;
	}

	void Drag(int X, int Y)
	{
		int deltaX = X - previousX;
		int deltaY = Y - previousY;
		double 
			deltaX_W = ((R - L) * deltaX)/W,
			deltaY_W = ((T - B) * deltaY)/H;
		L -= deltaX_W;
		R -= deltaX_W;
		B += deltaY_W;
		T += deltaY_W;
		previousX = X;
		previousY = Y;
	}
	void StopDraging()
	{
		isDragging = false;
	}
	bool IsGragging()
	{
		return isDragging;
	}

	void Size(double k, int X, int Y)
	{
		double 
			X_W = ScreenToWorldX(X),
			Y_W = ScreenToWorldY(Y);
		L = X_W - (X_W - L) / k;
		R = X_W + (R - X_W) / k;
		T = Y_W + (T - Y_W) / k;
		B = Y_W - (Y_W - B) / k;
		pixel_w = ScreenToWorldX(2) - ScreenToWorldX(1);
	}
};

#endif CAMERA_2D_H
