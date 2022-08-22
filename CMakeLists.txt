cmake_minimum_required(VERSION 3.22)

project(App_Calibracao LANGUAGES C CXX VERSION 0.1)

set(EXE_NAME "Calibracao-${CMAKE_PROJECT_VERSION}")



add_executable(${EXE_NAME} "src/main.cpp")


if(CMAKE_GENERATOR MATCHES "Visual Studio 17")
	message("-- Visual Studio Matched!!")
	list(APPEND CMAKE_CXX_FLAGS "/O2" "/std:c++20")
	
endif()

target_compile_definitions(${EXE_NAME} PUBLIC LOW_FPS)




########################## -- Adding other source files -- ##########################

set(OTHER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/GUI_to_application_processing.cpp")

list(APPEND OTHER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/GUI_to_application_processing.h")
list(APPEND OTHER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/thread_functions.cpp")
list(APPEND OTHER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/thread_functions.h")
list(APPEND OTHER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Instrument.h")
list(APPEND OTHER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Instrument.cpp")
list(APPEND OTHER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/FirstOrderInst.h")
list(APPEND OTHER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/FirstOrderInst.cpp")
list(APPEND OTHER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/entry_t.h")
list(APPEND OTHER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/entry_t.cpp")
list(APPEND OTHER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/senoidal_entry.h")
list(APPEND OTHER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/senoidal_entry.cpp")

#message("IMGUI_SOURCES = ${IMGUI_SOURCES}")

target_include_directories(${EXE_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_sources(${EXE_NAME} PUBLIC ${OTHER_SOURCES})


##############################################################################









########################## -- IMGUI Section -- ##########################


set(IMGUI_LIB "ImGUI")

set(IMGUI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/vendor/imgui.cpp")

list(APPEND IMGUI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/vendor/imgui_demo.cpp")
list(APPEND IMGUI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/vendor/imgui_draw.cpp")
list(APPEND IMGUI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/vendor/imgui_tables.cpp")
list(APPEND IMGUI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/vendor/imgui_widgets.cpp")
list(APPEND IMGUI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/vendor/imgui_impl_glfw.cpp")
list(APPEND IMGUI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/vendor/imgui_impl_opengl3.cpp")

#message("IMGUI_SOURCES = ${IMGUI_SOURCES}")

add_library(${IMGUI_LIB} STATIC ${IMGUI_SOURCES})

target_compile_definitions(${IMGUI_LIB} PUBLIC GLEW_STATIC)

target_link_libraries(${EXE_NAME} PUBLIC ${IMGUI_LIB})


target_compile_definitions(${EXE_NAME} PUBLIC GLEW_STATIC)

##############################################################################




######################### -- IMPlot source files -- ##########################

set(IMPLOT "ImPlot")

set(IMPLOT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/vendor/implot.cpp")

list(APPEND IMPLOT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/vendor/implot_items.cpp")

#message("IMGUI_SOURCES = ${IMGUI_SOURCES}")

add_library(${IMPLOT} STATIC ${IMPLOT_SOURCES})

target_link_libraries(${IMPLOT} PUBLIC ${IMGUI_LIB})

target_link_libraries(${EXE_NAME} PUBLIC ${IMPLOT})


##############################################################################





#including the vendor folder as a include folder
target_include_directories(${EXE_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/vendor")


##################### -- Other dependencies root path -- #####################

set(DEPENDENCIES_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)
set(GLEW "${DEPENDENCIES_ROOT_PATH}/GLEW")

set(GLFW "${DEPENDENCIES_ROOT_PATH}/GLFW")

set(DEPENDENCIES_PATH_LIST ${GLEW})
list(APPEND DEPENDENCIES_PATH_LIST ${GLFW})

foreach(DEP_PATH ${DEPENDENCIES_PATH_LIST})
	#message("current path = ${DEP_PATH}")
	target_include_directories(${IMGUI_LIB} PUBLIC "${DEP_PATH}/include")
	target_link_directories(${IMGUI_LIB} PUBLIC "${DEP_PATH}/lib")
	target_include_directories(${EXE_NAME} PUBLIC "${DEP_PATH}/include")
	target_link_directories(${EXE_NAME} PUBLIC "${DEP_PATH}/lib")
endforeach()

##############################################################################




##################### -- Other dependencies lib names -- #####################

set(DEPENDENCIES_LIBS glfw3)
list(APPEND DEPENDENCIES_LIBS glew32s)
list(APPEND DEPENDENCIES_LIBS opengl32)



foreach(LIB ${DEPENDENCIES_LIBS})
	#message("LIB = ${LIB}")
	target_link_libraries(${IMGUI_LIB} PUBLIC ${LIB})
	target_link_libraries(${EXE_NAME} PUBLIC ${LIB})
endforeach()

##############################################################################


#set(AUX a)

#get_target_property(AUX ${EXE_NAME} INCLUDE_DIRECTORIES)
#message("INCLUDE_DIRS = ${AUX}")


