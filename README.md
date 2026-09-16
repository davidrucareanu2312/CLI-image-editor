# CLI Image editor

## General overview
* The aim of this project was that of implementing a simple
*CLI image editor* program, which allows file manipulation through PPM
and PGM formats. The program offers a few functionalities such as *loading*
and *saving* images, *applying filters*, *rotating* and *cropping* portions
of the image.


## Commands implemented

### Load
* When the command is applied effectively, it will try to load the file
and save its content in a struct that was defined in *utils.h* and declared
in *main*. 
* In case of a succesful opening of the file, depending on the format,
either one of the *SAVE_TXT* or *SAVE_BIN* function will be called, otherwise,
a corresponding message will be printed and any saved image will be discarded.

### Select
* When the select command is applied successfully, depending on its input, it
will either reduce the area of effect to the specified region, or resize it
back to the whole image.

### Histogram
* The histogram command when applied effectively, will attempt to generate an
histogram of a grayscale image using *Y* bins and *X* max stars.
* he frequencies of each intensity are calculated and stored, and the number
of stars is determined based on a normalization depending on the X value and
the maximal frequency.

### Equalize
* The equalize command will equalize the intensities of the pixels of an entire
grayscale image, through a calculation based on the frequency of each pixel
value, similarly as the *HISTOGRAM*.

### Rotate
* The rotate command in case of a successful use, will rotate the image stored
in memory with 0, 90, 180 or 270 degrees. Depending on whether there is a
full selection or only a partial one, either the *ROTATE_SELECTION* or the
*ROTATE_ALL* function will be called.
* In both cases, the 0 degree rotation is skipped, the 180 degree rotation is
done through a flip, and the 90 and 270 degree rotation are realized through a
flip and a transposition.

### Crop
* The crop command is reducing the stored image to just the selected region
in the case of an successful apply.

### Apply
* If applied correctly, this command will attempt to apply a filter(*EDGE*,
*SHARPEN*, *BLUR*, *GAUSSIAN_BLUR*) to the selected region of the stored image. 
* It will generate a new temporary image based on the multiplication of the
corresponding kernel, which will afterwards be copied to the stored image.

### Save
* When the save command is applied correctly, it will save in the file at the
given path the stored image, depending on the format already saved and on
whether the *ascii* specifier is mentioned, through the use of either
*SAVE_TXT* or *SAVE_BIN* function.

### Exit
* When this command is called, it will update the *read* variable and allow
exiting the program safely.

### Edge cases
* Every command has its edge cases treated(mostly).
* When no image is loaded, every command call (with some exceptions) will
print the message *"No image loaded"*.

## Other functions used
* The *mallocation* and *free_matrix* functions are defined and used to
effectively alloc memory and free memory to matrices.
* The *min*, *max* and *isnumber* functions are used for the reading of a
*SELECT* command.
* The *ispowertwo* function is used for the reading of the *HISTOGRAM* command.
* The *clamp* function is used to apply effectively an *EDGE* or *SHARPEN*
filter, by ensuring that the pixel luminosities remain in the correct range.