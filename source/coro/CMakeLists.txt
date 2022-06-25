target_sources(
  ${CORO_LIBRARY}
    PRIVATE
      utils.cpp generator.cpp iterator.cpp task.cpp traits.cpp
      memory.cpp
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/generator.h>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/generator.impl.h>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/utils.h>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/traits.h>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/utils.impl.h>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/iterator.h>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/iterator.impl.h>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/memory.h>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/task.h>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/task.impl.h>)
