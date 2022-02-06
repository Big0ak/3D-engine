#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <fstream>

using namespace std;


template <typename Cell = double>
class Matrix
{
protected:
	int cols, rows;
	Cell** cells;
	void AllocateCells(int, int); // ��������� ������
	void FreeCells(); // ������������ ������
public:
	Matrix() : cols(0), rows(0), cells(nullptr) {}	// ����������� �� ���������
	Matrix(const Matrix&);							// ����������� �����������
	Matrix(int, int);								// ����������� ������� �������
	Matrix(int, int, Cell*);						// ����������� ������� �� ������
	~Matrix();										// ����������

	Cell& operator()(int i, int j) { return cells[i][j]; }					   // ��������� ��������
	void operator()(int i, int j, Cell data) { cells[i][j] = data; }   // ��������� ��������

	int GetRows() { return rows; }
	int GetCols() { return cols; }

	Matrix& operator = (const Matrix&);		// ���������� ��������� ������������
	Matrix  operator + (const Matrix&);		// �������� ������
	Matrix  operator - (const Matrix&);		// ��������� ������
	Matrix  operator * (const Matrix&);		// ��������� ������

	friend istream& operator >> <> (istream&, Matrix&);			// ���������� ��������� >> ��� ����� �������
	friend ostream& operator << <> (ostream&, const Matrix&);	// ���������� ��������� << ��� ������ �������
};

template <typename Cell>
Matrix<Cell>::Matrix(const Matrix<Cell>& M) // ����������� �����������
{
	AllocateCells(M.rows, M.cols);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			cells[i][j] = M.cells[i][j];
}

template <typename Cell>
Matrix<Cell>::Matrix(int ROWS, int COLS) // ������� �������
{
	AllocateCells(ROWS, COLS);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			cells[i][j] = 0;
}

template <typename Cell>
Matrix<Cell>::Matrix(int ROWS, int COLS, Cell* list) // ������� �� ������
{
	AllocateCells(ROWS, COLS);
	int count = 0;
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
		{
			double tmp = list[count];
			cells[i][j] = list[count];
			count++;
		}
}

template <typename Cell>
Matrix<Cell>::~Matrix() // ����������
{
	FreeCells();
}

template <typename Cell>
Matrix<Cell>& Matrix<Cell>::operator=(const Matrix& M)
{
	if (rows != M.rows || cols != M.cols)
	{
		FreeCells();
		AllocateCells(M.rows, M.cols);
	}
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			cells[i][j] = M.cells[i][j];
	return *this;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator+(const Matrix& M)
{
	Matrix<Cell> res(*this);
	if (rows == M.rows && cols == M.cols)
	{
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				res.cells[i][j] += M.cells[i][j];
	}
	return res;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator-(const Matrix& M)
{

	Matrix<Cell> res(*this);
	if (rows == M.rows && cols == M.cols)
	{
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				res.cells[i][j] -= M.cells[i][j];
	}
	return res;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator*(const Matrix& M)
{
	Matrix<Cell> res(rows, M.cols);
	if (cols == M.rows)
	{
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < M.cols; j++)
			{
				Cell sum = 0;
				for (int k = 0; k < cols; k++)
					sum += this->cells[i][k] * M.cells[k][j];
				res.cells[i][j] = sum;
			}
	}
	return res;
}

template <typename Cell>
void Matrix<Cell>::AllocateCells(int ROWS, int COLS)
{
	cells = new Cell * [ROWS];
	for (int i = 0; i < ROWS; i++)
		cells[i] = new Cell[COLS];
	rows = ROWS;
	cols = COLS;
}

template <typename Cell>
void Matrix<Cell>::FreeCells()
{
	for (int i = 0; i < rows; i++)
		delete cells[i];
	delete cells;
	rows = 0;
	cols = 0;
}

template <typename Cell>
istream& operator >> (istream& fi, Matrix<Cell>& M)
{
	for (int i = 0; i < M.rows; i++)
		for (int j = 0; j < M.cols; j++)
			fi >> M.cells[i][j];
	return fi;
}

template <typename Cell>
ostream& operator << (ostream& fo, const Matrix<Cell>& M)
{
	for (int i = 0; i < M.rows; i++)
	{
		fo << "  ";
		for (int j = 0; j < M.cols; j++)
			fo << M.cells[i][j] << " \t";
		fo << endl;
	}
	return fo;
}

#endif MATRIX_H
