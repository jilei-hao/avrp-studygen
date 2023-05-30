#ifndef __configurations_hxx
#define __configurations_hxx

#include "common.h"

namespace studygen
{

struct MeshStep
{
  double decimateRate;
  uint16_t smoothIteration = 20;
  double smoothPassband = 0.1;
  uint16_t smoothFeatureAngle = 0;
  MeshStep(double dc, uint16_t iter, double pb, uint16_t fa)
    : decimateRate(dc), smoothIteration(iter), smoothPassband(pb), smoothFeatureAngle(fa) {};
};

struct ImageStep
{
  uint16_t resampleRate;
  double gaussianSigma;
  ImageStep(uint16_t rs, double sigma)
    :resampleRate(rs), gaussianSigma(sigma) {};
};

struct LabelMeshConfig
{
  std::vector<ImageStep> imgSteps;
  std::vector<MeshStep> meshSteps;
};

struct SegmentationConfig
{
  LabelImage3DType::Pointer refSeg;
  TimePointType refTP;
  std::vector<TimePointType> targetTPList;
  LabelMeshConfig labelMeshConfig;
};

}
#endif // __configurations_hxx