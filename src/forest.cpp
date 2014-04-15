#include "forest.h"
#include "texture.h"


void Forest::addTreeType(fptype minHeight, fptype maxHeight,
			 fptype minWidth, fptype maxWidth,
			 fptype commonality, GLuint textureId,
			 std::string textureFilename)
{
  TreeType* treeType = new TreeType(minHeight, maxHeight, 
				    minWidth, maxWidth, 
				    commonality, textureId,
				    textureFilename);
  treeTypes.push_back(treeType);
  countCommonalitySum();
}


Tree* Forest::getNewTree(fptype x, fptype y, fptype z)
{
  fptype rand = myrandom(0, commonalitySum);

  for (TreeTypes::const_iterator i = treeTypes.begin();
       i != treeTypes.end(); ++i) 
    {
      TreeType* treeType = *i;
      fptype commonality = treeType->getCommonality();
      rand -= commonality;
      if (rand <= 0)
	{
	  Tree* tree = treeType->getTree(x, y, z);
	  return tree;
	}
    }
  return NULL;
}

fptype Forest::countCommonalitySum()
{
  commonalitySum = 0.0;
  for (TreeTypes::const_iterator i = treeTypes.begin();
       i != treeTypes.end(); ++i) 
    {
      TreeType* treeType = *i;
      commonalitySum += treeType->getCommonality();
    }
  return commonalitySum;
}

Tree* TreeType::getTree(fptype x, fptype y, fptype z)
{
  fptype height = myrandom(minHeight, maxHeight);
  fptype width = myrandom(minWidth, maxWidth);
  Tree* tree = new Tree(textureId, x, y, z, width, height);
  return tree;
}

TreeType::TreeType(fptype minh, fptype maxh,
		   fptype minw, fptype maxw,
		   fptype comm, GLuint tId,
		   std::string texFn)
  : minHeight(minh), maxHeight(maxh), minWidth(minw),
    maxWidth(maxw), commonality(comm), textureId(tId),
    textureFilename(texFn)
{
  loadTexture(textureId, textureFilename.c_str());
}
