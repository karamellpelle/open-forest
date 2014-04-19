#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <math.h>

#define GLEW_STATIC 1
#include <GL/glew.h>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include <stdexcept>
#include "common.h"
#include "matrix.h"
#include "mapproxy.h"
#include "mesh.h"
#include "observer.h"
#include "eventproxy.h"
#include "orienteer.h"
#include "run.h"
#include "conditions.h"
#include "oriproxy.h"
#include "database.h"
#include "mainWindow.h"
#include "shader.h"


#define USE_VERTEX_ARRAY
#if 1
#define USE_SHADERS
#endif

using std::max;
using std::min;

extern GLuint treeTextures[]; // in mainWindow.cxx
extern GLuint groundTextures[]; // in mainWindow.cxx
extern GLuint noiseTextures[]; // in mainWindow.cxx


void Coord::setVertex(fptype x, fptype y, fptype z)
{
    vertex[0] = x;
    vertex[1] = y;
    vertex[2] = z;
}

void Coord::setNormal(fptype x, fptype y, fptype z)
{
    normal[0] = x;
    normal[1] = y;
    normal[2] = z;
}

// put trees in random locations
void Mesh::initTrees(int seed)
{
    int width = getWidth();
    int height = getHeight();
    int treeDensity = 50; // smaller number = more trees
    int ntrees = width*height/treeDensity;

    myrandomSeed(seed);

    for (int i = 0; i < ntrees; i++) 
    {
	fptype x = myrandom(5, width-5);
	fptype y = myrandom(5, height-5);
	fptype z = calcZ(x, y);
	fptype treeWidth = 3.0;
	fptype treeHeight = myrandom(3, 15);
	GLuint textureId = treeTextures[(int)myrandom(0, 4)];
	Tree* tree = new Tree(textureId, x, y, z, treeWidth, treeHeight);
	Facet* facet = getFacet(x, y);
	facet->addTree(tree);
    }
}

Tree::Tree(GLuint _textureId, fptype _x, fptype _y, fptype _z, fptype _width, fptype _height) 
    : textureId(_textureId), x(_x), y(_y), z(_z), width(_width), height(_height) 
{
    texture[ 0] = 0.0; texture[ 1] = 0.0;
    texture[ 2] = 1.0; texture[ 3] = 0.0;
    texture[ 4] = 1.0; texture[ 5] = 1.0;
    texture[ 6] = 0.0; texture[ 7] = 1.0;

    texture[ 8] = 0.0; texture[ 9] = 0.0;
    texture[10] = 1.0; texture[11] = 0.0;
    texture[12] = 1.0; texture[13] = 1.0;
    texture[14] = 0.0; texture[15] = 1.0;


    normal[ 0] =  0.0; normal[ 1] = -1.0; normal[ 2] = 0.0;
    normal[ 3] =  0.0; normal[ 4] = -1.0; normal[ 5] = 0.0;
    normal[ 6] =  0.0; normal[ 7] = -1.0; normal[ 8] = 0.0;
    normal[ 9] =  0.0; normal[10] = -1.0; normal[11] = 0.0;

    normal[12] = -1.0; normal[13] =  0.0; normal[14] = 0.0;
    normal[15] = -1.0; normal[16] =  0.0; normal[17] = 0.0;
    normal[18] = -1.0; normal[19] =  0.0; normal[20] = 0.0;
    normal[21] = -1.0; normal[22] =  0.0; normal[23] = 0.0;


    vertex[ 0] = x - width/2; vertex[ 1] = y;           vertex[ 2] = z + 0.0;
    vertex[ 3] = x + width/2; vertex[ 4] = y;           vertex[ 5] = z + 0.0;
    vertex[ 6] = x + width/2; vertex[ 7] = y;           vertex[ 8] = z + height;
    vertex[ 9] = x - width/2; vertex[10] = y;           vertex[11] = z + height;

    vertex[12] = x;           vertex[13] = y - width/2; vertex[14] = z + 0.0;
    vertex[15] = x;           vertex[16] = y + width/2; vertex[17] = z + 0.0;
    vertex[18] = x;           vertex[19] = y + width/2; vertex[20] = z + height;
    vertex[21] = x;           vertex[22] = y - width/2; vertex[23] = z + height;
}

int Tree::draw()
{
#ifdef USE_VERTEX_ARRAY
    return drawVertexArray();
#else
    return drawImmediate();
#endif
}

int Tree::drawVertexArray()
{
    glBindTexture(GL_TEXTURE_2D, textureId);

    glVertexPointer(3, GL_FLOAT, 0, vertex);
    glNormalPointer(GL_FLOAT, 0, normal);
    glTexCoordPointer(2, GL_FLOAT, 0, texture);
    glDrawArrays(GL_QUADS, 0, 8);
    return 0;
}

