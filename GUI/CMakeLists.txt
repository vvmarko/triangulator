# Adding TriangulatorGUI executable.

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Gui OpenGL OpenGLWidgets)
find_package(Qt6 OPTIONAL_COMPONENTS Widgets)

qt_standard_project_setup()

set(SOURCES
  main.cpp
  DrawComplexGLWidget.cpp
  DrawComplex.cpp
  Inspector.cpp
  LogViewer.cpp
  MainWindow.cpp
  PrintComplex.cpp
  RenameComplex.cpp
  SeedComplex.cpp
  SimpCompTableModel.cpp
  Utils.cpp
)

qt_add_executable(TriangulatorGUI ${SOURCES} ) # For executables                                                                                                                                                        

target_include_directories(TriangulatorGUI PRIVATE
    lib/include
    lib/include/third_party_software/rapidxml
)

target_link_libraries(TriangulatorGUI PRIVATE
    Qt::Core
    Qt::Gui
    Qt::OpenGL
    Qt::OpenGLWidgets
)

if((QT_VERSION_MAJOR GREATER 4))
    target_link_libraries(TriangulatorGUI PRIVATE
        Qt::Widgets
    )
endif()

if(UNIX AND NOT MACOS)
  target_link_libraries(TriangulatorGUI PUBLIC ${TRIANG_INCLUDE_LIBS})
  target_link_libraries(TriangulatorGUI INTERFACE compiler_flags)
endif()

