#--------------------------------------------------------------------------
# exts
#--------------------------------------------------------------------------
ADD_LIBRARY(exts INTERFACE)

ADD_SUBDIRECTORY(glew)
SET_TARGET_PROPERTIES(glew PROPERTIES FOLDER "exts/glew")
TARGET_INCLUDE_DIRECTORIES(exts INTERFACE glew/include)

ADD_SUBDIRECTORY(misc)
SET_TARGET_PROPERTIES(misc PROPERTIES FOLDER "exts/misc")
TARGET_INCLUDE_DIRECTORIES(exts INTERFACE misc)

SET(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
SET(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
SET(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
SET(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")		  

ADD_SUBDIRECTORY(glfw-3.2.1)
SET_TARGET_PROPERTIES(glfw PROPERTIES FOLDER "exts/glfw")
TARGET_INCLUDE_DIRECTORIES(exts INTERFACE glfw-3.2.1/include)

ADD_SUBDIRECTORY(assimp-3.3.1)
SET_TARGET_PROPERTIES(assimp PROPERTIES FOLDER "exts/assimp")
TARGET_INCLUDE_DIRECTORIES(exts INTERFACE assimp-3.3.1/include)

ADD_SUBDIRECTORY(bullet)
#SET_TARGET_PROPERTIES(BULLET_PHYSICS PROPERTIES FOLDER "exts/bullet")
#TARGET_INCLUDE_DIRECTORIES(exts INTERFACE bullet)

ADD_LIBRARY(json INTERFACE)

TARGET_LINK_LIBRARIES(json INTERFACE exts)
TARGET_INCLUDE_DIRECTORIES(json INTERFACE json/src)
#SET_TARGET_PROPERTIES(json PROPERTIES FOLDER "exts/json")
TARGET_LINK_LIBRARIES(exts INTERFACE json)

ADD_LIBRARY(imgui STATIC 
			imgui/imconfig.h 
			imgui/imgui.cpp 
			imgui/imgui_demo.cpp 
			imgui/imgui_draw.cpp 
			imgui/imgui.h 
			imgui/imgui_internal.h 
			imgui/stb_rect_pack.h 
			imgui/stb_textedit.h 
			imgui/stb_truetype.h 
			imgui/examples/opengl3_example/imgui_impl_glfw_gl3.h
			imgui/examples/opengl3_example/imgui_impl_glfw_gl3.cpp			
			imgui/imgui_dock.h
			imgui/imgui_dock.cpp
			imgui/imgui_user.h
			imgui/imgui_user.inl
			imgui/imgui_color_gradient.h
			imgui/imgui_color_gradient.cpp
			)

TARGET_LINK_LIBRARIES(imgui PUBLIC exts)
TARGET_INCLUDE_DIRECTORIES(imgui PUBLIC imgui imgui/examples/opengl3_example)
SET_TARGET_PROPERTIES(imgui PROPERTIES FOLDER "exts/imgui")
TARGET_LINK_LIBRARIES(exts INTERFACE imgui)

IF(CMAKE_COMPILER_IS_GNUCXX)	

	# Use the package PkgConfig to detect GTK+ headers/library files
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
	include_directories(${GTK3_INCLUDE_DIRS})
	link_directories(${GTK3_LIBRARY_DIRS})
	add_definitions(${GTK3_CFLAGS_OTHER})

	SET(files_nativefiledialog
	nativefiledialog/src/nfd_gtk.c
	)
ENDIF()

IF(MSVC)
	SET(files_nativefiledialog
	nativefiledialog/src/nfd_win.cpp
	)
ENDIF()	

ADD_LIBRARY(nativefiledialog STATIC
			nativefiledialog/src/include/nfd.h
			nativefiledialog/src/nfd_common.h
			nativefiledialog/src/nfd_common.c
			${files_nativefiledialog}
			)
			

TARGET_LINK_LIBRARIES(nativefiledialog PUBLIC exts ${GTK3_LIBRARIES})
TARGET_INCLUDE_DIRECTORIES(nativefiledialog PUBLIC nativefiledialog nativefiledialog/src/include)
SET_TARGET_PROPERTIES(nativefiledialog PROPERTIES FOLDER "exts/nativefiledialog")
			

