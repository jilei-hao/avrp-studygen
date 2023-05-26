#include "config_factories.h"

using namespace studygen;

// SegConfigFactory

SegConfigFactory
::SegConfigFactory()
{

}

SegConfigFactory
::~SegConfigFactory()
{

}



// LabelMeshConfigFactory
LabelMeshConfigFactory
::LabelMeshConfigFactory()
{

}

LabelMeshConfigFactory
::~LabelMeshConfigFactory()
{

}

LabelMeshConfig
LabelMeshConfigFactory
::CreateBySize(char size)
{
  LabelMeshConfig ret;

  switch(size)
  {
    case 'S': 
    {
      ret.imgSteps.push_back(ImageStep(200, 2));
      ret.imgSteps.push_back(ImageStep(200, 2));
      ret.meshSteps.push_back(MeshStep(0.2, 20, 0.1, 0));
      ret.meshSteps.push_back(MeshStep(0.5, 20, 0.1, 0));
      ret.meshSteps.push_back(MeshStep(0.5, 20, 0.1, 0));
      break;
    }
    case 'M':
    {
      ret.imgSteps.push_back(ImageStep(200, 2));
      ret.imgSteps.push_back(ImageStep(200, 2));
      ret.meshSteps.push_back(MeshStep(0.2, 20, 0.1, 0));
      ret.meshSteps.push_back(MeshStep(0.5, 20, 0.1, 0));
      ret.meshSteps.push_back(MeshStep(0.5, 20, 0.1, 0));
      ret.meshSteps.push_back(MeshStep(0.5, 20, 0.1, 0));
      break;
    }
    case 'L':
    {
      ret.imgSteps.push_back(ImageStep(200, 2));
      ret.imgSteps.push_back(ImageStep(200, 3));
      ret.meshSteps.push_back(MeshStep(0.2, 100, 0.01, 0));
      ret.meshSteps.push_back(MeshStep(0.75, 20, 0.1, 0));
      ret.meshSteps.push_back(MeshStep(0.75, 20, 0.1, 0));
      break;
    }
  }
  return ret;
}