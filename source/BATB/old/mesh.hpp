#ifndef _MESH_H
#define _MESH_H

#include <vector>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "matrix.h"
#include "point.h"
#include "mapproxy.h"

#include "util.h"

/***
 * OpenGL has right-handed coordinate system.
 *
 * Terrain is positioned so that it is on the x-y plane
 * with southwest corner in origin (0,0).
 * When x coordinate increases, you go east
 * When y coordinate increases, you go north
 * When z coordinate increases, you go up
 *
 ***
 * Facet is one square, composed of two right triangles, 0 and 1:
 *
 *      +-----+
 *      |\   1|
 *      |  \  |
 *      |0   \|
 *      +-----+
 *
 *   0 = lower left  triangle (LL)
 *   1 = upper right triangle (UR)
 *
 * Each triangle has three edges numbered 1, 2, 3.
 * Hypotenuse is labeled 1, others (2 and 3) are found
 * by traversing triangle edges clockwise:
 *
 *         2
 *      +-----+
 *      |\    |
 *    3 |  1  | 3
 *      |    \|
 *      +-----+
 *         2
 *
 * Coord is a coordinate (four corners of a facet)
 * (the plus signs in the picture above)
 *
 * Facets, triangles and egdes are not explicitly modeled,
 * whereas Facet normals and coordinates are.
 *
 * If the width (height) of a mesh is w (h),
 * there is w (h) facets and w+1 (h+1) coords.
 *
 ***
 * Mesh is a two dimensional array of facets and coordinates:
 *
 *  7    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 *       |\    |\    |\    |\    |\    |\    |\    |\    |\    |\    |\    |
 *     6 |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |
 *       |    \|    \|    \|    \|    \|    \|    \|    \|    \|    \|    \|
 *  6    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 *       |\    |\    |\    |\    |\    |\    |\    |\    |\    |\    |\    |
 *     5 |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |
 *       |    \|    \|    \|    \|    \|    \|    \|    \|    \|    \|    \|
 *  5    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 *       |\    |\    |\    |\    |\    |\    |\    |\    |\    |\    |\    |
 *     4 |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |
 *       |    \|    \|    \|    \|    \|    \|    \|    \|    \|    \|    \|
 *  4    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 *       |\    |\    |\    |\    |\    |\    |\    |\    |\    |\    |\    |
 *     3 |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |
 *       |    \|    \|    \|    \|    \|    \|    \|    \|    \|    \|    \|
 *  3    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 *       |\    |\    |\    |\    |\    |\    |\    |\    |\    |\    |\    |
 *     2 |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |
 *       |    \|    \|    \|    \|    \|    \|    \|    \|    \|    \|    \|
 *  2    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 *       |\    |\    |\    |\    |\    |\    |\    |\    |\    |\    |\    |
 *     1 |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |
 *       |    \|    \|    \|    \|    \|    \|    \|    \|    \|    \|    \|
 *  1    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 *       |\    |\    |\    |\    |\    |\    |\    |\    |\    |\    |\    |
 *     0 |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |
 *       |    \|    \|    \|    \|    \|    \|    \|    \|    \|    \|    \|
 *  0    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 *     B    0     1     2     3     4     5     6     7     8     9    10
 *
 *  A    0     1     2     3     4     5     6     7     8     9    10    11
 *
 *  A = Coord coordinates
 *  B = Vertex coordinates
 *
 * Mesh represents the terrain as a heightfield.
 */

class Mesh;

class Tree 
{
 private:
    GLuint textureId;

    // location of tree
    fptype x;
    fptype y;
    fptype z;

    // height and width of tree
    fptype width;
    fptype height;

    // vertex arrays
    fptype vertex[24];
    fptype normal[24];
    fptype texture[16];

    int drawImmediate();
    int drawVertexArray();

 public:
    Tree(GLuint _textureId, fptype _x, fptype _y, fptype _z, fptype _width, fptype _height);
    int draw();
};

class Coord 
{
 private:
    friend class Facet;
    friend class Mesh;
    fptype vertex[3];
    fptype normal[3];
 public:
    void setVertex(fptype x, fptype y, fptype z);
    void setNormal(fptype x, fptype y, fptype z);
    Coord()
    {
	vertex[0] = 0;
	vertex[1] = 0;
	vertex[2] = 0;
	normal[0] = 0;
	normal[1] = 0;
	normal[2] = 0;
    }
};
#define coordGetVertex(coord) ((coord)->vertex)
void   coordSetVertex(Coord*, fptype x, fptype y, fptype z);
#define coordGetNormal(coord) ((coord)->normal)
void   coordSetNormal(Coord*, fptype x, fptype y, fptype z);

class Facet 
{
 private:
    friend class Mesh;
    // normal is a vector perpendicular to the triangle
    fptype lowerNormal[3]; // normal of the lower left triangle (triangle 0)
    fptype upperNormal[3]; // normal of the upper right triangle (triangle 1)
    bool lowerUsed; // true if triangle 0 has been used in this level contour
    bool upperUsed; // true if triangle 1 has been used in this level contour
    std::vector<Tree*> trees;     // trees that are in the area specified by this facet
    Tree* tree;

    void calculateVertexNormals();

    void calcNormal(const fptype pt1[3], const fptype pt2[3], 
		    const fptype pt3[3], fptype* result);

 public:
    // length of the facet side
    static int size;