int Tree::drawImmediate()
{
    glBindTexture(GL_TEXTURE_2D, textureId);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0);
    glNormal3f(0.0, -1.0, 0.0);
    glVertex3f(x - width/2, y, z + 0.0);

    glTexCoord2f(1.0, 0.0);
    glNormal3f(0.0, -1.0, 0.0);
    glVertex3f(x + width/2, y, z + 0.0);

    glTexCoord2f(1.0, 1.0);
    glNormal3f(0.0, -1.0, 0.0);
    glVertex3f(x + width/2, y, z + height);

    glTexCoord2f(0.0, 1.0);
    glNormal3f(0.0, -1.0, 0.0);
    glVertex3f(x - width/2, y, z + height);


    glTexCoord2f(0.0, 0.0);
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f(x, y - width/2, z + 0.0);

    glTexCoord2f(1.0, 0.0);
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f(x, y + width/2, z + 0.0);

    glTexCoord2f(1.0, 1.0);
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f(x, y + width/2, z + height);

    glTexCoord2f(0.0, 1.0);
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f(x, y - width/2, z + height);

    glEnd();
	
    return 0;
}

int Facet::size = 2;

void Facet::addTree(Tree* _tree) 
{
#if 0
    trees.push_back(_tree); 
#else
    tree = _tree;
#endif
}

void Facet::drawTrees() const
{
    // TODO: put trees with different textures into different lists
    // and render them separately, this avoids binding the texture
    // on each call
    // UPDATE: not so easy, as each facet has its own list of trees
    // and we are rendering only a subset of all facets at each time
    // It does not work to put several lists on each facet, because
    // each list would probably have at most one tree anyway.
    // Also, making global lists one per texture type does not work
    // because then we would have difficulties in determining which 
    // trees from that list to render.
    // 
#if 0
    std::for_each(trees.begin(), trees.end(), std::mem_fun(&Tree::draw));
#else
    if (tree != NULL)
	tree->draw();
#endif
}

bool Facet::triangleMarked(int triangle)
{
    return (triangle == 0) ? lowerUsed : upperUsed;
}

void Facet::markTriangle(int triangle, bool value)
{
    if (triangle == 0)
	lowerUsed = value;
    else
	upperUsed = value;
}

void Facet::clearTriangles()
{
    lowerUsed = upperUsed = false;
}

Facet::~Facet()
{
    std::for_each(trees.begin(), trees.end(), del_fun<Tree>());
}

int Mesh::density = 2;

Mesh::Mesh(int w, int h)
    : width(w), height(h)
{
    lowestPoint = FLT_MAX;
    highestPoint = FLT_MIN;
    dliMesh = 0;
    drawRadius = 10;
    coords = new Matrix<Coord>(width+1, height+1);
    facets = new Matrix<Facet>(width, height);
    vertex = NULL;
    normal = NULL;
    texture = NULL;

}

Mesh::~Mesh()
{
    freeVertexArray();

    delete facets;
    delete coords;
}

// TODO: this is not the way to do vertex arrays
// I think the base data structuring should be immediately 
// suitable for vertex arrays before it is beneficial, 
// and not like this where the data is converted into a format
// suitable for vertex arrays just before rendering
void Mesh::drawGroundVertexArray(int minx, int maxx, int miny, int maxy)
{
    static GLfloat vertex[255];
    static GLfloat normal[255];

    glColor3f(0.0, 1.0, 0.0);
    glVertexPointer(3, GL_FLOAT, 0, vertex);
    glNormalPointer(GL_FLOAT, 0, normal);
    for (int i = minx; i < maxx; i++) 
    {
	for (int j = miny; j < maxy; j++) 
	{
	    Coord* coord = getCoord(i, j);
	    vertex[6*(j-miny) + 0] = i;
	    vertex[6*(j-miny) + 1] = j;
	    vertex[6*(j-miny) + 2] = coord->vertex[2];
	    normal[6*(j-miny) + 0] = coord->normal[0];
	    normal[6*(j-miny) + 1] = coord->normal[1];
	    normal[6*(j-miny) + 2] = coord->normal[2];

	    coord = getCoord(i+1, j);
	    vertex[6*(j-miny) + 3] = i+1;
	    vertex[6*(j-miny) + 4] = j;
	    vertex[6*(j-miny) + 5] = coord->vertex[2];
	    normal[6*(j-miny) + 3] = coord->normal[0];
	    normal[6*(j-miny) + 4] = coord->normal[1];
	    normal[6*(j-miny) + 5] = coord->normal[2];

	}
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*(maxy-miny));
    }
}

