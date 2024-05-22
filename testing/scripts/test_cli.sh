#! /bin/bash

gen=/Users/jileihao/dev/avrspt-dev/AVRSPT-StudyGen/build/StudyGen

#time $gen -i /Users/jileihao/data/avrspt/tav48/img4d_tav48_LAS.nii.gz \
#-s "/Users/jileihao/data/avrspt/tav48/seg02_tav48_NY.nii.gz" \
#-s_ref 2 \
#-s_tgt "1,3" \
#-lc "/Users/jileihao/dev/avrspt-dev/AVRSPT-StudyGen/testing/data/test_label_config.json" \
#-nt 3


#time $gen -i /Users/jileihao/data/avrspt/tav48/img4d_tav48_LAS.nii.gz \
#-o "/Users/jileihao/data/studygen/output" \
#-s "/Users/jileihao/data/avrspt/tav48/seg02_tav48_NY.nii.gz" \
#-s_ref 2 \
#-s_tgt "1,3,4,5" \
#-s "/Users/jileihao/data/avrspt/tav48/seg09_tav48_NY.nii.gz" \
#-s_ref 9 \
#-s_tgt "6,7,8,9,10,11,12,13,14" \
#-lc "/Users/jileihao/dev/avrspt-dev/AVRSPT-StudyGen/testing/data/test_label_config.json" \
#-nt 14


time $gen -i "/Users/jileihao/data/avrp-data/bavcta008_rs20/i4_tp01-20_rs20.nii.gz" \
-o "/Users/jileihao/data/avrp-data/bavcta008_rs20/output" \
-s "/Users/jileihao/data/avrp-data/bavcta008_rs20/srs_02_tp01-07_rs20.nii.gz" \
-s_ref 2 \
-s_tgt "1,3,4,5,6,7" \
-s "/Users/jileihao/data/avrp-data/bavcta008_rs20/srd_19_tp08-20_rs20.nii.gz" \
-s_ref 19 \
-s_tgt "8,9,10,11,12,13,14,15,16,17,18,20" \
-lc "/Users/jileihao/dev/avrspt-dev/AVRSPT-StudyGen/testing/data/test_label_config.json" \
-nt 20
