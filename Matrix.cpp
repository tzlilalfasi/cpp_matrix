#include "Matrix.h"
#include <iostream>
using namespace std;

Matrix::~Matrix()
{
	for (int i = 0; i < row; ++i)
		delete[] _array[i];
	delete[] _array;
}

int Matrix::getRows()
{
	return row; 
}

int Matrix::getCols()
{
	return col;
}

Matrix::Matrix(int rows, int cols)
{
	row = rows;
	col = cols; 
	_array = new double*[row]; 
	for (int i = 0; i < row; i++)
			_array[i]= new double[col]();

	for (int i = 0; i < row; i++) 
	{
		for (int j = 0; j < col; j++)
			_array[i][j] = 0;
	}
}

void Matrix::setElement(int i, int j, double data)
{
	_array[i][j] = data;
}

double Matrix::getElement(int i, int j)
{
	return _array[i][j];
}

void Matrix::add(Matrix & other, Matrix & result)
{
	if (row == other.row && col == other.col)  //if they are in the same order
	{
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
				result._array[i][j] = _array[i][j] + other._array[i][j];
		}
	}
	else 
		exit(-1);
}

Matrix Matrix::secPower(Matrix current, Matrix other) //a function to raise to the power of 2
{
	Matrix r(row, col);

	for (int i = 0; i < row; ++i)
		for (int j = 0; j < row; ++j)
			for (int k = 0; k < row; ++k)
			{
				r._array[i][j] += current._array[i][k] * other._array[k][j];
			}
	return r;
}

void Matrix::copy(Matrix & seconedThis, Matrix * thisMatrix)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
			seconedThis._array[i][j] = thisMatrix->getElement(i, j);
	}
}

void Matrix::pow(int power, Matrix & result)
{
	if (row != col)
	{
		Matrix r(result.getRows(), result.getCols());
		result = r; 
		return; 
	}
	if (power == 1)
		return;

	Matrix seconedThis(row, col); //creating a duplicate of "this" ,so it can be used in secPower function
	copy(seconedThis, this);

	result =secPower(seconedThis, seconedThis); //"this" is raised by power of 2
	power -=  2;

	while (power > 0)
	{
		if (power == 1) 
			result = secPower(seconedThis, result);

		else
			result = secPower(result, result);
		power -= 2;
	}
}

void Matrix::transpose()
{
	int temp;
	Matrix newM(col, row); ///////
	for (int i = 0; i < row; i++) 
	{
		for (int j = 0; j < col; j++)
			newM._array[j][i] = _array[i][j];
	}
	delete[] _array;
	temp = row;
	row = col;
	col = temp;
	_array = newM._array;
}

void Matrix::print()
{
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (j == col - 1)  //to print w/o comma
				cout << _array[i][j];
			else
				cout << _array[i][j] << ",";
		}
		cout << "\n";
	}
}
