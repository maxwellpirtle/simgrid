if(enable_smpi AND enable_smpi_MPICH3_testsuite AND SMPI_FORTRAN)
  set(CMAKE_C_COMPILER "${CMAKE_BINARY_DIR}/smpi_script/bin/smpicc")
  set(CMAKE_Fortran_COMPILER "${CMAKE_BINARY_DIR}/smpi_script/bin/smpiff")

  include_directories(BEFORE "${CMAKE_HOME_DIRECTORY}/include/smpi")
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include/")

  #  add_executable(allocmemf EXCLUDE_FROM_ALL allocmemf.f)
  add_executable(c2fmult    EXCLUDE_FROM_ALL c2fmult.c)
  add_executable(c2f2cf    EXCLUDE_FROM_ALL c2f2cf.f c2f2c.c)
  set_property(TARGET c2f2cf PROPERTY INTERPROCEDURAL_OPTIMIZATION FALSE)
  #add_executable(ctypesinf EXCLUDE_FROM_ALL ctypesinf.f ctypesfromc.c)
  add_dependencies(tests c2f2cf c2fmult
                   #ctypesinf allocmemf
  )
  # target_link_libraries(allocmemf simgrid mtest_f77)
  target_link_libraries(c2fmult simgrid mtest_c)
  target_link_libraries(c2f2cf simgrid mtest_f77)
  #target_link_libraries(ctypesinf simgrid mtest_f77)
endif()

set(examples_src
  ${examples_src}
  ${CMAKE_CURRENT_SOURCE_DIR}/add1size.h
  ${CMAKE_CURRENT_SOURCE_DIR}/allocmemf.f
  ${CMAKE_CURRENT_SOURCE_DIR}/c2f2c.c
  ${CMAKE_CURRENT_SOURCE_DIR}/c2f2cf.f
  ${CMAKE_CURRENT_SOURCE_DIR}/c2fmult.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ctypesfromc.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ctypesinf.f
  PARENT_SCOPE)
set(txt_files
  ${txt_files}
  ${CMAKE_CURRENT_SOURCE_DIR}/testlist
  PARENT_SCOPE)
