#ifndef __image_helpers_hxx_
#define __image_helpers_hxx_

#include "common.hxx"
#include "mesh_helpers.hxx"
#include <utility>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkNearestNeighborInterpolateImageFunction.h>
#include <itkLinearInterpolateImageFunction.h>
#include <itkResampleImageFilter.h>
#include <itkSmoothingRecursiveGaussianImageFilter.h>
#include <itkRegionOfInterestImageFilter.h>
#include <itkPasteImageFilter.h>
#include <itkBinaryBallStructuringElement.h>
#include <itkBinaryDilateImageFilter.h>
#include <itkBinaryErodeImageFilter.h>
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
  static typename TImage::Pointer ReadImage(std::string filename)
  {
    return itk::ReadImage<TImage>(filename);
  }

  template <typename TImage>
  static void WriteImage(typename TImage::Pointer image, std::string filename)
  {
    auto writer = itk::ImageFileWriter<TImage>::New();
    writer->SetInput(image);
    writer->SetFileName(filename);
    writer->Write();
  }

  template <typename TImage>
  static void WriteVTKImage(typename TImage::Pointer image, std::string filename)
  {
    auto vtkImg = MeshHelpers::GetVTKImage<TImage>(image);
    vtkNew<vtkXMLImageDataWriter> writer;
    writer->SetInputData(vtkImg);
    writer->SetFileName(filename.c_str());
    writer->Write();
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

  template <typename TImage>
  static typename TImage::Pointer
  ExtractRegion(typename TImage::Pointer image, typename TImage::RegionType region)
  {
    std::cout << "-- Extracting Region... " << region.GetIndex() << " " << region.GetSize() << std::endl;

    // Make sure the bounding box is within the contents of the image
    region.Crop(image->GetBufferedRegion());

    // Chop off the region
    auto fltTrim = itk::RegionOfInterestImageFilter<TImage, TImage>::New();
    fltTrim->SetInput(image);
    fltTrim->SetRegionOfInterest(region);
    fltTrim->Update();

    typename TImage::Pointer output = fltTrim->GetOutput();
    return output;
  }

  // expand region to cover the target coordinate
  template <typename TImage>
  static void
  ExpandRegion(typename TImage::RegionType &region, const typename TImage::IndexType &idx)
  {
    if (region.GetNumberOfPixels() == 0)
    {
      region.SetIndex(idx);
      for (size_t i = 0; i < 3; i++)
      {
        region.SetSize(i, 1);
      }
    }
    else
    {
      for (size_t i = 0; i < 3; i++)
      {
        if (region.GetIndex(i) > idx[i])
        {
          region.SetSize(i, region.GetSize(i) + (region.GetIndex(i) - idx[i]));
          region.SetIndex(i, idx[i]);
        }
        else if (region.GetIndex(i) + (long) region.GetSize(i) <= idx[i])
        {
          region.SetSize(i, 1 + idx[i] - region.GetIndex(i));
        }
      }
    }
  }

  template <typename TImage>
  static typename TImage::Pointer
  TrimImage(typename TImage::Pointer image, int padding, typename TImage::RegionType &region)
  {
    std::cout << "-- Trimming Image... padding: " << padding << std::endl;

    // Find the extent of the non-background region of the image
    typedef itk::ImageRegionIteratorWithIndex<TImage> Iterator;
    Iterator it(image, image->GetBufferedRegion());
    for( ; !it.IsAtEnd(); ++it)
      if(it.Value() != 0)
        ExpandRegion<TImage>(region, it.GetIndex());

    typename TImage::SizeType radius;
    radius.Fill(padding);
    region.PadByRadius(radius);

    return ExtractRegion<TImage>(image, region);
  }

  template <typename TImage>
  static typename TImage::Pointer
  RestoreImage(typename TImage::Pointer trimmed, typename TImage::Pointer original, typename TImage::RegionType &region)
  {
    std::cout << "-- Restoring Image... " << std::endl;
    
    // Replace original image with trimmed region
    auto fltReplace = itk::PasteImageFilter<TImage, TImage, TImage>::New();
    fltReplace->SetSourceImage(trimmed);
    fltReplace->SetSourceRegion(trimmed->GetLargestPossibleRegion());
    fltReplace->SetDestinationImage(original);
    fltReplace->SetDestinationIndex(region.GetIndex());
    fltReplace->Update();

    return fltReplace->GetOutput();
  }

  template <typename TImage>
  static typename TImage::Pointer
  DilateImage(typename TImage::Pointer image, double radius)
  {
    std::cout << "-- Dilating Image... radius: " << radius << std::endl;

    // Define the structuring element
    typedef itk::BinaryBallStructuringElement<typename TImage::PixelType, 3> Element;
    Element elt;
    elt.SetRadius(radius);
    elt.CreateStructuringElement();

    typedef itk::BinaryDilateImageFilter<TImage, TImage, Element> FilterType;
    typename FilterType::Pointer filter = FilterType::New();
    filter->SetInput(image);
    filter->SetDilateValue(1);
    filter->SetKernel(elt);
    filter->Update();

    return filter->GetOutput();
  }

  template <typename TImage>
  static typename TImage::Pointer
  ErodeImage(typename TImage::Pointer image, double radius)
  {
    std::cout << "-- Eroding Image... radius: " << radius << std::endl;

    // Define the structuring element
    typedef itk::BinaryBallStructuringElement<typename TImage::PixelType, 3> Element;
    Element elt;
    elt.SetRadius(radius);
    elt.CreateStructuringElement();

    typedef itk::BinaryErodeImageFilter<TImage, TImage, Element> FilterType;
    typename FilterType::Pointer filter = FilterType::New();
    filter->SetInput(image);
    filter->SetErodeValue(1);
    filter->SetKernel(elt);
    filter->Update();

    return filter->GetOutput();
  }

  // Scale the region by a factor. Input is intact and a new region is returned.
  template <typename TImage>
  static typename TImage::RegionType
  ScaleRegion(typename TImage::RegionType &region, double scale)
  {
    typename TImage::SizeType radius = region.GetSize();
    for (size_t i = 0; i < 3; i++)
    {
      radius[i] = (radius[i] * scale - radius[i]) / 2.0;
    }
    // copy the region for returning
    typename TImage::RegionType scaledRegion = region;

    if (scale < 1)
    {
      scaledRegion.ShrinkByRadius(radius);
    }
    else
    {
      scaledRegion.PadByRadius(radius);
    }

    return scaledRegion;
  }

  // Crop the region by the bounding box. The region is modified in place.
  template <typename TImage>
  static void CropRegionByBoundingBox(typename TImage::RegionType &region, typename TImage::RegionType &boundingBox)
  {
    typename TImage::IndexType idx = boundingBox.GetIndex();
    typename TImage::SizeType size = boundingBox.GetSize();
    typename TImage::IndexType regionIdx = region.GetIndex();
    typename TImage::SizeType regionSize = region.GetSize();
    for (size_t i = 0; i < 3; i++)
    {
      if (regionIdx[i] < idx[i])
      {
        regionSize[i] -= idx[i] - regionIdx[i];
        regionIdx[i] = idx[i];
      }
      if (regionIdx[i] + regionSize[i] > idx[i] + size[i])
      {
        regionSize[i] = size[i] - (regionIdx[i] - idx[i]);
      }
    }
    region.SetIndex(regionIdx);
    region.SetSize(regionSize);
  }

  template <typename TGreyImage3D, typename TMaskImage3D>
  static std::pair<typename TGreyImage3D::Pointer, typename TMaskImage3D::Pointer>
  TrimImageByMask(typename TGreyImage3D::Pointer image, typename TMaskImage3D::Pointer mask, double scale = 1.3)
  {
    std::cout << "-- [image_helpers::TrimImageByMask] scale: " << scale << std::endl;

    // Play with the regions
    LabelImage3DType::RegionType trimmedRegion = GetTrimmedRegion<TMaskImage3D>(mask, 5, scale);

    // Crop the images
    auto outMask = ExtractRegion<LabelImage3DType>(mask, trimmedRegion);

    auto outImage = ExtractRegion<Image3DType>(image, trimmedRegion);

    return std::pair<typename TGreyImage3D::Pointer, typename TMaskImage3D::Pointer>(outImage, outMask);
  }

  template <typename TImage>
  static typename TImage::RegionType
  GetTrimmedRegion(typename TImage::Pointer image, int padding, double scale)
  {
    typename TImage::RegionType trimmedRegion;
    auto originalRegion = image->GetLargestPossibleRegion();
    auto trimmedImg = TrimImage<TImage>(image, padding, trimmedRegion);
    auto scaledRegion = ScaleRegion<TImage>(trimmedRegion, scale);
    CropRegionByBoundingBox<TImage>(scaledRegion, originalRegion);

    return scaledRegion;
  }

  template<class TTPImage, class TTimeSeriesImage>
  static typename TTPImage::Pointer
  ExtractTimePointImage(TTimeSeriesImage *ts_img, unsigned int tp)
  {
    // Logic adapated from SNAP ImageWrapper method:
    // ConfigureTimePointImageFromImage4D()
    assert(tp >= 0);

    unsigned int nt = ts_img->GetBufferedRegion().GetSize()[3u];
    unsigned int bytes_per_volume = ts_img->GetPixelContainer()->Size() / nt;

    typename TTPImage::Pointer tp_img = TTPImage::New();

    typename TTPImage::RegionType region;
    typename TTPImage::SpacingType spacing;
    typename TTPImage::PointType origin;
    typename TTPImage::DirectionType dir;
    for(unsigned int j = 0; j < 3; j++)
      {
      region.SetSize(j, ts_img->GetBufferedRegion().GetSize()[j]);
      region.SetIndex(j, ts_img->GetBufferedRegion().GetIndex()[j]);
      spacing[j] = ts_img->GetSpacing()[j];
      origin[j] = ts_img->GetOrigin()[j];
      for(unsigned int k = 0; k < 3; k++)
        dir(j,k) = ts_img->GetDirection()(j,k);
      }

    // All of the information from the 4D image is propagaged to the 3D timepoints
    tp_img->SetRegions(region);
    tp_img->SetSpacing(spacing);
    tp_img->SetOrigin(origin);
    tp_img->SetDirection(dir);
    tp_img->SetNumberOfComponentsPerPixel(ts_img->GetNumberOfComponentsPerPixel());
    tp_img->Allocate();

    // Set the buffer pointer
    tp_img->GetPixelContainer()->SetImportPointer(
          ts_img->GetBufferPointer() + bytes_per_volume * tp,
          bytes_per_volume);

    return tp_img;
  }

  template <class TTPImage, class TTimeSeriesImage>
  static typename TTimeSeriesImage::Pointer
  CreateTimeSeriesImageFromList(std::vector<typename TTPImage::Pointer> tpImgList)
  {
    typename TTimeSeriesImage::Pointer tsImg = TTimeSeriesImage::New();
    typename TTimeSeriesImage::RegionType region;
    typename TTimeSeriesImage::SpacingType spacing;
    typename TTimeSeriesImage::PointType origin;
    typename TTimeSeriesImage::DirectionType direction;

    auto firstTP = tpImgList[0];
    auto nd = firstTP->GetImageDimension();
    auto nt = tpImgList.size();
    auto nc = 1; // we only consider 1 component per pixel for now


    // Set the size of the time series image
    typename TTPImage::SizeType tpSize = firstTP->GetLargestPossibleRegion().GetSize();
    typename TTimeSeriesImage::SizeType tsSize;
    for (size_t i = 0; i < nd; i++) {
      tsSize[i] = tpSize[i];
    }
    tsSize[nd] = nt;
    region.SetSize(tsSize);
    region.SetIndex({0, 0, 0, 0});

    // Set the spacing, origin, and direction of the time series image
    origin.Fill(0);
    spacing.Fill(1);
    direction.SetIdentity();

    for (auto i = 0; i < nd; ++i)
    {
      origin[i] = firstTP->GetOrigin()[i];
      spacing[i] = firstTP->GetSpacing()[i];
      for (auto j = 0; j < nd; ++j)
      {
        direction[i][j] = firstTP->GetDirection()[i][j];
      }
    }

    // Set the region, spacing, origin, and direction of the time series image
    tsImg->SetRegions(region);
    tsImg->SetSpacing(spacing);
    tsImg->SetOrigin(origin);
    tsImg->SetDirection(direction);
    tsImg->SetNumberOfComponentsPerPixel(nc);

    // Allocate memory for the time series image
    tsImg->Allocate();

    // compose the tsBuffer
    auto tsBuffer = tsImg->GetPixelContainer()->GetImportPointer();

    for (unsigned int i = 0; i < nt; ++i)
    {
      auto tp = tpImgList[i];
      auto tpBuffer = tp->GetPixelContainer()->GetImportPointer();
      for (unsigned int j = 0; j < tp->GetLargestPossibleRegion().GetNumberOfPixels(); ++j)
      {
        tsBuffer[i * tp->GetLargestPossibleRegion().GetNumberOfPixels() + j] = tpBuffer[j];
      }
    }

    return tsImg;
  }
};


}
#endif // __image_helpers_hxx_
