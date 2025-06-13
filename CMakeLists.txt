cmake_minimum_required(VERSION 3.10)
project(GLCanvasApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set wxWidgets path (can be overridden by environment variable or command line)
if(NOT DEFINED wxWidgets_ROOT_DIR AND DEFINED ENV{WXWIN})
    set(wxWidgets_ROOT_DIR $ENV{WXWIN})
    # Set additional paths for better detection
    set(wxWidgets_LIB_DIR "$ENV{WXWIN}/wxMSW-3.3.0_vc14x_x64_Dev/lib/vc14x_x64_dll")
    set(wxWidgets_INCLUDE_DIRS "$ENV{WXWIN}/include" "$ENV{WXWIN}/wxMSW-3.3.0_vc14x_x64_Dev/lib/vc14x_x64_dll/mswu")
elseif(WIN32)
    # Fallback for current setup
    set(wxWidgets_ROOT_DIR "C:/wxWidgets-3.3.0")
    set(wxWidgets_LIB_DIR "C:/wxWidgets-3.3.0/wxMSW-3.3.0_vc14x_x64_Dev/lib/vc14x_x64_dll")
    set(wxWidgets_INCLUDE_DIRS "C:/wxWidgets-3.3.0/include" "C:/wxWidgets-3.3.0/wxMSW-3.3.0_vc14x_x64_Dev/lib/vc14x_x64_dll/mswu")
endif()

# Find required packages
find_package(wxWidgets REQUIRED COMPONENTS core base gl)
include(${wxWidgets_USE_FILE})

# Add OpenGL
if(WIN32)
    set(OPENGL_LIBRARIES opengl32 glu32)
else()
    find_package(OpenGL REQUIRED)
    set(OPENGL_LIBRARIES ${OPENGL_LIBRARIES})
endif()

# Add source files
set(SOURCES
    src/MainFrame.cpp
    src/GLCanvas.cpp
)

# Add header files
set(HEADERS
    include/MainFrame.h
    include/GLCanvas.h
)

# Create executable (Windows app, not console)
add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${wxWidgets_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${wxWidgets_LIBRARIES}
    ${OPENGL_LIBRARIES}
)

# Copy resources to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# On Windows, copy required DLLs to the output directory
if(WIN32 AND wxWidgets_FOUND)
    # Copy button.png to the executable directory
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/button.png"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/button.png"
    )
    
    # Try to find and copy wxWidgets DLLs
    if(DEFINED ENV{WXWIN})
        set(WXWIDGETS_DLL_DIR "$ENV{WXWIN}/wxMSW-3.3.0_vc14x_x64_ReleaseDLL/lib/vc14x_x64_dll")
        if(EXISTS "${WXWIDGETS_DLL_DIR}")
            file(GLOB WXWIDGETS_DLLS "${WXWIDGETS_DLL_DIR}/*.dll")
            foreach(DLL ${WXWIDGETS_DLLS})
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${DLL}"
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
                )
            endforeach()
        endif()
    endif()
endif() 