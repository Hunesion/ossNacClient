cmake_minimum_required (VERSION 3.8)

add_library(hune_core  
            model/ClientConfig.cpp
            model/GlobalVar.cpp
            model/StringResource.cpp
            base64.cpp
            core.cpp
            crypto.cpp
            DirectoryMonitor.cpp
            EventManager.cpp
            FileInfo.cpp
            FileUtils.cpp
            INIManager.cpp
            IonexLogic.cpp
            IonexShell.cpp
            Logger.cpp
            StringUtils.cpp
            sys_utils.cpp
            TimeUtils.cpp
            )

IF (CMAKE_BUILD_TYPE MATCHES Debug)
    TARGET_COMPILE_DEFINITIONS( hune_core PUBLIC -DHUNE_DEBUG=1 )
ENDIF()

TARGET_LINK_LIBRARIES(hune_core ${GTK3_LIBRARIES} ${OPENSSL_LIBRARIES} -lpthread)

