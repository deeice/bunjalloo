# Convert png icons to .c and .h files

grit -pw 16 -gB 8 -m! -ftc -fh -q -pe64 checkbox_img.png
grit -pw 16 -gB 8 -m! -ftc -fh -q -pe64 radiobutton_img.png
grit -pw 16 -gB 8 -m! -ftc -fh -q -pe64 scroll_img.png

gcc -c checkbox_img.c
gcc -c radiobutton_img.c
gcc -c scroll_img.c
