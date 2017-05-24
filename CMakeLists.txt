cmake_minimum_required(VERSION 2.6)

project(wikimage)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(wikimage src/wikimage.c)

include_directories (include)
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
endif (ImageMagick_FOUND)

# X11
find_package(X11)
if (X11_FOUND)
	include_directories (${X11_INCLUDE_DIR})
	target_link_libraries (wikimage -lX11)
	option(USE_X11 ON)
else ()
	set(IMAGE_WIDTH 1920 CACHE INT "Image default width")
	set(IMAGE_HEIGHT 1080 CACHE INT "Image default height")
endif (X11_FOUND)
