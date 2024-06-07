#include "study_generator.h"
#include "image_helpers.hxx"
#include "mesh_helpers.hxx"
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
  m_Config.Validate();

  if (!m_Initialized)
    throw FormatException("StudyGenerator::ValidateInput: generator not initialized!");
}

void
StudyGenerator
::WriteVolumes()
{
  std::cout << "-- writing volumes..." << std::endl;
  for (auto &[tp, data] : GetOutputData())
  {
    std::string fn = ssprintf("%s/img_%02d.vti", m_Config.dirOut.c_str(), tp);
    ImageHelpers::WriteVTKImage<Image3DType>(data.image, fn);
  }
}

void
StudyGenerator
::WriteSegmentations()
{
  std::cout << "-- writing segmentations..." << std::endl;

  for (auto &[tp, data] : GetOutputData())
  {
    std::string fn = ssprintf("%s/seg_%02d.vti", m_Config.dirOut.c_str(), tp);
    ImageHelpers::WriteVTKImage<LabelImage3DType>(data.segmentation, fn);
  }
}

void
StudyGenerator
::WriteSegmentation4D()
{
  std::cout << "-- writing 4d segmentation..." << std::endl;
  std::string fn = ssprintf("%s/seg4d.nii.gz", m_Config.dirOut.c_str());
  ImageHelpers::WriteImage<LabelImage4DType>(m_Data.segmentation4D, fn);
}

void
StudyGenerator
::WriteImage4D()
{
  std::cout << "-- writing 4d image..." << std::endl;
  std::string fn = ssprintf("%s/img4d.nii.gz", m_Config.dirOut.c_str());
  ImageHelpers::WriteImage<Image4DType>(m_Data.image4D, fn);
}

void
StudyGenerator
::WriteModels()
{
  std::cout << "-- writing models..." << std::endl;
  WriteSingleLabelModels();
  WriteLabelModels();
  WriteAssembledModels();
}

void
StudyGenerator
::WriteSingleLabelModels()
{
  std::cout << "-- writing single label models..." << std::endl;

  for (auto &[tp, data] : GetOutputData())
  {
    std::string fn = ssprintf("%s/model-sl_%02d.vtp", m_Config.dirOut.c_str(), tp);
    MeshHelpers::WriteMesh(data.unifiedMesh, fn);
  }
}

void
StudyGenerator
::WriteLabelModels()
{
  std::cout << "-- writing label models..." << std::endl;

  for (auto &[tp, data] : GetOutputData())
  {
    for (auto &[lb, mesh] : data.labelMeshMap)
    {
      std::string fn = ssprintf("%s/mesh_lb%02d_tp%02d.vtp", m_Config.dirOut.c_str(), lb, tp);
      MeshHelpers::WriteMesh(data.labelMeshMap.at(lb), fn);
    }
  }
}

void
StudyGenerator
::WriteAssembledModels()
{
  std::cout << "-- writing assembled models..." << std::endl;

  for (auto &[tp, data] : GetOutputData())
  {
    std::string fn = ssprintf("%s/assembled_mesh_tp%02d.vtp", m_Config.dirOut.c_str(), tp);
    MeshHelpers::WriteMesh(data.assembledMesh, fn);
  }
}

void
StudyGenerator
::WriteOutput()
{
  std::cout << "-- writing output..." << std::endl;
  WriteVolumes();
  WriteSegmentations();
  WriteModels();
  WriteSegmentation4D();
  WriteImage4D();
}

std::map<TimePointType, studygen::TimePointData>
StudyGenerator
::GetOutputData()
{
  return m_Data.tpStudyData;
}

Image3DType::Pointer
StudyGenerator
::GetImage(TimePointType tp)
{
  return m_Data.tpStudyData.at(tp).image;
}

LabelImage3DType::Pointer
StudyGenerator
::GetSegmentation(TimePointType tp)
{
  return m_Data.tpStudyData.at(tp).segmentation;
}

MeshPointer
StudyGenerator
::GetUnifiedMesh(TimePointType tp)
{
  return m_Data.tpStudyData.at(tp).unifiedMesh;
}

MeshPointer
StudyGenerator
::GetLabelMesh(TimePointType tp, LabelType label)
{
  return m_Data.tpStudyData.at(tp).labelMeshMap.at(label);
}

std::string
StudyGenerator
::GetLabelMeshTag(LabelType label) const
{
  return ssprintf("%s_%d", labelMeshTag, label);
}

std::vector<LabelType>
StudyGenerator
::GetLabelList() const
{
  std::vector<LabelType> ret;
  // always use first seg config as the label list
  for (auto &[label, config] : m_Config.labelConfigMap)
    {
    ret.push_back(label);
    }

  return ret;
}


