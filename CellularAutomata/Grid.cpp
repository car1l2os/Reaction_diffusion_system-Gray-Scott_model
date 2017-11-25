#include "Grid.h"
#include <iostream>
#include <SDL.h>

int refrescos = 0;

/*Esta clase se va a encargar de comprobar
vecinos y  otros metodos   dentro de la matriz
que quedaran enmascarados */


Grid::Grid(int definition, int* cellSize)
{
	matrix.clear();
	this->definition = definition;
	this->cellSize = cellSize;

	for (int k = 0, i = 0, j = 0;k < definition*definition;k++, i++)
	{
		if (k != 0 && k%definition == 0)
		{
			j++;
			i = 0;
		}
		matrix.push_back(new Cell((i*(*cellSize)), *cellSize * j, cellSize, cellSize, 0.0f));
	}
}
Grid::Grid(const int* canvasSide, bool d2)
{
	if (d2)
	{
		// Set up sizes. (HEIGHT x WIDTH)
		matrix_2D.resize(*canvasSide);
		matrix_2D_aux.resize(*canvasSide);
		for (int i = 0; i < *canvasSide; ++i)
		{
			matrix_2D[i].resize(*canvasSide);
			matrix_2D_aux[i].resize(*canvasSide);
		}
	
		for (int x = 0;x < 200; x++)
		{
			for (int y = 0;y < 200;y++)
			{
				if (x > 50 && x < 110 && y>50 && y < 110)
				{
					matrix_2D[x][y] = new Cell(0.0f, 1.0f, x, y);
					matrix_2D_aux[x][y] = new Cell(0.0f, 1.0f, x, y);
				}
				else
				{
					matrix_2D[x][y] = new Cell(1.0f, 0.0f, x, y);
					matrix_2D_aux[x][y] = new Cell(1.0f, 0.0f, x, y);
				}
			}
		}
		definition = 200;
	}

	else
	{
		matrix.clear(); //No se si tengo que hacer esto.
		definition = *canvasSide;
		for (int y = 0; y < *canvasSide;y++) //filas
		{
			for (int x = 0; x<*canvasSide;x++) //columnas
			{
				if (x > 80 && x < 120 && y>90 && y < 115)
				{
					matrix.push_back(new Cell(1.0f, 1.0f,x,y));
					matrixAuxA.push_back(1.0f);
					matrixAuxB.push_back(1.0f);

					//next.push_back(new Cell(0.0f,1.0f, x, y));
				}
				else
				{
					matrix.push_back(new Cell(1.0f,0.0f,x,y));
					matrixAuxA.push_back(1.0f);
					matrixAuxB.push_back(0.0f);

					//next.push_back(new Cell(1.0f, 0.0f, x, y));
				}
			}
		}
	}
}

Cell* Grid::identifyCellByPos(int x, int y)
{
		return matrix[((y / *cellSize) * definition) + (x / *cellSize)];
}

void Grid::GetMatrixValues()
{
	matrixAux.clear();

	for (int i = 0;i < matrix.size();i++)
	{
		matrixAux.push_back(matrix.at(i)->getValue());
	}
}
void Grid::SetMatrixValues()
{
	if (matrix.size() == matrixAux.size())
	{
		for (int i = 0;i < matrixAux.size();i++)
		{
			matrix.at(i)->setValue(matrixAux[i]);
		}
	}
}

