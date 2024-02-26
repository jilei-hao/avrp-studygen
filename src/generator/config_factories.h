#ifndef __config_factories_h
#define __config_factories_h

#include "common.hxx"
#include "configurations.hxx"

namespace studygen
{

class LabelConfigFactory
{
public:
  LabelConfigFactory();
  ~LabelConfigFactory();

  // create a pre-defined label mesh config. size: S, M, L
  static LabelConfig CreateBySize(char size);

  static std::map<LabelType, LabelConfig> CreateDefaultMap();

  static std::map<LabelType, LabelConfig> CreateFromConfigFile(std::string filename);
};

class StudyGenConfigFactory
{
public:
  StudyGenConfigFactory();
  ~StudyGenConfigFactory();

  // create a config using commandline parser
  static StudyGenConfig CreateFromArgs(int argc, char *argv[]);

};

} // namespace studygen

#endif // __config_factories_h