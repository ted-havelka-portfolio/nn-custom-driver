# SPDX-License-Identifier: MIT

zephyr_syscall_header(${CMAKE_CURRENT_SOURCE_DIR}/include/nn-custom-driver.h)

if (CONFIG_NN_DRIVER)
  zephyr_include_directories(include)
  zephyr_library_sources(nn-custom-driver.c)
endif()
