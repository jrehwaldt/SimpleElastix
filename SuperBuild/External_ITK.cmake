
#-----------------------------------------------------------------------------
# Get and build itk

# Determine if we need to set to use 64BITS_IDS for windows64
if(CMAKE_CL_64)
  option(ITK_USE_64BITS_IDS "When ON, ITK will use 64 bits integers to
index pixels. This is needed for managing images larger than 4Gb in
some platforms."
    ON)
  mark_as_advanced(ITK_USE_64BITS_IDS)
endif()

get_cmake_property( _varNames VARIABLES )

foreach (_varName ${_varNames})
  if(_varName MATCHES "^ITK_" OR _varName MATCHES "FFTW")
    message( STATUS "Passing variable \"${_varName}=${${_varName}}\" to ITK external project.")
    list(APPEND ITK_VARS ${_varName})
  endif()
endforeach()

list(APPEND ITK_VARS
  PYTHON_EXECUTABLE
  PYTHON_INCLUDE_DIR
  PYTHON_LIBRARY
  PYTHON_DEBUG_LIBRARY
  )

VariableListToCache( ITK_VARS  ep_itk_cache )
VariableListToArgs( ITK_VARS  ep_itk_args )


set(proj ITK)  ## Use ITK convention of calling it ITK
set(ITK_REPOSITORY "${git_protocol}://github.com/jrehwaldt/ITK.git")

set(ITK_USE_GIT_PROTOCOL 0)
if("${git_protocol}" STREQUAL "git")
  set(ITK_USE_GIT_PROTOCOL 1)
endif()

# NOTE: it is very important to update the ITK_DIR path with the ITK version
set(ITK_TAG_COMMAND GIT_TAG c80178ae_c057f30d ) # just before 4.10.1 release + 057f30d cherry picked

if( ${ITK_WRAPPING} OR ${BUILD_SHARED_LIBS} )
  set( ITK_BUILD_SHARED_LIBS ON )
else()
  set( ITK_BUILD_SHARED_LIBS OFF )
  list( APPEND ep_itk_args"-DCMAKE_C_VISIBILITY_PRESET:BOOL=hidden" "-DCMAKE_CXX_VISIBILITY_PRESET:BOOL=hidden" )
endif()


file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/${proj}-build/CMakeCacheInit.txt" "${ep_itk_cache}\n${ep_common_cache}" )

ExternalProject_Add(${proj}
  GIT_REPOSITORY ${ITK_REPOSITORY}
  ${ITK_TAG_COMMAND}
  UPDATE_COMMAND ""
  SOURCE_DIR ${proj}
  BINARY_DIR ${proj}-build
  CMAKE_GENERATOR ${gen}
  CMAKE_ARGS
  --no-warn-unused-cli
  -C "${CMAKE_CURRENT_BINARY_DIR}/${proj}-build/CMakeCacheInit.txt"
  ${ep_itk_args}
  ${ep_common_args}
  -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=ON
  -DCMAKE_VISIBILITY_INLINES_HIDDEN:BOOL=ON
  -DBUILD_EXAMPLES:BOOL=OFF
  -DBUILD_TESTING:BOOL=OFF
  -DBUILD_SHARED_LIBS:BOOL=${ITK_BUILD_SHARED_LIBS}
  -DCMAKE_SKIP_RPATH:BOOL=ON
  -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
  -DITK_LEGACY_REMOVE:BOOL=ON
  -DITK_BUILD_DEFAULT_MODULES:BOOL=ON
  -DModule_ITKReview:BOOL=ON
  -DITK_USE_GIT_PROTOCOL:BOOL=${ITK_USE_GIT_PROTOCOL}
  -DITK_WRAP_float:BOOL=ON
  -DITK_WRAP_unsigned_char:BOOL=ON
  -DITK_WRAP_signed_short:BOOL=ON
  -DITK_WRAP_unsigned_short:BOOL=ON
  -DITK_WRAP_complex_float:BOOL=ON
  -DITK_WRAP_vector_float:BOOL=ON
  -DITK_WRAP_covariant_vector_float:BOOL=ON
  -DITK_WRAP_rgb_signed_short:BOOL=ON
  -DITK_WRAP_rgb_unsigned_char:BOOL=ON
  -DITK_WRAP_rgb_unsigned_short:BOOL=ON
  -DITK_WRAP_PYTHON:BOOL=${ITK_WRAPPING}
  # Required as to not install into system
  -DINSTALL_WRAP_ITK_COMPATIBILITY:BOOL=OFF
  # Swig
  -DSWIG_DIR:PATH=${SWIG_DIR}
  -DSWIG_EXECUTABLE:PATH=${SWIG_EXECUTABLE}
  BUILD_COMMAND ${BUILD_COMMAND_STRING}
  DEPENDS
  ${ITK_DEPENDENCIES}
  )


ExternalProject_Get_Property(ITK install_dir)
set(ITK_DIR "${install_dir}/lib/cmake/ITK-4.10" )
set(WrapITK_DIR "${ITK_DIR}/WrapITK")
