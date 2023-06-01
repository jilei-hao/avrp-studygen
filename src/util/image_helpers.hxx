#ifndef __image_helpers_hxx_
#define __image_helpers_hxx_

#include "common.h"
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

namespace studygen
{

class ImageHelpers
{
public:
  ImageHelpers() {}
  ~ImageHelpers() {}

  // IO
  template <typename TImage>
  static typename TImage::Pointer ReadImage(std::string &filename)
  {
    return itk::ReadImage<TImage>(filename);
  }

  template <typename TImage>
  static void WriteImage(typename TImage::Pointer image, std::string &filename)
  {
    itk::WriteImage<TImage>(image, filename);
  }

};


}
#endif // __image_helpers_hxx_