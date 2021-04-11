include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
include(${CMAKE_ROOT}/Modules/SelectLibraryConfigurations.cmake)
include(${CMAKE_ROOT}/Modules/CMakeFindDependencyMacro.cmake)


set(Fluidsynth_VERSION "2.2.0")
set(Fluidsynth_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/libs/fluidsynth)

if(NOT Fluidsynth_INCLUDE_DIRS)
    find_path(Fluidsynth_INCLUDE_DIRS NAMES libfluidsynth/fluidsynth.h fluidsynth.h PATHS ${Fluidsynth_ROOT}/include/ NO_DEFAULT_PATH)
endif()

if(NOT Fluidsynth_LIBRARY)
    find_library(Fluidsynth_LIBRARY NAMES fluidsynth libfluidsynth PATHS ${Fluidsynth_ROOT}/lib64/ NO_DEFAULT_PATH)
    get_filename_component(Fluidsynth_LIBRARY_DIR ${Fluidsynth_LIBRARY} DIRECTORY)
endif()

find_library( GLIB_LIB NAMES glib glib-2.0 PATH GLIB_LIBRARY_DIR )
find_library( GTHREAD_LIB NAMES gthread gthread-2.0 PATH GTHREAD_LIBRARY_DIR )

list(APPEND Fluidsynth_LIBRARIES ${GLIB_LIB} ${GTHREAD_LIB})

if(Fluidsynth_LIBRARY AND Fluidsynth_INCLUDE_DIRS)
    set(Fluidsynth_FOUND 1)
    list(APPEND Fluidsynth_LIBRARY_DIRS ${Fluidsynth_LIBRARY_DIR})
    list(APPEND Fluidsynth_LIBRARIES ${Fluidsynth_LIBRARY})

    set(Fluidsynth_LIBRARY ${Fluidsynth_LIBRARIES})

  	set(Fluidsynth_LIBRARIES ${Fluidsynth_LIBRARIES} CACHE STRING "")
  	set(Fluidsynth_INCLUDE_DIRS ${Fluidsynth_INCLUDE_DIRS} CACHE STRING "")
  	set(Fluidsynth_LIBRARY_DIRS ${Fluidsynth_LIBRARY_DIRS} CACHE STRING "")

endif()


find_package_handle_standard_args(Fluidsynth REQUIRED_VARS Fluidsynth_LIBRARIES Fluidsynth_LIBRARY_DIRS Fluidsynth_INCLUDE_DIRS)


