#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "image_helpers.hxx"


int main (int argc, char *argv[])
{
  if (argc < 4)
    {
    std::cerr << "Usage: SmoothBImage binaryImage.nii.gz output.nii.gz sigma_in_vox" << std::endl;
    return EXIT_FAILURE;
    }

  std::string fnIn = argv[1];
  std::string fnOut = argv[2];
  double sigma = std::stod(argv[3]);

  using namespace studygen;
  using ih = ImageHelpers;

  auto imgIn = ih::ReadImage<LabelImage3DType>(fnIn);
  LabelImage3DType::Pointer imgTail = imgIn;

  LabelImage3DType::RegionType trimmedRegion;
  auto trimmedImg = ih::TrimImage<LabelImage3DType>(imgTail, 5, trimmedRegion);

  auto smImg = ih::SmoothImage<LabelImage3DType>(trimmedImg, sigma);
  imgTail = ih::ThresholdImage<Image3DType, LabelImage3DType>(smImg, 0.5, REALTYPE_INFINITY, 1, 0);
  imgTail = ih::RestoreImage<LabelImage3DType>(imgTail, imgIn, trimmedRegion);

  ih::WriteImage<LabelImage3DType>(imgTail, fnOut);

  return EXIT_SUCCESS;
}