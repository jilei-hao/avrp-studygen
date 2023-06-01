#include "study_generator.h"
#include "image_helpers.hxx"
#include "study_generator_data.hxx"
#include "format_exception.hxx"

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
  m_Data.Initialize(m_Config.nT);
  m_Initialized = true;
}

void
StudyGenerator
::ValidateInput()
{
  if (!m_Initialized)
    throw FormatException("StudyGenerator::ValidateInput: generator not initialized!");
}


int
StudyGenerator
::Run()
{
  std::cout << "[StudyGenerator] Run" << std::endl;

  // Process Inputs
  ValidateInput();

  using ihelpers = ImageHelpers;

  m_Data.image4D = ihelpers::ReadImage<Image4DType>(m_Config.fnImage4D);

  m_Data.image4D->Print(std::cout);

  // Process Seg Configs
  for (auto &sc : m_Config.segConfigList)
  {
    auto segImg = ihelpers::ReadImage<LabelImage3DType>(sc.fnRefSeg);
    segImg->Print(std::cout);
    m_Data.tpInputData.at(sc.refTP - 1).seg = segImg;
  }

  // Create Applicaiton Data

  return EXIT_SUCCESS;
}

