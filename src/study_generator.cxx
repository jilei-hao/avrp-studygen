#include "study_generator.h"

using namespace studygen;

StudyGenerator
::StudyGenerator()
{

}

StudyGenerator
::~StudyGenerator()
{

}

void
StudyGenerator
::SetImage4D(Image4DType::Pointer img4d)
{
  m_Image4D = img4d;
}

void
StudyGenerator
::SetNumberOfTimePoints(TimePointType nT)
{
  m_NumberOfTimePoints = nT;
}

void
StudyGenerator
::AddSegmentationConfig(SegmentationConfig segConfig)
{
  m_SegConfigs.push_back(segConfig);
}

void
StudyGenerator
::SetSegmentationConfigs(std::vector<SegmentationConfig> &segConfigList)
{
  m_SegConfigs = segConfigList;
}
