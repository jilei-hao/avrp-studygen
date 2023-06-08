#ifndef __configurations_hxx
#define __configurations_hxx

#include "common.h"
#include <ostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace studygen
{

struct MeshStep
{
  double decimateRate;
  uint16_t smoothIteration = 20;
  double smoothPassband = 0.1;

  MeshStep() {}
  MeshStep(double dc, uint16_t iter, double pb)
    : decimateRate(dc), smoothIteration(iter), smoothPassband(pb) {};
  
  void Print(std::ostream &os, std::string prefix = "") const
  {
    os << prefix << "-- [Mesh Step] decimation: " << decimateRate
                 << " , iter: " << smoothIteration
                 << " , passband: " << smoothPassband << std::endl;
  }
};

inline void to_json(json& j, const MeshStep& ms) {
  j = json
  { 
    {"DecimationRate", ms.decimateRate}, 
    {"SmoothIteration", ms.smoothIteration}, 
    {"SmoothPassband", ms.smoothPassband} 
  };
};

inline void from_json(const json& j, MeshStep& ms) {
  j.at("DecimationRate").get_to(ms.decimateRate);
  j.at("SmoothIteration").get_to(ms.smoothIteration);
  j.at("SmoothPassband").get_to(ms.smoothPassband);
};

struct ImageStep
{
  uint16_t resampleRate;
  double gaussianSigmaInVox = 1.0;
  double gaussianCutOff = 0.5;

  ImageStep() {}
  ImageStep(uint16_t rs, double sigma, double cutoff)
    :resampleRate(rs), gaussianSigmaInVox(sigma), gaussianCutOff(cutoff) {};

  void Print(std::ostream &os, std::string prefix = "") const
  {
    os << prefix << "-- [Image Step] rs: " << resampleRate
                 << ", sigma: " << gaussianSigmaInVox << "vox" 
                 << ", cutoff: " << gaussianCutOff << std::endl;
  }
};

inline void to_json(json& j, const ImageStep& is) {
  j = json
  { 
    {"ResampleRate", is.resampleRate}, 
    {"GaussianSigmaInVox", is.gaussianSigmaInVox}, 
    {"GaussianCutOff", is.gaussianCutOff} 
  };
};

inline void from_json(const json& j, ImageStep& is) {
  j.at("ResampleRate").get_to(is.resampleRate);
  j.at("GaussianSigmaInVox").get_to(is.gaussianSigmaInVox);
  j.at("GaussianCutOff").get_to(is.gaussianCutOff);
};

struct LabelConfig
{
  std::vector<ImageStep> imgSteps;
  std::vector<MeshStep> meshSteps;
  
  void Print(std::ostream &os, std::string prefix = "") const
  {
    os << prefix << "[LabelMeshConfig]" << std::endl;
    os << prefix << "-- Image Steps: " << std::endl;
    for (auto &is : imgSteps)
      is.Print(os, prefix + " ");
    os << prefix << "-- Mesh Steps: " << std::endl;
    for (auto &ms : meshSteps)
      ms.Print(os, prefix + " ");
  }
};

struct SegmentationConfig
{
  std::string fnRefSeg;
  TimePointType refTP;
  std::vector<TimePointType> targetTPList;
  std::map<LabelType, LabelConfig> labelConfigMap;

  void Print(std::ostream &os, std::string prefix = "") const
  {
    os << prefix << "[SegmentationConfig] (" << this << ")" << std::endl;
    os << prefix << "-- Seg Filename: " << fnRefSeg << std::endl;
    os << prefix << "-- Reference Time Point: " << refTP << std::endl;
    os << prefix << "-- Target Time Points: ";
    for (auto tp : targetTPList)
      os << tp << " ";
    os << std::endl;
  }
};

struct StudyGenConfig
{
  std::string fnImage4D;
  TimePointType nT;
  std::vector<SegmentationConfig> segConfigList;
  std::map<LabelType, LabelConfig> labelConfigMap;

  void Validate()
  {
    // seg config should not be empty
    if (segConfigList.size() == 0)
      throw FormatException("[SegGenConfig::Validate] segConfigList is empty!");

    // seg config target time points should cover all timepoints from 1 to nT
    std::set<TimePointType> tpSet;
    for (auto &sc : segConfigList)
      {
      tpSet.insert(sc.refTP);
      for (auto tp : sc.targetTPList)
        tpSet.insert(tp);
      }


    if (tpSet.size() != nT)
      throw FormatException("[SegGenConfig::Validate] number of tps covered by"
                            " the seg configs %d does not equal to the number of total tps %d",
                            tpSet.size(), nT);

    for (auto tp = 1u; tp <= nT; ++tp)
      {
      if (!tpSet.count(tp))
        throw FormatException("[SegGenConfig::Validate] tp %d is not covered by any seg config", tp);
      }

  }

  void Print(std::ostream &os, std::string prefix = "") const
  {
    os << prefix << "[StudyGenConfig] (" << this << ")" << std::endl;
    os << prefix << "-- Image 4D Filename: " << fnImage4D << std::endl;
    os << prefix << "-- Number of Time Point Requested: " << nT << std::endl;
    os << prefix << "-- Numer of Segmentation Configs: " << segConfigList.size() << std::endl;
    os << prefix << "-- Label Configs: " << std::endl;
    for (auto &kv : labelConfigMap)
    {
      os << prefix << " -- Label " << kv.first << ":" << std::endl;
      kv.second.Print(os, prefix + " ");
    }
    for (auto &sc : segConfigList)
      sc.Print(os, prefix + "  ");
  }
};

}
#endif // __configurations_hxx
