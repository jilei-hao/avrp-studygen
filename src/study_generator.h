#ifndef __study_generator_h
#define __study_generator_h

#include "common.h"
#include "configurations.hxx"

namespace studygen
{

class StudyGenerator
{
public:
  StudyGenerator();
  ~StudyGenerator();

  // Set Image4D as input
  void SetImage4D(Image4DType::Pointer img4d);

  // Set number of time points to process
  void SetNumberOfTimePoints(TimePointType nT);

  // Add segmentation configuration
  void AddSegmentationConfig(SegmentationConfig segConfig);

  // Set segmentation configuration list
  void SetSegmentationConfigs(std::vector<SegmentationConfig> &segConfigList);


private:
  Image4DType::Pointer m_Image4D;
  std::vector<SegmentationConfig> m_SegConfigs;
  TimePointType m_NumberOfTimePoints;
};

}

#endif // #ifndef __study_generator_h