#ifndef __study_generator_h
#define __study_generator_h

#include "common.h"
#include "configurations.hxx"
#include "study_generator_data.hxx"
#include <propagation/PropagationIO.h>
#include <propagation/PropagationAPI.h>
#include <memory>

using namespace propagation;

namespace studygen
{

class StudyGenerator
{
public:
  StudyGenerator();
  ~StudyGenerator();

  using TReal = RealType;
  PROPAGATION_DATA_TYPEDEFS
  using PropagationInputType = PropagationInput<TReal>;
  using PropagationInputPointer = std::shared_ptr<PropagationInputType>;

  void SetStudyGenConfig(StudyGenConfig config);

  int Run();

private:
  void ValidateInput();
  void PrepareInputData();
  void RunPropagations();
  void PropagateSegmentation(SegmentationConfig &segConfig);
  PropagationInputPointer CreatePropagationInput(SegmentationConfig &segConfig);

  StudyGeneratorData m_Data;
  StudyGenConfig m_Config;
  bool m_Initialized = false;
};

}

#endif // #ifndef __study_generator_h
