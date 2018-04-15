#include <valarray>
#include <vector>

#include "database.h"

#include "matrix.h"
#include "mesh.h"
#include "terrain.h"
#include "terrainproxy.h"
#include "dlgCommon.h"
#include "dlgStack.h"


void TerrainProxy::meshRealize()
{
    int width  = (terrain->width()/Mesh::density);
    int height = (terrain->height()/Mesh::density);

    if (mesh != 0)
	delete mesh;
    mesh = new Mesh(width, height);

    fptype heightDiff = getHighest()-getLowest();
    if (heightDiff == 0) heightDiff = 1.0;

    fptype heightCoeff = terrain->altitudeVariation()/heightDiff;

    for (int x = 0; x <= width; x++) 
    {
	for (int y = 0; y <= height; y++) 
	{
	    Coord* coord = mesh->getCoord(x, y);
	    fptype z = getData(x*Mesh::density, y*Mesh::density);
	    coord->setVertex(x*Facet::size, y*Facet::size, z*heightCoeff);
	    mesh->setHighAndLow(z*heightCoeff);
	}
    }

    mesh->calcNormals();
    mesh->initTrees(terrain->seed());
    mesh->makeDisplayList();
    mesh->makeVertexArray();
}

void xmInit(TerrainProxy& terrainProxy);
void xmPlotColumn(TerrainProxy& terrainProxy, int p);
void xmFinish();

float TerrainProxy::initRealize()
{
    float percentComplete;
    Database& db = Database::instance();
    Terrain* selectedTerrain = db.getEventTerrain();

    if (terrain != selectedTerrain)
    {
	terrain = selectedTerrain;
	terrainRealizeWidth = terrain->width()+1;
	terrainRealizeHeight = terrain->height()+1;
	terrainRealizeCounter = 0;

	if (data != 0) 
	    delete data;
	data = new Matrix<fptype>(terrainRealizeWidth, terrainRealizeHeight);
	
	xmInit(*this);
	percentComplete = 0.0;
    }
    else
    {
	percentComplete = 1.0;
    }

    return percentComplete;
}

float TerrainProxy::continueRealize()
{
    if (terrainRealizeCounter < terrainRealizeWidth)
    {
	xmPlotColumn(*this, terrainRealizeCounter);
	float percentComplete = terrainRealizeCounter/float(terrainRealizeWidth);
	terrainRealizeCounter++;
	return percentComplete;
    }
    
    meshRealize();
    return 1.0;
}

void TerrainProxy::finishRealize()
{
    xmFinish();
}

void TerrainProxy::setData(int i, int j, fptype dt)
{
    (*data)(i,j) = dt;
	
    if (dt < getLowest())
	setLowest(dt);

    if (dt > getHighest())
	setHighest(dt);
}

void TerrainProxy::setHighest(fptype h) 
{
    highest = h;
}

void TerrainProxy::setLowest(fptype l)
{
    lowest = l;
}
