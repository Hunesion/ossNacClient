cmake_minimum_required (VERSION 3.8)


add_library(hune_ui 
            HuneEventCallback.cpp
            huneuicommon.cpp
            nacapplication.cpp
            nacmainwindow.cpp
            nacwebviewwindow.cpp
            )

TARGET_LINK_LIBRARIES(hune_ui ${GTK3_LIBRARIES} ${WEBKIT_LIBRARIES} ${OPENSSL_LIBRARIES} hune_core)
