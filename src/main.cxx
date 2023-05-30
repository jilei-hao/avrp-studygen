#include "common.h"
#include "usage.hxx"
#include "study_generator.h"
#include "config_factories.h"

using namespace studygen;

int main (int argc, char *argv[])
{
  if (argc < 2)
  {
    usage(std::cerr);
    return EXIT_FAILURE;
  }

  StudyGenerator SG;
  SG.SetStudyGenConfig(StudyGenConfigFactory::CreateFromArgs(argc, argv));
  return SG.Run();
}