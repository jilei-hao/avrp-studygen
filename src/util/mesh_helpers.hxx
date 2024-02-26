#ifndef __mesh_helpers_hxx
#define __mesh_helpers_hxx

#include "common.hxx"
#include <vtkMarchingCubes.h>
#include <itkVTKImageExport.h>
#include <vtkImageImport.h>
#include <vtkFlyingEdges3D.h>
#include <vtkDiscreteFlyingEdges3D.h>
#include <vtkPointDataToCellData.h>
#include <vtkCellData.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTriangleFilter.h>
#include <vtkQuadricDecimation.h>
#include <vtkWindowedSincPolyDataFilter.h>
#include <vtkPolyDataWriter.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkAppendPolyData.h>

class vtkTransform;

namespace studygen
{

class MeshHelpers
{
public:
  static MeshPointer GetMeshFromBinaryImage(LabelImage3DType::Pointer bImage)
  {
    std::cout << "-- getting mesh from binary image. " << std::endl;

    VTKImagePointer vtkImg = GetVTKImage<LabelImage3DType>(bImage);
    auto vtk2niiTransform = getVTKToNiftiTransform(bImage);

    MeshPointer mesh = RunMarchingCubes(vtkImg, 1.0);
    vtkNew<vtkTransformPolyDataFilter> fltTransform;
    fltTransform->SetTransform(vtk2niiTransform);
    fltTransform->SetInputData(mesh);
    fltTransform->Update();

    return fltTransform->GetOutput();
  }

  static MeshPointer TaubinSmooth(MeshPointer input, uint32_t iter, double passband)
  {
    std::cout << "-- smoothing mesh... iter: " << iter << "; passband: " << passband << std::endl;
    vtkNew<vtkWindowedSincPolyDataFilter> fltTaubin;
    fltTaubin->SetInputData(input);
    fltTaubin->SetNumberOfIterations(iter);
    fltTaubin->SetPassBand(passband);
    fltTaubin->Update();
    return fltTaubin->GetOutput();
  }

  static MeshPointer Decimate(MeshPointer input, double targetReduction)
  {
    std::cout << "-- decimating mesh... reduction: " << targetReduction << std::endl;
    vtkNew<vtkQuadricDecimation> fltQDecimate;
    fltQDecimate->SetInputData(input);
    fltQDecimate->SetTargetReduction(targetReduction);
    fltQDecimate->SetVolumePreservation(true);
    fltQDecimate->SetMapPointData(false);
    fltQDecimate->Update();
    return fltQDecimate->GetOutput();
  }

  static MeshPointer TriangulateMesh(MeshPointer input)
  {
    std::cout << "-- triangulating mesh" << std::endl;
    vtkNew<vtkTriangleFilter> fltTriangle;
    fltTriangle->SetInputData(input);
    fltTriangle->Update();
    return fltTriangle->GetOutput();
  }

  static void WriteMesh(MeshPointer mesh, std::string filename)
  {
    std::string ext = getFileExtension(filename);
    if (ext == "vtp")
      {
      vtkNew<vtkXMLPolyDataWriter> vtpWriter;
      vtpWriter->SetInputData(mesh);
      vtpWriter->SetFileName(filename.c_str());
      vtpWriter->Write();
      }
    else if (ext == "vtk")
      {
      vtkNew<vtkPolyDataWriter> vtkWriter;
      vtkWriter->SetInputData(mesh);
      vtkWriter->SetFileName(filename.c_str());
      vtkWriter->Write();
      }
    else
      throw FormatException("[MeshHelpers::WriteMesh] Unknown file extension %s", ext.c_str());
  }

  static MeshPointer RunMarchingCubes(VTKImagePointer image, RealType value)
  {
    vtkNew<vtkMarchingCubes> fltMC;
    fltMC->SetInputData(image);
    fltMC->SetValue(0, value);
    fltMC->ComputeNormalsOn();
    fltMC->Update();
    return fltMC->GetOutput();
  }

  static MeshPointer RunFlyingEdges(VTKImagePointer image, RealType value)
  {
    vtkNew<vtkFlyingEdges3D> fltFE;
    fltFE->SetInputData(image);
    fltFE->SetValue(0, value);
    fltFE->ComputeNormalsOn();
    fltFE->Update();
    return fltFE->GetOutput();
  }

