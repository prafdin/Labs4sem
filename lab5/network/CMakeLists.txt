find_package(Qt5 COMPONENTS Core Gui Network Widgets)
set(CMAKE_AUTOMOC ON)

add_library("lab5-network" STATIC "protocol/protocol.cpp" "protocol/protocol.h")
target_include_directories("lab5-network" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/protocol")

add_executable("lab5-network-server" "server/main.cpp" "server/server.cpp" "server/server.h")
add_executable("lab5-network-client" "client/main_window.h" "client/main_window.cpp" "client/server_request.h" "client/server_request.cpp" "client/main.cpp"  )
target_link_libraries("lab5-network-server" "lab5-network" Qt5::Core Qt5::Network)
target_link_libraries("lab5-network-client" "lab5-network" Qt5::Core Qt5::Gui Qt5::Network Qt5::Widgets)

if (WIN32)
	set_property(TARGET "lab5-network-client" PROPERTY WIN32_EXECUTABLE true)

	add_custom_command(TARGET "lab5-network-server" POST_BUILD COMMAND ${Qt5_DIR}/../../../bin/windeployqt $<TARGET_FILE:lab5-network-server>)
	add_custom_command(TARGET "lab5-network-client" POST_BUILD COMMAND ${Qt5_DIR}/../../../bin/windeployqt $<TARGET_FILE:lab5-network-client>)
endif (WIN32)