void Mesh::makeVertexArray()
{
    freeVertexArray();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    vertex = new fptype*[width];
    normal = new fptype*[width];
    texture = new fptype*[width];

    for (int i = 0; i < width; i++) 
    {
	int vnamount = 6*(height+1);
	int tamount = 4*(height+1);
	vertex[i] = new fptype[vnamount];
	normal[i] = new fptype[vnamount];
	texture[i] = new fptype[tamount];
	for (int j = 0; j < height; j++) 
	{
	    Coord* coord = getCoord(i, j);
	    vertex[i][6*j + 0] = texture[i][4*j + 0] = coord->vertex[0];
	    vertex[i][6*j + 1] = texture[i][4*j + 1] = coord->vertex[1];
	    vertex[i][6*j + 2] = coord->vertex[2];

	    normal[i][6*j + 0] = coord->normal[0];
	    normal[i][6*j + 1] = coord->normal[1];
	    normal[i][6*j + 2] = coord->normal[2];

	    coord = getCoord(i+1, j);
	    vertex[i][6*j + 3] = texture[i][4*j + 2] = coord->vertex[0];
	    vertex[i][6*j + 4] = texture[i][4*j + 3] = coord->vertex[1];
	    vertex[i][6*j + 5] = coord->vertex[2];

	    normal[i][6*j + 3] = coord->normal[0];
	    normal[i][6*j + 4] = coord->normal[1];
	    normal[i][6*j + 5] = coord->normal[2];
	}
    }
}

void Mesh::freeVertexArray()
{
    if (vertex == NULL)
	return;

    for (int i = 0; i < width; i++) 
    {
	delete vertex[i];
	delete normal[i];
	delete texture[i];
    }
    delete vertex;
    delete normal;
    delete texture;
}

// draw the ground using vertex array
void Mesh::drawGroundVertexArray2(int minx, int maxx, int miny, int maxy)
{
    glColor4f(1.0, 1.0, 1.0, 1.0);
    for (int i = minx; i <= maxx; i++)
    {
	glVertexPointer(3, GL_FLOAT, 0, vertex[i]+(6*miny));
	glNormalPointer(GL_FLOAT, 0, normal[i]+(6*miny));
	glTexCoordPointer(2, GL_FLOAT, 0, texture[i]+(4*miny));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*(maxy-miny));
    }
}

// draw the ground using immmediate mode
void Mesh::drawGroundImmediate(int minx, int maxx, int miny, int maxy)
{
    // if lighting is disabled, do not specify normals
    // if texturing is disabled, do not specify texture coordinates
    // do the tests outside of the loops
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, groundTextures[0]);

    for (int i = minx; i < maxx; i++) 
    {
	glBegin(GL_TRIANGLE_STRIP);
	for (int j = miny; j < maxy; j++) 
	{
	    try 
	    {
		Coord* coord = getCoord(i, j);
		glTexCoord2f(coord->vertex[0], coord->vertex[1]);
		glNormal3fv(coord->normal);
		glVertex3fv(coord->vertex);
		
		coord = getCoord(i+1, j);
		glTexCoord2f(coord->vertex[0], coord->vertex[1]);
		glNormal3fv(coord->normal);
		glVertex3fv(coord->vertex);
	    }
	    catch (std::out_of_range&)
	    {
	    }
 	}
	glEnd();
    }

    glDisable(GL_TEXTURE_2D);
}

// draw the tree
void Mesh::drawTrees(int minx, int maxx, int miny, int maxy)
{
    // draw trees
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glDisable(GL_CULL_FACE);

    for (int i = minx; i < maxx; i++) 
    {
	for (int j = miny; j < maxy; j++) 
	{
	    try 
	    {
		Facet* facet = getFacet(i, j);
		facet->drawTrees();
	    }
	    catch (std::out_of_range&)
	    {
	    }
	}
    }

    glEnable(GL_CULL_FACE);
}

/*
 * Draw the terrain and everything that goes with it (stationary objects).
 * pos and horizontal specify the location and direction on eye.
 * This is used to draw only a visible subset the the terrain.
 * Currently very crude approximation.
 */
void Mesh::draw(Point& pos, fptype horizontal)
{
#ifdef USE_SHADERS
  drawWithShadersNight(pos, horizontal);
#else
  drawWithoutShaders(pos, horizontal);
#endif
}

void Mesh::drawDay(Point& pos, fptype horizontal)
{
  drawWithShadersDay(pos, horizontal);
}

void Mesh::drawNight(Point& pos, fptype horizontal)
{
  drawWithShadersNight(pos, horizontal);
}

void Mesh::drawWithoutShaders(Point& pos, fptype horizontal)
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GEQUAL, 0.9);

    if (dliMesh != 0) 
    {
	glCallList(dliMesh);
    }
    else 
    {
#if 1
	int drawRadius = 15;
#else
	int targetFps = 150;
	drawRadius += int((MainWindow::framesPerSecond - targetFps)/10);
#endif
	// (x,y) is the center of the area we are drawing
	int x = (int)(pos.x/Facet::size + (drawRadius-3)*cos(horizontal));
	int y = (int)(pos.y/Facet::size + (drawRadius-3)*sin(horizontal));

	// now calculate corner points of the area
	int minx = max(x - drawRadius, 0);
	int maxx = min(x + drawRadius, width-1);
	int miny = max(y - drawRadius, 0);
	int maxy = min(y + drawRadius, height-1);

    glBindTexture(GL_TEXTURE_2D, groundTextures[0]);

#ifdef USE_VERTEX_ARRAY
	drawGroundVertexArray2(minx, maxx, miny, maxy);
#else
	drawGroundImmediate(minx, maxx, miny, maxy);
#endif

	drawTrees(minx, maxx, miny, maxy);
    }    
    glDisable(GL_ALPHA_TEST);
}

