#!/usr/bin/env bash

#clang-format -style=file -i

echo "clang-tidy"
find ui \( -name \*.c -or -name \*.cpp -or -name \*.cc \) |xargs -i -n1 -P4 clang-tidy -fix -fix-errors {} --
find core \( -name \*.c -or -name \*.cpp -or -name \*.cc \) |xargs -i -n1 -P4 clang-tidy -fix -fix-errors {} --
find cli \( -name \*.c -or -name \*.cpp -or -name \*.cc \) |xargs -i -n1 -P4 clang-tidy -fix -fix-errors {} --
echo "clang-format"
find ui \( -name \*.c -or -name \*.cpp -or -name \*.hpp -or -name \*.h \) |xargs -n12 -P4 clang-format -i -style=file
find core \( -name \*.c -or -name \*.cpp -or -name \*.hpp -or -name \*.h \) |xargs -n12 -P4 clang-format -i -style=file
find cli \( -name \*.c -or -name \*.cpp -or -name \*.hpp -or -name \*.h \) |xargs -n12 -P4 clang-format -i -style=file
