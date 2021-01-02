vcpkg_fail_port_install(ON_ARCH "arm64")
set(PHYSFS_VERSION 3.1.0)
set(SOURCE_PATH ${CURRENT_BUILDTREES_DIR}/src/physfs-4bef0bb415c06377ee854a9d79b24174dcab3252)
vcpkg_download_distfile(ARCHIVE
    URLS "https://github.com/dgengin/physfs/archive/4bef0bb415c06377ee854a9d79b24174dcab3252.tar.gz"
    FILENAME "physfs-${PHYSFS_VERSION}.tar.gz"
    SHA512 e547a1bc7d507f1042525d49451a6ce85ddf6fac194a9d2e34a56ecc1c54f045aba7d6230c603e65eb0be6bef280faa59ce10cd78a5ac66146b0e161c03e2114
)

vcpkg_extract_source_archive_ex(
    OUT_SOURCE_PATH SOURCE_PATH
    ARCHIVE ${ARCHIVE}
    REF ${PHYSFS_VERSION}
)

string(COMPARE EQUAL "${VCPKG_LIBRARY_LINKAGE}" "static" PHYSFS_STATIC)
string(COMPARE EQUAL "${VCPKG_LIBRARY_LINKAGE}" "dynamic" PHYSFS_SHARED)

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
    OPTIONS
        -DPHYSFS_BUILD_STATIC=${PHYSFS_STATIC}
        -DPHYSFS_BUILD_SHARED=${PHYSFS_SHARED}
        -DPHYSFS_BUILD_TEST=OFF
        -DPHYSFS_USE_EXTERNAL_STORMDLL=TRUE
)

vcpkg_install_cmake()
vcpkg_copy_pdbs()

file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/include)
file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/lib/cmake)
file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/lib/cmake)

# Handle copyright
file(COPY ${SOURCE_PATH}/LICENSE.txt DESTINATION ${CURRENT_PACKAGES_DIR}/share/physfs)
file(RENAME ${CURRENT_PACKAGES_DIR}/share/physfs/LICENSE.txt ${CURRENT_PACKAGES_DIR}/share/physfs/copyright)
