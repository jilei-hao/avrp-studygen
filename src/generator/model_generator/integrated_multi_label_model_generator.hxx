#ifndef INTEGRATED_MULTI_LABEL_MODEL_GENERATOR_HXX
#define INTEGRATED_MULTI_LABEL_MODEL_GENERATOR_HXX

#include <string>
#include "abstract_model_generator.hxx"
#include "common.h"

class IntegratedMultiLabelModelGenerator 
  : public AbstractModelGenerator<LabelImage3DType, MeshType>
{
public:
  IntegratedMultiLabelModelGenerator(std::string config_file_path) : AbstractModelGenerator(config_file_path) {}
  ~IntegratedMultiLabelModelGenerator() {}
  IntegratedMultiLabelModelGenerator(const IntegratedMultiLabelModelGenerator &other) = delete;
  IntegratedMultiLabelModelGenerator &operator=(const IntegratedMultiLabelModelGenerator &other) = delete;

  void GenerateModel() override
  {

  }

  void SetInput(LabelImage3DType *input) override
  {

  }

protected:
  static MeshPointer
  RunMeshSteps(MeshPointer mesh, std::vector<MeshStep> &meshSteps, LabelType lb)
  {
    std::cout << "MeshProcessor::RunMeshSteps: Label: " << lb << std::endl;
    MeshPointer meshTail = mesh;
    for (auto &ms : meshSteps)
      {
      auto trMesh = mhelpers::TriangulateMesh(meshTail);
      auto dcMesh = mhelpers::Decimate(trMesh, ms.decimateRate);
      meshTail = mhelpers::TaubinSmooth(dcMesh, ms.smoothIteration, ms.smoothPassband);
      }

    return meshTail;
  }
};

#endif