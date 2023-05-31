#include "study_generator.h"
#include "image_helpers.hxx"
#include "study_generator_data.hxx"

using namespace studygen;

StudyGenerator
::StudyGenerator()
{

}

StudyGenerator
::~StudyGenerator()
{

}

void 
StudyGenerator
::SetStudyGenConfig(StudyGenConfig config)
{
  m_Config = config;
}

void
StudyGenerator
::ValidateInput()
{

}


int
StudyGenerator
::Run()
{
  std::cout << "[StudyGenerator] Run" << std::endl;

  // Process Inputs
  ValidateInput();

  using ihelpers = ImageHelpers;

  m_Data.m_Image4D = ihelpers::ReadImage<Image4DType>(m_Config.fnImage4D);


  // Generate Segmentation Mesh

  // Run Propagation

  // Create Applicaiton Data

  return EXIT_SUCCESS;
}

