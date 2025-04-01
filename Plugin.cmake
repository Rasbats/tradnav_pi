# ~~~
# Summary:      Local, non-generic plugin setup
# Copyright (c) 2020-2024 Mike Rossiter
# License:      GPLv3+
# ~~~

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.


# -------- Options ----------

set(OCPN_TEST_REPO
    "mike-rossiter/tradnav-alpha"
    CACHE STRING "Default repository for untagged builds"
)
set(OCPN_BETA_REPO
    "mike-rossiter/tradnav-beta"
    CACHE STRING
    "Default repository for tagged builds matching 'beta'"
)
set(OCPN_RELEASE_REPO
    "mike-rossiter/tradnav-prod"
    CACHE STRING
    "Default repository for tagged builds not matching 'beta'"
)

#
#
# -------  Plugin setup --------
#
set(PKG_NAME TradNav_pi)
set(PKG_VERSION  0.2.0)
set(PKG_PRERELEASE "")  # Empty, or a tag like 'beta'

set(DISPLAY_NAME TradNav)    # Dialogs, installer artifacts, ...
set(PLUGIN_API_NAME TradNav) # As of GetCommonName() in plugin API
set(PKG_SUMMARY "Traditional nav")
set(PKG_DESCRIPTION [=[
Traditional Navigation.
]=])

set(PKG_AUTHOR "Mike Rossiter")
set(PKG_IS_OPEN_SOURCE "yes")
set(PKG_HOMEPAGE https://github.com/Rasbats/tradnav_pi)
set(PKG_INFO_URL https://opencpn.org/OpenCPN/plugins/tradnav.html)

add_definitions(-DocpnUSE_GL)

SET(SRC
        src/TradNav_pi.h
        src/TradNav_pi.cpp
        src/TradNavOverlayFactory.cpp
        src/TradNavOverlayFactory.h
        src/TradNavUIDialogBase.cpp
        src/TradNavUIDialogBase.h
        src/TradNavUIDialog.cpp
        src/TradNavUIDialog.h
        src/icons.h
        src/icons.cpp
        src/NavFunc.cpp
        src/NavFunc.h
        src/pugixml.hpp
    )

set(PKG_API_LIB api-18)  #  A directory in libs/ e. g., api-17 or api-16

macro(late_init)
  # Perform initialization after the PACKAGE_NAME library, compilers
  # and ocpn::api is available.
endmacro ()

macro(add_plugin_libraries)
  # Add libraries required by this plugin

  if(WIN32)
    add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/WindowsHeaders")
    target_link_libraries(${PACKAGE_NAME} windows::headers)

    # add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/glu")
    # target_link_libraries(${PACKAGE_NAME} ocpn::glu_static)
  endif()

  add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/pugixml")
  target_link_libraries(${PACKAGE_NAME} ocpn::pugixml)

  add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/wxJSON")
  target_link_libraries(${PACKAGE_NAME} ocpn::wxjson)

  add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/plugin_dc")
  target_link_libraries(${PACKAGE_NAME} ocpn::plugin-dc)

  add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/jsoncpp")
  target_link_libraries(${PACKAGE_NAME} ocpn::jsoncpp)

  add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/sqlite")
  target_link_libraries(${PACKAGE_NAME} ocpn::sqlite)


  # The wxsvg library enables SVG overall in the plugin
  add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/wxsvg")
  target_link_libraries(${PACKAGE_NAME} ocpn::wxsvg)
endmacro ()
