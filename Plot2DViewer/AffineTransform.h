#ifndef AFFINE_TRANSFORM_H
#define AFFINE_TRANSFORM_H

Matrix<> Identity()
{
	double T[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 };
	return Matrix<>(4, 4, T);
}

Matrix<> Translation(double x, double y, double z)
{
	double T[16] = {
			1, 0, 0, x,
			0, 1, 0, y,
			0, 0, 1, z,
			0, 0, 0, 1};
	return Matrix<>(4, 4, T);
}

Matrix<> RotationX(double t)
{
	double T[16] = {
			1, 0, 0, 0,
			0, cos(t), -sin(t), 0,
			0, sin(t), cos(t), 0,
			0, 0, 0, 1 };
	return Matrix<>(4, 4, T);
}

Matrix<> RotationY(double t)
{
	double T[16] = {
			cos(t), 0, sin(t), 0,
			0, 1, 0, 0,
			-sin(t), 0, cos(t), 0,
			0, 0, 0, 1};
	return Matrix<>(4, 4, T);
}

Matrix<> RotationZ(double t) {
	double T[16] = {
			cos(t), -sin(t), 0, 0,
			sin(t), cos(t), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1};
	return Matrix<>(4, 4, T);
}

// rotation(c, s) - поворот на угол, косинус и синус которого пропорциональны величинам c и s;
Matrix<> RotationX(double c, double s)
{
	double d = 1.0 / sqrt(c * c + s * s);
	double T[16] = {
			1, 0, 0, 0,
			0, c * d, -s * d, 0,
			0, s * d, c * d, 0,
			0, 0, 0, 1 };
	return Matrix<>(4,4, T);
}

Matrix<> RotationY(double c, double s)
{
	double d = 1.0 / sqrt(c * c + s * s);
	double T[16] = {
			c * d, 0, s * d, 0,
			0, 1, 0, 0,
			-s * d, 0, c * d, 0,
			0, 0, 0, 1 };
	return Matrix<>(4,4, T);
}

Matrix<> RotationZ(double c, double s)
{
	double d = 1.0 / sqrt(c * c + s * s);
	double T[16] = {
			c * d, -s * d, 0, 0,
			s * d, c * d, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1 };
	return Matrix<>(4,4, T);
}

Matrix<> Scaling(double kx, double ky, double kz)
{
	double T[16] = {
			kx, 0, 0, 0,
			0, ky, 0, 0,
			0, 0, kz, 0,
			0, 0, 0, 1 };
	return Matrix<>(4,4, T);
}

Matrix<> Mapping(double x, double y, double z)
{
	double T[16] = {
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1};
	return Matrix<>(4, 4, T);
}
	
#endif AFFINE_TRANSFORM_H
