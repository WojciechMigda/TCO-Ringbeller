set( CMAKE_CXX_STANDARD 17 )

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(APP tests)

add_executable( ${APP} "src/dummy.cpp" )

target_sources( ${APP} PRIVATE
  "src/tests.cpp"
  "src/sync_read.cpp"
  "src/sync_write.cpp"
)

include_directories( "src" )

target_link_libraries( ${APP} ringbeller_static )
target_include_directories( ${APP} PRIVATE
  "$<TARGET_PROPERTY:boost::exp,MY_INCLUDE_DIRS>" )
