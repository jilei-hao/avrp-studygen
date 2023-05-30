#include "study_generator.h"

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


int
StudyGenerator
::Run()
{
  // Process Inputs

  // Generate Segmentation Mesh

  // Run Propagation

  // Create Applicaiton Data

  return EXIT_SUCCESS;
}

