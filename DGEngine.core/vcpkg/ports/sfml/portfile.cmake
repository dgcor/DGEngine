vcpkg_from_github(OUT_SOURCE_PATH SOURCE_PATH
    REPO dgcor/SFML
    REF 24e1d0d21b8287ef062ab9567c584ba8bc10fdb5
    HEAD_REF feature/scancode
    SHA512 7ab871ed185707f80b169f84c6c862efe237da0b4fa0575be56538cc62e20932b0c737c2b480326947e938fd7763755a8054b501a06d8fd7e2abf2f5132962b3
)

#file(REMOVE_RECURSE "${SOURCE_PATH}/extlibs")
# Without this, we get error: list sub-command REMOVE_DUPLICATES requires list to be present.
#file(MAKE_DIRECTORY "${SOURCE_PATH}/extlibs/libs")
#file(WRITE "${SOURCE_PATH}/extlibs/libs/x" "")
# The embedded FindFreetype doesn't properly handle debug libraries
#file(REMOVE_RECURSE "${SOURCE_PATH}/cmake/Modules/FindFreetype.cmake")

if(VCPKG_TARGET_IS_LINUX)
    message(STATUS "SFML currently requires the following libraries from the system package manager:\n    libudev\n    libx11\n    libxrandr\n    opengl\n\nThese can be installed on Ubuntu systems via apt-get install libx11-dev libxrandr-dev libxi-dev libudev-dev libgl1-mesa-dev")
endif()

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DSFML_BUILD_FRAMEWORKS=OFF
        -DSFML_USE_SYSTEM_DEPS=ON
        -DSFML_MISC_INSTALL_PREFIX=share/sfml
        -DSFML_GENERATE_PDB=OFF
)

vcpkg_cmake_install()
vcpkg_cmake_config_fixup(CONFIG_PATH lib/cmake/SFML)
vcpkg_copy_pdbs()

FILE(READ "${CURRENT_PACKAGES_DIR}/share/sfml/SFMLConfig.cmake" SFML_CONFIG)
if(VCPKG_LIBRARY_LINKAGE STREQUAL "static")
    FILE(WRITE "${CURRENT_PACKAGES_DIR}/share/sfml/SFMLConfig.cmake" "set(SFML_STATIC_LIBRARIES true)\ninclude(CMakeFindDependencyMacro)\nfind_dependency(Freetype)\n${SFML_CONFIG}")
else()
    FILE(WRITE "${CURRENT_PACKAGES_DIR}/share/sfml/SFMLConfig.cmake" "set(SFML_STATIC_LIBRARIES false)\n${SFML_CONFIG}")
endif()

# move sfml-main to manual link dir
if(EXISTS "${CURRENT_PACKAGES_DIR}/lib/sfml-main.lib")
    file(COPY "${CURRENT_PACKAGES_DIR}/lib/sfml-main.lib" DESTINATION "${CURRENT_PACKAGES_DIR}/lib/manual-link")
    file(REMOVE "${CURRENT_PACKAGES_DIR}/lib/sfml-main.lib")
    file(COPY "${CURRENT_PACKAGES_DIR}/debug/lib/sfml-main-d.lib" DESTINATION "${CURRENT_PACKAGES_DIR}/debug/lib/manual-link")
    file(REMOVE "${CURRENT_PACKAGES_DIR}/debug/lib/sfml-main-d.lib")
    file(GLOB FILES "${CURRENT_PACKAGES_DIR}/share/sfml/SFML*Targets-*.cmake")
    foreach(FILE ${FILES})
        vcpkg_replace_string("${FILE}" "/lib/sfml-main" "/lib/manual-link/sfml-main")
    endforeach()
endif()

file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/include ${CURRENT_PACKAGES_DIR}/debug/share)

vcpkg_fixup_pkgconfig()

configure_file("${CMAKE_CURRENT_LIST_DIR}/usage" "${CURRENT_PACKAGES_DIR}/share/${PORT}/usage" COPYONLY)
configure_file("${SOURCE_PATH}/license.md" "${CURRENT_PACKAGES_DIR}/share/${PORT}/copyright" COPYONLY)
