#ifndef __study_generator_h
#define __study_generator_h

#include "common.h"
#include "configurations.hxx"
#include "study_generator_data.hxx"

namespace studygen
{

class StudyGenerator
{
public:
  StudyGenerator();
  ~StudyGenerator();

  void SetStudyGenConfig(StudyGenConfig config);

  int Run();

private:
  void ValidateInput();
  void PrepareInputData();

  StudyGeneratorData m_Data;
  StudyGenConfig m_Config;
  bool m_Initialized = false;
};

}

#endif // #ifndef __study_generator_h