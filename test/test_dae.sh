#!/bin/bash

# xmllint comes in 'libxml2-utils'
../build/kovelcli -i Cube.kvl -x dae -o Cube.dae
xmllint Cube.dae
