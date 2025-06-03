# The Zappy - User Tutorial

## Installing OGRE 3D

```bash
git clone git@github.com:OGRECave/ogre.git
```

### Dependencies
```bash
cd ogre/build
make clean
# or delete the build directory
rm -rf build
```

```bash
sudo dnf install cmake gcc-c++ make git \
                 boost-devel freeimage-devel freetype-devel \
                 libX11-devel libXaw-devel libXrandr-devel libXi-devel \
                 mesa-libGL-devel mesa-libGLU-devel \
                 zziplib-devel SDL2-devel \
                 pugixml-devel assimp-devel

sudo dnf install mesa-libGL-devel mesa-libEGL-devel \
                 mesa-vulkan-devel glslang-devel

sudo dnf install libXrandr-devel
sudo dnf install pkgconfig wayland-devel egl-wayland

# Optional dependencies
sudo dnf install SDL2-devel doxygen
```

### Building OGRE
```bash
cd ogre
mkdir build && cd build

cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr/local \
    -DOGRE_BUILD_SAMPLES=ON \
    -DOGRE_INSTALL_SAMPLES=ON
```

### Compiling
```bash
make -j$(nproc)

sudo make install
sudo ldconfig
```

### Verifying Installation
```bash
pkg-config --modversion OGRE
# or
find /usr/local -name "*ogre*" -type f
```
