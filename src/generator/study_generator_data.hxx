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

  // internal storage of data for processing. not final data for output
  std::map<TimePointType, TimePointData> tpData;

  // final output data
  std::map<TimePointType, TimePointData> tpStudyData;
  
  void Initialize(TimePointType nT)
  {
    for (auto i = 1u; i <= nT; ++i)
    {
      tpData.insert(std::make_pair(i, TimePointData()));
      tpStudyData.insert(std::make_pair(i, TimePointData()));
    }
  }
};

}

#endif // __study_generator_data_hxx_
