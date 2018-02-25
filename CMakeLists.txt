cmake_minimum_required(VERSION 2.6)

project(wikimage)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(wikimage src/wikimage.c src/image.c src/download.c src/string_manipulate.c)

# Options
option(USE_X11 "Use X11 to determine screen size" ON)
set(IMAGE_WIDTH 1920 CACHE INT "Default image width when X11 is disable")
set(IMAGE_HEIGHT 1080 CACHE INT "Default image height when X11 is disable")

include_directories (include ${PROJECT_BINARY_DIR}/include)
configure_file (
	"include/config.h.in"
	"include/config.h"
)

# CURL
find_package(CURL REQUIRED)
if (CURL_FOUND)
	include_directories (${CURL_INCLUDE_DIRS})
	target_link_libraries (wikimage ${CURL_LIBRARIES})
endif (CURL_FOUND)

# ImageMagick
# We have to get the ImageMagick_VERSION_STRING for source compatibility
# between 6 and 7
find_package(ImageMagick REQUIRED)
find_package(ImageMagick COMPONENTS MagickCore REQUIRED)
if (ImageMagick_FOUND)
	include_directories (${ImageMagick_INCLUDE_DIRS} ${ImageMagick_MagickCore_INCLUDE_DIRS})
	target_link_libraries (wikimage ${ImageMagick_LIBRARIES})
	# The ImageMagick library names come decorated with High Dynamic Range configuration.
	# A compilation flag is required to choose the correct library.
	# Wikimake is not using HDRI
	add_definitions(-DMAGICKCORE_HDRI_ENABLE=0)
	# Same for Quantum Depth value where 16 is selected
	add_definitions(-DMAGICKCORE_QUANTUM_DEPTH=16)
	if (${ImageMagick_VERSION_STRING} LESS 7)
		add_definitions(-DHAVE_MAGICK_6)
	endif (${ImageMagick_VERSION_STRING} LESS 7)
endif (ImageMagick_FOUND)

# X11
find_package(X11)
if (X11_FOUND)
	include_directories (${X11_INCLUDE_DIR})
	target_link_libraries (wikimage ${X11_LIBRARIES})
endif (X11_FOUND)
