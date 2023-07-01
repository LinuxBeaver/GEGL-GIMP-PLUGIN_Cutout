# GEGL-CutOut-Effect
GEGL does Cut out effect. Enjoy the new Gimp filter. This also comes with a bonus filter called GEGL Invert transparency.
![image preview](preview4_cutout.png )



## OS specific location to put GEGL Filter binaries 

Windows
 C:\\Users\<YOUR NAME>\AppData\Local\gegl-0.4\plug-ins
 
 Linux 
 /home/(USERNAME)/.local/share/gegl-0.4/plug-ins
 
 Linux (Flatpak)
 /home/(USERNAME)/.var/app/org.gimp.GIMP/data/gegl-0.4/plug-ins

 
## Compiling and Installing

### Linux

To compile and install you will need the GEGL header files (`libgegl-dev` on
Debian based distributions or `gegl` on Arch Linux) and meson (`meson` on
most distributions).

```bash
meson setup --buildtype=release build
ninja -C build
```

If you have an older version of gegl you may need to copy to `~/.local/share/gegl-0.3/plug-ins`
instead (on Ubuntu 18.04 for example).



### Windows

The easiest way to compile this project on Windows is by using msys2.  Download
and install it from here: https://www.msys2.org/

Open a msys2 terminal with `C:\msys64\mingw64.exe`.  Run the following to
install required build dependencies:

```bash
pacman --noconfirm -S base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-meson mingw-w64-x86_64-gegl
```

Then build the same way you would on Linux:



## More Previews

![image](https://github.com/LinuxBeaver/GEGL-CutOut-Effect/assets/78667207/03b84623-1060-4791-baf2-d98023f091ec)

![image](https://github.com/LinuxBeaver/GEGL-CutOut-Effect/assets/78667207/4995f08e-65d7-48bc-b3fa-ffc261997698)

## Bonus Filter Preview

![image](https://github.com/LinuxBeaver/GEGL-CutOut-Effect/assets/78667207/3cfb69a3-a960-4626-82b6-2e5389069114)
