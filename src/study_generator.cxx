#include "study_generator.h"
#include "image_helpers.hxx"
#include "mesh_helpers.h"
#include "study_generator_data.hxx"
#include "format_exception.hxx"
#include "mesh_processor.hxx"

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

void
StudyGenerator
::PrepareInputData()
{
  using ihelpers = ImageHelpers;
  using mhelpers = MeshHelpers;

  std::cout << "-- preparing input data..." << std::endl;
  ValidateInput();
  
  std::cout << "---- reading 4d image..." << std::endl;
  m_Data.image4D = ihelpers::ReadImage<Image4DType>(m_Config.fnImage4D);
  // m_Data.image4D->Print(std::cout);

  // Process Seg Configs
  std::cout << "---- processing segmentation configs..." << std::endl;
  for (auto &sc : m_Config.segConfigList)
  {
    std::cout << "------ refTP: " << sc.refTP << std::endl;
    auto segImg = ihelpers::ReadImage<LabelImage3DType>(sc.fnRefSeg);
    auto &tpData = m_Data.tpInputData.at(sc.refTP - 1);
    tpData.seg = segImg;
    tpData.labelMeshMap = MeshProcessor::GenerateLabelMeshMap(segImg, sc.labelConfigMap);

    for (auto &[lb, mesh] : tpData.labelMeshMap)
      {
      std::string fndbg =
        ssprintf("/Users/jileihao/data/studygen/debug/mesh_tp%02d_lb%02d.vtp", sc.refTP, lb);
      mhelpers::WriteMesh(mesh, fndbg);
      }
  }
}


int
StudyGenerator
::Run()
{
  std::cout << "[StudyGenerator] Run" << std::endl;

  PrepareInputData();

  
  // Create Applicaiton Data

  return EXIT_SUCCESS;
}

