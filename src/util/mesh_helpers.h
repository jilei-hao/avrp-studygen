#ifndef __mesh_helpers_h
#define __mesh_helpers_h

#include "common.h"
#include <vtkMarchingCubes.h>
#include <itkVTKImageExport.h>
#include <vtkImageImport.h>

class vtkTransform;

namespace studygen
{

class MeshHelpers
{
public:
  static MeshPointer GetMeshFromBinaryImage(LabelImage3DType::Pointer bImage);

  static MeshPointer TaubinSmooth(MeshPointer input, uint32_t iter, double passband);

  static MeshPointer Decimate(MeshPointer input, double targetReduction);

  static MeshPointer TriangulateMesh(MeshPointer input);

  static void WriteMesh(MeshPointer mesh, std::string &filename);

  static MeshPointer RunMarchingCubes(VTKImagePointer image, RealType value)
  {
    vtkNew<vtkMarchingCubes> fltMC;
    fltMC->SetInputData(image);
    fltMC->SetValue(0, value);
    fltMC->ComputeNormalsOn();
    fltMC->Update();
    return fltMC->GetOutput();
  }

  template<typename TImage>
  static VTKImagePointer GetVTKImage(typename TImage::Pointer image)
  {
    auto fltExport = itk::VTKImageExport<TImage>::New();
    fltExport->SetInput(image);
//    fltExport->Update();
    vtkNew<vtkImageImport> fltImport;
    ConnectITKToVTK(fltExport.GetPointer(), fltImport);
    fltImport->Update();
    return fltImport->GetOutput();
  }

private:
  static vnl_matrix_fixed<double, 4, 4> ConstructNiftiSform(
    vnl_matrix<double> m_dir, vnl_vector<double> v_origin, vnl_vector<double> v_spacing);

  static vnl_matrix_fixed<double, 4, 4> ConstructVTKtoNiftiTransform(
    vnl_matrix<double> m_dir, vnl_vector<double> v_origin, vnl_vector<double> v_spacing);

  static vtkSmartPointer<vtkTransform> getVTKToNiftiTransform(
    itk::ImageBase<3>::Pointer img3d);

  template<class TImage>
  static void ConnectITKToVTK(itk::VTKImageExport<TImage> *fltExport,vtkImageImport *fltImport)
  {
    fltImport->SetUpdateInformationCallback( fltExport->GetUpdateInformationCallback());
    fltImport->SetPipelineModifiedCallback( fltExport->GetPipelineModifiedCallback());
    fltImport->SetWholeExtentCallback( fltExport->GetWholeExtentCallback());
    fltImport->SetSpacingCallback( fltExport->GetSpacingCallback());
    fltImport->SetOriginCallback( fltExport->GetOriginCallback());
    fltImport->SetScalarTypeCallback( fltExport->GetScalarTypeCallback());
    fltImport->SetNumberOfComponentsCallback( fltExport->GetNumberOfComponentsCallback());
    fltImport->SetPropagateUpdateExtentCallback( fltExport->GetPropagateUpdateExtentCallback());
    fltImport->SetUpdateDataCallback( fltExport->GetUpdateDataCallback());
    fltImport->SetDataExtentCallback( fltExport->GetDataExtentCallback());
    fltImport->SetBufferPointerCallback( fltExport->GetBufferPointerCallback());
    fltImport->SetCallbackUserData( fltExport->GetCallbackUserData());
  }

};



}

#endif // __mesh_helpers_h
