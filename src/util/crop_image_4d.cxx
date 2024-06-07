#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "image_helpers.hxx"


int main (int argc, char *argv[])
{
  if (argc < 6)
    {
    std::cerr << "Usage: CropImage4D binaryImage.nii.gz greyImage4D.nii.gz scale croppedBinary.nii.gz croppedGrey.nii.gz" << std::endl;
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
  std::cout << "Reading Segmentation Image..." << std::endl;
  auto imgInBinary = ih::ReadImage<LabelImage3DType>(fnInBImage);

  std::cout << "Reading Grey Image..." << std::endl;
  auto imgInGrey = ih::ReadImage<Image4DType>(fnInGreyImage);

  // Trimming
  LabelImage3DType::RegionType trimmedRegion = ih::GetTrimmedRegion<LabelImage3DType>(imgInBinary, 5, scale);
  std::cout << "Trimmed Region: " << trimmedRegion << std::endl;

  std::vector<Image3DType::Pointer> tpImgList;
  for (unsigned int i = 0; i < imgInGrey->GetLargestPossibleRegion().GetSize(3); ++i)
    {
    std::cout << "Processing TP: " << i << std::endl;

    auto tpImg = ih::ExtractTimePointImage<Image3DType, Image4DType>(imgInGrey, i);
    tpImg = ih::ExtractRegion<Image3DType>(tpImg, trimmedRegion);
    tpImgList.push_back(tpImg);
    }

  auto out4D = ih::CreateTimeSeriesImageFromList<Image3DType, Image4DType>(tpImgList);
  auto outBinary = ih::ExtractRegion<LabelImage3DType>(imgInBinary, trimmedRegion);


  std::cout << "Writing 4D Grey Image..." << std::endl;
  ih::WriteImage<Image4DType>(out4D, fnOutGreyImage);

  std::cout << "Writing Segmentation Image..." << std::endl;
  ih::WriteImage<LabelImage3DType>(outBinary, fnOutBImage);
  
  return EXIT_SUCCESS;
}