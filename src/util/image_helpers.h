#ifndef __image_helpers_h
#define __image_helpers_h

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
  static typename TImage::Pointer ReadImage(std::string &filename);

  template <typename TImage>
  static void WriteImage(typename TImage::Pointer image, std::string &filename);

  

};

template <typename TImage>
typename TImage::Pointer 
ImageHelpers::ReadImage(std::string &filename)
{
  return itk::ReadImage<TImage>(filename);
}

template <typename TImage>
void 
ImageHelpers::WriteImage(typename TImage::Pointer image, std::string &filename)
{
  itk::WriteImage<TImage>(image, filename);
}



}
#endif // __image_helpers_h