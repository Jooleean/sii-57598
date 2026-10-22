// Mundo.cpp: implementation of the CMundo class.
//
//////////////////////////////////////////////////////////////////////
#include <fstream>
#include "Mundo.h"
#include "glut.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMundo::CMundo()
{
	Init();
}

CMundo::~CMundo()
{
	close(fd_logger);
}

void CMundo::InitGL()
{
	//Habilitamos las luces, la renderizacion y el color de los materiales
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	
	glMatrixMode(GL_PROJECTION);
	gluPerspective( 40.0, 800/600.0f, 0.1, 150);
}

void print(char *mensaje, int x, int y, float r, float g, float b)
{
	glDisable (GL_LIGHTING);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT) );

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glColor3f(r,g,b);
	glRasterPos3f(x, glutGet(GLUT_WINDOW_HEIGHT)-18-y, 0);
	int len = strlen (mensaje );
	for (int i = 0; i < len; i++) 
		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, mensaje[i] );
		
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable( GL_DEPTH_TEST );
}
void CMundo::OnDraw()
{
	//Borrado de la pantalla	
   	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Para definir el punto de vista
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
	
	gluLookAt(0.0, 0, 17,  // posicion del ojo
		0.0, 0.0, 0.0,      // hacia que punto mira  (0,0,0) 
		0.0, 1.0, 0.0);      // definimos hacia arriba (eje Y)    

	/////////////////
	///////////
	//		AQUI EMPIEZA MI DIBUJO
	char cad[100];
	sprintf(cad,"Jugador1: %d",puntos1);
	print(cad,10,0,1,1,1);
	sprintf(cad,"Jugador2: %d",puntos2);
	print(cad,650,0,1,1,1);
	int i;
	for(i=0;i<paredes.size();i++)
		paredes[i].Dibuja();

	fondo_izq.Dibuja();
	fondo_dcho.Dibuja();
	jugador1.Dibuja();
	jugador2.Dibuja();
	esfera.Dibuja();

	/////////////////
	///////////
	//		AQUI TERMINA MI DIBUJO
	////////////////////////////

	//Al final, cambiar el buffer
	glutSwapBuffers();
}

void CMundo::OnTimer(int value)
{	
	jugador1.Mueve(0.025f);
	jugador2.Mueve(0.025f);
	esfera.Mueve(0.025f);
	int i;
	for(i=0;i<paredes.size();i++)
	{
		paredes[i].Rebota(esfera);
		paredes[i].Rebota(jugador1);
		paredes[i].Rebota(jugador2);
	}

	jugador1.Rebota(esfera);
	jugador2.Rebota(esfera);
	
	if(fondo_izq.Rebota(esfera))
	{
		esfera.centro.x=0;
		esfera.centro.y=rand()/(float)RAND_MAX;
		esfera.velocidad.x=2+2*rand()/(float)RAND_MAX;
		esfera.velocidad.y=2+2*rand()/(float)RAND_MAX;
		puntos2++;
		
		std::string mensaje = "Jugador 2 marca punto \n    J1 (" + std::to_string(puntos1) + " - " + std::to_string(puntos2) + ") J2\n";
		
		write(fd_logger, mensaje.c_str(), mensaje.length());
		
	}

	if(fondo_dcho.Rebota(esfera))
	{
		esfera.centro.x=0;
		esfera.centro.y=rand()/(float)RAND_MAX;
		esfera.velocidad.x=-2-2*rand()/(float)RAND_MAX;
		esfera.velocidad.y=-2-2*rand()/(float)RAND_MAX;
		puntos1++;
		
		std::string mensaje = "Jugador 1 marca punto \n    J1 (" + std::to_string(puntos1) + " - " + std::to_string(puntos2) + ") J2\n";
		
		write(fd_logger, mensaje.c_str(), mensaje.length());
	}
	
	if (puntos1 > 2 || puntos2 > 2) // comprueba si un jugador llega a 3 puntos
	{
		int ganador = puntos1>puntos2?1:2;
		std::string mensaje = "\nGana Jugador " + std::to_string(ganador) + " por (" + std::to_string(puntos1) + " - " + std::to_string(puntos2) + ")\n";
		write(fd_logger, mensaje.c_str(), mensaje.length());
		
		close(fd_logger);
		exit(0);
	}
	

}

void CMundo::OnKeyboardDown(unsigned char key, int x, int y)
{
	switch(key)
	{
//	case 'a':jugador1.velocidad.x=-1;break;
//	case 'd':jugador1.velocidad.x=1;break;
	case 's':jugador1.velocidad.y=-4;break;
	case 'w':jugador1.velocidad.y=4;break;
	case 'l':jugador2.velocidad.y=-4;break;
	case 'o':jugador2.velocidad.y=4;break;

	}
}

void CMundo::Init()
{

fd_logger = open("tuberia_logger", O_WRONLY);

	Plano p;
//pared inferior
//la asignación de color y vértices sería más compacta desde constructor o con estructura color = r,g,b
        p.r=0.4; p.g=0.1; p.b=0.4;
	p.x1=-7;p.y1=-5;
	p.x2=7;p.y2=-5;
	paredes.push_back(p);

//superior
        p.r=0.4; p.g=0.1; p.b=0.4;
	p.x1=-7;p.y1=5;
	p.x2=7;p.y2=5;
	paredes.push_back(p);

	fondo_izq.r=0.7; fondo_izq.g=0.1; fondo_izq.b=0.7;
	fondo_izq.x1=-7;fondo_izq.y1=-5;
	fondo_izq.x2=-7;fondo_izq.y2=5;

	fondo_dcho.r=0.7; fondo_dcho.g=0.1; fondo_dcho.b=0.7;
	fondo_dcho.x1=7;fondo_dcho.y1=-5;
	fondo_dcho.x2=7;fondo_dcho.y2=5;

	//a la izq
	jugador1.r=0.2; jugador1.g=0.1; jugador1.b=0.8;
	jugador1.x1=-6;jugador1.y1=-1;
	jugador1.x2=-6;jugador1.y2=1;

	//a la dcha
	jugador2.r=0.8; jugador2.g=0.1; jugador2.b=0.2;
	jugador2.x1=6;jugador2.y1=-1;
	jugador2.x2=6;jugador2.y2=1;
}
