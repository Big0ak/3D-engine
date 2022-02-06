#ifndef CAMERA_3D_H
#define CAMERA_3D_H

#include"Camera2D.h"
#include"Matrix.h"
#include"Vector.h"

class Camera3D : public Camera2D
{
protected:
	bool isDraggingCamera;
	int previousCameraX, previousCameraY;

	Vector<>
		Ov, // координаты точки экранной плоскости (мировые)
		T,  // вертикальное направление для наблюдателя
		N;  // нормаль
	double D;

	Matrix<> 
		WorldToView,       // 4x4 матрицы перехода
		ViewToProject,     // 3x4
		WorldToProject;    // 3x4

public:
	void SetOv(double* ov) { Ov = ov; }
	void SetT(double* t) { T = t; }
	void SetN(double* n) { N = n; }
	void SetD(double d) { D = D * d; }

	Camera3D(double L, double R, double B, double t) : Camera2D(L, R, B, t)
	{
		D = 16;
		T(1, 1); // T(0,1,0)
		N(2, 1); // N(0,0,1)
		UpdateCamera();
	}

	void UpdateCamera()
	{
		Vector<> k = Normalization(N);
		Vector<> i = (Normalization(T.VectorProd(N)));
		Vector<> j = (k.VectorProd(i));
		
		// W -> V
		double Wv[] = {
			i(0), i(1), i(2), -(Ov.Scalar(i)),
			j(0), j(1), j(2), -(Ov.Scalar(j)),
			k(0), k(1), k(2), -(Ov.Scalar(k)),
			0, 0, 0, 1
		};

		WorldToView = Matrix<>(4, 4, Wv);

		// V -> P
		double Vp[] = {
				1, 0, 0, 0,
				0, 1, 0 ,0,	
				0, 0, -(1 / D), 1
		};
		ViewToProject = Matrix<>(3, 4, Vp);

		// W -> P
		WorldToProject = ViewToProject * WorldToView;
	}

	Vector<> Normalization(Vector<> a)
	{
		double norma = a.Norma();
		a(0, a(0) / norma);
		a(1, a(1) / norma);
		a(2, a(2) / norma);
		return a;
	}

	void StartDraggingCamera(int X, int Y)
	{
		isDraggingCamera = true;
		previousCameraX = X;
		previousCameraY = Y;
	}

	bool IsGraggingCamera()
	{
		return isDraggingCamera;
	}

	void StopDraggingCanera ()
	{
		isDraggingCamera = false;
	}

	void Distance(double k)
	{
		D = D + k > 6 ? D + k : D; 
	}

	void MoveCamera(int X, int Y)
	{
		//const double shift = 0.03125;

		//if (X < previousCameraX) // движение мыши влево
		//	if (N(0) >= 0)
		//	{
		//		N(2, N(2) - shift);
		//		N(0, N(0) + ((N(2) >= 0) ? shift : -shift));
		//	}
		//	else
		//	{
		//		N(2, N(2) + shift);
		//		N(0, N(0) + ((N(2) <= 0) ? -shift : shift));
		//	}
		//else
		//	if (X > previousCameraX) // вправо
		//		if (N(0) <= 0)
		//		{
		//			N(2, N(2) - shift);
		//			N(0, N(0) + ((N(2) >= 0) ? -shift : shift));
		//		}
		//		else
		//		{
		//			N(2, N(2) + shift);
		//			N(0, N(0) + ((N(2) <= 0) ? shift : -shift));
		//		}

		/////////////////////////////////////

		 //   if (Y < previousCameraY) // движение мыши вверх
			//	if (N(1) >= 0)
			//	{
			//		N(2, N(2) - shift);
			//		N(1, N(1) + ((N(2) >= 0) ? shift : -shift));
			//	}
			//	else
			//	{
			//		N(2, N(2) + shift);
			//		N(1, N(1) + ((N(2) <= 0) ? -shift : shift));
			//	}
			//else
			//	if (Y > previousCameraY) // вниз
			//		if (N(1) <= 0)
			//		{
			//			N(2, N(2) - shift);
			//			N(1, N(1) + ((N(2) >= 0) ? -shift : shift));
			//		}
			//		else
			//		{
			//			N(2, N(2) + shift);
			//			N(1, N(1) + ((N(2) <= 0) ? shift : -shift));
			//		}
		
		double corner = 0.027;
		double a = 4, b = 3, c = 2.75;

		double TX[3] = { N(0), N(1), N(2) };
		Matrix<> AX(3, 1, TX);
		if (X < previousCameraX) // движение мыши влево
		{
			Matrix<> R = RotationCameraY(corner) * AX;
			if (R(2, 0) > 0)
			{
				N(0, R(0, 0));
				N(1, R(1, 0));
				N(2, R(2, 0) );
			}
		}
		else
			if (X > previousCameraX) // вправо
			{
				Matrix<> R = RotationCameraY(-corner) * AX;
				if (R(2, 0) > 0)
				{
					N(0, R(0, 0));
					N(1, R(1, 0));
					N(2, R(2, 0));
				}
			}

		corner = abs(corner);

		double TY[3] = { N(0), N(1), N(2) };
		Matrix<> AY(3, 1, TY);
		if (Y < previousCameraY)   // движение мыши вверх
		{
			Matrix<> R = RotationCameraX(corner) * AY;
			if (R(2, 0) > 0)
				{
					N(0, R(0, 0));
				N(1, R(1, 0));
				N(2, R(2, 0));
			}
				
		}
		else
			if (Y > previousCameraY) // вниз
			{
				Matrix<> R = RotationCameraX(-corner) * AY;
				if (R(2, 0) > 0)
				{
					N(0, R(0, 0));
					N(1, R(1, 0));
					N(2, R(2, 0));
				}
			}

		previousCameraX = X;
		previousCameraY = Y;
	}

private:
	Matrix<> RotationCameraX(double t)
	{
		double T[9] = {
				1, 0, 0,
				0, cos(t), -sin(t),
				0, sin(t), cos(t)};
		return Matrix<>(3, 3, T);
	}

	Matrix<> RotationCameraY(double t)
	{
		double T[9] = {
				cos(t), 0, sin(t),
				0, 1, 0,
				-sin(t), 0, cos(t)};
		return Matrix<>(3, 3, T);
	}
};

#endif CAMERA_3D_H
