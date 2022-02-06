#ifndef MODEL_3D_H
#define MODEL_3D_H

#include <string>
#include <fstream>
#include "Matrix.h"
#include"AffineTransform.h"

class Model3D
{
	Matrix<> InitialVertices;    // изначальная матрица
	Matrix<> Vertices;			 // текущая матрица
	Matrix<int> Edges;			 // матрица ребер
	Matrix<double> CumulativeAT; // накопленное афинное преобразование

	Matrix<> ProjectedVertices; // проекция
	Matrix<int> Faces; // грани
	Matrix<int> ImEdges; //мнимые ребра

public:
	Model3D() : Vertices(), Edges(), InitialVertices(), CumulativeAT(Identity()), Faces(), ProjectedVertices(), ImEdges() {} // конструтор по умолчанию
	Model3D(const Matrix<> vertices, const Matrix<int> faces, const Matrix<int> inEdges); //конструктор создани модели по заданным карте вершин и карте граней
	Model3D(string f_vertices, string f_faces, string f_imEdges); // конструктор создания модели по именам файлов, в которых лежат карта вершин и карта граней

	Matrix<> GetVertices() { return Vertices; }
	Matrix<int> GetFaces() { return Faces; }
	Matrix<int> GetEdges() { return Edges; }

	double GetVerticesX(int j) { return Vertices(0, j) / Vertices(3, j); }
	double GetVerticesY(int j) { return Vertices(1, j) / Vertices(3, j); }
	double GetVerticesZ(int j) { return Vertices(2, j) / Vertices(3, j); }

	double GetProjectedX(int j) { return ProjectedVertices(0, j) / ProjectedVertices(2, j); }
	double GetProjectedY(int j) { return ProjectedVertices(1, j) / ProjectedVertices(2, j); }

	void SetEdges(); // создание карты рёбер по заданной карте граней
	void Apply(Matrix<> AT);
	void Project(Matrix<> P) { ProjectedVertices =  P * Vertices ; } // проецирование модели.
};

Model3D::Model3D(const Matrix<> vertices, const Matrix<int> faces, const Matrix<int> inEdges) :
	Edges(), InitialVertices(), CumulativeAT(Identity()), Faces(faces), ProjectedVertices(), ImEdges(inEdges)
{
	InitialVertices = Vertices = vertices;
	SetEdges();
}

Model3D::Model3D(string f_vertices, string f_faces, string f_imEdges) : Edges(), CumulativeAT(Identity()), ProjectedVertices(), ImEdges()
{
	int n;
	ifstream file(f_vertices);
	file >> n;
	file.ignore();
	Vertices = Matrix<double>(4, n);
	file >> Vertices;
	file.close();

	InitialVertices = Vertices;

	ifstream file_2(f_faces);
	file_2 >> n;
	file_2.ignore();
	Faces = Matrix<int>(n, 3);
	file_2 >> Faces;
	file_2.close();

	ifstream file_3(f_imEdges);
	file_3 >> n;
	file_3.ignore();
	ImEdges = Matrix<int>(n, 2);
	file_3 >> ImEdges;
	file_3.close();

	SetEdges();
}

void Model3D::SetEdges()
{
	int N = Faces.GetRows(), count = 0, N_adj = Vertices.GetCols() + 1;
	Matrix<> matrix_adj = Matrix<>(N_adj, N_adj);

	for (int i = 0; i < N; i++)
		for (int j = 0; j < 3; j++)
		{
			int a, b;
			if (j == 2)
			{
				a = Faces(i, 0); 
				b = Faces(i, j);
			}
			else
			{
				a = Faces(i, j);
				b = Faces(i, j + 1);
			}

			if (b < a)
			{
				int tmp = a;
				a = b;
				b = tmp;
			}

			if (!matrix_adj(a, b))
			{
				matrix_adj(a, b, 1);
				count++;
			}
		}

	for (int i = 0; i < ImEdges.GetRows(); i++)
	{
		int a = ImEdges(i, 0), b = ImEdges(i, 1);
		if (b < a)
		{
			int tmp = a;
			a = b;
			b = tmp;
		}
		matrix_adj(a, b, 0);
		count--;
	}

	Edges = Matrix<int>(count, 2);
	count = 0;
	for (int i = 0; i < N_adj; i++)
		for (int j = i + 1; j < N_adj; j++)
		{
			if (matrix_adj(i, j))
			{
				Edges(count, 0, i);
				Edges(count, 1, j);
				count++;
			}
		}
}

void Model3D::Apply(Matrix<> AT)
{
	CumulativeAT = AT * CumulativeAT;
	Vertices = CumulativeAT * InitialVertices;
}

#endif MODEL_3D_H#pragma once
