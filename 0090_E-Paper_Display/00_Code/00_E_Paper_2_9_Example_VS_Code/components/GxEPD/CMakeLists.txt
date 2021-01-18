set(COMPONENT_REQUIRES   arduino Adafruit_GFX)
set(COMPONENT_PRIV_REQUIRES )
set(COMPONENT_SRCS  src/GxEPD.cpp
                    src/GxGDEH029A1/GxGDEH029A1.cpp
                    src/GxIO/GxIO.cpp
                    src/GxIO/GxIO_SPI/GxIO_SPI.cpp
                    src/GxFont_GFX.cpp)
set(COMPONENT_ADD_INCLUDEDIRS src/GxGDEH029A1
                              src/GxIO
                              src/GxIO/GxIO_SPI
                              src)
register_component()