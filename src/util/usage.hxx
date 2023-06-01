#ifndef __usage_hxx_
#define __usage_hxx_

#include <iostream>
#include <ostream>

namespace studygen
{

int usage(std::ostream &os)
{
  os << "Usage: StudyGen -i image.nii.gz -s seg1.nii.gz {seg1_options}"
  " -s seg2.nii.gz {seg2_options} {global_options}" << std::endl;

  os << std::endl;
  os << "segmentation options: " << std::endl;
  os << "-s seg_file_path         :create a new segmentation group, add image file as "
  "the reference 3d segmentation, :and set the config as the current seg config" << std::endl;
  os << "-s_ref tp                :set reference timepoint for the current seg config" << std::endl;
  os << "-s_tgt tp,tp,tp          :set a comma separated list of target time points "
  "for the current seg config" << std::endl;
  os << "-s_lc config_file_path   :(optional) set custom label config file path" << std::endl;

  os << std::endl;
  os << "global options: " << std::endl;
  os << "-nt number_of_tp         :set number of tp to be processed" << std::endl;

  return EXIT_FAILURE;
}

}

#endif // __usage_hxx