#include "common.h"
#include "study_generator.h"
#include "config_factories.h"

using namespace studygen;

int main (int argc, char *argv[])
{
  StudyGenerator SG;
  StudyGenConfig config = StudyGenConfigFactory::CreateFromArgs(argc, argv);
  config.Print(std::cout);
  SG.SetStudyGenConfig(config);
  return SG.Run();
}