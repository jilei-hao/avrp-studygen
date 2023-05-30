#ifndef __study_generator_data_hxx_
#define __study_generator_data_hxx_

#include "common.h"

namespace studygen
{

struct TimePointData
{
  Image3DType::Pointer image;
  LabelImage3DType::Pointer seg;
  MeshPointer mesh;
};

struct StudyGeneratorData
{
  Image4DType::Pointer m_Image4D;
  std::vector<TimePointData> m_TPDataInput;
  std::vector<TimePointData> m_TPDataProcessed;
};

}

#endif // __study_generator_data_hxx_