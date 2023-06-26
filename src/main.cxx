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
  else
    return rc;

  sg.WriteOutput();

  return EXIT_SUCCESS;
}
