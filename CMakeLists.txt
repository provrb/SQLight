cmake_minimum_required(VERSION 3.8)

project(SQLight LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Collect source files
set(SQLITE_SOURCES
    "ext/sqlite3.c"
    "ext/sqlite3.h"
)

file(GLOB_RECURSE SRC_FILES 
    "src/frontend/*.cxx"
    "src/backend/*.cxx"
    "include/frontend/*.hxx"
    "include/backend/*.hxx"
)

find_package(wxWidgets REQUIRED COMPONENTS core base stc aui)

# Define the executable
add_executable(SQLight ${SRC_FILES} ${SQLITE_SOURCES})

# Tell compiler to look for WinMain, not main, since this is a GUI app
set_target_properties(SQLight PROPERTIES WIN32_EXECUTABLE TRUE)

# Targets
target_include_directories(SQLight PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    include
    ext
    ${wxWidgets_INCLUDE_DIRS}
)

target_compile_definitions(SQLight PRIVATE ${wxWidgets_DEFINITIONS})
target_link_libraries(SQLight PRIVATE ${wxWidgets_LIBRARIES})

# MSVC Specific
if (MSVC)
    target_compile_definitions(SQLight PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
