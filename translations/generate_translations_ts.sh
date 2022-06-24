#!/bin/bash

project_path=$(cd `dirname $0`; pwd)
cd $project_path

translations_dir='./ukui-notebook'
src_dir='../src'

locales=('en' 'zh_CN' 'bo_CN')

for locale in "${locales[@]}"
do
    lupdate ${src_dir} -ts ${translations_dir}/ukui-notebook_${locale}.ts
done