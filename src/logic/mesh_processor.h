#ifndef __mesh_processor_h
#define __mesh_processor_h

#include "common.h"
#include "configurations.hxx"

namespace studygen
{

class MeshProcessor
{
public:
  MeshProcessor();
  ~MeshProcessor();

  void SetConfig(LabelConfig config);

  void SetImage(LabelImage3DType::Pointer img);

  MeshPointer GetOutput();

private:
  LabelConfig m_Config;
  LabelImage3DType::Pointer m_Image;
  MeshPointer m_Output;

};

}

#endif // __mesh_processor_h