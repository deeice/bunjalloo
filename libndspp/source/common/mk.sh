# Convert png icons to .c and .h files
#grit minimessage_data.png -pw 16 -gB 4 -m! -ftc -fh -q -pe2 -pT 1

gcc -g -c minimessage_data.c

g++ -g -c -DZIPIT_Z2 -I ../../include CanvasCommon.cpp
g++ -g -c -DZIPIT_Z2 -I ../../include -I ../pc Client.cpp
g++ -g -c -DZIPIT_Z2 -I ../../include FileCommon.cpp
g++ -g -c -DZIPIT_Z2 -I ../../include Image.cpp
g++ -g -c -DZIPIT_Z2 -I ../../include MiniMessage.cpp
g++ -g -c -DZIPIT_Z2 -I ../../include Rectangle.cpp
g++ -g -c -DZIPIT_Z2 -I ../../include -I ../pc VideoGL_common.cpp
