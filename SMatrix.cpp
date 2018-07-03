#include "SMatrix.h"
#include <iostream>
using namespace std;


SMatrix::~SMatrix()
{
	for (int i = t->totalR-1; i >= 0; i--)
		freeList(t->arrR[i]);

	delete[] t->arrR;
	delete[] t->arrC;
	delete t;
}
//========================================//
void SMatrix::freeList(Object * head) 
{
	if (head == NULL)
		return;
	freeList(head->NextR);
	delete(head);
}
//========================================//
SMatrix::SMatrix(int rows, int cols)
{
	t = (Table*)malloc(sizeof(Table));
	if (t == NULL) //in case it couldn't allocate
		exit(-1);

	t->arrR = NULL;
	t->arrC = NULL;
	t->row = rows;
	t->col = cols;
	t->totalR = 0;
	t->totalC = 0;
}
//========================================//
int SMatrix::getRows()
{
	return t->row;
}
//========================================//
int SMatrix::getCols()
{
	return t->col;
}
//========================================//
int SMatrix::getTotalR()
{
	return t->totalR;
}
//========================================//
int SMatrix::getTotalC()
{
	return t->totalC;
}
//========================================//
Object * SMatrix::createObj(double data, int i, int j)
{
	Object *p1 = (Object*)malloc(sizeof(Object));
	if (p1 == NULL) //in case it couldn't allocate
		exit(-1);
	p1->data = data;
	p1->i = i;
	p1->j = j;
	p1->NextR = NULL;
	p1->NextC = NULL;
	return p1;
}
//========================================//
void SMatrix::addToList(Object *& headI, Object *& headJ, double data, int i, int j)
{
	Object *runner1 = headI;
	Object *runner2 = headJ;

	Object *p1 = createObj(data, i, j);
	if (headI == NULL && headJ == NULL) // in case arrR[i]=NULL and arrC[j]=NULL 
	{
		headI = p1;
		headJ = p1;
	}

	else if (headI == NULL && headJ != NULL)
	{
		headI = p1;

		if (i < headJ->i)
		{
			p1->NextC = headJ;
			headJ = p1;
		}

		else if (headJ->i < i)
		{
			if (runner2->NextC != NULL)
			{
				while (i > runner2->NextC->i)
				{
					runner2 = runner2->NextC;
					if (runner2->NextC == NULL)
						break;
				}
			}
			Object *temp = runner2->NextC;
			runner2->NextC = p1;
			p1->NextC = temp;
		}
	}

	else if (headI != NULL && headJ == NULL)
	{ 
		headJ = p1;

		if (j < headI->j)
		{
			p1->NextR = headI;
			headI = p1;
		}

		else if (headI->j < j)
		{
			if (runner1->NextR != NULL)
			{
				while (j > runner1->NextR->j)
				{
					runner1 = runner1->NextR;
					if (runner1->NextR == NULL)
						break;
				}
			}
			Object *temp = runner1->NextR;
			runner1->NextR = p1;
			p1->NextR = temp;
		}
	}

	else if (headI != NULL && headJ != NULL)
	{
		if (j < headI->j)
		{
			p1->NextR = headI;
			headI = p1;
		}
		else if (headI->j < j)
		{
			if (runner1->NextR != NULL)
			{
				while (j > runner1->NextR->j)
				{
					runner1 = runner1->NextR;
					if (runner1->NextR == NULL)
						break;
				}
			}
			Object *temp = runner1->NextR;
			runner1->NextR = p1;
			p1->NextR = temp;
		}

		if (i < headJ->i)
		{
			p1->NextC = headJ;
			headJ = p1;
		}
		else if (headJ->i < i)
		{
			if (runner2->NextC != NULL)
			{
				while (i > runner2->NextC->i)
				{
					runner2 = runner2->NextC;
					if (runner2->NextC == NULL)
						break;
				}
			}
			Object *temp = runner2->NextC;
			runner2->NextC = p1;
			p1->NextC = temp;
		}

	}
}
//========================================//
void SMatrix::newI(int i, int getSize)
{
	if (getSize == 1) //when getSize=1 it'll expand the array. otherwise it'll shrink 
		i = i + 1;
	Object** newR = (Object**)malloc(sizeof(Object*)* (i));

	for (int k = 0; k < i; k++)
	{
		if (t->totalR > k)
			newR[k] = t->arrR[k];
		else //filing the rest of the array with NULLs
			newR[k] = NULL;
	}

	delete t->arrR;
	t->arrR = newR;
	t->totalR = i;
}
//========================================//
void SMatrix::newJ(int j, int getSize)
{
	if (getSize == 1) //when getSize=1 it'll expand the array. otherwise it'll shrink 
		j = j + 1;
	Object** newC = (Object**)malloc(sizeof(Object*)* (j)); // create new column
	for (int k = 0; k < j; k++)
	{
		if (t->totalC > k)
			newC[k] = t->arrC[k];
		else
			newC[k] = NULL;
	}
	delete t->arrC;
	t->arrC = newC;
	t->totalC = j;
}
//========================================//
void SMatrix::setElement(int i, int j, double data)
{
	if (i >= t->row || j >= t->col) // in order to avoid inserting values that their index is higher then the matrix order
		exit(-1);
	if (data != 0) //if its exist, then change its value , if not- add it 
	{

		if (i >= t->totalR)
			newI(i, 1); // 1 means to expand the array
		if (j >= t->totalC)
			newJ(j, 1);

		if (isExist(t->arrR[i], i, j, true, data) == data) // if the data already exist, it will change
			return;

		addToList(t->arrR[i], t->arrC[j], data, i, j); //adding to the matix 

	}
	if (data == 0) // check if its exist, if it is, delete, if not -do nothing
	{
		if (isExist(t->arrR[i], i, j, false, data) == -1)
			return;

		double value = getElement(i, j);
		deleteNumR(t->arrR[i], t->arrC[j], value);

		if ((t->arrR[i] == NULL) && (i == t->totalR - 1)) //if one row has been deleted from the end
			newI(i, -1); //-1 means to shrink the array

		if ((t->arrC[j] == NULL) && (j == t->totalC - 1)) //if one col has been deleted from the end
			newJ(j, -1);
	}
}
//========================================//
void SMatrix::deleteNumR(Object *& headI, Object *& headJ, double data)
{
	Object *p1 = headI;
	Object *p2 = headJ;

	if (headI->data == data && headJ->data == data) //8 //////check the freening 
	{
		Object *del = headI; //the object that is going to be deleted and freed
		headI = headI->NextR;

		headJ = headJ->NextC;
		delete(del);
	}

	else if (headI->data != data && headJ->data == data) // 5
	{
		while (p1->NextR->data != data)
			p1 = p1->NextR;
		//using a "temporary object" to save the other objects linked to the one that's been deleted 
		Object *temp = p1->NextR->NextR;
		Object *del1 = p1->NextR;
		p1->NextR = temp;


		headJ = headJ->NextC;
		delete(del1);
	}

	else if (headI->data == data && headJ->data != data) //3
	{
		while (p2->NextC->data != data)
			p2 = p2->NextC;
		//using a "temporary object" to save the other objects linked to the one that's been deleted 
		Object *temp = p2->NextC->NextC;
		Object *del1 = p2->NextC;
		p2->NextC = temp;


		headI = headI->NextR;
		delete(del1);
	}

	else if (headI->data != data && headJ->data != data) //2
	{
		while (p2->NextC->data != data)
			p2 = p2->NextC;
		//using a "temporary object" to save the other objects linked to the one that's been deleted 
		Object *temp1 = p2->NextC->NextC;
		Object *del1 = p2->NextC;
		p2->NextC = temp1;

		while (p1->NextR->data != data)
			p1 = p1->NextR;
		//using a "temporary object" to save the other objects linked to the one that's been deleted 
		Object *temp2 = p1->NextR->NextR;
		p1->NextR = temp2;

		delete(del1);
	}
}
//========================================//
double SMatrix::getElement(int i, int j)
{
	if (t->arrR == NULL || t->arrC == NULL)
		return 0;

	if (i >= t->totalR || j >= t->totalC) 
		return 0; 

	if (isExist(t->arrR[i], i, j, false, 0) == -1) // if the data doesnt exist
			return 0;

	return isExist(t->arrR[i], i, j, false, 0);

}
//========================================//
void SMatrix::add(SMatrix & other, SMatrix & result)
{
	if (t->row != other.getRows() && t->col != other.getCols())
		exit(-1);

	for (int i = 0; i < t->row; i++)
	{
		for (int j = 0; j < t->col; j++)
		{
			if (j >= t->totalC) //in case there are less columns then the actual columns of "this" (the same goes with i)
			{
				double data = other.getElement(i, j);
				if (data != 0)
					result.setElement(i, j, data);
			}
			else if (j >= other.getTotalC()) //in case there are less columns then the actual columns of "other"
			{
				double data = getElement(i, j);
				if (data != 0)
					result.setElement(i, j, data);
			}
			else if (i < t->totalR && i < other.getTotalR()) { // in case it's in the range of totalR and totalC
				double data = getElement(i, j) + other.getElement(i, j);
				if (data != 0)
					result.setElement(i, j, data);
			}
			else if (i >= t->totalR)
			{
				double data = other.getElement(i, j);
				if (data != 0)
					result.setElement(i, j, data);
			}
			else if (i >= other.getTotalR())
			{
				double data = getElement(i, j);
				if (data != 0)
					result.setElement(i, j, data);
			}
		}
	}
}
//========================================//
SMatrix SMatrix::secPower(SMatrix & current, SMatrix & other) //a function to raise to the power of 2
{
	SMatrix r(t->row, t->col); // r is the result matrix 

	for (int i = 0; i < t->row; ++i)
	{
		for (int j = 0; j < t->row; ++j)
		{
			double data = r.getElement(i, j);
			for (int k = 0; k < t->row; ++k)
			{
				if (j >= t->totalC)  //in case there are less columns then the actual columns of "this" (the same goes with i)
				{
					data += 0;
					if (data != 0)
						r.setElement(i, j, data);
				}
				if (i < t->totalR)  // in case i is in the range of totalR
				{
					data += current.getElement(i, k)* other.getElement(k, j);
					if (data != 0)
						r.setElement(i, j, data);
				}
				else if (i >= t->totalR)
				{
					data += 0;
					if (data != 0)
						r.setElement(i, j, data);
				}
			}
		}
	}

	return r;
}
//========================================//
void SMatrix::copy(SMatrix & seconedThis, SMatrix * thisMatrix)
{
	for (int i = 0; i < t->totalR; i++)
	{
		for (int j = 0; j < t->totalC; j++)
			seconedThis.setElement(i, j, thisMatrix->getElement(i, j));
	}
}
//========================================//
void SMatrix::pow(int power, SMatrix & result)
{
	if (t->row != t->col)
	{
		SMatrix r(result.getRows(), result.getCols());
		result = r;
		return;
	}
	if (power <= 0)
	{
		SMatrix r(result.getRows(), result.getCols());
		result = r;
		return;
	}
	if (power == 1)
		return;

	SMatrix seconedThis(t->row, t->col); //creating a duplicate of "this" ,so it can be used in secPower function
	copy(seconedThis, this);

	result = secPower(seconedThis, seconedThis); //"this" is raised by power of 2
	power -= 2; // decrementing the value of power by 2 so it can keep use "secPower" function again if needed

	while (power > 0)
	{
		if (power == 1)
			result = secPower(seconedThis, result);

		else
			result = secPower(result, result);
		power -= 2;
	}
}
//========================================//
void SMatrix::swapElement(Object *& head)
{
	Object* temp = head->NextR;
	head->NextR = head->NextC;
	head->NextC = temp;

	swap(head->i, head->j);
}
//========================================//
void SMatrix::swapEach(Object *& head)
{
	int k = head->j;
	while (k < t->totalC)
	{
		if (head != NULL) {
			if (isExist(head, head->i, head->j, false, 0) != -1) //for each node that exists, it'll swap its elements
			{
				swapElement(head);
				head = head->NextC;
			}
		}
		k++;
	}
}
//========================================//
void SMatrix::swap(int &a, int &b)
{
	int temp = a;
	a = b;
	b = temp;
}
//========================================//
void SMatrix::transpose()
{
	for (int i = 0; i < t->totalR; i++)
		swapEach(t->arrR[i]);

	Object** temp = t->arrR;
	t->arrR = t->arrC;
	t->arrC = temp;
	// check the free! 
	swap(t->totalR, t->totalC);
	swap(t->row, t->col);
}
//========================================//
double SMatrix::isExist(Object * head, int i, int j, bool toChange, double value) // if the data exist, it'll return it, value variable is to be changed
{
	while (head != NULL)
	{
		if (head->i == i && head->j == j)
		{
			if (toChange)
				head->data = value;
			return head->data;
		}
		else
			head = head->NextR;
	}
	return -1;
}
//========================================//
void SMatrix::print()
{
	for (int i = 0; i < t->row; i++)
	{
		for (int j = 0; j < t->col; j++)
		{
			if (j < t->totalC)
			{
				if (i < t->totalR)
				{
					double data = getElement(i, j);
					if (j == t->col - 1)
						cout << data;
					else
						cout << data;
				}
				else
					cout << "0";
			}
			else
				cout << "0";

			if (j + 1 < t->col)
				cout << ",";
		}
		cout << endl;
	}
}
//========================================//
void SMatrix::printColumnsIndexes()
{
	for (int j = 0; j < t->col; j++)
	{
		cout << j << ": ";
		for (int i = 0; i < t->row; i++)
		{
			double data = getElement(i, j);
			if (data != 0)
				cout << i << "->";
		}
		cout << endl;
	}
}
//========================================//
void SMatrix::printRowsIndexes()
{
	for (int i = 0; i < t->row; i++)
	{
		cout << i << ": ";
		for (int j = 0; j < t->col; j++)
		{
			double data = getElement(i, j);
			if (data != 0)
				cout << j << "->";
		}
		cout << endl;
	}
}
