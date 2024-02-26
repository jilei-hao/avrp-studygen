#ifndef __mesh_processor_hxx_
#define __mesh_processor_hxx_

#include "common.hxx"
#include "configurations.hxx"
#include "image_helpers.hxx"
#include "mesh_helpers.hxx"

namespace studygen
{

class MeshProcessor
{
public:
  MeshProcessor() {}
  ~MeshProcessor() {}

  using ihelpers = ImageHelpers;
  using mhelpers = MeshHelpers;

  static std::map<LabelType, MeshPointer> GenerateLabelMeshMap(
    LabelImage3DType::Pointer segImg, 
    std::map<LabelType, LabelConfig> &labelConfigMap)
  {
    std::cout << "MeshProcessor::GenerateLabelMeshMap" << std::endl;
    std::map<LabelType, MeshPointer> labelMeshMap;
    
    for(auto &[lb, lc] : labelConfigMap)
    {
      std::cout << "-- processing label: " << lb << std::endl;
      auto bImage = ihelpers::ThresholdImage<LabelImage3DType, LabelImage3DType>(segImg, lb, lb, 1, 0);
      auto processedImg = RunImageSteps(bImage, lc.imgSteps, lb);
      auto rawMesh = mhelpers::GetMeshFromBinaryImage(processedImg);
      auto processedMesh = RunMeshSteps(rawMesh, lc.meshSteps, lb);
      labelMeshMap.insert(std::make_pair(lb, processedMesh));
    }

    return labelMeshMap;
  }
private:
  static LabelImage3DType::Pointer
  RunImageSteps(LabelImage3DType::Pointer image, std::vector<ImageStep> &imgSteps, LabelType lb)
  {
    std::cout << "MeshProcessor::RunImageSteps: Label: " << lb << std::endl;
    LabelImage3DType::Pointer imgTail = image;
    for (auto &is : imgSteps)
      {
      auto rsImg = ihelpers::ResampleImage<LabelImage3DType>(imgTail, is.resampleRate/100.0, ihelpers::NN);
      auto smImg = ihelpers::SmoothImage<LabelImage3DType>(rsImg, is.gaussianSigmaInVox);
      imgTail = ihelpers::ThresholdImage<Image3DType, LabelImage3DType>(
            smImg, is.gaussianCutOff, REALTYPE_INFINITY, 1, 0);
      }

    return imgTail;
  }

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

}

#endif // __mesh_processor_hxx_
