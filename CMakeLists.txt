if (CONFIG_NN_CUSTOM_DRIVER)
  zephyr_include_directories(include)
  zephyr_library_sources(nn-custom-driver.c)
endif()
