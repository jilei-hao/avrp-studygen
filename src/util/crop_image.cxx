#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "image_helpers.hxx"


int main (int argc, char *argv[])
{
  if (argc < 6)
    {
    std::cerr << "Usage: CropImage binaryImage.nii.gz greyImage.nii.gz scale croppedBinary.nii.gz croppedGrey.nii.gz" << std::endl;
    return EXIT_FAILURE;
    }


  std::string fnInBImage = argv[1];
  std::string fnInGreyImage = argv[2];
  double scale = std::stod(argv[3]);
  std::string fnOutBImage = argv[4];
  std::string fnOutGreyImage = argv[5];

  using namespace studygen;
  using ih = ImageHelpers;

  // Read Images
  auto imgInBinary = ih::ReadImage<LabelImage3DType>(fnInBImage);
  LabelImage3DType::Pointer imgTailBinary = imgInBinary;

  auto imgInGrey = ih::ReadImage<Image3DType>(fnInGreyImage);
  Image3DType::Pointer imgTailGrey = imgInGrey;

  auto outPair = ih::TrimImageByMask<Image3DType, LabelImage3DType>(imgInGrey, imgInBinary, scale);
  ih::WriteImage<Image3DType>(outPair.first, fnOutGreyImage);
  ih::WriteImage<LabelImage3DType>(outPair.second, fnOutBImage);
  
  return EXIT_SUCCESS;
}