void Mesh::drawWithShadersDay(Point& pos, fptype horizontal)
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GEQUAL, 0.9);

    int drawRadius = 15;
    // (x,y) is the center of the area we are drawing
    int x = (int)(pos.x/Facet::size + (drawRadius-3)*cos(horizontal));
    int y = (int)(pos.y/Facet::size + (drawRadius-3)*sin(horizontal));
    
    // now calculate corner points of the area
    int minx = max(x - drawRadius, 0);
    int maxx = min(x + drawRadius, width-1);
    int miny = max(y - drawRadius, 0);
    int maxy = min(y + drawRadius, height-1);
    glUseProgram(dayShader);
    drawGroundVertexArray2(minx, maxx, miny, maxy);
    glUseProgram(0); // disable shaders
    drawTrees(minx, maxx, miny, maxy);
    glDisable(GL_ALPHA_TEST);
}

void Mesh::drawWithShadersNight(Point& pos, fptype horizontal)
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GEQUAL, 0.9);

    int drawRadius = 15;
    // (x,y) is the center of the area we are drawing
    int x = (int)(pos.x/Facet::size + (drawRadius-3)*cos(horizontal));
    int y = (int)(pos.y/Facet::size + (drawRadius-3)*sin(horizontal));
    
    // now calculate corner points of the area
    int minx = max(x - drawRadius, 0);
    int maxx = min(x + drawRadius, width-1);
    int miny = max(y - drawRadius, 0);
    int maxy = min(y + drawRadius, height-1);
    glUseProgram(nightShader);
    drawGroundVertexArray2(minx, maxx, miny, maxy);
    glUseProgram(nightTrees);
    drawTrees(minx, maxx, miny, maxy);
    glUseProgram(0); // disable shaders
    glDisable(GL_ALPHA_TEST);
}

// draw all of the terrain
// for testing the display list approach
void Mesh::drawAll()
{
    int minx, maxx, miny, maxy;

    minx = 0;
    maxx = width-2;
    miny = 0;
    maxy = height-2;

#ifdef USE_VERTEX_ARRAY
    drawGroundVertexArray(minx, maxx, miny, maxy);
#else
    drawGroundImmediate(minx, maxx, miny, maxy);
#endif
    drawTrees(minx, maxx, miny, maxy);

#ifdef CHECK_GL_ERROR
    checkGLError();
#endif
}

void Mesh::makeDisplayList()
{
#if 0
    /* if we are using display lists, we cannot dynamically
     * adjust the area to be drawn, but we have to draw everything.
     * When not using display list we can draw just the visible 
     * area near in front of us. This seems to be faster.
     */
    if ((dliMesh = glGenLists(1)) != 0) 
    {
	glNewList(dliMesh, GL_COMPILE);
	drawAll();
	glEndList();
    }
    else 
    {
	printf("meshMakeDisplayList: can't generate display lists\n");
    }
#endif
}

Facet* Mesh::getFacet(int i, int j)
{
    return &((*facets)(i,j));
}

Facet* Mesh::getFacet(fptype x, fptype y)
{
    int i = (int)x/Facet::size;
    int j = (int)y/Facet::size;
    return getFacet(i, j);
}

Coord* Mesh::getCoord(int i, int j)
{
    return &((*coords)(i,j));
}

void Facet::calcNormal(const fptype pt1[3], const fptype pt2[3], 
		       const fptype pt3[3], fptype* result)
{
    fptype dp1[3], dp2[3];
    fptype d;

    /* calculate vector from pt1 to pt2 */
    dp1[0] = pt2[0] - pt1[0];
    dp1[1] = pt2[1] - pt1[1];
    dp1[2] = pt2[2] - pt1[2];

    /* calculate vector from pt1 to pt3 */
    dp2[0] = pt3[0] - pt1[0];
    dp2[1] = pt3[1] - pt1[1];
    dp2[2] = pt3[2] - pt1[2];

    /* calculate cross product of the two vectors */
    result[0] = dp1[1]*dp2[2] - dp1[2]*dp2[1];
    result[1] = dp1[2]*dp2[0] - dp1[0]*dp2[2];
    result[2] = dp1[0]*dp2[1] - dp1[1]*dp2[0];

    /* make the normal one unit long */
    d = 1.0 / sqrt(result[0]*result[0]+
		   result[1]*result[1]+
		   result[2]*result[2]);
    result[0] *= d;
    result[1] *= d;
    result[2] *= d;
}

