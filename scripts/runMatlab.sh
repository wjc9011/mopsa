#!/bin/bash

chip=${1}

echo matlab -nodesktop -r "rehash toolboxcache; addpath('chip_simulation_setting'), setting_${chip}, SecondStepMain, quit"
matlab -nodesktop -r "rehash toolboxcache; addpath('chip_simulation_setting'), setting_${chip}, SecondStepMain, quit"
