#!/bin/bash

readonly root=$PWD/$(dirname $0)

find $root -type f -name "*.sh" -exec chmod 777 {} \;
find $root -type f -name "*.cmd" -exec chmod 777 {} \;
find $root -type f -name "*.lua" -exec chmod 777 {} \;
