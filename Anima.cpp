#include "anima.h"


void anima::resetall() {
	 X=0;		// Variable para PosicionX
	 Y=0;		// Variable para PosicionY
	 Z=0;		// Variable para PosicionZ
     GX=0;		// Variable para Giro
	 GY=0;		// Variable para Giro
	 GZ=0;		// Variable para Giro
}

void anima::animacion() {
	{
		//Movimiento del objeto
		if (play)
		{
			printf("ENTRO? %i\n", play?1:2);
			printf("Estoy? %i\n", playIndex);
			printf("Max? %fi\n", FrameIndex);
			//primer interpolacion
			if (playIndex == 0 and i_curr_steps == 0)
				interpolation(playIndex);

			if (i_curr_steps >= i_max_steps) //fin de un frame
			{
				//le sumo uno al indice
				playIndex++;
				if (playIndex > FrameIndex - 1)//fin de la animacion 
				{
					play = false;
					playIndex = 0;
					resetall();
					i_curr_steps = 0;
				}
				else //Siguiente frame
				{
					i_curr_steps = 0; //Reset 
					//Interpolation
					interpolation(playIndex);

				}
			}
			else
			{
				//Animacion
				X += KeyFrame[playIndex].XPlus;
				Y += KeyFrame[playIndex].YPlus;
				Z += KeyFrame[playIndex].ZPlus;
				GX += KeyFrame[playIndex].GXPlus;
				GY += KeyFrame[playIndex].GYPlus;
				GZ += KeyFrame[playIndex].GZPlus;
				i_curr_steps++;
			}

		}
	}
}

anima::anima() {
	FrameIndex = 0;
	i_max_steps = 300;		//tiempo de la animacion en pasos
	i_curr_steps = 0;		//ira avanzando en la animacion
	FrameIndex = 0;			//Maximo KeyFrame sol
	play = true;			//Variable para detener o iniciar animacion de sol
	playIndex = 0;
}

void anima::setMaxIndex(int max) {
	FrameIndex = max;
}

void anima::interpolation(int playIndex) {
	KeyFrame[playIndex].XPlus = (KeyFrame[playIndex + 1].X - KeyFrame[playIndex].X) / i_max_steps;
	KeyFrame[playIndex].YPlus = (KeyFrame[playIndex + 1].Y - KeyFrame[playIndex].Y) / i_max_steps;
	KeyFrame[playIndex].ZPlus = (KeyFrame[playIndex + 1].Z - KeyFrame[playIndex].Z) / i_max_steps;
	KeyFrame[playIndex].GXPlus = (KeyFrame[playIndex + 1].GX - KeyFrame[playIndex].GX) / i_max_steps;
	KeyFrame[playIndex].GYPlus = (KeyFrame[playIndex + 1].GY - KeyFrame[playIndex].GY) / i_max_steps;
	KeyFrame[playIndex].GZPlus = (KeyFrame[playIndex + 1].GZ - KeyFrame[playIndex].GZ) / i_max_steps;
	printf("--------------------SOL FRAME %i--------------------------------\n\n", playIndex);
	printf("XPLUS=(%f)-(%f)\n", KeyFrame[playIndex + 1].X, KeyFrame[playIndex].X);
	printf("YPLUS=(%f)-(%f)\n", KeyFrame[playIndex + 1].Y, KeyFrame[playIndex].Y);
}

anima::~anima()
{
}