void Facet::calcNormals(const Coord* coordNW, const Coord* coordNE,
			const Coord* coordSW, const Coord* coordSE)
{
    calcNormal(coordSW->vertex, coordSE->vertex, coordNW->vertex, lowerNormal);
    calcNormal(coordNE->vertex, coordNW->vertex, coordSE->vertex, upperNormal);
}

void Mesh::calculateFacetNormals()
{
    /*
     *
     *   ptNW     ptNE
     *      +-----+
     *      |\    |
     *      |  \  |
     *      |    \|
     *      +-----+
     *   ptSW     ptSE
     *
     */

    for (int i = 0; i < width; i++) 
    {
	for (int j = 0; j < height; j++) 
	{
	    try 
	    {
		const Coord* coordSW = getCoord(i,   j);
		const Coord* coordNW = getCoord(i,   j+1);
		const Coord* coordSE = getCoord(i+1, j);
		const Coord* coordNE = getCoord(i+1, j+1);
		Facet* facet = getFacet(i, j);
		facet->calcNormals(coordNW, coordNE,
				   coordSW, coordSE);
	    }
	    catch (std::out_of_range&)
	    {
		std::cout << "calculateFacetNormals std::out_of_range" << std::endl;
	    }
	}
    }
}

void Mesh::calculateVertexNormals()
{
    /* Calculate vertex normals as average of surrounding facet normals
     * There is four facets and six triangles meeting at each vertex
     *
     *     2+-----+-----+
     *      |\    |\    |
     *      |  \  |  \  |
     *      |   NW|NE  \|
     *     1+-----+-----+
     *      |\  SW|SE   |
     *      |  \  |  \  |
     *      |    \|    \|
     *     0+-----+-----+
     *      0     1     2
     *
     *
     */
    for (int i = 1; i < width; i++) 
    {
	for (int j = 1; j < height; j++) 
	{
	    try 
	    {
		Coord* coord = getCoord(i, j);
		Facet* facetSW = getFacet(i-1, j-1);
		Facet* facetNW = getFacet(i-1, j);
		Facet* facetSE = getFacet(i, j-1);
		Facet* facetNE = getFacet(i, j);
		
		// normal is an average of surrounding facets
		coord->normal[0] =
		    (2*facetSW->upperNormal[0] +
		     facetNW->lowerNormal[0] + facetNW->upperNormal[0] +
		     facetSE->lowerNormal[0] + facetSE->upperNormal[0] +
		     2*facetNE->lowerNormal[0])/8;
		coord->normal[1] =
		    (2*facetSW->upperNormal[1] +
		     facetNW->lowerNormal[1] + facetNW->upperNormal[1] +
		     facetSE->lowerNormal[1] + facetSE->upperNormal[1] +
		     2*facetNE->lowerNormal[1])/8;
		coord->normal[2] =
		    (2*facetSW->upperNormal[2] +
		     facetNW->lowerNormal[2] + facetNW->upperNormal[2] +
		     facetSE->lowerNormal[2] + facetSE->upperNormal[2] +
		     2*facetNE->lowerNormal[2])/8;
		
		// make the normal one unit long
		fptype d = 1.0 / sqrt(coord->normal[0]*coord->normal[0]+
				      coord->normal[1]*coord->normal[1]+
				      coord->normal[2]*coord->normal[2]);
		coord->normal[0] *= d;
		coord->normal[1] *= d;
		coord->normal[2] *= d;
	    }
	    catch (std::out_of_range&)
	    {
		std::cout << "calculateVertexNormals std::out_of_range" << std::endl;
	    }
	}
    }
}

void Mesh::calcNormals()
{
    calculateFacetNormals();
    calculateVertexNormals();
}

/*
 * Return the height (z-coord) at (x,y)
 *
 * The equation of a plane containing the point (x0,y0,z0) 
 * with normal vector n = (a,b,c) is 
 * 
 *     a(x-x0)+ b(y-y0)+c(z-z0) = 0
 *
 * Solving z:
 *
 *       a(x-x0)+b(y-y0)+c(z-z0) = 0
 * <=>                   c(z-z0) =  -a(x-x0) - b(y-y0)
 * <=>                    (z-z0) = (-a(x-x0) - b(y-y0))/c
 * <=>                     z     = (-a(x-x0) - b(y-y0))/c + z0
 * 
 */
fptype Mesh::calcZ(fptype x, fptype y)
{
    fptype dx = x/Facet::size;
    fptype dy = y/Facet::size;
    int ix = (int)dx;
    int iy = (int)dy;

    Facet* facet = getFacet(ix, iy);
    fptype* n = ((dx - ix) + (dy - iy) < 1.0) ? facet->lowerNormal : facet->upperNormal;
    Coord* coord = getCoord(ix+1, iy);
    fptype* p = coord->vertex;
    fptype dz = ((-n[0]*(x-p[0]) - n[1]*(y-p[1])) / n[2]) + p[2];
    return dz;
}

