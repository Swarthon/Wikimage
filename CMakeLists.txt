cmake_minimum_required(VERSION 2.6)

project(wikimage)

add_executable(wikimage src/wikimage.c)

find_package(CURL REQUIRED)
if (CURL_FOUND)
	include_directories (${CURL_INCLUDE_DIRS})
	target_link_libraries (wikimage ${CURL_LIBRARIES})
endif (CURL_FOUND)

# ImageMagick
# The ImageMagick library names come decorated with High Dynamic Range
# configuration.
# A compilation flag is required to choose the correct library.
# Wikimake is not using HDRI
add_definitions(-DMAGICKCORE_HDRI_ENABLE=0)
# Same for Quantum Depth value where 16 is selected
add_definitions(-DMAGICKCORE_QUANTUM_DEPTH=16)

find_package(ImageMagick REQUIRED)
find_package(ImageMagick COMPONENTS MagickCore REQUIRED)
if (ImageMagick_FOUND)
	include_directories (${ImageMagick_INCLUDE_DIRS} ${ImageMagick_MagickCore_INCLUDE_DIRS})
	target_link_libraries (wikimage -lMagickCore-6.Q16)
	set(MAGICKCORE_HDRI_ENABLE 0)
	set(MAGICKCORE_QUANTUM_DEPTH 16)
endif (ImageMagick_FOUND)

find_package(X11 REQUIRED)
if (X11_FOUND)
	include_directories (${X11_INCLUDE_DIR})
	target_link_libraries (wikimage -lX11)
endif (X11_FOUND)
