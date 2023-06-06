#ifndef __common_h
#define __common_h

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <limits>

#include <itkImage.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>

#include "format_exception.hxx"

namespace studygen {
  using RealType = double;
  using LabelType = int16_t;
  using TimePointType = uint32_t;
  using Image4DType = itk::Image<RealType, 4u>;
  using Image3DType = itk::Image<RealType, 3u>;
  using LabelImage4DType = itk::Image<LabelType, 4u>;
  using LabelImage3DType = itk::Image<LabelType, 3u>;
  using MeshPointer = vtkSmartPointer<vtkPolyData>;
  using VTKImagePointer = vtkSmartPointer<vtkImageData>;

  static constexpr LabelType LABELTYPE_MAX = std::numeric_limits<LabelType>::max();
  static constexpr RealType REALTYPE_MAX = std::numeric_limits<RealType>::max();
  static constexpr RealType REALTYPE_INFINITY = std::numeric_limits<RealType>::infinity();

  static std::string ssprintf(const char *format, ...)
  {
    if(format && strlen(format))
    {
      char buffer[4096];
      va_list args;
      va_start (args, format);
      vsnprintf (buffer, 4096, format, args);
      va_end (args);
      return std::string(buffer);
    }
    else
      return std::string();
  }

  static std::string getFileExtension(const std::string& filename) {
    size_t dotIndex = filename.find_last_of('.');
    if (dotIndex != std::string::npos && dotIndex != filename.length() - 1) {
        return filename.substr(dotIndex + 1);
    }
    return ""; // No extension found
  }
}

#endif //#ifndef __common_h
