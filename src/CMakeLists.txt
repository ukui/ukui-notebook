project(ukui-notebook)

include(${PROJECT_SOURCE_DIR}/../cmake/common.cmake)

option(TEST_DEBUG "option for debug" OFF)

if (TEST_DEBUG) 
	add_definitions(-DTEST_DEBUG)
    add_definitions(-DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
    add_definitions(-DPROJECT_BINARY_DIR=\"${PROJECT_BINARY_DIR}\")
endif(TEST_DEBUG)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools X11Extras Network DBus Sql Concurrent REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools X11Extras Network DBus Sql Concurrent REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}
                    ${PROJECT_SOURCE_DIR}/common
                    ${PROJECT_SOURCE_DIR}/utils
                    )

file(GLOB SRC 
    "*.cpp"
    "*.h"
    "utils/*.cpp"
    "utils/*.h"
)

file(GLOB file_ui "*.ui")
QT5_WRAP_UI(ui_FILES ${file_ui} )

add_executable(${PROJECT_NAME}
    ${SRC}
    ${ui_FILES}
)

target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::X11Extras Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::DBus Qt${QT_VERSION_MAJOR}::Sql Qt${QT_VERSION_MAJOR}::Concurrent)

#dependencies
target_link_libraries(${PROJECT_NAME} "-lpthread -lz")

target_link_kysdk_libraries(${PROJECT_NAME})
target_link_KWindowSystem_libraries(${PROJECT_NAME})
target_link_X11_libraries(${PROJECT_NAME})
target_link_qgsettings_libraries(${PROJECT_NAME})

########### install ###############
install(TARGETS ${PROJECT_NAME} DESTINATION /usr/bin/)
file(GLOB TRANS "${PROJECT_SOURCE_DIR}/../translations/${PROJECT_NAME}/*.qm")
install(FILES ${TRANS} DESTINATION  /usr/share/${PROJECT_NAME}/translations/)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/../data/${PROJECT_NAME} DESTINATION /usr/share/kylin-user-guide/data/guide/)
install(FILES ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.desktop DESTINATION /usr/share/applications/)
