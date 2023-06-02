#ifndef __study_generator_data_hxx_
#define __study_generator_data_hxx_

#include "common.h"

namespace studygen
{

struct TimePointData
{
  Image3DType::Pointer image;
  LabelImage3DType::Pointer seg;
  MeshPointer fullMesh;
  std::map<LabelType, MeshPointer> labelMeshMap;
};

struct StudyGeneratorData
{
  Image4DType::Pointer image4D;
  std::vector<TimePointData> tpInputData;
  std::vector<TimePointData> tpOutputData;
  
  void Initialize(TimePointType nT)
  {
    for (auto i = 0; i < nT; ++i)
    {
      tpInputData.push_back(TimePointData());
      tpOutputData.push_back(TimePointData());
    }
  }
};

}

#endif // __study_generator_data_hxx_