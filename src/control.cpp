#include <string>
#include <sstream>
#include <cmath>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "common.h"
#include "matrix.h"
#include "mesh.h"
#include "terrain.h"
#include "terrainproxy.h"

#include "control.h"
#include "util.h"


Control::Control(fptype x, fptype y, fptype z)
    : position(Point(x, y, z))
{
}

Control::Control(Point& p)
    : position(p)
{
}

std::string Control::genID() const
{
    std::ostringstream ostr;
    ostr << position.genID();
    return std::string(md5sum(ostr.str().c_str()));
}

int Control::reset()
{
    TerrainProxy& terrainProxy = TerrainProxy::instance();
    Mesh* mesh = terrainProxy.getMesh();
    position.z = mesh->calcZ(position.x, position.y);
	return 0;
}

void Control::draw() const
{
    int i;
    GLfloat offset = (GLfloat)tan(PI/6)/2;

    glPushMatrix();
    glTranslatef((GLfloat)position.x, (GLfloat)position.y, (GLfloat)position.z);

    glShadeModel(GL_FLAT);
    glDisable(GL_CULL_FACE);

    for (i = 0; i < 3; i++) 
    {
	glPushMatrix();

	glScalef(0.3, 0.3, 0.3);
	glRotatef(i*120, 0.0, 0.0, 1.0);
	glTranslatef(-0.5, -offset, 0.0);

	// draw one face of control flag
	glBegin(GL_TRIANGLES);

	glColor3f(1.0, 1.0, 1.0);   // white

	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(0.0, 0.0, 1.0);

	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(1.0, 0.0, 1.0);

	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);

	glColor3f(1.0, 0.0, 0.0);   // red

	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(1.0, 0.0, 1.0);

	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);

	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);

	glEnd();
	
	glPopMatrix();
    }

    glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);

    glPopMatrix();

#ifdef CHECK_GL_ERROR
    checkGLError();
#endif
}

std::ostream& Control::toXML(std::ostream& ostr, int indent) const
{
    position.toXML(ostr, indent);
    return ostr;
}

std::ostream& operator<<(std::ostream& ostr, Control& control)
{
    ostr << control.position;
    return ostr;
}

std::istream& operator>>(std::istream& istr, Control& control)
{
    istr >> control.position;
    return istr;
}