fptype* Mesh::getFacetNormal(fptype x, fptype y)
{
    fptype dx = x/Facet::size;
    fptype dy = y/Facet::size;
    int ix = (int)dx;
    int iy = (int)dy;
    Facet* facet = getFacet(ix, iy);

    return ((dx - ix) + (dy - iy) < 1.0) ? facet->lowerNormal : facet->upperNormal;
}

// record lowest and highest points
void Mesh::setHighAndLow(fptype z)
{
    lowestPoint = min(lowestPoint, z);
    highestPoint = max(highestPoint, z);
}

fptype Mesh::getLowestPoint()
{
    return lowestPoint;
}

fptype Mesh::getHighestPoint()
{
    return highestPoint;
}

/* Map generation */

/*
 * Parametric equation of a 3D edge:
 *     x = a1(1 - t) + b1t
 *     y = a2(1 - t) + b2t
 *     z = a3(1 - t) + b3t
 * where (a1, a2, a3) and (b1, b2, b3) are the endpoints
 *
 * Solve t in the last equation:
 *
 *                         z = a3(1 - t) + b3t
 * <=>                     z = a3 - a3t  + b3t
 * <=>                     z = a3 + b3t - a3t
 * <=>                     z = a3 + (b3 - a3)t
 * <=>                z - a3 = (b3 - a3)t
 * <=>    (z - a3)/(b3 - a3) = t
 *
 * Now if 0 <= t <= 1, the point is between the endpoints,
 * and x and y can be calculated by inserting t to the above equations.
 */
bool Edge::getHeightPoint(fptype z, Point& p, fptype& leftRight)
{
    // if d is negative, then the lower ground is to the left
    fptype d = p1.z - p0.z;
    fptype t = (d == 0) ? -1 : (z - p0.z)/d;

    if ((0 < t) && (t < 1)) 
    {
	p.x = p0.x*(1-t) + p1.x*t;
	p.y = p0.y*(1-t) + p1.y*t;
	p.z = z;
	leftRight = d;
	return true;
    }

    return false;
}

/*
 * Get endpoints of edge
 * (facetx, facety) is coordinates of the facet
 * triangle (0 or 1) is the triangle we re looking at
 * egde (1, 2, or 3) is the edge whose endpoints we want
 * start, stop are the returned endpoints
 * Return TRUE if endpoints are found, FALSE otherwise
 */

/* See if a given edge crosses a height value. */
bool Mesh::edgeCrossesHeight(MeshCoordinate* mc, int edgeId, fptype altitude, Point& p, fptype& leftRight)
{
    Edge edge;
    if (getEdge(mc, edgeId, edge))
    {
	if (edge.getHeightPoint(altitude, p, leftRight))
	{
	    return true;
	}
    }
    return false;
}

bool Mesh::getEdge(MeshCoordinate* mc, int edgeId, Edge& edge)
{
    Coord* start;
    Coord* end;

    try
    {
	if (mc->triangle == 0) 
	{
	    switch (edgeId)
	    {
	    case 1:
		start = getCoord(mc->facetx, mc->facety+1);
		end = getCoord(mc->facetx+1, mc->facety);
		break;
	    case 2:
		start = getCoord(mc->facetx+1, mc->facety);
		end = getCoord(mc->facetx, mc->facety);
		break;
	    case 3:
		start = getCoord(mc->facetx, mc->facety);
		end = getCoord(mc->facetx, mc->facety+1);
		break;
	    default:
		printf("Unknown edge: %d\n", edgeId);
		break;
	    }
	} 
	else if (mc->triangle == 1) 
	{
	    switch (edgeId)
	    {
	    case 1:
		start = getCoord(mc->facetx+1, mc->facety);
		end = getCoord(mc->facetx, mc->facety+1);
		break;
	    case 2:
		start = getCoord(mc->facetx, mc->facety+1);
		end = getCoord(mc->facetx+1, mc->facety+1);
		break;
	    case 3:
		start = getCoord(mc->facetx+1, mc->facety+1);
		end = getCoord(mc->facetx+1, mc->facety);
		break;
	    default:
		printf("Unknown edge: %d\n", edgeId);
		break;
	    }
	} 
	else 
	{
	    printf("Unknown triangle: %d\n", mc->triangle);
	}

	edge = Edge(Point(start->vertex[0], start->vertex[1], start->vertex[2]),
		    Point(end->vertex[0], end->vertex[1], end->vertex[2]));

	return true;
    } 
    catch (std::out_of_range&)
    {
	return false;
    }
}

