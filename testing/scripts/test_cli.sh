#! /bin/bash

gen=/Users/jileihao/dev/avrspt-dev/AVRSPT-StudyGen/build/avrspt-studygen/StudyGen

time $gen -i /Users/jileihao/data/avrspt/tav48/img4d_tav48_LAS.nii.gz \
-s "/Users/jileihao/data/avrspt/tav48/seg02_tav48_NY.nii.gz" \
-s_ref 2 \
-s_tgt "1,3,4,5" \
-s "/Users/jileihao/data/avrspt/tav48/seg09_tav48_NY.nii.gz" \
-s_ref 9 \
-s_tgt "6,7,8,9,10,11,12,13,14" \
-lc "/Users/jileihao/dev/avrspt-dev/AVRSPT-StudyGen/testing/data/test_label_config.json" \
-nt 14
