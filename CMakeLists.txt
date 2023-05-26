cmake_minimum_required(VERSION 3.22.1)
project(KeysTracker VERSION 1.0)
add_subdirectory(src)

install(TARGETS KeysTracker DESTINATION bin)

# setup installer
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${KeysTracker_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${KeysTracker_VERSION_MINOR}")
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_PACKAGE_CONTACT "sample@gmail.com")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libavcodec-dev (>= 4.4.2) libswscale-dev (>= 4.4.2)")
set(CPACK_DEBIAN_PACKAGE_SECTION "misc")

message(STATUS ${AVCODEC_LIBRARY})
include(CPack)



#cpack -G DEB -C Release
#cpack -G DEB -C Debug 

