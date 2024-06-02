FIND_PACKAGE(glog QUIET)
FIND_PACKAGE(spdlog QUIET)

FILE(GLOB cpp_sources *.cc)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/target/lib)
# 遍历所有的.cpp文件
FOREACH(source_file ${cpp_sources})
  # 获取文件名，不包含扩展名
  GET_FILENAME_COMPONENT(exec_name ${source_file} NAME_WE)
  # 添加可执行文件目标
  ADD_EXECUTABLE(${exec_name} ${source_file})
  #  ADD_DEPENDENCIES(${exec_name} ${PROJECT_TARGET})
  TARGET_LINK_LIBRARIES(${exec_name} ${PROJECT_TARGET} ${YAML_CPP_LIBRARIES})
  IF(glog_FOUND)
    MESSAGE(STATUS "glog_FOUND: ${glog_FOUND}")
    TARGET_COMPILE_DEFINITIONS(${exec_name} PRIVATE HAVE_GLOG)
    TARGET_LINK_LIBRARIES(${exec_name} PRIVATE glog::glog)
  ENDIF()

  IF(spdlog_FOUND)
    MESSAGE(STATUS "spdlog_FOUND: ${spdlog_FOUND}")
    TARGET_COMPILE_DEFINITIONS(${exec_name} PRIVATE HAVE_SPDLOG)
    TARGET_LINK_LIBRARIES(${exec_name} PRIVATE spdlog::spdlog_header_only)
  ENDIF()
ENDFOREACH()

