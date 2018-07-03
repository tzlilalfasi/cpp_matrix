#pragma once
typedef struct Object {
	double data;
	int i, j;
	struct Object *NextR;
	struct Object *NextC;
} Object;

typedef struct Table {
	Object **arrR;
	Object **arrC;
	int row, col;
	int totalR, totalC; // the current amount of row,col
}Table;

class SMatrix {
private:
	Table *t;

	// delete each node from end to start 
	void freeList(Object *head);
	
	// creats an object (node)
	Object * createObj(double data, int i, int j);

	void addToList(Object *& headI, Object *& headJ, double data, int i, int j);

	// function that expand/shrink the arrR array
	void newI(int i, int getSize);

	// function that expand/shrink the arrC array
	void newJ(int j, int getSize);

	// deletes a node from the list
	void deleteNumR(Object *& headI, Object *& headJ, double data);

	// raise a matrix to the power of 2/ multiply two matrices
	SMatrix secPower(SMatrix & current, SMatrix & other);

	// copy "this" matrix to "seconedThis"
	void copy(SMatrix & seconedThis, SMatrix * thisMatrix);

	void swap(int &a, int &b);

	// swaps the node's NextR and NextC pointers and the node's i,j indexes
	void swapElement(Object *& head);

	// for each list, the function will go over each node and swap its pointers and i,j indexes
	void swapEach(Object *& head);

	// checks if a node exist in a list , when toChange=true ,it'll change it to 'value' in case it exist
	double isExist(Object * head, int i, int j, bool toChange, double value);

public:
	// construct a 'rows X cols' matrix.
	SMatrix(int rows, int cols);

	int getRows(); 

	int getCols();

	int getTotalR();

	int getTotalC();

	// set the (i,j) element to be 'data'
	void setElement(int i, int j, double data);

	// return the (i,j) element
	double getElement(int i, int j);

	// add 'this' to 'other' and put the result in 'result'
	void add(SMatrix& other, SMatrix& result);

	// add 'this'^power and put the result in 'result'
	void pow(int power, SMatrix& result);

	// transpose 'this'
	void transpose();

	// print the contents of this matrix on the screen.
	void print();

	// print the row-indexes of the column linked lists.
	void printColumnsIndexes();

	// print the column-indexes of the row linked lists.
	void printRowsIndexes();

	// destroy this matrix.
	~SMatrix();
};
