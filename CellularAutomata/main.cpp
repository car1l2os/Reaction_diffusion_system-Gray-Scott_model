//Using SDL, SDL_image, standard IO, math, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <time.h>
#include <math.h>
#include <cmath>
#include "Grid.h"

//Variables
Grid* grid;

//The timer starting  time
Uint32 start = 0;

//The timer start/stop flag
bool running = false;

//Screen dimension constants
const int SCREEN_WIDTH = 200;
const int SCREEN_HEIGHT = 200;

//Colores pantalla
int onColor = 0x00;
int offColor = 0xFF;

//Tamaño celda
int definition = 25;
int cellSize = SCREEN_WIDTH / definition;
//Viejos 
int cellWidth = SCREEN_WIDTH / 2;
int cellHeight = SCREEN_HEIGHT / 2;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture(std::string path);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Nothing to load
	return success;
}

void close()
{
	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}


void DrawCell_GameOfLive(Cell* cell, bool highlighted)
{
	//Render filled quad
	SDL_Rect fillRect = { cell->getX(), cell->getY(), *(cell->getWidth()), *(cell->getHeight()) }; //foo->bar() is the same as (*foo).bar() --> Como getWidth devuelve un puntero hay que coger el valor con *. Parentesis no necesario, puesto para claridad 
	if (highlighted)
	{
		if (cell->getValue() == 1.0f)
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x7F, 0x50, 0xFF);
		else
			SDL_SetRenderDrawColor(gRenderer, 0xAA, 0x5A, 0x20, 0xFF);


		//Otra posiblidad es recuadrar la casilla en la que estás . Hay que pintar cuadro con if/else y luego recuadrar con este if solo 
		/*//Render outlined quad
		SDL_Rect outlineRect = { cell->x, cell->y, cell->width, cell->height };
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderDrawRect(gRenderer, &outlineRect);*/
	}
	else	if (cell->getValue() == 1.0f)
	{
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
	}

	else
	{
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	}

	SDL_RenderFillRect(gRenderer, &fillRect);



}
void DrawCell_WireWorld(Cell* cell, bool highlighted)
{
	//Render filled quad
	SDL_Rect fillRect = { cell->getX(), cell->getY(), *(cell->getWidth()), *(cell->getHeight()) }; //foo->bar() is the same as (*foo).bar() --> Como getWidth devuelve un puntero hay que coger el valor con *. Parentesis no necesario, puesto para claridad 

	if (cell->getValue() == 0.0f)
	{
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	}

	else if (cell->getValue() == 0.25f)
	{
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
	}
	else if (cell->getValue() == 0.5f)
	{
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
	}
	else if (cell->getValue() == 0.75f)
	{
		SDL_SetRenderDrawColor(gRenderer, 0x66, 0x66, 0x00, 0xFF);
	}

	SDL_RenderFillRect(gRenderer, &fillRect);



	if (highlighted)
	{
		SDL_Rect outlineRect = { cell->getX(), cell->getY(), *(cell->getWidth()), *(cell->getHeight()) };
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderDrawRect(gRenderer, &outlineRect);
	}




}

void DrawCell_Diffusion(Cell* cell, int x, int y)
{
	int value = (cell->valueA - cell->valueB) * 255;

	if (value < 0 && value != -255)
		value = 0;
	if (value > 255)
		value = 255;

	SDL_SetRenderDrawColor(gRenderer, value, value, value, 0xFF);


	SDL_RenderDrawPoint(gRenderer,x,y);
}

