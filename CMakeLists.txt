cmake_minimum_required(VERSION 3.8)

project(OpenGL_Arkonoid)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(${CMAKE_PROJECT_NAME}
	"src/Graphics/Shader.cpp"
	"src/Graphics/Texture2D.cpp"
	"src/Graphics/SpriteRenderer.cpp"
	"src/ResourceManager.cpp"
	"src/Game.cpp"
	"src/GameObject.cpp"
	"src/GameLevel.cpp"
	"src/Ball.cpp"
	"src/Physics/Collision.cpp"
	"src/Effects/ParticleGenerator.cpp"
	"src/Effects/PostProcessor.cpp"
	Main.cpp
)

find_package(glad CONFIG REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glfw)

find_package(Stb REQUIRED)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})

find_package(glm CONFIG REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glm::glm)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)