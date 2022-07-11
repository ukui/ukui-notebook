project(ukui-notebook-server)

find_package(PkgConfig REQUIRED)
pkg_check_modules(KYSDKQTWIDGETS_PKG kysdk-qtwidgets)
pkg_check_modules(KYSDKKABASE_PKG kysdk-kabase)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools X11Extras Network DBus Sql Concurrent REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools X11Extras Network DBus Sql Concurrent REQUIRED)
find_package(KF5WindowSystem REQUIRED)

#Gsetting
find_package(PkgConfig REQUIRED)
pkg_check_modules(QGSETTINGS REQUIRED IMPORTED_TARGET gsettings-qt)

#add include 
include_directories(${PROJECT_SOURCE_DIR})

file(GLOB SRC 
    "*.cpp"
    "*.h"
)

add_executable(${PROJECT_NAME}
    ${SRC}
    ${ui_FILES}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${KYSDKQTWIDGETS_PKG_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PRIVATE ${KYSDKQTWIDGETS_PKG_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets ${KYSDKQTWIDGETS_PKG_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${KYSDKKABASE_PKG_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PRIVATE ${KYSDKKABASE_PKG_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME}  ${KYSDKKABASE_PKG_LIBRARIES})

target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::X11Extras Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::DBus Qt${QT_VERSION_MAJOR}::Sql Qt${QT_VERSION_MAJOR}::Concurrent)

#dependencies
target_link_libraries(${PROJECT_NAME} "-lpthread -lz")

target_link_libraries(${PROJECT_NAME} KF5::WindowSystem)

target_link_libraries(${PROJECT_NAME} -lX11)
target_link_libraries(${PROJECT_NAME} PkgConfig::QGSETTINGS)

########### install ###############
install(TARGETS ${PROJECT_NAME} DESTINATION /usr/bin/)
install(FILES ${PROJECT_SOURCE_DIR}/../data/org.ukui.note.conf DESTINATION /etc/dbus-1/system.d)
install(FILES ${PROJECT_SOURCE_DIR}/../data/org.ukui.note.service DESTINATION /usr/share/dbus-1/services/)
