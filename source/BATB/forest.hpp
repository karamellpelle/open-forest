#ifndef _FOREST_H
#define _FOREST_H

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif


#include "util.h"


/*
filename        height  type commonality  typical environment
manty.rgb       20-35    n    10
kuusi.rgb       20-35    n    10
rauduskoivu.rgb 10-25    l     9
pihlaja.rgb      5-15    l     5
douglas.rgb     15-30    n     5
haapa.rgb       15-25    l     6
harmaaleppa.rgb 10-20    l     4
hieskoivu.rgb   10-20    l     8
jalava.rgb      20-30    l     1
kataja.rgb       2-8     n     2
lehmus.rgb      15-25    l     3
lehtikuusi.rgb  20-35    n     6
pihta.rgb       20-30    n     3
raita.rgb        5-12    l     2
saarni.rgb      20-35    l     1
tammi.rgb       15-25    l     4
tervaleppa.rgb  10-25    l     6
tuomi.rgb        5-15    l     3
vaahtera.rgb    10-20    l     5
*/

// ability to set points where a certain tree type probability increases
// ability to set points where general tree density increases

// make one random voronoi diagram and assign one tree to each area.
// The probability of that tree apprearing is increased for that area

// one random voronoi diagram, assign weights to the areas, these weights
// control the overall density of the forest

class Tree
{
 public:
  Tree(GLuint _textureId, fptype _x, fptype _y, fptype _z, fptype _width, fptype _height);

 private:
  GLuint textureId;
};

class TreeType
{
 public:
  TreeType(fptype minHeight, fptype maxHeight,
	   fptype minWidth, fptype maxWidth,
	   fptype commonality, GLuint _textureId,
	   std::string textureFilename);

  Tree* getTree(fptype x, fptype y, fptype z);

  fptype getCommonality() const { return commonality; }
 private:
  fptype minHeight;
  fptype maxHeight;
  fptype minWidth;
  fptype maxWidth;

  fptype commonality;

  GLuint textureId;
  std::string textureFilename;
};

class Forest
{
 public:
  Forest();
  void addTreeType(fptype minHeight, fptype maxHeight,
		   fptype minWidth, fptype maxWidth,
		   fptype commonality, GLuint textureId,
		   std::string textureFilename);
  Tree* getNewTree(fptype x, fptype y, fptype z);

 private:
  fptype treeDensity;
  typedef std::vector<TreeType*> TreeTypes;
  fptype commonalitySum;
  fptype countCommonalitySum();

 public:
  TreeTypes treeTypes;
};

#endif // _FOREST_H
