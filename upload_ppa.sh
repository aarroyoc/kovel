#!/bin/bash

# Build Debian Package
#dpkg-buildpackage -rfakeroot -kadrian.arroyocalle@gmail.com
# Build Source package
# Check CHANGELOG file
debuild -S -sa -kadrian.arroyocalle@gmail.com
dput ppa:adrian-arroyocalle/kovel ../kovel*source.changes # dput kovel-ppa
