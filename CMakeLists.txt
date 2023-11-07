cmake_minimum_required(VERSION 3.12)
project(MidifilePerformer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add_subdirectory(src)

if(EMSCRIPTEN)
  add_executable(${PROJECT_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/src/Embindings.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/libMidifilePerformer/src/cpp/impl/ChordVelocityMapping.cpp
  )

  # see all options here :
  # https://github.com/emscripten-core/emscripten/blob/main/src/settings.js

  set(LINK_FLAGS "-lembind")

  if(NODE_VERSION)
    # nothing special to do here for now
  endif()

  if(BROWSER_VERSION)
    set(LINK_FLAGS "${LINK_FLAGS} -sENVIRONMENT=web -sMODULARIZE=1 -sEXPORT_ES6=1")
  endif()

  set_target_properties(${PROJECT_NAME}
    PROPERTIES LINK_FLAGS ${LINK_FLAGS}
  )
endif()