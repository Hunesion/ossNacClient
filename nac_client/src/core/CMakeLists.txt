cmake_minimum_required (VERSION 3.8)

add_library(hune_core  
            EventManager.cpp
            )

IF (CMAKE_BUILD_TYPE MATCHES Debug)
    TARGET_COMPILE_DEFINITIONS( hune_core PUBLIC -DHUNE_DEBUG=1 )
ENDIF()

TARGET_LINK_LIBRARIES(hune_core ${GTK3_LIBRARIES} -lpthread)

