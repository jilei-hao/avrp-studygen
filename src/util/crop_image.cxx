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


  // // Play with the regions
  // auto regionBinary = imgInBinary->GetLargestPossibleRegion();
  // auto regionGrey = imgInGrey->GetLargestPossibleRegion();
  
  // LabelImage3DType::RegionType trimmedRegion;
  // auto trimmedImg = ih::TrimImage<LabelImage3DType>(imgTailBinary, 5, trimmedRegion);

  // std::cout << "Trimmed Region with basic padding: " << std::endl;
  // trimmedRegion.Print(std::cout);

  // auto scaledRegion = ih::ScaleRegion<LabelImage3DType>(trimmedRegion, scale);
  // std::cout << "Scaled Region with scaling : " << std::endl;
  // scaledRegion.Print(std::cout);

  // ih::CropRegionByBoundingBox<LabelImage3DType>(scaledRegion, regionBinary);
  // std::cout << "Cropped Region with bounding box: " << std::endl;
  // scaledRegion.Print(std::cout);
  

  // // Crop the images
  // auto imgOutBinary = ih::ExtractRegion<LabelImage3DType>(imgTailBinary, scaledRegion);
  // ih::WriteImage<LabelImage3DType>(imgOutBinary, fnOutBImage);

  // auto imgOutGrey = ih::ExtractRegion<Image3DType>(imgTailGrey, scaledRegion);
  // ih::WriteImage<Image3DType>(imgOutGrey, fnOutGreyImage);

  auto outPair = ih::TrimImageByMask<Image3DType, LabelImage3DType>(imgInGrey, imgInBinary, scale);
  ih::WriteImage<Image3DType>(outPair.first, fnOutGreyImage);
  ih::WriteImage<LabelImage3DType>(outPair.second, fnOutBImage);
  
  return EXIT_SUCCESS;
}