  static MeshPointer RunMultiLabelFlyingEdges(VTKImagePointer image, RealType cut)
  {
    double range[2];
    image->GetScalarRange(range);
    size_t lo = ceil(cut);
    size_t hi = floor(range[1]);
    size_t n = hi - lo + 1;

    std::cout << "[MeshHelpers::RunMultiLabelFlyingEdges] range: " << range[0] << " " << range[1] << std::endl;
    std::cout << "-- Preserving " << n << " labels from " << lo << " to " << hi << std::endl;

    vtkNew<vtkDiscreteFlyingEdges3D> fltFE;
    fltFE->SetInputData(image);
    fltFE->GenerateValues(n, lo, hi);
    fltFE->Update();
    return fltFE->GetOutput();
  }

  static MeshPointer ConvertPointDataToCellData(MeshPointer mesh)
  {
    vtkNew<vtkPointDataToCellData> fltPD2CD;
    fltPD2CD->SetInputData(mesh);
    fltPD2CD->PassPointDataOn();
    fltPD2CD->Update();
    return fltPD2CD->GetPolyDataOutput();
  }

  static MeshPointer RenameCellData(MeshPointer mesh, std::string name)
  {
    mesh->GetCellData()->GetScalars()->SetName(name.c_str());
    return mesh;
  }

  static MeshPointer AssembleMeshes(std::vector<MeshPointer> meshes)
  {
    vtkNew<vtkAppendPolyData> fltAppend;
    for (auto mesh : meshes)
      {
      fltAppend->AddInputData(mesh);
      }
    fltAppend->Update();
    return fltAppend->GetOutput();
  }

  static void CreateAndFillCellDataWithLabel(MeshPointer mesh, std::string name, LabelType label)
  {
    vtkNew<vtkIntArray> scalars;
    scalars->SetNumberOfComponents(1);
    scalars->SetName(name.c_str());
    scalars->SetNumberOfTuples(mesh->GetNumberOfCells());
    for (size_t i = 0; i < mesh->GetNumberOfCells(); i++)
      {
      scalars->SetValue(i, label);
      }
    mesh->GetCellData()->SetScalars(scalars);
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
    vnl_matrix<double> m_dir, vnl_vector<double> v_origin, vnl_vector<double> v_spacing)
  {
    // Set the NIFTI/RAS transform
    vnl_matrix<double> m_ras_matrix;
    vnl_diag_matrix<double> m_scale, m_lps_to_ras;
    vnl_vector<double> v_ras_offset;

    // Compute the matrix
    m_scale.set(v_spacing);
    m_lps_to_ras.set(vnl_vector<double>(3, 1.0));
    m_lps_to_ras[0] = -1;
    m_lps_to_ras[1] = -1;
    m_ras_matrix = m_lps_to_ras * m_dir * m_scale;

    // Compute the vector
    v_ras_offset = m_lps_to_ras * v_origin;

    // Create the larger matrix
    vnl_vector<double> vcol(4, 1.0);
    vcol.update(v_ras_offset);

    vnl_matrix_fixed<double,4,4> m_sform;
    m_sform.set_identity();
    m_sform.update(m_ras_matrix);
    m_sform.set_column(3, vcol);
    return m_sform;
  }

  static vnl_matrix_fixed<double, 4, 4> ConstructVTKtoNiftiTransform(
    vnl_matrix<double> m_dir, vnl_vector<double> v_origin, vnl_vector<double> v_spacing)
  {
    vnl_matrix_fixed<double,4,4> vox2nii = ConstructNiftiSform(m_dir, v_origin, v_spacing);
    vnl_matrix_fixed<double,4,4> vtk2vox;
    vtk2vox.set_identity();
    for(size_t i = 0; i < 3; i++)
      {
      vtk2vox(i,i) = 1.0 / v_spacing[i];
      vtk2vox(i,3) = - v_origin[i] / v_spacing[i];
      }

    return vox2nii * vtk2vox;
  }

  static vtkSmartPointer<vtkTransform> getVTKToNiftiTransform(
    itk::ImageBase<3>::Pointer img3d)
  {
    auto vtk2nii = ConstructVTKtoNiftiTransform(
    img3d->GetDirection().GetVnlMatrix().as_ref(),
    img3d->GetOrigin().GetVnlVector(),
    img3d->GetSpacing().GetVnlVector()
  );

    vtkNew<vtkTransform> transform;
    transform->SetMatrix(vtk2nii.data_block());
    transform->Update();

    return transform;
  }

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

#endif // __mesh_helpers_hxx
