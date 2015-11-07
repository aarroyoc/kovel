#!/bin/bash
../build/kovelcli -i Cube.kvl -x json -o Cube.json
cat Cube.json | python -m json.tool
