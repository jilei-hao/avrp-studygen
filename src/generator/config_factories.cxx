#include "config_factories.h"
#include "cl_helper.hxx"
#include "configurations.hxx"
#include "usage.hxx"
#include "format_exception.hxx"

#include <fstream>
#include <nlohmann/json.hpp>

using namespace studygen;
using json = nlohmann::json;

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
LabelConfigFactory::CreateFromConfigFile(std::string filename)
{
  std::ifstream f(filename);
  json data = json::parse(f);

  auto options = data["Options"];
  std::map<std::string, LabelConfig> lcOptions;
  for (auto &item : options)
  {
    std::string name = item["Name"];
    LabelConfig lc;

    for (auto &is : item["ImageSteps"])
    {
      ImageStep s = is.get<ImageStep>();
      lc.imgSteps.push_back(s);
    }
    for (auto &ms : item["MeshSteps"])
    {
      MeshStep s = ms.get<MeshStep>();
      lc.meshSteps.push_back(s);
    }
    lcOptions.insert(std::make_pair(name, lc));
  }

  auto labels = data["LabelConfigs"];

  std::map<LabelType, LabelConfig> configMap;
  for (auto &item : labels)
  {
    LabelType lb = item["Label"];
    std::string optionKey = item["Option"];

    if (!lcOptions.count(optionKey))
      throw FormatException("LabelConfigFactory::CreateFromConfigFile"
      " Option \"%s\" does not exist in the Options section!", optionKey.c_str());

    configMap.insert(std::make_pair(lb, lcOptions.at(optionKey)));
  }

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
  size_t seg_config_cnt = 0;
  

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
      if (seg_config_cnt > 0)
      {
        std::cout << "Saving previous Seg Config..." << std::endl;
        config.segConfigList.push_back(CurrentSegConfig);
      }
      
      SegmentationConfig newConfig;
      newConfig.fnRefSeg = cl.read_existing_filename();
      CurrentSegConfig = newConfig;

      ++seg_config_cnt;
    }

    else if (cmd == "-s_ref")
    {
      CurrentSegConfig.refTP = cl.read_unsigned_long();
    }

    else if (cmd == "-s_tgt")
    {
      CurrentSegConfig.targetTPList = cl.read_uint_vector(',');
    }

    else if (cmd == "-lc")
    {
      std::string fnLabelConfig = cl.read_existing_filename();
      
      // parse json to get a LabelConfig
      auto lcMap = LabelConfigFactory::CreateFromConfigFile(fnLabelConfig);

      // overwrite default LabelConfig in CurrentSegConfig
      config.labelConfigMap = lcMap;
    }

    else if (cmd == "-nt")
    {
      config.nT = cl.read_unsigned_long();
    }

    else if (cmd == "-o")
    {
      config.dirOut = cl.read_output_dir();
    }
  }

  // always push seg config at the end of the parsing
  config.segConfigList.push_back(CurrentSegConfig);

  return config;
}
