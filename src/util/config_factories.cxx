#include "config_factories.h"
#include "cl_helper.hxx"
#include "configurations.hxx"
#include "usage.hxx"

using namespace studygen;

// LabelConfigFactory
LabelConfigFactory
::LabelConfigFactory()
{

}

LabelConfigFactory
::~LabelConfigFactory()
{

}

LabelConfig
LabelConfigFactory
::CreateBySize(char size)
{
  LabelConfig ret;

  switch(size)
  {
    case 'S': 
    {
      ret.imgSteps.push_back(ImageStep(200, 2, 0.5));
      ret.imgSteps.push_back(ImageStep(200, 2, 0.5));
      ret.meshSteps.push_back(MeshStep(0.2, 20, 0.1));
      ret.meshSteps.push_back(MeshStep(0.5, 20, 0.1));
      ret.meshSteps.push_back(MeshStep(0.5, 20, 0.1));
      break;
    }
    case 'M':
    {
      ret.imgSteps.push_back(ImageStep(200, 2, 0.5));
      ret.imgSteps.push_back(ImageStep(200, 2, 0.5));
      ret.meshSteps.push_back(MeshStep(0.2, 20, 0.1));
      ret.meshSteps.push_back(MeshStep(0.5, 20, 0.1));
      ret.meshSteps.push_back(MeshStep(0.5, 20, 0.1));
      ret.meshSteps.push_back(MeshStep(0.5, 20, 0.1));
      break;
    }
    case 'L':
    {
      ret.imgSteps.push_back(ImageStep(200, 2, 0.5));
      ret.imgSteps.push_back(ImageStep(200, 3, 0.5));
      ret.meshSteps.push_back(MeshStep(0.2, 100, 0.01));
      ret.meshSteps.push_back(MeshStep(0.75, 20, 0.1));
      ret.meshSteps.push_back(MeshStep(0.75, 20, 0.1));
      break;
    }
  }
  return ret;
}

std::map<LabelType, LabelConfig>
LabelConfigFactory::CreateDefaultMap()
{
  std::map<LabelType, LabelConfig> configMap;

  for (size_t i = 1; i <= 20; ++i)
  {
    configMap.insert(std::make_pair(i, CreateBySize('M')));
  }

  return configMap;
}

std::map<LabelType, LabelConfig> 
LabelConfigFactory::CreateFromConfigFile()
{
  std::map<LabelType, LabelConfig> configMap;


  return configMap;
}

// StudyGenConfig Factory
StudyGenConfigFactory
::StudyGenConfigFactory()
{

}

StudyGenConfigFactory
::~StudyGenConfigFactory()
{

}

StudyGenConfig
StudyGenConfigFactory
:: CreateFromArgs(int argc, char *argv[])
{
  std::cout << "[StudyGenConfigFactory] CreateFromArgs" << std::endl;
  StudyGenConfig config;
  CommandLineHelper cl(argc, argv);
  SegmentationConfig CurrentSegConfig;
  

  while (!cl.is_at_end())
  {
    std::string cmd = cl.read_command();

    if (cmd == "-h" || cmd == "-help" || cmd == "--help")
    {
      usage(std::cerr);
      exit(EXIT_FAILURE);
    }

    else if (cmd == "-i")
    {
      config.fnImage4D = cl.read_existing_filename();
    }

    else if (cmd == "-nt")
    {
      config.nT = cl.read_unsigned_long();
    }

    else if (cmd == "-s")
    {
      // push the previous configured seg to studygen config
      if (config.segConfigList.size() > 0)
      {
        config.segConfigList.push_back(CurrentSegConfig);
      }
      
      SegmentationConfig newConfig;
      newConfig.fnRefSeg = cl.read_existing_filename();
      newConfig.labelConfigMap = LabelConfigFactory::CreateDefaultMap();
      CurrentSegConfig = newConfig;
    }

    else if (cmd == "-s_ref")
    {
      CurrentSegConfig.refTP = cl.read_unsigned_long();
    }

    else if (cmd == "-s_tgt")
    {
      CurrentSegConfig.targetTPList = cl.read_uint_vector(',');
    }

    else if (cmd == "-s_lc")
    {
      std::string LabelConfigFile = cl.read_existing_filename();
      
      // parse json to get a LabelConfig

      // overwrite default LabelConfig in CurrentSegConfig
    }
  }

  // always push seg config at the end of the parsing
  config.segConfigList.push_back(CurrentSegConfig);

  return config;
}