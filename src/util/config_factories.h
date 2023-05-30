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

  // create a pre-defined label mesh config. size: S, M, L
  static LabelMeshConfig CreateBySize(char size);
};

class StudyGenConfigFactory
{
public:
  StudyGenConfigFactory();
  ~StudyGenConfigFactory();

  // create a config using commandline parser
  static StudyGenConfig CreateFromArgs(int argc, char *argv[]);

};

} // namespace studygen

#endif // __config_factories_h