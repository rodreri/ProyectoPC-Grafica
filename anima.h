#pragma once
#include<stdio.h>


class anima
{
public:
	//poner max animacion
	anima();
	void animacion();
	void resetall();
	void setMaxIndex(int max);
	//Funcion de Interpolacion general
	void interpolation(int playIndex);

	~anima();

	#define MAX_FRAMES 50		//maximo de frames
	int i_max_steps;		//tiempo de la animacion en pasos
	int i_curr_steps;		//ira avanzando en la animacion
	int FrameIndex;			//Maximo KeyFrame sol
	bool play;			//Variable para detener o iniciar animacion de sol
	int playIndex;
	float X;		// Variable para PosicionX
	float Y;		// Variable para PosicionY
	float Z;		// Variable para PosicionZ
	float GX;		// Variable para Giro
	float GY;		// Variable para Giro
	float GZ;		// Variable para Giro
	//Definiendo una estructura FRAME con movimiento en X , Y , Z y giros
	typedef struct _frame
	{
		// Variables para guardar los Key Frames
		float X;		// Variable para PosicionX
		float Y;		// Variable para PosicionY
		float Z;		// Variable para PosicionZ
		float GX;		// Variable para Giro
		float GY;		// Variable para Giro
		float GZ;		// Variable para Giro
		float XPlus;	// Variable para IncrementoX
		float YPlus;	// Variable para IncrementoY
		float ZPlus;	// Variable para IncrementoZ
		float GXPlus;	// Variable para IncrementoG
		float GYPlus;	// Variable para IncrementoG
		float GZPlus;	// Variable para IncrementoG
	}FRAME;

	FRAME KeyFrame[MAX_FRAMES];
};

