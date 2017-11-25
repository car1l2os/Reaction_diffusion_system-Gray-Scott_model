#pragma once

class Cell {
private:
	//Variables
	bool status; 
	float value;

	int* width;
	int* height;


public:
	//Constructor
	Cell(int x, int y, int* width, int* height, float value);
	Cell(float valueA, float valueB, int x, int y);
		
	int x, y;
	float valueA, valueB;

	//Getters 
	bool getStatus();
	float getValue();
	int getX();
	int getY();
	int* getWidth();
	int* getHeight();

	//Per pixel grid
	int getA();
	int getB();


	//Setters
	void setValue(float value);
	
	//Per pixel grid
	void setA(float value);
	void setB(float value);

	//Functions
	void Cell::switchIt();
	void Cell::nextValue();

};