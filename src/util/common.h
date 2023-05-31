#ifndef __common_h
#define __common_h

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <itkImage.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>

#include "itk_oriented_ras_image.hxx"

namespace studygen {
  using RealType = double;
  using LabelType = uint32_t;
  using TimePointType = uint32_t;
  using Image4DType = itk::OrientedRASImage<RealType, 4u>;
  using Image3DType = itk::OrientedRASImage<RealType, 3u>;
  using LabelImage4DType = itk::OrientedRASImage<LabelType, 4u>;
  using LabelImage3DType = itk::OrientedRASImage<LabelType, 3u>;
  using MeshPointer = vtkSmartPointer<vtkPolyData>;
}

#endif //#ifndef __common_h