#ifndef VECTOR_H
#define VECTOR_H

template <typename Cell = double>
class Vector : public Matrix<Cell>
{
public:
	Vector() : Matrix<Cell>(1, 3) {}

	Vector(Cell* list) : Matrix<Cell>(1, 3, list) {}

	Cell operator()(int i) { return (i >= 0 && i <= 2) ? this->cells[0][i] : -1; } // получение значения из вектора
	void operator() (int i, Cell data) { this->cells[0][i] = data; } // задание значения вектора

	double Scalar(Vector<Cell> right) // скалярное произведение
	{
		double result = 0;
		for (int i = 0; i < 3; i++)
			result += this->cells[0][i] * right.cells[0][i];
		return result;
	}

	Vector<Cell> VectorProd(Vector<Cell> right) // векторное произведение
	{
		Vector<Cell> result;
		result(0, this->cells[0][1] * right.cells[0][2] - this->cells[0][2] * right.cells[0][1]);
		result(1, this->cells[0][2] * right.cells[0][0] - this->cells[0][0] * right.cells[0][2]);
		result(2, this->cells[0][0] * right.cells[0][1] - this->cells[0][1] * right.cells[0][0]);
		return result;
	}

	double Norma()
	{
		double norm = 0;
		for (int i = 0; i < 3; i++)
			norm += cells[0][i] * cells[0][i];
		return sqrt(norm);
	}

	Vector<Cell> operator * (double num)  // Умножение вектора на число
	{
		Vector<Cell> result;
		for (int i = 0; i < 3; i++)
			result(i, this->cells[0][i] * num);
		return result;
	}
};

#endif VECTOR_H