void DrawGrid(Grid* grid)
{
	if (grid->getSimulationFlag() == grid->GAME_OF_LIVE)
	{
		for (unsigned i = 0;i < grid->matrix.size();i++) //unsigned i porque size es unsigned y así evitamos un warning(C4018)
		{
			if (grid->highlightedCell != grid->matrix.at(i))
				DrawCell_GameOfLive(grid->matrix.at(i), false);
			else
				DrawCell_GameOfLive(grid->matrix.at(i), true);
		}
	}
	else if (grid->getSimulationFlag() == grid->WIRE_WORLD)
	{
		for (unsigned i = 0; i < grid->matrix.size(); i++) //unsigned i porque size es unsigned y así evitamos un warning(C4018)
		{
			if (grid->highlightedCell != grid->matrix.at(i))
				DrawCell_WireWorld(grid->matrix.at(i), false);
			else
				DrawCell_WireWorld(grid->matrix.at(i), true);
		}
	}
	else if (grid->getSimulationFlag() == grid->REACTION_DIFFUSION)
	{
		/*int i = 0;
		for (unsigned x = 0; x < SCREEN_WIDTH; x++) //unsigned i porque size es unsigned y así evitamos un warning(C4018)
		{
			for (unsigned y = 0; y < SCREEN_WIDTH; y++)
			{
				DrawCell_Diffusion(grid->matrix.at(i),x,y);
				i++;
			}
		}*/

		/*for (int i = 0;i < grid->matrix.size();i++)
		{
			DrawCell_Diffusion(grid->matrix.at(i), grid->matrix.at(i)->getX(), grid->matrix.at(i)->getY());
		}*/

		for (int x = 0;x < 200;x++)
		{
			for (int y = 0;y < 200;y++)
			{
				DrawCell_Diffusion(grid->matrix_2D[x][y], x, y);
			}
		}
	}
}




std::string ReadFile(std::string fileName)
{
	std::ifstream inFile(fileName);
	std::string inString((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());

	return inString;
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Simulation flag 
			bool simulate = false;
			bool nextStep = false;

			//Event handler
			SDL_Event e;

			//Grid creation
			//Grid* grid = new Grid(definition, &cellSize);
			Grid* grid = new Grid(&SCREEN_WIDTH,true);

			//Start the timer
			clock_t t;
			float timeCounter = 0.0f;

			//Tomamos el tiempo
			t = clock();

			//While application is running
			while (!quit)
			{


				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}

					if (grid != NULL)
					{

						//If mouse event happened
						if (e.type == SDL_MOUSEBUTTONDOWN)
						{
							//Get mouse position
							int x, y;
							SDL_GetMouseState(&x, &y);


							grid->clickOn(x, y);
							//Pasar estas coordenadas al grid para saber en que casilla se ha pulsado
						}

						else if (e.type == SDL_MOUSEMOTION)
						{
							if (grid->getSimulationFlag() == grid->GAME_OF_LIVE || grid->getSimulationFlag() == grid->WIRE_WORLD)
							{
								int x, y;
								SDL_GetMouseState(&x, &y);

								grid->mouseOver(x, y);
								//Pasar estas coordenadas al grid para resaltar la casilla en la que estás puesto
							}
						}


						//If space bar pressed - Stop/resume simulation
						else if (e.type == SDL_KEYDOWN)
						{
							Grid* n_grid;
							//Select surfaces  based on key press
							switch (e.key.keysym.sym)
							{
							case SDLK_SPACE:
								simulate = !simulate;
								break;

							case SDLK_RIGHT:
								grid->Simulate();
								simulate = false;
								break;

							case SDLK_l: //Load state
										 //No se podían hacer las definiciones directamente en un switch

								grid->chargeState(ReadFile("saveFile.txt"), &cellSize);
								//Destruir grid antigua con el destructor

								break;

							case SDLK_c: //Change simulation
								grid->ChangeSimulation();
								break;

							case SDLK_s: //Save state
								std::ofstream saveFile;
								saveFile.open("saveFile.txt");
								saveFile << grid->saveState();
								saveFile.close();
								break;




							}
						}
					}

				}


				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				if (simulate)
				{
					//Update Grid
					t = clock() - t;
					timeCounter = (float)t / CLOCKS_PER_SEC;

					if (timeCounter >= 1.0f)
					{
						grid->Simulate();
						timeCounter = 0.0f;
						t = clock();
					}
				}
				else
				{
					timeCounter = 0.0f;
				}


				//Update screen
				DrawGrid(grid);
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
