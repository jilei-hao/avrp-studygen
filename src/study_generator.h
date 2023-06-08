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

  std::map<TimePointType, TimePointData> GetOutputData();

private:
  void ValidateInput();
  void PrepareInputData();
  void RunPropagations();
  void PropagateSegmentation(SegmentationConfig &segConfig);
  PropagationInputPointer CreatePropagationInput(SegmentationConfig &segConfig);
  std::string GetLabelMeshTag(LabelType label) const;

  // Get a list of labels processed by the generator
  std::vector<LabelType> GetLabelList() const;

  StudyGeneratorData m_Data;
  StudyGenConfig m_Config;
  bool m_Initialized = false;
  const char *labelMeshTag = "LabelMesh";
};

}

#endif // #ifndef __study_generator_h