void Grid::SetMatrixAValues() //Si quito las comprobaciones dentro del for funciona mal pero al menos se mueve
{
	//if (matrix.size() == matrixAuxA.size())
	{
		for (int i = 0; i < matrixAuxA.size(); i++)
		{
			/*if (matrixAux[i] > 1.0f)
				matrix.at(i)->setA(1.0f);
			else if(matrixAux[i] < 0.0f)
				matrix.at(i)->setA(0.0f);
			else*/
			matrix.at(i)->setA(matrixAuxA[i]);
		}
	}
}
void Grid::SetMatrixBValues() //Si quito las comprobaciones dentro del for funciona mal pero al menos se mueve
{
//	if (matrix.size() == matrixAuxB.size())
	{
		for (int i = 0; i < matrixAuxB.size(); i++)
		{
			/*if (matrixAuxB[i] > 1.0f)
				matrix.at(i)->setB(1.0f);
			else if(matrixAux[i] < 0.0f)
				matrix.at(i)->setB(0.0f);
			else*/
				matrix.at(i)->setB(matrixAuxB[i]);
		}
	}
}
void Grid::GetAValues()
{
	//matrixAuxA.clear();

	if (matrixAuxA.size() == 0)
	{
		for (int i = 0;i < matrix.size();i++)
		{
			matrixAuxA.push_back(matrix.at(i)->getA());
		}
	}
	else
	{
		for (int i = 0;i < matrix.size();i++)
		{
			matrixAuxA[i] = matrix.at(i)->getA();
		}
	}
}
void Grid::GetBValues() 
{
	//matrixAuxB.clear();

	if (matrixAuxB.size() == 0)
	{
		for (int i = 0;i < matrix.size();i++)
		{
			matrixAuxB.push_back(matrix.at(i)->getA());
		}
	}
	else
	{
		for (int i = 0;i < matrix.size();i++)
		{
			matrixAuxB[i] = matrix.at(i)->getA();
		}
	}
}
void Grid::SetAValues()
{
	if (matrix.size() == matrixAuxA.size())
	{
		for (int i = 0;i < matrixAuxA.size();i++)
		{
			matrix.at(i)->setA(matrixAuxA[i]);
		}
	}
}
void Grid::SetBValues()
{
	if (matrix.size() == matrixAuxB.size())
	{
		for (int i = 0;i < matrixAuxB.size();i++)
		{
			matrix.at(i)->setB(matrixAuxB[i]);
		}
	}
}

float Grid::LaplaceA(Cell* currentCell, int index)
{
	float sumA = 0;
	sumA += (currentCell->getA()) * -1;

	std::vector<Cell*> neighbours = GetToroidalNeighbours(currentCell, index);

	for (int i = 0; i < neighbours.size(); i++)
	{
		if (neighbours.at(i)->getX() == currentCell->getX() || neighbours.at(i)->getY() == currentCell->getY())
			sumA += neighbours.at(i)->getA()*0.2f;
		else
			sumA += neighbours.at(i)->getA()*0.05f;
	}

	return sumA;
}
float Grid::LaplaceA(int x,int y)
{
	float sumA = 0;
	sumA += (matrix_2D[x][y]->valueA) * -1;

	std::vector<Cell*> neighbours = GetToroidalNeighbours(x,y);

	for (int i = 0; i < neighbours.size(); i++)
	{
		if (neighbours[i]->x == matrix_2D[x][y]->x || neighbours[i]->y == matrix_2D[x][y]->y)
			sumA += neighbours[i]->valueA *0.2f;
		else
			sumA += neighbours[i]->valueA *0.05f;
	}

	//if (abs(sumA) < 0.0001) return 0;
	return sumA;
}
float Grid::LaplaceB(Cell* currentCell, int index)
{
	float sumB = 0;
	sumB += (currentCell->getB()) *-1;

	std::vector<Cell*> neighbours = GetToroidalNeighbours(currentCell, index);

	for (int i = 0; i < neighbours.size(); i++)
	{
		if (neighbours.at(i)->getX() == currentCell->getX() || neighbours.at(i)->getY() == currentCell->getY())
			sumB += neighbours.at(i)->getB()*0.2f;
		else
			sumB += neighbours.at(i)->getB()*0.05f;
	}

	return sumB;
}
float Grid::LaplaceB(int x, int y){

	float sumB = 0;
	sumB += (matrix_2D[x][y]->valueB) * -1;

	std::vector<Cell*> neighbours = GetToroidalNeighbours(x, y);

	for (int i = 0; i < neighbours.size(); i++)
	{
		if (neighbours[i]->x == matrix_2D[x][y]->x || neighbours[i]->y == matrix_2D[x][y]->y)
			sumB += neighbours[i]->valueB *0.2f;
		else
			sumB += neighbours[i]->valueB *0.05f;
	}

	//if (std::abs(sumB) < 0.0001) return 0;
	return sumB;

}

