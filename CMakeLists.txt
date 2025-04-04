cmake_minimum_required(VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Project Name
project(SQLight LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Find all source files (fixing the wildcard issue)
file(GLOB_RECURSE SRC_FILES 
    "src/frontend/*.cxx"
    "src/backend/*.cxx"
    "include/frontend/*.hxx"
    "include/backend/*.hxx"
)

# Define the executable
add_executable(SQLight ${SRC_FILES})

# wxWidgets Integration
find_package(wxWidgets REQUIRED COMPONENTS core base)
if(wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
    target_link_libraries(SQLight PRIVATE ${wxWidgets_LIBRARIES})
endif()

# Additional configurations
if (MSVC)
    target_compile_definitions(SQLight PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
