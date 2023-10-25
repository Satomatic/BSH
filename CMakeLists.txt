project(BSH)                          # Create project "simple_example"
set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard

include_directories(src/include)

# Add main.cpp file of the project root directory as a source file
set(SOURCE_FILES "src/main.cpp" "src/include/global.h" 
                 "src/input.cpp" "src/include/input.h"
                 "src/parser.cpp" "src/include/parser.h"
                 "src/builtins.cpp" "src/include/builtins.h"
                 "src/error.cpp" "src/include/serror.h"
                 "src/exec.cpp" "src/include/exec.h" 
                 "src/config.cpp" "src/include/config.h")

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(bsh ${SOURCE_FILES})