    void drawTrees() const;
    void addTree(Tree* tree);
    bool triangleMarked(int triangle);
    void markTriangle(int triangle, bool value);
    void clearTriangles();
    void calcNormals(const Coord* coordNW, const Coord* coordNE,
		     const Coord* coordSW, const Coord* coordSE);
    Facet()
    {
	tree = NULL;
	lowerNormal[0] = 0;
	lowerNormal[1] = 0;
	lowerNormal[2] = 0;
	upperNormal[0] = 0;
	upperNormal[1] = 0;
	upperNormal[2] = 0;
    }

    ~Facet();
};

/*
 * MeshCoordinate identifies a particular triangle in a mesh.
 * facetx and facety indentify the facet
 * and triangle identifies the triangle (0 or 1)
 */
class MeshCoordinate 
{
 private:
    int facetx;
    int facety;
    int triangle; // 0 or 1
    int edge;     // 1, 2, or 3
    Mesh* mesh;

    friend class Mesh;

 public:

    MeshCoordinate()
	: facetx(0), facety(0), triangle(0), edge(1) {}

    MeshCoordinate(int fx, int fy, int t, int e)
	: facetx(fx), facety(fy), triangle(t), edge(e) {}

    void set(int fx, int fy, int t, int e);

    /* Change the MeshCoordinate to point to a triangle that is on 
     * the other side of the given edge. */
    void otherSide(int edge);

    Point* trace(fptype height);

    bool crosses(fptype height);
};


// Edge in the mesh
// represented by the two endpoints
class Edge
{
 private:

 public:
    Point p0;
    Point p1;
    Edge() {}
    Edge(Point _p0, Point _p1) : p0(_p0), p1(_p1) {}
    Edge(fptype p0x, fptype p0y, fptype p0z,
	 fptype p1x, fptype p1y, fptype p1z) 
	: p0(p0x, p0y, p0z), p1(p1x, p1y, p1z) {}
    //Egde(Mesh* mesh, MeshCoordinate* mc, int edge);

    /* Given an edge whose end points are (start, end),
     * find a point in that edge that has height value z.
     * Return false if no such point exists.
     */
    bool getHeightPoint(fptype z, Point& p, fptype& leftRight);
};

class Mesh
{
 public:

    /* 
     * 
     * density is the scale between the xy-dimension of
     * the terrain data (as generated by xmountains) and the 
     * polygon mesh shown in the terrain window 
     * (= terrain data sampling rate).
     * 
     * In the picture below, the dots represent data as generated 
     * by xmountains, facet coordinates are taken from every 3rd 
     * (density == 3) point, represented by plus signs.
     * 
     * ....................
     * ....................
     * .....+--+...........
     * .....|\.|...........
     * .....|.\|...........
     * .....+--+...........
     * ....................
     * ....................
     * 
     * Small values make terrain look smoother, but consume more memory.
     * 
     */
    static int density;

    Mesh(int w, int h);
    ~Mesh();

    Facet* getFacet(int, int);
    Facet* getFacet(fptype x, fptype y);
    Coord* getCoord(int, int);
    fptype* getFacetNormal(fptype x, fptype y);
    bool getEdge(MeshCoordinate* mc, int edgeId, Edge& edge);

    void calculateFacetNormals();
    void calculateVertexNormals();
    
    // return mesh width and height
    // = X (Y) size multiplied by Facet::size
    int getWidth() const { return Facet::size*width; }
    int getHeight() const { return Facet::size*height; }

    // "raw" size of facet = number of facets in X (Y) direction
    int getXSize() const { return width; }
    int getYSize() const { return height; }

    void calcNormals();
    void draw(Point& pos, fptype horizontal);
    void drawDay(Point& pos, fptype horizontal);
    void drawNight(Point& pos, fptype horizontal);

    fptype calcZ(fptype x, fptype y);

    /*
     * Finds all contours for the map of this mesh.
     * Returns a list of Contours
     */
    void findContours(int contourInterval, MapProxy::Contours& contours);
    void findContours2(MapProxy::Contours& contours, fptype altitude, int i, int j);

    void clearTriangles();
    fptype getLowestPoint();
    fptype getHighestPoint();
    void setHighAndLow(fptype z);
    
    void initTrees(int seed);
    void makeDisplayList();

    void makeVertexArray();

 private:
    int    width;
    int    height;

    Matrix<Coord>* coords;
    Matrix<Facet>* facets;

    fptype** vertex;
    fptype** normal;
    fptype** texture;

    fptype  lowestPoint;
    fptype  highestPoint;
    GLuint  dliMesh;
    int     drawRadius;

	void drawWithShadersDay(Point& pos, fptype horizontal);
	void drawWithShadersNight(Point& pos, fptype horizontal);
	void drawWithoutShaders(Point& pos, fptype horizontal);

    bool edgeCrossesHeight(MeshCoordinate* mc, int edgeId, fptype z, Point& p, fptype& leftRight);
    Contour* traceContour(MeshCoordinate* mc, int ignoreEdge, fptype z);
    Contour::ContourType traceContour2(MeshCoordinate* mc, int ignoreEdge, fptype z,
				       Contour* contour, bool addTail);

    void freeVertexArray();
    void drawGroundVertexArray2(int minx, int maxx, int miny, int maxy);

    void drawGroundVertexArray(int minx, int maxx, int miny, int maxy);
    void drawGroundImmediate(int minx, int maxx, int miny, int maxy);
    void drawTrees(int minx, int maxx, int miny, int maxy);
    void drawAll();

};

#endif // _MESH_H
