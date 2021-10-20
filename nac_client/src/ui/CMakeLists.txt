cmake_minimum_required (VERSION 3.8)


add_library(hune_ui 
            HuneEventCallback.cpp
            huneuicommon.cpp
            nacapplication.cpp
            nacmainwindow.cpp
            )

TARGET_LINK_LIBRARIES(hune_ui ${GTK3_LIBRARIES} ${OPENSSL_LIBRARIES} hune_core)
