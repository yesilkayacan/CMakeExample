# Add source files in the src directory
file(GLOB_RECURSE FILES_SRC CONFIGURE_DEPENDS "*.cpp")
add_library(gui_applib ${FILES_SRC})
target_compile_features(gui_applib PUBLIC cxx_std_17)

# Add include directories
target_include_directories(gui_applib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_include_directories(gui_applib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../extern/glfw-3.3.8/include)
target_include_directories(gui_applib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../extern/glm)
target_include_directories(gui_applib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../extern/vulkan/macOS/include)

# Specify any additional dependencies for this target
target_link_libraries(gui_applib PRIVATE glfw ${Vulkan_LIBRARY} glm)
