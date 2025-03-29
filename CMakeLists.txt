cmake_minimum_required(VERSION 3.18)

project(DBBeagle VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

# Build type options
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Sql LinguistTools)

# Source files
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/dbbeagleapplication.cpp
    src/dbconnectiondialog.cpp
    src/adaptors/mysqldialectadaptor.cpp
    src/adaptors/orasqldialectadaptor.cpp
    src/appaboutdialog.cpp
)

# Header files
set(HEADERS
    src/mainwindow.h
    src/dbbeagleapplication.h
    src/dbconnectiondialog.h
    src/sqldialectadaptor.h
    src/adaptors/mysqldialectadaptor.h
    src/adaptors/orasqldialectadaptor.h
    src/appaboutdialog.h
)

# UI files
set(FORMS
    src/mainwindow.ui
    src/dbconnectiondialog.ui
    src/appaboutdialog.ui
)

# Resources
set(RESOURCES
    src/dbbeagle.qrc
)

# Get version info from Git
execute_process(
    COMMAND git describe
    OUTPUT_VARIABLE GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Get timestamp and ensure it's a numeric value
execute_process(
    COMMAND git log -n 1 --format=format:"%at"
    OUTPUT_VARIABLE GIT_TIMESTAMP
    OUTPUT_STRIP_TRAILING_WHITESPACE
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Remove any quotes from the timestamp string
string(REPLACE "\"" "" GIT_TIMESTAMP "${GIT_TIMESTAMP}")

# Extract version components from git version string
string(REGEX MATCH "^v([0-9]+)\\.([0-9]+)\\.([0-9]+)" VERSION_MATCH "${GIT_VERSION}")
if(VERSION_MATCH)
    set(APP_MAJOR ${CMAKE_MATCH_1})
    set(APP_MINOR ${CMAKE_MATCH_2})
    set(APP_PATCH ${CMAKE_MATCH_3})
    set(APP_VERSION "${APP_MAJOR}.${APP_MINOR}.${APP_PATCH}")
else()
    # Default values if the regex doesn't match
    set(APP_MAJOR 0)
    set(APP_MINOR 0)
    set(APP_PATCH 0)
    set(APP_VERSION "0.0.0")
endif()

string(REGEX MATCH "^v([0-9]+)\\.([0-9]+)\\.([0-9]+)-([0-9]+)-(.*)" VERSION_MATCH_EXTENDED "${GIT_VERSION}")
if(VERSION_MATCH_EXTENDED)
    set(APP_BUILD ${CMAKE_MATCH_4})
    set(APP_REVISION ${CMAKE_MATCH_5})
    set(APP_VERSION_RC "${APP_MAJOR},${APP_MINOR},${APP_PATCH},${APP_BUILD}")
endif()

# Configure version.h from template
configure_file(
    ${CMAKE_SOURCE_DIR}/src/version.h.cmake.in
    ${CMAKE_BINARY_DIR}/version.h
)

# Create executable
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${FORMS}
    ${RESOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}  # For generated version.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
)

# Enable warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
elseif(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
endif()

# Windows-specific resources
if(WIN32)
    target_sources(${PROJECT_NAME} PRIVATE src/dbbeagle_win.rc)
endif()

# macOS-specific icon
if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_ICON_FILE icon.icns
    )
    set_source_files_properties(src/icons/icon.icns PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
    )
    target_sources(${PROJECT_NAME} PRIVATE src/icons/icon.icns)
endif()

# Translations
qt_add_translations(${PROJECT_NAME}
    TS_FILES src/dbbeagle_ru.ts
    QM_FILES_OUTPUT_VARIABLE QM_FILES
)

# Installation rules
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install translation files
if(APPLE)
    # For macOS, install inside the bundle
    install(FILES ${QM_FILES} 
            DESTINATION "${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}.app/Contents/Resources/translations")
elseif(WIN32)
    # For Windows, install alongside the executable
    install(FILES ${QM_FILES} 
            DESTINATION ${CMAKE_INSTALL_BINDIR}/translations)
else()
    # For Linux/Unix, install to standard data directory
    install(FILES ${QM_FILES} 
            DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/translations)
endif()

# Packaging with CPack
include(CPack)
set(CPACK_PACKAGE_VENDOR "Andrew Novikov")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Database exploration tool")
set(CPACK_PACKAGE_VERSION ${APP_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "DBBeagle")
