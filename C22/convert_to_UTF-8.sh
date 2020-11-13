#!/bin/bash

readonly sources=$(ls */lab_*.cpp 2> /dev/null)

[ ${#sources} == 0 ] && echo "No files to convert." && exit 0

for original_file_path in ${sources}
do
    converted_file_path=$(dirname ${original_file_path})/u8_$(basename ${original_file_path})
    if [ -f ${converted_file_path} ]
    then
        echo "File ${converted_file_path} already exists - no conversion will be performed."
        continue
    fi
    iconv -f cp1251 -t UTF-8 ${original_file_path} > ${converted_file_path}
    if [ $? == 0 ]
    then
        echo "File ${original_file_path} successfully converted to ${converted_file_path}."
        hidden_file_path=$(dirname ${original_file_path})/.$(basename ${original_file_path})
        mv ${original_file_path} ${hidden_file_path}
    fi
done