void
StudyGenerator
::PrepareInputData()
{
  std::cout << "-- preparing input data..." << std::endl;
  ValidateInput();
  
  std::cout << "---- reading 4d image..." << std::endl;
  std::cout << "---- filename: " << m_Config.fnImage4D << std::endl;
  m_Data.image4D = ihelpers::ReadImage<Image4DType>(m_Config.fnImage4D);

  // Process Seg Configs
  std::cout << "---- processing segmentation configs..." << std::endl;

  // Get region to trim the image
  LabelImage3DType::RegionType trimmedRegion;
  LabelImage3DType::Pointer firstSegImg = nullptr;

  for (auto &sc : m_Config.segConfigList)
    {
    std::cout << "------ refTP: " << sc.refTP << std::endl;
    auto segImg = ihelpers::ReadImage<LabelImage3DType>(sc.fnRefSeg);

    // Trim the Seg Image
    if (m_Config.trim)
      {
      if (!firstSegImg)
        {
          firstSegImg = segImg;
          trimmedRegion = ihelpers::GetTrimmedRegion<LabelImage3DType>(segImg, 5, m_Config.trimmedRegionScale);
        }
       
      std::cout << "------ trimming seg image..." << std::endl;
      segImg = ihelpers::ExtractRegion<LabelImage3DType>(segImg, trimmedRegion);
      }

    auto &tpData = m_Data.tpData.at(sc.refTP);
    tpData.segmentation = segImg;
    tpData.labelMeshMap = MeshProcessor::GenerateLabelMeshMap(segImg, m_Config.labelConfigMap);
    }

  // Trim the Image
  if (m_Config.trim)
    {
    std::cout << "---- trimming image..." << std::endl;
    std::vector<Image3DType::Pointer> tpImgList;
    for (unsigned int i = 0; i < m_Config.nT; ++i)
      {
      std::cout << "------ processing tp: " << i << std::endl;

      auto tpImg = ihelpers::ExtractTimePointImage<Image3DType, Image4DType>(m_Data.image4D, i);
      tpImg = ihelpers::ExtractRegion<Image3DType>(tpImg, trimmedRegion);
      tpImgList.push_back(tpImg);
      }

    m_Data.image4D = ihelpers::CreateTimeSeriesImageFromList<Image3DType, Image4DType>(tpImgList);
    }

}

StudyGenerator
::PropagationInputPointer
StudyGenerator
::CreatePropagationInput(SegmentationConfig &segConfig)
{
  std::cout << "---- Creating propagation input..." << std::endl;
  PropagationInputBuilder<TReal> ib;
  ib.SetImage4D(m_Data.image4D);

  TimePointType refTP = segConfig.refTP;
  auto &refTPData = m_Data.tpData.at(refTP);
  ib.SetReferenceSegmentationIn3D(refTPData.segmentation);
  ib.SetReferenceTimePoint(refTP);
  ib.SetTargetTimePoints(segConfig.targetTPList);
  ib.SetAffineDOF(GreedyParameters::AffineDOF::DOF_RIGID);
  ib.SetMultiResolutionSchedule({10, 10});
  ib.SetResliceMetricToLabel(0.2, false);
  ib.SetGreedyVerbosity(GreedyParameters::VERB_NONE);
  ib.SetPropagationVerbosity(PropagationParameters::VERB_DEFAULT);

  // add label mesh
  std::vector<MeshPointer> labelMeshes;

  for (auto &[label, mesh] : refTPData.labelMeshMap)
    {
    MeshHelpers::CreateAndFillCellDataWithLabel(mesh, "Label", label);
    labelMeshes.push_back(mesh);
    auto lbMeshTag = GetLabelMeshTag(label);
    ib.AddExtraMeshToWarp(mesh, lbMeshTag);
    }

  // add assembled mesh

  std::cout << "-- Add Assembled Mesh ... " << std::endl;
  auto assembledMesh = MeshHelpers::AssembleMeshes(labelMeshes);
  std::string meshTag = "AssembledMesh";
  refTPData.assembledMesh = assembledMesh;

  ib.AddExtraMeshToWarp(assembledMesh, meshTag);

  return ib.BuildPropagationInput();
}

StudyGenerator
::PropagationOutputPointer
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

  return api.GetOutput();
}


void
StudyGenerator
::ProcessPropagationOutput(SegmentationConfig &segConfig, PropagationOutputPointer propaOut)
{
  std::cout << "-- Processing propagation output..." << std::endl;

  m_Data.segmentation4D = propaOut->GetSegmentation4D();

  // put result to target time points in the outputTPData
  for (auto tp : segConfig.targetTPList)
    {
    std::cout << "---- Processing tp " << tp << std::endl;
    auto &tpOut = m_Data.tpData.at(tp);
    tpOut.segmentation = propaOut->GetSegmentation3D(tp);
    tpOut.image = propaOut->GetImage3D(tp);
    tpOut.unifiedMesh = propaOut->GetMeshSeries().at(tp);
    for (auto &lb : GetLabelList())
      {
      auto labelMesh = propaOut->GetExtraMesh(GetLabelMeshTag(lb), tp);
      std::cout << "------ label: " << lb << " mesh: " << labelMesh << std::endl;
      tpOut.labelMeshMap.insert(std::make_pair(lb, labelMesh));
      }

    tpOut.assembledMesh = propaOut->GetExtraMesh("AssembledMesh", tp);
    }

  // process reference frame
  TimePointType refTP = segConfig.refTP;
  std::cout << "---- processing reference tp: " << refTP << std::endl;
  m_Data.tpData.at(refTP).image = propaOut->GetImage3D(refTP);
  m_Data.tpData.at(refTP).unifiedMesh = propaOut->GetMeshSeries().at(refTP);
}


void
StudyGenerator
::RunPropagations()
{
  std::cout << "-- Running propagations... " << std::endl;

  for (auto &sc : m_Config.segConfigList)
    {
    auto propaOut = PropagateSegmentation(sc);
    ProcessPropagationOutput(sc, propaOut);
    }
}

void
StudyGenerator
::GenerateStudyData()
{
  std::cout << "-- Generating study data..." << std::endl;
  m_Data.tpStudyData = m_Data.tpData; // direct copy for now. images needs to be resampled
}


int
StudyGenerator
::Run()
{
  std::cout << "[StudyGenerator] Run" << std::endl;

  PrepareInputData();

  RunPropagations();
  
  GenerateStudyData();

  return EXIT_SUCCESS;
}

