# g++ -g -o bunji arm9/*.o bwt/img/*.o bwt/source/*.o ../libndspp/source/common/*.o ../libndspp/source/pc/*.o ../tools/otherlibs/zlib/contrib/minizip/libunzip.a ../tools/otherlibs/matrixssl-1.8.7b/matrixssl/src/libmatrixsslstatic.a -lGL -lGLU -lSDL -ljpeg -lgif -lpng -lz

 g++ -g -o bunji arm9/*.o bwt/img/*.o bwt/source/*.o ../libndspp/source/common/*.o ../libndspp/source/pc/*.o ../tools/otherlibs/zlib/contrib/minizip/libunzip.a ../tools/otherlibs/matrixssl-1.8.7b/matrixssl/src/libmatrixsslstatic.a -lSDL -ljpeg -lgif -lpng -lz

# g++ -static -o bunji arm9/*.o bwt/img/*.o bwt/source/*.o ../libndspp/source/common/*.o ../libndspp/source/pc/*.o ../tools/otherlibs/zlib/contrib/minizip/libunzip.a ../tools/otherlibs/matrixssl-1.8.7b/matrixssl/src/libmatrixsslstatic.a -lSDL -ljpeg -lgif -lpng -lz -lpthread -ldl
