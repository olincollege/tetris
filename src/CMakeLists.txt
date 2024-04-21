# If you have a program that's just a .c file and it has a main method, define
# an executable. If it has both a .c and .h file, make it a library. Also, link
# libraries appropriately here. Targets should be added in dependency order - if
# executable bar needs library foo, then you should do
#   add_library(foo foo.c foo.h)
#   add_executable(bar bar.c)
#   target_link_libraries(bar PRIVATE foo)
# or similar. The PRIVATE/INTERFACE/PUBLIC keyword will depend on whether the
# library is used only in function bodies (PRIVATE), only in function
# signatures/types (INTERFACE), or both (PUBLIC).

# 1. Look for a SDL2 package, 2. look for the SDL2 component and 3. fail if none can be found
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

# 1. Look for a SDL2 package, 2. Look for the SDL2maincomponent and 3. DO NOT fail when SDL2main is not available
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

# Create your game executable target as usual
add_library(view view.c view.h)
add_library(controller controller.c controller.h)
add_library(model model.c model.h)
add_executable(main main.c)
target_link_libraries(main PRIVATE view controller)
target_link_libraries(main PUBLIC model)
target_link_libraries(controller PUBLIC model)
target_link_libraries(view PUBLIC model)
target_link_libraries(model PRIVATE m)

# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(view PRIVATE SDL2::SDL2main)
    target_link_libraries(controller PRIVATE SDL2::SDL2main)
    target_link_libraries(model PRIVATE SDL2::SDL2main)
endif()

# Link to the actual SDL2 library. SDL2::SDL2 is the shared SDL library, SDL2::SDL2-static is the static SDL libarary.
target_link_libraries(view PRIVATE SDL2::SDL2)
target_link_libraries(controller PRIVATE SDL2::SDL2)
target_link_libraries(model PRIVATE SDL2::SDL2)


# # If you have a program that's just a .c file and it has a main method, define
# # an executable. If it has both a .c and .h file, make it a library. Also, link
# # libraries appropriately here. Targets should be added in dependency order - if
# # executable bar needs library foo, then you should do
# #   add_library(foo foo.c foo.h)
# #   add_executable(bar bar.c)
# #   target_link_libraries(bar PRIVATE foo)
# # or similar. The PRIVATE/INTERFACE/PUBLIC keyword will depend on whether the
# # library is used only in function bodies (PRIVATE), only in function
# # signatures/types (INTERFACE), or both (PUBLIC).

# # 1. Look for a SDL2 package, 2. look for the SDL2 component and 3. fail if none can be found
# find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

# # 1. Look for a SDL2 package, 2. Look for the SDL2maincomponent and 3. DO NOT fail when SDL2main is not available
# find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

# # Create your game executable target as usual
# add_executable(main falling_shapes.c)

# # SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
# if(TARGET SDL2::SDL2main)
#     # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
#     target_link_libraries(main PRIVATE SDL2::SDL2main)
# endif()

# # Link to the actual SDL2 library. SDL2::SDL2 is the shared SDL library, SDL2::SDL2-static is the static SDL libarary.
# target_link_libraries(main PRIVATE SDL2::SDL2)