// flip the mesh coordinate to the other side of edge
void MeshCoordinate::otherSide(int edge)
{
    if (edge == 1)
    {
	; // we stay in the same facet, only triangle changes
    }
    else if (edge == 2)
    {
	// we move up or down (y-coord changes)
	facety = (triangle == 0) ? facety-1 : facety+1;
    }
    else if (edge == 3)
    {
	// we move left or right (x-coord changes)
	facetx = (triangle == 0) ? facetx-1 : facetx+1;
    }
    else
    {
	printf("MeshCoordinate::otherSide: Unknown edge: %d\n", edge);
    }

    // triangle always changes
    triangle = 1-triangle; // toggle between 0 and 1
}

// copy the first point and add it to the tail
// to make the contour a proper loop
void Contour::joinHeadAndTail()
{
    const Point* first = points.front();
    Point* last = new Point(*first);
    addTail(last);
}

/*
 * traceContour(facetx, facety, triangle, ignoreEdge)
 * current facet is (x,y)
 * mark that this facet has been used for this height
 * 
 * o if current triangle is t and point is found on edge 1 then the
 *   next edge to ignore is [(x,y), (1-t), 1]
 * 
 * o if current triangle is 0 and point is found on edge 2 then the
 *   next edge to ignore is [(x,y-1), (1-t), 2]
 * 
 * o if current triangle is 1 and point is found on edge 2 then the
 *   next edge to ignore is [(x,y+1), (1-t), 2]
 * 
 * o if current triangle is 0 and point is found on edge 3 then the
 *   next edge to ignore is [(x-1,y), (1-t), 3]
 * 
 * o if current triangle is 1 and point is found on edge 3 then the
 *   next edge to ignore is [(x+1,y), (1-t), 3]
 * 
 */
Contour::ContourType 
Mesh::traceContour2(MeshCoordinate* mc, int ignoreEdge, fptype altitude,
		    Contour* contour, bool addTail)
{
    Point point;
    Facet* facet;
    Contour::ContourType contourType;

    fptype leftRight;

    /*
     * terminate looping if:
     *   this triangle has already been used for this height value,
     *   i.e. we have come full circle to the beginning of this contour,
     *   it is a closed contour
     * OR
     *   there is no facet at these coordinates, 
     *   i.e. the contour goes off the edge of the mesh, 
     *   it is an open contour
     */
    while (true) 
    {
	leftRight = 0;

	try 
	{
	    facet = getFacet(mc->facetx, mc->facety);
	}
	catch (std::out_of_range&)
	{
	    contourType = Contour::CT_OPEN;
	    break;
	}

	if (facet->triangleMarked(mc->triangle)) 
	{
	    // it is a closed contour
	    if (contour->size() < 3) 
	    {
		contourType = Contour::CT_TOO_SHORT;
	    }
	    else 
	    {
		contour->joinHeadAndTail();
		contourType = Contour::CT_CLOSED;
	    }
	    break;
	}
	
	if (ignoreEdge == 1) 
	{
	    if (edgeCrossesHeight(mc, 2, altitude, point, leftRight))
		ignoreEdge = 2;
	    else if (edgeCrossesHeight(mc, 3, altitude, point, leftRight))
		ignoreEdge = 3;
	    else 
	    {
		contourType = Contour::CT_ERROR;
		break;
	    }
	} 
	else if (ignoreEdge == 2) 
	{
	    if (edgeCrossesHeight(mc, 1, altitude, point, leftRight))
		ignoreEdge = 1;
	    else if (edgeCrossesHeight(mc, 3, altitude, point, leftRight))
		ignoreEdge = 3;
	    else 
	    {
		contourType = Contour::CT_ERROR;
		break;
	    }
	} 
	else if (ignoreEdge == 3) 
	{
	    if (edgeCrossesHeight(mc, 1, altitude, point, leftRight))
		ignoreEdge = 1;
	    else if (edgeCrossesHeight(mc, 2, altitude, point, leftRight))
		ignoreEdge = 2;
	    else 
	    {
		contourType = Contour::CT_ERROR;
		break;
	    }
	}
	else
	{
	    printf("Mesh::traceContour2: Unknown ignoreEdge: %d\n", ignoreEdge);
	}


	if (addTail)
	{
	    contour->addTail(new Point(point));
	    if (leftRight < 0)
		contour->addLeft();
	    else if (leftRight > 0)
		contour->addRight();
	}
	else
	{
	    contour->addHead(new Point(point));
	    if (leftRight < 0)
		contour->addRight();
	    else if (leftRight > 0)
		contour->addLeft();
	}

	facet->markTriangle(mc->triangle, true);
	mc->otherSide(ignoreEdge);
    }

    return contourType;
}

