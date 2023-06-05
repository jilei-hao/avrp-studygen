#include "mesh_helpers.h"
#include <vtkTransform.h>
#include <itkImageToVTKImageFilter.h>
#include <vtkMarchingCubes.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTriangleFilter.h>
#include <vtkQuadricDecimation.h>
#include <vtkWindowedSincPolyDataFilter.h>
#include <vtkPolyDataWriter.h>
#include <vtkXMLPolyDataWriter.h>

using namespace studygen;

vnl_matrix_fixed<double, 4, 4> 
MeshHelpers
::ConstructNiftiSform(vnl_matrix<double> m_dir, vnl_vector<double> v_origin,
  vnl_vector<double> v_spacing)
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

vnl_matrix_fixed<double, 4, 4>
MeshHelpers
::ConstructVTKtoNiftiTransform(vnl_matrix<double> m_dir, vnl_vector<double> v_origin, 
  vnl_vector<double> v_spacing)
{
  std::cout << "---- [ConstructVTKtoNiftiTransform] dir:\n " << m_dir << "\norg: " 
            << v_origin << "\nspc:" << v_spacing << std::endl;

  vnl_matrix_fixed<double,4,4> vox2nii = ConstructNiftiSform(m_dir, v_origin, v_spacing);
  vnl_matrix_fixed<double,4,4> vtk2vox;
  vtk2vox.set_identity();
  for(size_t i = 0; i < 3; i++)
    {
    vtk2vox(i,i) = 1.0 / v_spacing[i];
    vtk2vox(i,3) = - v_origin[i] / v_spacing[i];
    }

  std::cout << "---- vox2nii: \n" << vox2nii << "\nvtk2vox: \n" << vtk2vox << std::endl;
  return vox2nii * vtk2vox;
}

vtkSmartPointer<vtkTransform> 
MeshHelpers::
getVTKToNiftiTransform(itk::ImageBase<3>::Pointer img3d)
{
  auto vtk2nii = ConstructVTKtoNiftiTransform(
    img3d->GetDirection().GetVnlMatrix().as_ref(),
    img3d->GetOrigin().GetVnlVector(),
    img3d->GetSpacing().GetVnlVector()
  );

  std::cout << "---- [getVTKToNiftiTransform] vtk2nii:" << std::endl;
  vtk2nii.print(std::cout);

  vtkNew<vtkTransform> transform;
  transform->SetMatrix(vtk2nii.data_block());
  transform->Update();

  return transform;
}

MeshPointer 
MeshHelpers
::GetMeshFromBinaryImage(LabelImage3DType::Pointer bImage)
{
  std::cout << "-- getting mesh from bianry image. " << std::endl;

  auto fltITK2VTK = itk::ImageToVTKImageFilter<LabelImage3DType>::New();
  fltITK2VTK->SetInput(bImage);
  fltITK2VTK->Update();
  VTKImagePointer vtkImg = fltITK2VTK->GetOutput();
  auto vtk2niiTransform = MeshHelpers::getVTKToNiftiTransform(bImage);

  vtkNew<vtkMarchingCubes> fltMC;
  fltMC->SetInputData(vtkImg);
  fltMC->SetValue(0, 1);
  fltMC->ComputeNormalsOn();
  fltMC->Update();

  MeshPointer polyTail = fltMC->GetOutput();

//  vtkNew<vtkTransformPolyDataFilter> fltTransform;
//  fltTransform->SetTransform(vtk2niiTransform);
//  fltTransform->SetInputData(polyTail);
//  fltTransform->Update();
//  polyTail = fltTransform->GetOutput();

  return polyTail;
}

MeshPointer
MeshHelpers
::TriangulateMesh(MeshPointer input)
{
  std::cout << "-- triangulating mesh" << std::endl;
  vtkNew<vtkTriangleFilter> fltTriangle;
  fltTriangle->SetInputData(input);
  fltTriangle->Update();
  return fltTriangle->GetOutput();
}

MeshPointer 
MeshHelpers
::TaubinSmooth(MeshPointer input, uint32_t iter, double passband)
{
  std::cout << "-- smoothing mesh... iter: " << iter << "; passband: " << passband << std::endl;
  vtkNew<vtkWindowedSincPolyDataFilter> fltTaubin;
  fltTaubin->SetInputData(input);
  fltTaubin->SetNumberOfIterations(iter);
  fltTaubin->SetPassBand(passband);
  fltTaubin->Update();
  return fltTaubin->GetOutput();
}

MeshPointer 
MeshHelpers
::Decimate(MeshPointer input, double targetReduction)
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

void
MeshHelpers
::WriteMesh(MeshPointer mesh, std::string &filename)
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
