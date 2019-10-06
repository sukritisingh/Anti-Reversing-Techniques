project(Technique1 C)

cmake_minimum_required(VERSION 3.0)

exec_program("/bin/sh"
    ${CMAKE_CURRENT_SOURCE_DIR}
    ARGS "-c 'cat /dev/urandom | tr -dc a-zA-Z0-9 | head -c 32'"
    OUTPUT_VARIABLE generated_password )


add_definitions(-Dorig_password="${generated_password}")

#set(CMAKE_C_COMPILER musl-gcc)

#set the compiler options
#set(CMAKE_C_FLAGS "-static -fvisibility=hidden -std=gnu11")
#set(CMAKE_C_FLAGS " -s -fvisibility=hidden -O3 -funroll-loops -static -std=gnu11")

set(CMAKE_C_FLAGS "-g -std=gnu11")


add_executable(${PROJECT_NAME} ./tech1.c)

# After the build is successful, display the random password to the user
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E echo
                   "The bind shell password is:" ${generated_password})
