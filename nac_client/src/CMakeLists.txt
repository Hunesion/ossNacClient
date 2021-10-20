
cmake_minimum_required (VERSION 3.8)
add_subdirectory ("core")
add_subdirectory("assets")
add_subdirectory ("ui")
add_executable (nacclient main.cpp resources.cpp)
TARGET_LINK_LIBRARIES(nacclient ${GTK3_LIBRARIES} ${OPENSSL_LIBRARIES} hune_core hune_ui pthread)
