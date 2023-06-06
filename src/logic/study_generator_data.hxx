#ifndef __study_generator_data_hxx_
#define __study_generator_data_hxx_

#include "common.h"

namespace studygen
{

struct TimePointData
{
  Image3DType::Pointer image;
  LabelImage3DType::Pointer segmentation;
  MeshPointer unifiedMesh;
  std::map<LabelType, MeshPointer> labelMeshMap;
};

struct StudyGeneratorData
{
  Image4DType::Pointer image4D;
  std::map<TimePointType, TimePointData> tpInputData;
  std::map<TimePointType, TimePointData> tpOutputData;
  
  void Initialize(TimePointType nT)
  {
    for (auto i = 1u; i <= nT; ++i)
    {
      tpInputData.insert(std::make_pair(i, TimePointData()));
      tpOutputData.insert(std::make_pair(i, TimePointData()));
    }
  }
};

}

#endif // __study_generator_data_hxx_
