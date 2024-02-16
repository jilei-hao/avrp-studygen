#ifndef INTEGRATED_MULTI_LABEL_MODEL_GENERATOR_HXX
#define INTEGRATED_MULTI_LABEL_MODEL_GENERATOR_HXX

#include <string>
#include "abstract_model_generator.hxx"
#include "common.h"

class IntegratedMultiLabelModelGenerator 
  : public AbstractModelGenerator<LabelImage3DType, MeshType>
{
public:
  IntegratedMultiLabelModelGenerator(std::string config_file_path) : AbstractModelGenerator(config_file_path) {}
  ~IntegratedMultiLabelModelGenerator() {}
  IntegratedMultiLabelModelGenerator(const IntegratedMultiLabelModelGenerator &other) = delete;
  IntegratedMultiLabelModelGenerator &operator=(const IntegratedMultiLabelModelGenerator &other) = delete;

  void GenerateModel() override
  {

  }

  void SetInput(LabelImage3DType *input) override
  {

  }
};

#endif