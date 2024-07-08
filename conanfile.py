from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout


class GameEngine(ConanFile):
    name = "GameEngine"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "CMakeLists.txt", "src/*"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    def requirements(self):
        self.requires("sdl/2.30.4")
        self.requires("sdl_image/2.6.3", override=True)
        self.requires("sdl_mixer/2.8.0", override=True)
        self.requires("sdl_ttf/2.22.0", override=True)
        self.requires("imgui/1.90.8")
        self.requires("glm/1.0.1")
        self.requires("lua/5.4.6")
        self.requires("sol2/3.3.1")
        self.requires("spdlog/1.14.1")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