std::string Grid::saveState()
{
	std::string saveString("#DEFINITION$" + std::to_string(definition) + "#CELLSIZE$" + std::to_string(*cellSize) + "#VALUES$");

	for (int i = 0;i < matrix.size();i++)
	{
		saveString = saveString + std::to_string(matrix[i]->getValue()) + ";";
	}

	saveString = saveString + "#";

	return saveString;
}
void Grid::chargeState(std::string inString, int* cellSizeDirection)
{

	int n_definition = -1;
	int n_cellSize = -1;

	//std::vector<float> n_values;
	int valuesIndex = 0;
	std::string readed("");

	for (std::string::size_type i = 0; i < inString.size();i++)
	{
		std::cout << i << std::endl;

		if (inString[i] == '$') //Star reading value
		{
			i++; //Pasamos a los valores 

			if (readed == "DEFINITION") //Almacenar  en ultima posicion del vector 
			{
				readed = "";
				while (inString[i] != '#') //Hasta que no encuentres otro parametro a leer	
				{
					readed += inString[i];
					i++;
				}
				n_definition = atoi(readed.c_str());
				readed = "";
			}
			else if (readed == "VALUES") //Almacenar en un vector 
			{
				readed = "";
				while (inString[i] != '#') //Hasta que no encuentres otro parametro a leer	
				{
					while (inString[i] != ';')
					{
						readed += inString[i];
						i++;
					}
					matrixAux.push_back(std::stof(readed));
					valuesIndex++;
					readed = "";
					i++;
				}
			}
			else if (readed == "CELLSIZE")
			{
				readed = "";
				while (inString[i] != '#') //Hasta que no encuentres otro parametro a leer	
				{
					readed += inString[i];
					i++;
				}
				n_cellSize = atoi(readed.c_str());
				readed = "";
			}
		}
		else if (inString[i] == '#') //Change parameter
		{
			readed = "";
		}
		/*else if (inString[i] == ';') //Change value of Cell
		{

		}*/
		else //Reading 
		{
			readed += inString[i];
		}

	}

	std::cout << "SALE DEL FOR" << std::endl;

	*cellSizeDirection = n_cellSize;
	definition = n_definition;
	SetMatrixValues();
	matrixAux.clear();



	//return new Grid(n_definition, cellSizeDirection, n_values);
}

