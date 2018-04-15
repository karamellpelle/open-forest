#ifndef _TERRAINPROXY_H
#define _TERRAINPROXY_H

#include "mesh.h"
#include "matrix.h"


class TerrainProxy
{
 public:

    ~TerrainProxy() 
    { 
	delete mesh;
	delete data;
    }

    static TerrainProxy& instance()
    {
	static TerrainProxy theSingleInstance;
	return theSingleInstance;
    }

    const Terrain* getTerrain() const { return terrain; }
    Mesh* getMesh() const { return mesh; }
    void setData(int i, int j, fptype data);
    fptype getData(int i, int j) const { return (*data)(i,j); }
    void setHighest(fptype h);
    fptype getHighest() const { return highest; }
    void setLowest(fptype l);
    fptype getLowest() const { return lowest; }
    void initTrees();

    float initRealize();
    float continueRealize();
    void finishRealize();

 private:
    Terrain* terrain;     // the terrain this realises
    fptype highest;       // highest point in data
    fptype lowest;        // lowest point in data
    Matrix<fptype>* data; // xmountains data is generated here
    Mesh* mesh;

    TerrainProxy() 
    {
	data = 0;
	terrain = 0;
    }


    void meshRealize();
    int terrainRealizeWidth;
    int terrainRealizeHeight;
    int terrainRealizeCounter;
};

#endif // _TERRAINPROXY_H
