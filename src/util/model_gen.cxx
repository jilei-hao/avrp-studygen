#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "image_helpers.hxx"
#include "mesh_helpers.hxx"
#include "mesh_processor.hxx"
#include "config_factories.h"


int main (int argc, char *argv[])
{
  if (argc < 4)
    {
    std::cerr << "Usage: ModelGen image.nii.gz dirOut fnConfig" << std::endl;
    return EXIT_FAILURE;
    }

  std::string fnImage = argv[1];
  std::string dirOut = argv[2];
  std::string fnConfig = argv[3];

  using namespace studygen;
  using ih = ImageHelpers;
  using mh = MeshHelpers;

  std::cout << "-- Reading Image ..." << std::endl;
  auto image = ih::ReadImage<LabelImage3DType>(fnImage);

  std::cout << "-- Reading Label Configs ... " << std::endl;
  auto labelConfigMap = LabelConfigFactory::CreateFromConfigFile(fnConfig);

  std::cout << "-- Generating Label Meshes ... " << std::endl;
  auto labelMeshMap = MeshProcessor::GenerateLabelMeshMap(image, labelConfigMap);

  // Write out the label meshes
  for (auto [label, meshPtr] : labelMeshMap)
  {
    std::string fnMesh = dirOut + "/label_" + std::to_string(label) + ".vtp";
    mh::WriteMesh(meshPtr, fnMesh);
  }

  std::vector<MeshPointer> labelMeshes;

  for (auto [label, meshPtr] : labelMeshMap)
  {
    mh::CreateAndFillCellDataWithLabel(meshPtr, "Label", label);
    labelMeshes.push_back(meshPtr);
  }

  std::cout << "-- Assembling Meshes ... " << std::endl;

  auto assembledMesh = mh::AssembleMeshes(labelMeshes);

  std::cout << "-- Writing Mesh ..." << std::endl;
  MeshHelpers::WriteMesh(assembledMesh, dirOut + "/assembled_mesh.vtp");

  return EXIT_SUCCESS;
}