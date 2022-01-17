#!/usr/bin/env bash



brew update
# Build our own versions of these for macOS 10.12 target systems.
brew install --build-from-source ./macos/HomebrewFormulae/berkeley-db.rb
brew install --build-from-source ./macos/HomebrewFormulae/libogg.rb
brew install --build-from-source ./macos/HomebrewFormulae/libvorbis.rb
brew install --build-from-source ./macos/HomebrewFormulae/libsndfile.rb

# TODO: do this in ./HomebrewFormulae instead
# NOTE: may not need qwt, because QTWidgets cmake is in the qt package,
#   and I currently don't see any other thing we need from the qwt package
brew install qt5 qwt libarchive jack pulseaudio cppunit ruby