Contour* Mesh::traceContour(MeshCoordinate* mc, int ignoreEdge, fptype altitude)
{
    Contour* contour = new Contour(altitude);
    MeshCoordinate original(*mc);
    bool addTail;

    Contour::ContourType contourType;

    contourType = traceContour2(mc, ignoreEdge, altitude, contour, addTail = true);

    switch (contourType) 
    {
    case Contour::CT_OPEN:
	//std::cout << "open contour 1, size " << contour->size() << std::endl;

	// if we have an open contour, 
	// we trace the same contour starting at the same 
	// position but going to the other direction
	original.otherSide(ignoreEdge);
	contourType = traceContour2(&original, ignoreEdge, altitude, contour, addTail = false);
	//std::cout << "open contour 2, size " << contour->size() << std::endl;
	contour->setDownhill(contourType);
	break;
    case Contour::CT_CLOSED:
	//std::cout << "closed contour, size " << contour->size() << std::endl;
	// closed contour is complete right away
	contour->setDownhill(contourType);
	break;
    case Contour::CT_TOO_SHORT:
	//printf("Mapper: contour too short\n");
	delete contour;
	contour = 0;
	break;
    case Contour::CT_ERROR:
	//printf("Mapper: neither egde crosses current level\n");
	delete contour;
	contour = 0;
	break;
    }

    return contour;
}

// clear the used flags of facet triangles
void Mesh::clearTriangles()
{
    for (int i = 0; i < width; i++) 
    {
	for (int j = 0; j < height; j++) 
	{
	    try
	    {
		Facet* facet = getFacet(i, j);
		facet->clearTriangles();
	    }
	    catch (std::out_of_range&)
	    {
	    }
	}
    }
}

/*
 * map generation
 * Edge coordinates: [facet(x, y), triangle(0, 1), edge(1, 2, 3)]
 * return the point and the coordinates of edge to look for the next point
 * 
 * outer loop:
 * for currentHeight from terrain lowest height to terrain highest height step is contourInterval
 *     mark all triangles unused
 *     for currentFacet in allFacets
 *         t = triangle 0 of currentFacet
 * 	edge2 = edge 2 of t
 * 	edge3 = edge 3 of t
 * 	if currentHeight crosses edge2
 * 	    traceContour(currentFacet, t, edge2)
 * 	if currentHeight crosses edge3
 * 	    traceContour(currentFacet, t, edge3)
 */
void Mesh::findContours(int contourInterval, MapProxy::Contours& contours)
{
    fptype high = getHighestPoint();
    fptype low  = getLowestPoint();
    fptype c = fmod(low, contourInterval);
    int lowint = int(low-c); // lowest contour level

    for (fptype altitude = lowint; altitude < high; altitude += contourInterval) 
    {
	clearTriangles();
	for (int i = 0; i < width; i++) 
	{
	    for (int j = 0; j < height; j++) 
	    {
		// test each 2 and 3 -edges of each 0-triangle 
		// this covers the entire mesh
		MeshCoordinate meshCoordinate;
		fptype leftRight;
		Facet* facet = getFacet(i, j);
		if (!facet->triangleMarked(0)) 
		{
		    Point point;
		    Contour* contour;
		    meshCoordinate.facetx = i;
		    meshCoordinate.facety = j;
		    meshCoordinate.triangle = 0;
		    if (edgeCrossesHeight(&meshCoordinate, 2, altitude, point, leftRight))
		    {
			if ((contour = traceContour(&meshCoordinate, 2, altitude)) != 0) 
			{
			    contours.push_back(contour);
			}
		    }

		    meshCoordinate.facetx = i;
		    meshCoordinate.facety = j;
		    meshCoordinate.triangle = 0;
		    if (edgeCrossesHeight(&meshCoordinate, 3, altitude, point, leftRight))
		    {
			if ((contour = traceContour(&meshCoordinate, 3, altitude)) != 0) 
			{
			    contours.push_back(contour);
			}
		    }
		}
	    }
	}
    }
}

void Mesh::findContours2(MapProxy::Contours& contours, fptype altitude, int i, int j)
{
    // test each 2 and 3 -edges of each 0-triangle 
    // this covers the entire mesh
    MeshCoordinate meshCoordinate;
    Facet* facet = getFacet(i, j);
    if (!facet->triangleMarked(0)) 
    {
	fptype leftRight;
	Point point;
	Contour* contour;
	meshCoordinate.facetx = i;
	meshCoordinate.facety = j;
	meshCoordinate.triangle = 0;
	if (edgeCrossesHeight(&meshCoordinate, 2, altitude, point, leftRight))
	{
	    if ((contour = traceContour(&meshCoordinate, 2, altitude)) != 0) 
	    {
		contours.push_back(contour);
	    }
	}
	
	meshCoordinate.facetx = i;
	meshCoordinate.facety = j;
	meshCoordinate.triangle = 0;
	if (edgeCrossesHeight(&meshCoordinate, 3, altitude, point, leftRight))
	{
	    if ((contour = traceContour(&meshCoordinate, 3, altitude)) != 0) 
	    {
		contours.push_back(contour);
	    }
	}
    }
}
