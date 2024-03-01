#include "common.hxx"
#include "mesh_helpers.hxx"
#include "image_helpers.hxx"

using namespace studygen;

int main (int argc, char *argv[])
{
  if (argc < 4)
    {
    std::cerr << "Usage: image2mesh image.nii.gz mesh.vtk value" << std::endl;
    return EXIT_FAILURE;
    }

  std::string fnImage = argv[1];
  std::string fnMesh = argv[2];
  double value = std::stod(argv[3]);

  std::cout << "-- Reading Image ..." << std::endl;
  auto image = ImageHelpers::ReadImage<Image3DType>(fnImage);
  auto vtkImg = MeshHelpers::GetVTKImage<Image3DType>(image);

  // std::cout << "-- Running Marching Cubes ..." << std::endl;
  // auto mesh = MeshHelpers::RunMarchingCubes(vtkImg, value);

  std::cout << "-- Running ML Flying Edges ..." << std::endl;
  auto mesh = MeshHelpers::RunMultiLabelFlyingEdges(vtkImg, 1.0);

  mesh = MeshHelpers::ConvertPointDataToCellData(mesh);

  std::cout << "-- Writing Mesh ..." << std::endl;
  MeshHelpers::WriteMesh(mesh, fnMesh);

  return EXIT_SUCCESS;
}
