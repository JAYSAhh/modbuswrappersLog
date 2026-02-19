from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout, CMakeDeps

class ComponentConan(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    exports_sources = "CMakeLists.txt", "src/*", "include/*", "examples/*", "envs/*", "cmake/*", "settings/*", "config.json"

    def layout(self):
        self.folders.source = "."
        self.folders.build = "build"
        self.folders.generators = "deps"

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()


    def configure(self):
        self.options["boost"].shared = True


    def requirements(self):
        self.requires(f"utils/[>=1.1.0 <2.0.0]@{self.user}/{self.channel}", package_id_mode="patch_mode")
        self.requires(f"modbus/[>=2.1.10 <3.0.0]@{self.user}/{self.channel}", package_id_mode="patch_mode")
        self.requires(f"threadpooling/[>=1.1.0 <2.0.0]@{self.user}/{self.channel}", package_id_mode="patch_mode"),
        self.requires("boost/1.82.0")
        self.requires("gtest/1.17.0")

    def build(self):
        cmake = CMake(self)
        cmake.configure(variables={"TARGET_SYSTEM": "Conan"})
        cmake.build(target="modbuswrappers")


    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = [self.name+"d" if self.settings.build_type == "Debug" else self.name]
        self.cpp_info.libdirs = ["lib"]
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.bindirs = ["bin"]
