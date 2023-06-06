#include "study_generator.h"
#include "image_helpers.hxx"
#include "mesh_helpers.h"
#include "study_generator_data.hxx"
#include "format_exception.hxx"
#include "mesh_processor.hxx"
#include <GreedyParameters.h>

using namespace studygen;
using ihelpers = ImageHelpers;
using mhelpers = MeshHelpers;


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
  std::cout << "-- preparing input data ..." << std::endl;
  ValidateInput();
  
  std::cout << "---- reading 4d image ..." << std::endl;
  m_Data.image4D = ihelpers::ReadImage<Image4DType>(m_Config.fnImage4D);
  // m_Data.image4D->Print(std::cout);

  // Process Seg Configs
  std::cout << "---- processing segmentation configs ..." << std::endl;
  for (auto &sc : m_Config.segConfigList)
    {
    std::cout << "------ refTP: " << sc.refTP << std::endl;
    auto segImg = ihelpers::ReadImage<LabelImage3DType>(sc.fnRefSeg);
    auto &tpData = m_Data.tpInputData.at(sc.refTP);
    tpData.segmentation = segImg;
    tpData.labelMeshMap = MeshProcessor::GenerateLabelMeshMap(segImg, sc.labelConfigMap);

    for (auto &[lb, mesh] : tpData.labelMeshMap)
      {
      std::string fndbg =
        ssprintf("/Users/jileihao/data/studygen/debug/mesh_tp%02d_lb%02d.vtp", sc.refTP, lb);
      mhelpers::WriteMesh(mesh, fndbg);
      }
    }
}

StudyGenerator
::PropagationInputPointer
StudyGenerator
::CreatePropagationInput(SegmentationConfig &segConfig)
{
  std::cout << "---- Creating propagation input ..." << std::endl;
  PropagationInputBuilder<TReal> ib;
  ib.SetImage4D(m_Data.image4D);

  TimePointType refTP = segConfig.refTP;
  auto &refTPData = m_Data.tpInputData.at(refTP);
  std::cout << "---- refseg: " << refTPData.segmentation << std::endl;
  ib.SetReferenceSegmentationIn3D(refTPData.segmentation);
  ib.SetReferenceTimePoint(refTP);
  ib.SetTargetTimePoints(segConfig.targetTPList);
  ib.SetAffineDOF(GreedyParameters::AffineDOF::DOF_RIGID);
  ib.SetMultiResolutionSchedule({10, 10});
  ib.SetResliceMetricToLabel(0.2, false);
  ib.SetGreedyVerbosity(GreedyParameters::VERB_NONE);
  ib.SetPropagationVerbosity(PropagationParameters::VERB_DEFAULT);

  return ib.BuildPropagationInput();
}

void
StudyGenerator
::PropagateSegmentation(SegmentationConfig &segConfig)
{
  TimePointType refTP = segConfig.refTP;

  std::cout << "-- Running propagation from tp: " << refTP
            << " to tps:";
  for (auto tp : segConfig.targetTPList)
    std::cout << " " << tp;
  std::cout << std::endl;

  PropagationAPI<TReal> api(CreatePropagationInput(segConfig));

  try
    {
    api.Run();
    }
  catch (std::exception &e)
    {
    throw(FormatException("Error from PropagationAPI run:\n %s", e.what()));
    }

  auto output = api.GetOutput();

  // put result to target time points in the outputTPData
  for (auto tp : segConfig.targetTPList)
    {
    m_Data.tpOutputData.at(tp).segmentation = output->GetSegmentation3D(tp);
    }

  // copy reference segmentation over to output data

  m_Data.tpOutputData.at(refTP) = m_Data.tpInputData.at(refTP);
}

void
StudyGenerator
::RunPropagations()
{
  std::cout << "-- Running propagations ... " << std::endl;

  for (auto &sc : m_Config.segConfigList)
    {
    PropagateSegmentation(sc);
    }
}




int
StudyGenerator
::Run()
{
  std::cout << "[StudyGenerator] Run" << std::endl;

  PrepareInputData();

  RunPropagations();

  
  // Create Applicaiton Data

  return EXIT_SUCCESS;
}

