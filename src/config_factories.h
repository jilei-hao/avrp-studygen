#ifndef __config_factories_h
#define __config_factories_h

#include "common.h"
#include "configurations.hxx"


namespace studygen
{

class SegConfigFactory
{
public:
  SegConfigFactory();
  ~SegConfigFactory();

  void SetReferenceSegmentation(LabelImage3DType::Pointer refSeg);

  void SetReferenceTimePoint(TimePointType refTp);

  void SetTargetTimePoints(std::vector<TimePointType> &tpList);
  

private:
  LabelImage3DType::Pointer m_ReferenceSegmentation;
  TimePointType m_RefTP;
  std::vector<TimePointType> m_TPList;

};

class LabelMeshConfigFactory
{
public:
  LabelMeshConfigFactory();
  ~LabelMeshConfigFactory();

};

} // namespace studygen

#endif // __config_factories_h