
# 
# External dependencies
# 

find_package(GLM REQUIRED)
find_package(glbinding REQUIRED)
find_package(globjects REQUIRED)
find_package(libzeug REQUIRED)

find_package(Qt5Core 5.1 REQUIRED)
find_package(Qt5Gui 5.1 REQUIRED)
find_package(Qt5Widgets 5.1 REQUIRED)

# 
# Library name and options
# 

# Target name
set(target gloperate-text-painters)

# Exit here if required dependencies are not met
message(STATUS "Plugin ${target}")


# 
# Sources
# 

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}")
set(source_path  "${CMAKE_CURRENT_SOURCE_DIR}")

set(headers  
    ${include_path}/textrenderingpipeline/GlyphSequenceDemoStage.h
    ${include_path}/textrenderingpipeline/TextRenderingPipeline.h
    ${include_path}/textrenderingpipeline/TextRenderingPainter.h
)

set(sources
    ${source_path}/textrenderingpipeline/GlyphSequenceDemoStage.cpp
    ${source_path}/textrenderingpipeline/TextRenderingPipeline.cpp
    ${source_path}/textrenderingpipeline/TextRenderingPainter.cpp
    
    ${source_path}/plugin.cpp
)

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${include_path} "\\\\.h$|\\\\.hpp$" 
    ${header_group} ${headers})
source_group_by_path(${source_path}  "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${source_group} ${sources})


# 
# Create library
# 

# Build library
add_library(${target} SHARED
    ${sources}
    ${headers}
)

# Create namespaced alias
add_library(${META_PROJECT_NAME}::${target} ALIAS ${target})

# Export library for downstream projects
export(TARGETS ${target} NAMESPACE ${META_PROJECT_NAME}:: FILE ${PROJECT_BINARY_DIR}/cmake/${target}/${target}-export.cmake)


# 
# Project options
# 

set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    FOLDER "${IDE_FOLDER}"
)


# 
# Include directories
# 

target_include_directories(${target}
    PRIVATE
    ${DEFAULT_INCLUDE_DIRECTORIES}
    ${PROJECT_BINARY_DIR}/source/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GLM_INCLUDE_DIR}

    PUBLIC

    INTERFACE
)


# 
# Libraries
# 

target_link_libraries(${target}
    PRIVATE
    cpplocate::cpplocate
    libzeug::iozeug
    libzeug::reflectionzeug
    libzeug::loggingzeug
    libzeug::threadingzeug
    glbinding::glbinding
    globjects::globjects
    ${META_PROJECT_NAME}::gloperate
    ${META_PROJECT_NAME}::gloperate-text
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets

    PUBLIC
    ${DEFAULT_LIBRARIES}

    INTERFACE
)


# 
# Compile definitions
# 

target_compile_definitions(${target}
    PRIVATE

    PUBLIC
    $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:${target_upper}_STATIC_DEFINE>
    ${DEFAULT_COMPILE_DEFINITIONS}
    GLM_FORCE_RADIANS

    INTERFACE
)


# 
# Compile options
# 

target_compile_options(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_COMPILE_OPTIONS}

    INTERFACE
)


# 
# Linker options
# 

target_link_libraries(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_LINKER_OPTIONS}

    INTERFACE
)


# 
# Module information file
# 

generate_module_info(${target}
    VALUES
    name        "${target}"
    version     "${META_VERSION}"
    description "${META_PROJECT_DESCRIPTION}"
    author      "${META_AUTHOR_ORGANIZATION}"

    BUILD_VALUES
    dataPath    "${PROJECT_SOURCE_DIR}/data"

    INSTALL_VALUES
    dataPath    "\${ModulePath}/../data"
)

export_module_info(${target}
    TARGET ${target}
    FOLDER "ModuleInfo"
    RENAME "$<TARGET_FILE_NAME:${target}>.modinfo"
)

install_module_info(${target}
    DESTINATION ${INSTALL_PLUGINS}
    RENAME "$<TARGET_FILE_NAME:${target}>.modinfo"
    COMPONENT runtime
)


# 
# Deployment
# 

# Plugin library
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_PLUGINS} COMPONENT runtime
    LIBRARY DESTINATION ${INSTALL_PLUGINS} COMPONENT runtime
)
