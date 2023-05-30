#ifndef __mesh_helpers_h
#define __mesh_helpers_h

#include "common.h"

class vtkTransform;

class MeshHelpers
{
public:
  static vnl_matrix_fixed<double, 4, 4> ConstructNiftiSform(
    vnl_matrix<double> m_dir, vnl_vector<double> v_origin, vnl_vector<double> v_spacing);

  static vnl_matrix_fixed<double, 4, 4> ConstructVTKtoNiftiTransform(
    vnl_matrix<double> m_dir, vnl_vector<double> v_origin, vnl_vector<double> v_spacing);

  static vtkSmartPointer<vtkTransform> getVTKToNiftiTransform(
    itk::ImageBase<3>::Pointer img3d);
};

#endif // __mesh_helpers_h