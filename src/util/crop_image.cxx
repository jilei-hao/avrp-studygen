#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "image_helpers.hxx"


int main (int argc, char *argv[])
{
  if (argc < 4)
    {
    std::cerr << "Usage: CropImage binaryImage.nii.gz output.nii.gz padding" << std::endl;
    return EXIT_FAILURE;
    }


  std::string fnIn = argv[1];
  std::string fnOut = argv[2];
  int padding = std::stoi(argv[3]);

  using namespace studygen;
  using ih = ImageHelpers;

  LabelImage3DType::Pointer imgIn = ih::ReadImage<LabelImage3DType>(fnIn);


  
  return EXIT_SUCCESS;
}