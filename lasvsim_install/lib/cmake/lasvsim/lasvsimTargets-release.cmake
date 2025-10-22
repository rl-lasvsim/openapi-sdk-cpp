#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "lasvsim::lasvsim" for configuration "Release"
set_property(TARGET lasvsim::lasvsim APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(lasvsim::lasvsim PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/liblasvsim.a"
  )

list(APPEND _cmake_import_check_targets lasvsim::lasvsim )
list(APPEND _cmake_import_check_files_for_lasvsim::lasvsim "${_IMPORT_PREFIX}/lib/liblasvsim.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
