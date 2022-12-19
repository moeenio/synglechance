import os
import shutil

from conans import CMake, ConanFile, tools


class MkxpConan(ConanFile):
	name = "oneshot"
	version = "0.0.0"
	license = "GPLv2"
	url = "https://github.com/GooborgStudios/synglechange"
	description = "OneShot game runtime"
	settings = "os", "compiler", "build_type", "arch"
	generators = "cmake", "CMakeDeps"
	exports_sources = "*"
	requires = (
		"boost/1.73.0",
		"openal/1.21.1",
		"physfs/3.0.2",
		"pixman/0.40.0",
		"ruby/2.7.5@queengooborg/stable",
		"sdl/2.26.0",
		"sdl_image/2.0.5",
		"sdl_ttf/2.0.18",
		"sdl_sound-mkxp/1.0.1@queengooborg/stable",
		"sigc++/2.10.0@bincrafters/stable",
		# Overrides
		"libpng/1.6.39",
		"zlib/1.2.13",
		"bzip2/1.0.8",
		"libiconv/1.17",
		"openssl/1.1.1s",
		"ogg/1.3.5"
	)
	if tools.os_info.is_windows:
		build_requires = ("ruby_installer/2.7.3@bincrafters/stable", )
	options = {
		"platform": ["standalone", "steam"],
		"debug": [True, False]
	}
	default_options = (
		"platform=standalone",
		"debug=False",
		"boost:without_test=True",
		"cygwin_installer:packages=xxd",
		# Avoid dead url bitrot in cygwin_installer
		"cygwin_installer:with_pear=False",
		"pixman:shared=True",
		"qt:shared=True",
		"qt:with_mysql=False",
		"qt:with_zstd=False"
	)

	#def build_requirements(self):
		# if tools.os_info.is_windows:
			# self.build_requires("cygwin_installer/2.9.0@bincrafters/stable")

	def requirements(self):
		if self.options.platform == "steam":
			steamworks_path = os.path.join(
				os.path.dirname(os.path.realpath(__file__)), 'steamworks'
			)
			if not os.path.exists(steamworks_path):
				raise Error("steamworks not found. Please download from https://partner.steamgames.com/dashboard and put in project root.")
		if tools.os_info.is_linux:
			# Overrides
			self.requires("sqlite3/3.40.0")
			self.requires("flac/1.3.3")
			self.requires("vorbis/1.3.7")
			self.requires("libalsa/1.2.7.2")
		if not tools.os_info.is_windows:
			# For Unix journal
			self.requires("qt/5.15.4")
			self.requires("freetype/2.12.1")
			self.requires("harfbuzz/4.4.1")
			self.requires("glib/2.75.0")
			self.requires("pcre2/10.42")
			self.requires("double-conversion/3.2.1")

	def configure(self):
		self.options["openal"].shared = True
		self.options["sdl2"].shared = True
		self.options["libiconv"].shared = True
		self.options["pcre2"].shared = True
		self.options["double-conversion"].shared = True

	def build_configure(self):
		cmake = CMake(self, msbuild_verbosity='minimal')

		if tools.os_info.is_macos:
			cmake._generator = 'Xcode'
		if self.options.platform == "steam":
			cmake.definitions["STEAM"] = "ON"
		if self.options.debug:
			cmake.definitions["DEBUG"] = "ON"
		cmake.definitions['CMAKE_OSX_ARCHITEXTURES'] = "arm64" if self.settings.arch == "armv8" else self.settings.arch

		cmake.configure()
		cmake.build()

		if tools.os_info.is_macos:
			# Copy Qt plugins to journal app bundle
			qtplugins_dir = os.path.join(
				self.deps_cpp_info['qt'].rootpath,
				'bin',
				'archdatadir',
				'plugins'
			)
			dest = 'bin/_______.app/Contents/Plugins'

			os.makedirs(dest)
			shutil.copytree(
				os.path.join(qtplugins_dir, 'platforms'),
				'bin/_______.app/Contents/Plugins/platforms'
			)
			shutil.copytree(
				os.path.join(qtplugins_dir, 'styles'),
				'bin/_______.app/Contents/Plugins/styles'
			)

	def build(self):
		#if tools.os_info.is_windows:
		#    cygwin_bin = self.deps_env_info["cygwin_installer"].CYGWIN_BIN
		#    with tools.environment_append({
		#            "PATH": [cygwin_bin],
		#            "CONAN_BASH_PATH":
		#            os.path.join(cygwin_bin, "bash.exe")
		#    }):
		#        self.build_configure()
		#else:
		#    self.build_configure()
		self.build_configure()

	def package(self):
		self.copy("*", dst="bin", src="bin")

	def imports(self):
		self.do_copy_deps(self.copy)

	def deploy(self):
		self.copy("*")
		self.do_copy_deps(self.copy_deps)

	def do_copy_deps(self, copy):
		deps = set(self.deps_cpp_info.deps) - set(
			("cygwin_installer", "msys2_installer", "ruby_installer"))
		for dep in deps:
			copy("*.dll",
				 dst="bin",
				 src="bin",
				 root_package=dep,
				 keep_path=False)
			copy("*.dylib*",
				 dst="lib",
				 src="lib",
				 root_package=dep,
				 keep_path=True)
			copy("*.so*",
				 dst="lib",
				 src="lib",
				 root_package=dep,
				 keep_path=True)
			if self.settings.build_type == "Debug":
				copy("*.pdb", dst="bin", root_package=dep, keep_path=False)
