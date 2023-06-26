#ifndef __image_helpers_hxx_
#define __image_helpers_hxx_

#include "common.h"
#include "mesh_helpers.h"
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkNearestNeighborInterpolateImageFunction.h>
#include <itkLinearInterpolateImageFunction.h>
#include <itkResampleImageFilter.h>
#include <itkSmoothingRecursiveGaussianImageFilter.h>
#include <vtkXMLImageDataWriter.h>


namespace studygen
{

class ImageHelpers
{
public:
  ImageHelpers() {}
  ~ImageHelpers() {}

  enum InterpolationMode
  {
    NN, Linear, COUNT
  };

  // IO
  template <typename TImage>
  static typename TImage::Pointer ReadImage(std::string &filename)
  {
    return itk::ReadImage<TImage>(filename);
  }

  template <typename TImage>
  static void WriteImage(typename TImage::Pointer image, std::string &filename)
  {
    std::string ext = getFileExtension(filename);
    if (ext == "vti")
      {
      auto vtkImg = MeshHelpers::GetVTKImage<TImage>(image);
      vtkNew<vtkXMLImageDataWriter> writer;
      writer->SetInputData(vtkImg);
      writer->SetFileName(filename.c_str());
      writer->Write();
      }
    else
      {
      auto writer = itk::ImageFileWriter<TImage>::New();
      writer->SetInput(image);
      writer->SetFileName(filename);
      writer->Write();
      }

  }

  template <typename TImageIn, typename TImageOut>
  static typename TImageOut::Pointer
  ThresholdImage(
      typename TImageIn::Pointer image,
      typename TImageIn::PixelType lo, typename TImageIn::PixelType hi,
      typename TImageOut::PixelType in, typename TImageOut::PixelType out)
  {
    std::cout << "-- Thresholding Image... lo: " << lo << "; hi: " << hi
              << "; in: " << in << "; out: " << out << std::endl;

    // threshold
    auto fltThreshold =
      itk::BinaryThresholdImageFilter<TImageIn, TImageOut>::New();

    fltThreshold->SetInput(image);
    fltThreshold->SetLowerThreshold(lo);
    fltThreshold->SetUpperThreshold(hi);
    fltThreshold->SetInsideValue(in);
    fltThreshold->SetOutsideValue(out);
    fltThreshold->Update();

    return fltThreshold->GetOutput();
  }

  template <typename TImage>
  static typename TImage::Pointer
  ResampleImage(typename TImage::Pointer image, double rate, InterpolationMode interpolation)
  {
    std::cout << "-- Resampling Image... rate:" << rate
              << "; interpolation: " << interpolation << std::endl;

    using ResampleFilterType = itk::ResampleImageFilter<TImage, TImage>;
    using NNInterpolatorType = itk::NearestNeighborInterpolateImageFunction<TImage>;
    using LinearInterpolatorType = itk::LinearInterpolateImageFunction<TImage>;

    auto fltResample = ResampleFilterType::New();
    fltResample->SetInput(image);

    auto transform = itk::IdentityTransform<double, 3>::New();
    transform->SetIdentity();
    fltResample->SetTransform(transform);

    switch (interpolation)
      {
      case (InterpolationMode::NN):
        fltResample->SetInterpolator(NNInterpolatorType::New());
        break;

      case (InterpolationMode::Linear):
        fltResample->SetInterpolator(LinearInterpolatorType::New());
        break;

      default:
        throw FormatException("ImageHelpers::ResampleImage: unknown interpolation mode %d", interpolation);
      }

    const typename TImage::RegionType &inRegion = image->GetLargestPossibleRegion();
    const typename TImage::SizeType &inSize = inRegion.GetSize();
    auto &inDirection = image->GetDirection();
    auto &inOrigin = image->GetOrigin();

    // set out spacing.
    const typename TImage::SpacingType &inSpacing = image->GetSpacing();
    double outSpacing[3];
    typename TImage::SizeType outSize;
    for (int i = 0; i < 3; ++i)
    {
      outSpacing[i] = inSpacing[i] / rate;
      outSize[i] = inSize[i] * rate;
    }

    fltResample->SetOutputOrigin(inOrigin);
    fltResample->SetSize(outSize);
    fltResample->SetOutputSpacing(outSpacing);
    fltResample->SetOutputDirection(inDirection);
    fltResample->Update();

    return fltResample->GetOutput();
  }


  template <typename TImage>
  static Image3DType::Pointer
  SmoothImage(typename TImage::Pointer image, double voxSigma)
  {
    std::cout << "-- gaussian smoothing sigma: " << voxSigma << "vox" << std::endl;

    using SmoothingFilterType = itk::SmoothingRecursiveGaussianImageFilter<TImage, Image3DType>;

    auto fltGaussian = SmoothingFilterType::New();

    LabelImage3DType::SpacingType spc = image->GetSpacing();

    fltGaussian->SetInput(image);

    typename SmoothingFilterType::SigmaArrayType sigmaArr;
    for (int i = 0; i < 3; ++i)
    {
      sigmaArr[i] = voxSigma * spc[i];
    }

    fltGaussian->SetSigmaArray(sigmaArr);
    fltGaussian->Update();

    return fltGaussian->GetOutput();
  }
};


}
#endif // __image_helpers_hxx_
