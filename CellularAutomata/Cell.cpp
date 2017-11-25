#include "Cell.h"

//Const ructor
Cell::Cell(int x, int y, int* width, int* height, float value)
{
	 this->x = x;
	 this->y = y;
	 this->width = width;
	 this->height = height;
	 this->value = value;
}

Cell::Cell(float valueA, float valueB, int x, int y)
{
	this->valueA = valueA;
	this->valueB = valueB;
	this->x = x;
	this->y = y;
}

//Getters
bool Cell::getStatus() {
	return value;
}
float Cell::getValue() {
	return value;
}
int Cell::getX() {
	return x;
}
int Cell::getY() {
	return y;
}
int* Cell::getHeight() {
	return height;
}
int Cell::getA()
{
	return valueA;
}
int Cell::getB()
{
	return valueB;
}
int* Cell::getWidth() {
	return width;
}

//Setters	
void Cell::setValue(float value) {
	this->value = value;
}

void Cell::setA(float value)
{
	valueA = value;
}

void Cell::setB(float value)
{
	valueB = value;
}


//Functions
void Cell::switchIt() 
{
	if (value == 1.0f)
	{
		value = 0.0f;
	}
	else if(value == 0.0f)
	{
		value = 1.0f;
	}
}
void Cell::nextValue()
{
	if (value == 0.0f)
	{
		value = 0.25f;
	}
	else if (value == 0.25f)
	{
		value = 0.5f;
	}
	else if (value == 0.5f)
	{
		value = 0.75f;
	}
	else if (value == 0.75f)
	{
		value = 0.0f;
	}

}