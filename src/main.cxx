#include "common.h"
#include "study_generator.h"
#include "config_factories.h"
#include "format_exception.hxx"

using namespace studygen;

int main (int argc, char *argv[])
{
  StudyGenerator SG;
  StudyGenConfig config = StudyGenConfigFactory::CreateFromArgs(argc, argv);
  //config.Print(std::cout);

  SG.SetStudyGenConfig(config);

  int rc = EXIT_FAILURE;

  try
  {
    rc = SG.Run();
  }
  catch (std::exception &ex)
  {
    std::cerr << std::endl << std::endl;
    std::cerr << "Exception caught during the generator run: " << std::endl;
    std::cerr << ex.what() << std::endl << std::endl;
  }

  if (rc == EXIT_SUCCESS)
    std::cout << "Generator run completed successfully!" << std::endl;

  return rc;
}
