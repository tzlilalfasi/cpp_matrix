#pragma once
class Matrix
{
private:
	double **_array;
	int row, col;
public:
	// construct a 'rows X cols' matrix.
	Matrix(int rows, int cols);

	// set the (i,j) element to be 'data'
	void setElement(int i, int j, double data);

	// return the (i,j) element
	double getElement(int i, int j);

	// add 'this' to 'other' and put the result in 'result'
	void Matrix::add(Matrix & other, Matrix & result);

	Matrix Matrix::secPower(Matrix current, Matrix other);

	void copy(Matrix &result, Matrix *copy);

	// add 'this'^power and put the result in 'result'
	void pow(int power, Matrix & result);

	// transpose 'this'
	void transpose();

	// print the contents of this matrix on the screen.
	void print();

	// destroy this matrix.
	~Matrix();

	int getRows(); 
	void setRows(int r);
	int getCols(); 
	void setCols(int c); 
};

