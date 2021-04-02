#!/bin/bash

chip=${1}

echo matlab -nodesktop -r "addpath('chip_simulation_setting'); rehash toolboxcache; setting_${chip}, SecondStepMain, quit"
time matlab -nodesktop -r "addpath('chip_simulation_setting'); rehash toolboxcache; setting_${chip}, SecondStepMain, quit"
