#!/bin/bash

chip=${1}

cd ../build
make -j16 && ../bin/main ../design_geometry_plain/$chip.txt  ../data/$chip-x.csv ../data/$chip-y.csv ../matlab/chip_simulation_setting/setting_$chip.m