std::vector<Cell*> Grid::GetNeighbours(Cell * cell, int cellIndex)
{
	std::vector<Cell*> neighbours;

	if ((cell->getY() / *(cell->getHeight())) == 0) //Franja superior
	{
		if ((cell->getX() / *(cell->getWidth())) == 0) //Esquina superior izquierda
		{
			neighbours.push_back(matrix.at(1));
			neighbours.push_back(matrix.at(definition));
			neighbours.push_back(matrix.at(definition + 1));
		}
		else if ((cell->getX() / *(cell->getWidth())) == definition - 1) //Esquina superior derecha
		{
			neighbours.push_back(matrix.at(definition - 2));
			neighbours.push_back(matrix.at((definition * 2) - 1));
			neighbours.push_back(matrix.at((definition * 2) - 2));
		}
		else
		{
			neighbours.push_back(matrix.at(cellIndex - 1));
			neighbours.push_back(matrix.at(cellIndex + 1));

			neighbours.push_back(matrix.at(cellIndex + definition - 1));
			neighbours.push_back(matrix.at(cellIndex + definition));
			neighbours.push_back(matrix.at(cellIndex + definition + 1));
		}
	}
	else if ((cell->getX() / *(cell->getWidth())) == 0) //Franja izquierda
	{
		if ((cell->getY() / *(cell->getHeight())) == (definition - 1)) //Esquina inferior izquierda
		{
			neighbours.push_back(matrix.at((definition*(definition - 2))));
			neighbours.push_back(matrix.at((definition*(definition - 2)) + 1));
			neighbours.push_back(matrix.at((definition*(definition - 1)) + 1));
		}
		else
		{
			neighbours.push_back(matrix.at(cellIndex - definition));
			neighbours.push_back(matrix.at(cellIndex - definition + 1));

			neighbours.push_back(matrix.at(cellIndex + 1));

			neighbours.push_back(matrix.at(cellIndex + definition));
			neighbours.push_back(matrix.at(cellIndex + definition + 1));
		}
	}
	else if ((cell->getY() / *(cell->getHeight())) == (definition - 1)) //Franja inferior
	{
		if ((cell->getX() / *(cell->getWidth())) == (definition - 1)) //Esquina inferior derecha
		{
			neighbours.push_back(matrix.at((definition*(definition - 1)) + (definition - 1) - 1)); //Anterior
			neighbours.push_back(matrix.at((definition*(definition - 2)) + (definition - 1))); //Una fila por encima
			neighbours.push_back(matrix.at((definition*(definition - 2)) + (definition - 1) - 1)); //Fila por encima una columna menos 
		}
		else
		{
			neighbours.push_back(matrix.at(cellIndex - definition - 1));
			neighbours.push_back(matrix.at(cellIndex - definition));
			neighbours.push_back(matrix.at(cellIndex - definition + 1));

			neighbours.push_back(matrix.at(cellIndex - 1));
			neighbours.push_back(matrix.at(cellIndex + 1));




		}
	}
	else if ((cell->getX() / *(cell->getWidth())) == (definition - 1)) //Franja derecha
	{
		neighbours.push_back(matrix.at(cellIndex - definition - 1));
		neighbours.push_back(matrix.at(cellIndex - definition));

		neighbours.push_back(matrix.at(cellIndex - 1));

		neighbours.push_back(matrix.at(cellIndex + definition - 1));
		neighbours.push_back(matrix.at(cellIndex + definition));
	}
	else //Caso general
	{
		neighbours.push_back(matrix.at(cellIndex - definition - 1));
		neighbours.push_back(matrix.at(cellIndex - definition));
		neighbours.push_back(matrix.at(cellIndex - definition + 1));

		neighbours.push_back(matrix.at(cellIndex - 1));
		neighbours.push_back(matrix.at(cellIndex + 1));

		neighbours.push_back(matrix.at(cellIndex + definition - 1));
		neighbours.push_back(matrix.at(cellIndex + definition));
		neighbours.push_back(matrix.at(cellIndex + definition + 1));
	}


	return neighbours;
}
std::vector<Cell*> Grid::GetToroidalNeighbours(Cell * cell, int cellIndex)
{
	std::vector<Cell*> neighbours;
	//neighbours.clear();

	if (cell->getY() == 0) //Franja superior
	{
		if (cell->getX() == 0) //Esquina superior izquierda
		{
			neighbours.push_back(matrix.at(cellIndex+1));
			neighbours.push_back(matrix.at(definition));
			neighbours.push_back(matrix.at(definition + 1));

			neighbours.push_back(matrix.at(definition * (definition - 1))); // Abajo izquierda
			neighbours.push_back(matrix.at((definition * (definition - 1)) + 1));


			neighbours.push_back(matrix.at((definition * definition)-1));//Abajo derecha esquina



			neighbours.push_back(matrix.at( definition-1));//Arriba derecha
			neighbours.push_back(matrix.at((definition * 2) - 1));
		}
		else if (cell->getX() == definition - 1) //Esquina superior derecha
		{
			neighbours.push_back(matrix.at(cellIndex-1));
			neighbours.push_back(matrix.at((definition * 2) - 1));
			neighbours.push_back(matrix.at((definition * 2) - 2));

			neighbours.push_back(matrix.at(0));//Superior izquierda
			neighbours.push_back(matrix.at(definition));

			neighbours.push_back(matrix.at(definition*(definition-1)));//Inferior izquierda


			neighbours.push_back(matrix.at((definition*definition)-1));//Inferior derecha
			neighbours.push_back(matrix.at((definition*definition) - 2));
		}
		else
		{
			neighbours.push_back(matrix.at(cellIndex - 1));
			neighbours.push_back(matrix.at(cellIndex + 1));

			neighbours.push_back(matrix.at(cellIndex + definition - 1));
			neighbours.push_back(matrix.at(cellIndex + definition));
			neighbours.push_back(matrix.at(cellIndex + definition + 1));


			//Toroidal
			neighbours.push_back(matrix.at((definition*(definition - 1)) + cellIndex - 1));
			neighbours.push_back(matrix.at(	(definition*(definition-1)) + cellIndex));
			neighbours.push_back(matrix.at((definition*(definition - 1)) + cellIndex + 1));



		}
	}
	else if (cell->getX() == 0) //Franja izquierda
	{
		if (cell->getY() == (definition - 1)) //Esquina inferior izquierda
		{
			neighbours.push_back(matrix.at((definition*(definition - 2))));
			neighbours.push_back(matrix.at((definition*(definition - 2)) + 1));
			neighbours.push_back(matrix.at((definition*(definition - 1)) + 1));

			neighbours.push_back(matrix.at(0));//Superior izquierda
			neighbours.push_back(matrix.at(1));

			neighbours.push_back(matrix.at(cellIndex - 1));//Inferior derecha
			neighbours.push_back(matrix.at(cellIndex + definition - 1));

			neighbours.push_back(matrix.at(definition - 1));//Superior derecha

		}
		else
		{
			neighbours.push_back(matrix.at(cellIndex - definition));
			neighbours.push_back(matrix.at(cellIndex - definition + 1));

			neighbours.push_back(matrix.at(cellIndex + 1));

			neighbours.push_back(matrix.at(cellIndex + definition));
			neighbours.push_back(matrix.at(cellIndex + definition + 1));

			//Toroidal
			neighbours.push_back(matrix.at(cellIndex - 1));
			neighbours.push_back(matrix.at(cellIndex + definition - 1));
			neighbours.push_back(matrix.at(cellIndex + (2 *definition)-1));
		}
	}
	else if (cell->getY() == (definition - 1)) //Franja inferior
	{
		if (cell->getX() == (definition - 1)) //Esquina inferior derecha
		{
			neighbours.push_back(matrix.at((definition*(definition - 1)) + (definition - 1) - 1)); //Anterior
			neighbours.push_back(matrix.at((definition*(definition - 2)) + (definition - 1))); //Una fila por encima
			neighbours.push_back(matrix.at((definition*(definition - 2)) + (definition - 1) - 1)); //Fila por encima una columna menos 

			neighbours.push_back(matrix.at(0)); //Superior izquierda
			

			neighbours.push_back(matrix.at(definition* (definition - 1)));//Inferior izquierda
			neighbours.push_back(matrix.at(definition* (definition - 2)));


			neighbours.push_back(matrix.at(definition - 1));//Superior derecha
			neighbours.push_back(matrix.at(definition - 2));


		}
		else
		{
			neighbours.push_back(matrix.at(cellIndex - definition - 1));
			neighbours.push_back(matrix.at(cellIndex - definition));
			neighbours.push_back(matrix.at(cellIndex - definition + 1));

			neighbours.push_back(matrix.at(cellIndex - 1));
			neighbours.push_back(matrix.at(cellIndex + 1));

			//Toroidal
			neighbours.push_back(matrix.at(cellIndex - (definition*(definition - 1))-1));
			neighbours.push_back(matrix.at(cellIndex - (definition*(definition - 1))));
			neighbours.push_back(matrix.at(cellIndex - (definition*(definition - 1))+1));
		}
	}
	else if (cell->getX() == (definition - 1)) //Franja derecha
	{
		neighbours.push_back(matrix.at(cellIndex - definition - 1));
		neighbours.push_back(matrix.at(cellIndex - definition));

		neighbours.push_back(matrix.at(cellIndex - 1));

		neighbours.push_back(matrix.at(cellIndex + definition - 1));
		neighbours.push_back(matrix.at(cellIndex + definition));


		//Toroidal
		neighbours.push_back(matrix.at(cellIndex - (definition*2) + 1));
		neighbours.push_back(matrix.at(cellIndex - (definition-1)));
		neighbours.push_back(matrix.at(cellIndex+1));

	}
	else //Caso general
	{
		neighbours.push_back(matrix.at(cellIndex - definition - 1));
		neighbours.push_back(matrix.at(cellIndex - definition));
		neighbours.push_back(matrix.at(cellIndex - definition + 1));

		neighbours.push_back(matrix.at(cellIndex - 1));
		neighbours.push_back(matrix.at(cellIndex + 1));

		neighbours.push_back(matrix.at(cellIndex + definition - 1));
		neighbours.push_back(matrix.at(cellIndex + definition));
		neighbours.push_back(matrix.at(cellIndex + definition + 1));
	}

	//Debug por consola 
	/*printf(std::to_string(cellIndex).c_str());
	printf("\n");
	for (int i = 0;i < 8;i++)
	{
		printf(std::to_string(neighbours.at(i)->getX()).c_str());
		printf("   ");
		printf(std::to_string(neighbours.at(i)->getY()).c_str());
		printf("\n");
	}*/

	return neighbours;
}
std::vector<Cell*> Grid::GetToroidalNeighbours(int x, int y)
{
	std::vector<Cell*> neighbours;
	//neighbours.clear();

	if (y == 0) //Franja superior
	{
		if (x == 0) //Esquina superior izquierda
		{
			neighbours.push_back(matrix_2D[199][199]);
			neighbours.push_back(matrix_2D[0][199]);
			neighbours.push_back(matrix_2D[1][199]);

			neighbours.push_back(matrix_2D[199][0]);
			neighbours.push_back(matrix_2D[1][0]);

			neighbours.push_back(matrix_2D[199][1]);
			neighbours.push_back(matrix_2D[0][1]);
			neighbours.push_back(matrix_2D[1][1]);
		}
		else if (x == definition - 1) //Esquina superior derecha
		{
			neighbours.push_back(matrix_2D[198][199]);
			neighbours.push_back(matrix_2D[199][199]);
			neighbours.push_back(matrix_2D[0][199]);

			neighbours.push_back(matrix_2D[198][0]);
			neighbours.push_back(matrix_2D[0][0]);

			neighbours.push_back(matrix_2D[198][1]);
			neighbours.push_back(matrix_2D[199][1]);
			neighbours.push_back(matrix_2D[0][1]);
		}
		else
		{
			neighbours.push_back(matrix_2D[x-1][199]);
			neighbours.push_back(matrix_2D[x][199]);
			neighbours.push_back(matrix_2D[x+1][199]);

			neighbours.push_back(matrix_2D[x-1][y]);
			neighbours.push_back(matrix_2D[x+1][y]);

			neighbours.push_back(matrix_2D[x-1][1]);
			neighbours.push_back(matrix_2D[x][1]);
			neighbours.push_back(matrix_2D[x+1][1]);
		}
	}
	else if (x == 0) //Franja izquierda
	{
		if (y == (definition - 1)) //Esquina inferior izquierda
		{
			neighbours.push_back(matrix_2D[199][198]);
			neighbours.push_back(matrix_2D[x][198]);
			neighbours.push_back(matrix_2D[x + 1][198]);

			neighbours.push_back(matrix_2D[199][199]);
			neighbours.push_back(matrix_2D[x + 1][199]);

			neighbours.push_back(matrix_2D[199][0]);
			neighbours.push_back(matrix_2D[x][0]);
			neighbours.push_back(matrix_2D[x + 1][0]);
		}
		else
		{
			neighbours.push_back(matrix_2D[199][y-1]);
			neighbours.push_back(matrix_2D[x][y - 1]);
			neighbours.push_back(matrix_2D[x + 1][y - 1]);

			neighbours.push_back(matrix_2D[199][y]);
			neighbours.push_back(matrix_2D[x + 1][y]);

			neighbours.push_back(matrix_2D[199][y+1]);
			neighbours.push_back(matrix_2D[x][y + 1]);
			neighbours.push_back(matrix_2D[x + 1][y + 1]);
		}
	}
	else if (y == (definition - 1)) //Franja inferior
	{
		if (x == (definition - 1)) //Esquina inferior derecha
		{
			neighbours.push_back(matrix_2D[x-1][y-1]);
			neighbours.push_back(matrix_2D[x][y - 1]);
			neighbours.push_back(matrix_2D[0][y - 1]);

			neighbours.push_back(matrix_2D[x-1][y]);
			neighbours.push_back(matrix_2D[0][y]);

			neighbours.push_back(matrix_2D[x-1][0]);
			neighbours.push_back(matrix_2D[x][0]);
			neighbours.push_back(matrix_2D[0][0]);
		}
		else
		{
			neighbours.push_back(matrix_2D[x - 1][y - 1]);
			neighbours.push_back(matrix_2D[x][y - 1]);
			neighbours.push_back(matrix_2D[x+1][y - 1]);

			neighbours.push_back(matrix_2D[x - 1][y]);
			neighbours.push_back(matrix_2D[x+1][y]);

			neighbours.push_back(matrix_2D[x - 1][0]);
			neighbours.push_back(matrix_2D[x][0]);
			neighbours.push_back(matrix_2D[x+1][0]);
		}
	}
	else if (x == (definition - 1)) //Franja derecha
	{
		neighbours.push_back(matrix_2D[x - 1][y - 1]);
		neighbours.push_back(matrix_2D[x][y - 1]);
		neighbours.push_back(matrix_2D[0][y - 1]);

		neighbours.push_back(matrix_2D[x - 1][y]);
		neighbours.push_back(matrix_2D[0][y]);

		neighbours.push_back(matrix_2D[x - 1][y+1]);
		neighbours.push_back(matrix_2D[x][y+1]);
		neighbours.push_back(matrix_2D[0][y+1]);

	}
	else //Caso general
	{
		neighbours.push_back(matrix_2D[x - 1][y - 1]);
		neighbours.push_back(matrix_2D[x][y - 1]);
		neighbours.push_back(matrix_2D[x + 1][y - 1]);

		neighbours.push_back(matrix_2D[x - 1][y]);
		neighbours.push_back(matrix_2D[x+1][y]);

		neighbours.push_back(matrix_2D[x - 1][y + 1]);
		neighbours.push_back(matrix_2D[x][y + 1]);
		neighbours.push_back(matrix_2D[x+1][y + 1]);
	}

	//Debug por consola 
	/*printf(std::to_string(cellIndex).c_str());
	printf("\n");
	for (int i = 0;i < 8;i++)
	{
	printf(std::to_string(neighbours.at(i)->getX()).c_str());
	printf("   ");
	printf(std::to_string(neighbours.at(i)->getY()).c_str());
	printf("\n");
	}*/

	return neighbours;
}

