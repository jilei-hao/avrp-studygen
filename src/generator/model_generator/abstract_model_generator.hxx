#ifndef ABSTRACT_MODEL_GENERATOR_HXX
#define ABSTRACT_MODEL_GENERATOR_HXX

#include <string>

template <typename InputType, typename OutputType>
class AbstractModelGenerator
{
public:
  virtual void GenerateModel() = 0;

  virtual void SetInput(InputType *input)
  {
    m_InputData = input;
  }

  virtual OutputType *GetOutput()
  {
    return m_OutputData;
  }

protected:
  // constructors
  AbstractModelGenerator(std::string config_file_path) : m_ConfigFilePath(config_file_path) {}
  ~AbstractModelGenerator() {}
  AbstractModelGenerator(const AbstractModelGenerator &other) = delete;
  AbstractModelGenerator &operator=(const AbstractModelGenerator &other) = delete;

  std::string m_ConfigFilePath;
  InputType *m_InputData;
  OutputType *m_OutputData;
};




#endif