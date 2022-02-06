#ifndef SCENE_3D_H
#define SCENE_3D_H

#include "Camera3D.h"
#include "Model3D.h"

class Scene3D : public Camera3D
{
public:
	Model3D* model;
	Scene3D(double L, double R, double B, double T) : Camera3D(L, R, B, T)
	{
		model = new Model3D("Vertices1.txt", "Faces1.txt", "ImEdges1.txt");
		UpdateProject();
	}

	void Render(HDC dc)
	{
		//Axes(dc);
		Matrix<int> Edges = model->GetEdges();
		int NumbEdges = Edges.GetRows();

		for (int i = 0; i < NumbEdges; i++)
		{
			int a = Edges(i, 0) - 1,
				b = Edges(i, 1) - 1;
			MoveTo(model->GetProjectedX(a), model->GetProjectedY(a));
			LineTo(dc, model->GetProjectedX(b), model->GetProjectedY(b));
		}
	}

	void UpdateProject()
	{
		model->Project(WorldToProject);
	}

	void Apply(Matrix<> AT)
	{
		model->Apply(AT);
		UpdateProject();
	}
};

#endif SCENE_3D_H