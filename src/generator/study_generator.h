#ifndef __study_generator_h
#define __study_generator_h

#include "common.h"
#include "configurations.hxx"
#include "study_generator_data.hxx"
#include <PropagationIO.h>
#include <PropagationAPI.h>
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
  using PropagationOutputType = PropagationOutput<TReal>;
  using PropagationOutputPointer = std::shared_ptr<PropagationOutputType>;

  void SetStudyGenConfig(StudyGenConfig config);

  int Run();

  /** Get a map of output data indexed by time points */
  std::map<TimePointType, TimePointData> GetOutputData();

  /** Get the output image data for a time point */
  Image3DType::Pointer GetImage(TimePointType tp);

  /** Get the output segmentation data for a time point */
  LabelImage3DType::Pointer GetSegmentation(TimePointType tp);

  /** Get the unified mesh for a time point */
  MeshPointer GetUnifiedMesh(TimePointType tp);

  /** Get the mesh of a label for a time point */
  MeshPointer GetLabelMesh(TimePointType tp, LabelType label);

  /** Write output data */
  void WriteOutput();

private:
  void ValidateInput();
  void PrepareInputData();
  void RunPropagations();
  PropagationOutputPointer PropagateSegmentation(SegmentationConfig &segConfig);

  /** write out files for model input */
  void WriteVolumes();
  void WriteSegmentations();
  void WriteModels();
  void WriteUnifiedModels();
  void WriteLabelModels();

  /** fill propagation output to tp data */
  void ProcessPropagationOutput(SegmentationConfig &segConfig, PropagationOutputPointer propaOut);

  /** final step generating study data */
  void GenerateStudyData();

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
