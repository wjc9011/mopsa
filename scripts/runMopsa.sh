#!/bin/bash

chip=${1}
setting=${2}
cwd=`pwd`

cd ../build
make -j16
cd $cwd

if [ "$setting" = "" ]
then
  echo  ../bin/main ../design_geometry_plain/$chip.txt  ../data/$chip-x.csv ../data/$chip-y.csv ../matlab/chip_simulation_setting/setting_$chip.m
  time ../bin/main ../design_geometry_plain/$chip.txt ../data/$chip-x.csv ../data/$chip-y.csv ../matlab/chip_simulation_setting/setting_$chip.m
else
  echo  ../bin/main ../design_geometry_plain/$chip.txt  ../data/$chip-x.csv ../data/$chip-y.csv $settting
  time ../bin/main ../design_geometry_plain/$chip.txt ../data/$chip-x.csv ../data/$chip-y.csv $setting
fi
