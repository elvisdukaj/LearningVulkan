function (add_vulkan_executable)
  set(options "")
  set(oneValueArgs TARGET)
  set(multiValueArgs SOURCES MODULES)
  cmake_parse_arguments(ADD_VULKAN_EXECUTABLE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  add_executable(${ADD_VULKAN_EXECUTABLE_TARGET})
  target_sources(${ADD_VULKAN_EXECUTABLE_TARGET}
      PRIVATE ${ADD_VULKAN_EXECUTABLE_SOURCES}
      PRIVATE FILE_SET CXX_MODULES FILES ${ADD_VULKAN_EXECUTABLE_MODULES}
  )

  target_compile_definitions(${ADD_VULKAN_EXECUTABLE_TARGET}
    PUBLIC
      # Project
      APPLICATION_NAME="${ADD_VULKAN_EXECUTABLE_TARGET}"
      # Vulkan
      $<$<PLATFORM_ID:Windows>:VK_USE_PLATFORM_WIN32_KHR>
      $<$<PLATFORM_ID:Darwin>:VK_USE_PLATFORM_METAL_EXT>
      # glfw
      $<$<PLATFORM_ID:Windows>:GLFW_EXPOSE_NATIVE_WIN32 GLFW_EXPOSE_NATIVE_WGL>
      $<$<PLATFORM_ID:Darwin>:GLFW_EXPOSE_NATIVE_COCOA>
  )

  target_link_libraries(${ADD_VULKAN_EXECUTABLE_TARGET}
    PRIVATE
      Vulkan::Loader
      vulkan-validationlayers::vulkan-validationlayers
      glfw
  )
endfunction()
