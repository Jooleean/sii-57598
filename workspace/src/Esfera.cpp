// Esfera.cpp: implementation of the Esfera class.
//
//////////////////////////////////////////////////////////////////////

#include "Esfera.h"
#include "glut.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Esfera::Esfera()
{
	radio=0.5f;
	velocidad.x=3;
	velocidad.y=3;
	r = g = b = 40;
}

Esfera::~Esfera()
{

}

void Esfera::Dibuja()
{

// Cambio de color 
r = r<255?r+=1:r=0;
g = g<255?g+=2:g=0;
b = b<255?b+=3:b=0;

	glColor3ub(r,g,b);
	glEnable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(centro.x,centro.y,0);
	glutSolidSphere(radio,15,15);
	glPopMatrix();
}

void Esfera::Mueve(float t)
{
     centro = centro + velocidad*t;
}
