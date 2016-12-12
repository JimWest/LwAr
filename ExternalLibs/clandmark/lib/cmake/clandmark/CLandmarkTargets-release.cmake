#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "CLandmark::clandmark" for configuration "Release"
set_property(TARGET CLandmark::clandmark APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(CLandmark::clandmark PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/clandmark.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/clandmark.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS CLandmark::clandmark )
list(APPEND _IMPORT_CHECK_FILES_FOR_CLandmark::clandmark "${_IMPORT_PREFIX}/lib/clandmark.lib" "${_IMPORT_PREFIX}/bin/clandmark.dll" )

# Import target "CLandmark::flandmark" for configuration "Release"
set_property(TARGET CLandmark::flandmark APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(CLandmark::flandmark PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/flandmark.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/flandmark.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS CLandmark::flandmark )
list(APPEND _IMPORT_CHECK_FILES_FOR_CLandmark::flandmark "${_IMPORT_PREFIX}/lib/flandmark.lib" "${_IMPORT_PREFIX}/bin/flandmark.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
