from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


class LearningVulkanRecipe(ConanFile):
    name = "LearningVulkan"
    version = "1.0"
    package_type = "application"

    # Optional metadata
    license = "MIT"
    author = "Elvis Dukaj <elvis.dukaj@gmail.com>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of the-vulkan-cookbook package here>"
    topics = ("vulkan", "graphics", "rendering")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*"

    def layout(self):
        # build directory will look like: android-31-armv8-clang-15-debug/windows-x86_64-msvc-193
        self.folders.build_folder_vars = [
            "settings.os", "settings.os.api_level",
            "settings.arch",
            "settings.compiler",
            "settings.compiler.version"
        ]
        cmake_layout(self)

    def requirements(self):
        self.requires("vulkan-loader/1.3.239.0")
        self.requires("vulkan-validationlayers/1.3.239.0")
        self.requires("glfw/3.4")

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.variables["CMAKE_EXPERIMENTAL_CXX_IMPORT_STD"] = "0e5b6991-d74f-4b3d-a41c-cf096e0b2508"
        tc.variables["CMAKE_CXX_MODULE_STD "] = "ON"
        tc.variables["CMAKE_VERBOSE_MAKEFILE"] = "ON"
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    

    