void Grid::ClearMatrix()
{
	for (int i = 0; i < matrix.size(); i++)
	{
		matrix.at(i)->setValue(0.0f);
	}
}
void Grid::ChangeSimulation()
{
	if (simulation_flag == GAME_OF_LIVE)
		simulation_flag = WIRE_WORLD;
	else if (simulation_flag == WIRE_WORLD)
		simulation_flag = GAME_OF_LIVE;

	ClearMatrix();
}

void Grid::GameOfLiveSimulation()
{
	GetMatrixValues(); //Guarda valores en aux

	for (int i = 0; i < matrix.size(); i++)
	{
		std::vector<Cell*> aux = GetNeighbours(matrix.at(i), i);
		int count = 0;

		if (matrix.at(i)->getValue() == 1.0f) //Si viva
		{
			for (int j = 0; j < aux.size();j++)
			{
				if (aux.at(j)->getValue() == 1.0f)
					count++;
			}

			if (count != 2 && count != 3)
				matrixAux[i] = 0.0f; //Matada
		}
		else if (matrix.at(i)->getValue() == 0.0f) //Si muerta
		{
			for (int j = 0; j < aux.size();j++)
			{
				if (aux.at(j)->getValue() == 1.0f)
					count++;
			}

			if (count == 3)
				matrixAux[i] = 1.0f; //Nacida
		}

	}

	SetMatrixValues();
	matrixAux.clear();

	//Hay que destruir matrixAux
}
void Grid::WireWorldSimulation()
{
	GetMatrixValues();

	for (int i = 0; i < matrix.size(); i++)
	{
		if (matrix.at(i)->getValue() == 0.75)
		{
			std::vector<Cell*> aux = GetNeighbours(matrix.at(i), i);
			int count = 0;

			for (int j = 0; j < aux.size(); j++)
			{
				if (aux.at(j)->getValue() == 0.25f)
					count++;
			}

			if (count == 1 || count == 2)
				matrixAux[i] = 0.25f; //Helectron head
		}
		else if (matrix.at(i)->getValue() == 0.5 || matrix.at(i)->getValue() == 0.25) //Not != 0.0f to control extrange values
		{
			matrixAux[i] += 0.25f;
		}
	}

	SetMatrixValues();
	matrixAux.clear();
}
void Grid::ReactionDifusionSimulation_2D()
{
	float n_A, n_B;
	std::cout << refrescos << "\n";
	refrescos++;

	for (int x = 0;x < 200; x++)
	{
		for (int y = 0;y < 200;y++)
		{
			float a = matrix_2D[x][y]->valueA;
			float b = matrix_2D[x][y]->valueB;

			float laplaceA = LaplaceA( x,y);
			float laplaceB = LaplaceB( x,y);

			float n_A = a + (dA * laplaceA - a*b*b + FEED * (1 - a));
			float n_B = b + (dB * laplaceB + a*b*b - (KILL + FEED) * b);

			

			if (n_A > 1.0f || n_B > 1.0f || n_A < 0.0f || n_B < 0.0f)
			{
				float k = LaplaceB(x, y);
				float l = LaplaceA(x, y);
				std::cout << n_B << "\n";
			}

			matrix_2D_aux[x][y]->valueA = n_A;
			matrix_2D_aux[x][y]->valueB = n_B;

			if (matrix_2D_aux[x][y]->getX() > 100 && matrix_2D_aux[x][y]->getX() < 105 && matrix_2D_aux[x][y]->getY() > 100 && matrix_2D_aux[x][y]->getY() < 105)
				int c = 2;
		}
	}

	std::vector<std::vector<Cell*> > temp;

	for (int x = 0;x < 200; x++)
	{
		for (int y = 0;y < 200;y++)
		{
			if (matrix_2D[x][y]->valueA > 1 || matrix_2D[x][y]->valueA < 0 || matrix_2D[x][y]->valueB > 1 || matrix_2D[x][y]->valueB < 0)
			{
				int a = 2;
			}

			matrix_2D[x][y]->valueA = matrix_2D_aux[x][y]->valueA;
			matrix_2D[x][y]->valueB = matrix_2D_aux[x][y]->valueB;
		}
	}


}
void Grid::ReactionDifusionSimulation()
{
	//GetAValues();
	//GetBValues();

	for (int i = 0; i < matrix.size(); i++)
	{
		float a = matrix.at(i)->valueA;
		float b = matrix.at(i)->valueB;

		float laplaceA = LaplaceA(matrix.at(i), i);
		float laplaceB = LaplaceB(matrix.at(i), i);

		float n_A = a + (dA * laplaceA - (a*b*b) + FEED * (1 - a));
		float n_B = b + (dB * laplaceB + (a*b*b) - (KILL + FEED) * b);

		if (n_A > 1.0f || n_B > 1.0f || n_A < 0.0f || n_B < 0.0f)
		{
			float casda = 2.3f; //B se está pasando de 1
		}

		matrixAuxA[i] = n_A;
		matrixAuxB[i] = n_B;

		/*next[i]->setA(n_A);
		next[i]->setB(n_B);*/

		/*float n_A = next[i]->getA();
		float n_B = next[i]->getB();*/

		if (matrix.at(i)->getX()>100 && matrix.at(i)->getX()<105 && matrix.at(i)->getY()>100 && matrix.at(i)->getY()<105)
			int c = 2;
	}

	SetMatrixAValues();
	SetMatrixBValues();
	//matrixAuxA.clear();
	//matrixAuxB.clear();

	/*std::vector<Cell*> temp = matrix;
	matrix = next;
	next = matrix;*/
}
void Grid::Simulate()
{
	if (simulation_flag == GAME_OF_LIVE)
		GameOfLiveSimulation();
	else if (simulation_flag == WIRE_WORLD)
		WireWorldSimulation();
	else if (simulation_flag == REACTION_DIFFUSION)
	{
		//ReactionDifusionSimulation();
		ReactionDifusionSimulation_2D();
	}

}

void Grid::ChangeMatrixToPixel()
{
	matrix.clear();
}
int Grid::getSimulationFlag()
{
	return simulation_flag;
}
void Grid::setSimulationFlag(int value)
{
	simulation_flag = value;
}
void Grid::clickOn(int x, int y)
{
	//Entonctrar casilla y cambiar valor

	if (simulation_flag == GAME_OF_LIVE)
		identifyCellByPos(x, y)->switchIt();

	else if (simulation_flag == WIRE_WORLD)
		identifyCellByPos(x, y)->nextValue();
}
void Grid::mouseOver(int x, int y)
{
	//Encontrar casilla y resaltar sobre la que estás 
	highlightedCell = identifyCellByPos(x, y);
}