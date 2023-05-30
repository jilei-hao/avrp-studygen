#include "mesh_processor.h"

using namespace studygen;

MeshProcessor
::MeshProcessor()
{

}

MeshProcessor
::~MeshProcessor()
{

}

void
MeshProcessor
::SetConfig(LabelMeshConfig config)
{
  m_Config = config;
}

void
MeshProcessor
::SetImage(LabelImage3DType::Pointer img)
{
  m_Image = img;
}

MeshPointer
MeshProcessor
::GetOutput()
{
  return m_Output;
}