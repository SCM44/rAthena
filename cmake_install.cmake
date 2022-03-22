# Install script for directory: /home/debian/rathena

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/debian/rathena/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xRuntime_basex" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE FILE FILES
    "/home/debian/rathena/athena-start"
    "/home/debian/rathena/charserv.bat"
    "/home/debian/rathena/dbghelp.dll"
    "/home/debian/rathena/libmysql.dll"
    "/home/debian/rathena/LICENSE"
    "/home/debian/rathena/logserv.bat"
    "/home/debian/rathena/mapserv.bat"
    "/home/debian/rathena/pcre3.dll"
    "/home/debian/rathena/README.txt"
    "/home/debian/rathena/runserver.bat"
    "/home/debian/rathena/serv.bat"
    "/home/debian/rathena/zlib1.dll"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xRuntime_basex" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/conf" TYPE DIRECTORY FILES "/home/debian/rathena/conf/" REGEX "/[\\\\.]svn$" EXCLUDE REGEX "/conf\\/import\\-tmpl$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xRuntime_basex" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/db" TYPE DIRECTORY FILES "/home/debian/rathena/db/" REGEX "/[\\\\.]svn$" EXCLUDE REGEX "/conf\\/import\\-tmpl$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xRuntime_basex" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/doc" TYPE DIRECTORY FILES "/home/debian/rathena/doc/" REGEX "/[\\\\.]svn$" EXCLUDE REGEX "/conf\\/import\\-tmpl$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xRuntime_basex" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/log" TYPE DIRECTORY FILES "/home/debian/rathena/log/" REGEX "/[\\\\.]svn$" EXCLUDE REGEX "/conf\\/import\\-tmpl$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xRuntime_basex" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/npc" TYPE DIRECTORY FILES "/home/debian/rathena/npc/" REGEX "/[\\\\.]svn$" EXCLUDE REGEX "/conf\\/import\\-tmpl$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xRuntime_basex" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/sql-files" TYPE DIRECTORY FILES "/home/debian/rathena/sql-files/" REGEX "/[\\\\.]svn$" EXCLUDE REGEX "/conf\\/import\\-tmpl$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xRuntime_basex" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/tools" TYPE DIRECTORY FILES "/home/debian/rathena/tools/" REGEX "/[\\\\.]svn$" EXCLUDE REGEX "/conf\\/import\\-tmpl$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xRuntime_templatesx" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/debian/rathena/InstallTemplates.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/debian/rathena/3rdparty/cmake_install.cmake")
  include("/home/debian/rathena/src/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/debian/rathena/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
