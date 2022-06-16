#!/bin/bash

project_path=$(cd `dirname $0`; pwd)
cd $project_path/..

ts_file_list=(`ls translations/ukui-notebook/*.ts`)

for ts in "${ts_file_list[@]}"
do
    lrelease "${ts}"
done