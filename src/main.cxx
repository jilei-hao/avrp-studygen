#include "common.h"
#include "study_generator.h"
#include "config_factories.h"
#include "format_exception.hxx"
#include "image_helpers.hxx"
#include "mesh_helpers.h"

using namespace studygen;

int main (int argc, char *argv[])
{
  StudyGenerator sg;
  StudyGenConfig config = StudyGenConfigFactory::CreateFromArgs(argc, argv);
  //config.Print(std::cout);

  sg.SetStudyGenConfig(config);

  int rc = EXIT_FAILURE;

  try
  {
    rc = sg.Run();
  }
  catch (std::exception &ex)
  {
    std::cerr << std::endl << std::endl;
    std::cerr << "Exception caught during the generator run: " << std::endl;
    std::cerr << ex.what() << std::endl << std::endl;
  }

  if (rc == EXIT_SUCCESS)
    std::cout << "Generator run completed successfully!" << std::endl;

  auto output = sg.GetOutputData();

  const char * dirdbg = "/Users/jileihao/data/studygen/debug";

  std::cout << "-- Exporting debugging data..." << std::endl;

  for (auto &[tp, data] : output)
    {
    std::cout << "---- Exporting tp " << tp << std::endl;
    std::cout << "------ seg: " << data.segmentation.GetPointer() << std::endl;

    // export segmentation
    std::string fndbgseg = ssprintf("%s/seg_%02d.nii.gz", dirdbg, tp);
    ImageHelpers::WriteImage<LabelImage3DType>(data.segmentation, fndbgseg);

    // export mesh
    for (auto &[lb, mesh] : data.labelMeshMap)
      {
      std::string fndbgmesh = ssprintf("%s/mesh_lb%02d_tp%02d.vtp", dirdbg, lb, tp);
        MeshHelpers::WriteMesh(data.labelMeshMap.at(lb), fndbgmesh);
      }
    }

  return rc;
}
