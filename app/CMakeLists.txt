set( CMAKE_CXX_STANDARD 17 )

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_library( cli "src/dummy.cpp" )
target_sources( cli PRIVATE "src/cli.cpp" )

target_compile_definitions( cli PRIVATE "$<TARGET_PROPERTY:fmt,INTERFACE_COMPILE_DEFINITIONS>" )

########################################################################

set(APP main)
add_executable( ${APP} "src/dummy.cpp" )
target_sources( ${APP} PRIVATE
  "src/main.cpp"

#  "src/do_at_ok.cpp"
#  "src/do_ati.cpp"
#  "src/do_at_cops.cpp"

#  "src/do_receive_sms.cpp"
#  "src/do_send_sms.cpp"

#  "src/do_receive_vcall.cpp"
#  "src/do_make_vcall.cpp"

  "src/serial_port_setup.cpp"
)

include_directories( "src" )
include_directories(
  "$<TARGET_PROPERTY:clipp,MY_INCLUDE_DIRS>" )
include_directories(
  "$<TARGET_PROPERTY:fmt,MY_INCLUDE_DIRS>" )
include_directories(
  "$<TARGET_PROPERTY:spdlog,MY_INCLUDE_DIRS>" )

target_compile_definitions( ${APP} PRIVATE "$<TARGET_PROPERTY:fmt,INTERFACE_COMPILE_DEFINITIONS>" )

target_link_libraries( ${APP} ringbeller_static cli )


add_executable( ${APP}_tests "src/dummy.cpp" )
target_sources( ${APP}_tests PRIVATE
  "src/tests.cpp"
)
target_link_libraries( ${APP}_tests ringbeller_static cli )
include_directories(
  "$<TARGET_PROPERTY:boost::exp,MY_INCLUDE_DIRS>" )
