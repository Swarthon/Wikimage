Wikimage
--------

This program is used to define a Wikimedia image as background image. It downloads, resize and crop to fit to your screen. It also adds a description of what it is.

Compiling
---------

To use it, you will need to be on a Linux system (running X11), and have ImageMagick, curl libraries installed. Then run `make`

Running
-------

To simply run the program like this `./wikimage`. If you want to use it as your desktop background image, run it once, then set your desktop image as the image you want (See [Naming](Naming) for more informations about what is what).

Naming
------

This is what each image name stands for :
- `Image.png` : Image downloaded from Wikimedia
- `Scaled_image.png` : Image scaled to fit to the screen (made from `Image.png`)
- `Croped_image.png` : Image croped to be at the exact size of the screen (made from `Scale_image.png`)
- `Text_image.png` : Image containing a text description of the image (made from `Croped_image.png`)
