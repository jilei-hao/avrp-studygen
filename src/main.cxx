#include "common.h"
#include "study_generator.h"
#include "image_helpers.h"

int main (int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cerr << "Usage: StudyGen image4d.nii.gz" << std::endl;
    return EXIT_FAILURE;
  }

  using namespace studygen;
  using ihelpers = ImageHelpers;

  std::string fnImage4D = argv[1];

  auto image4D = ihelpers::ReadImage<Image4DType>(fnImage4D);

  image4D->Print(std::cout);

  return EXIT_SUCCESS;
}