project('potlibR', 'cpp',
  version : '1.0.0',
  default_options : ['warning_level=1',
                     'cpp_std=c++17',
                     'wrap_mode=default'])

# Get the library
subproject('potlib')
# -------------------- Path Helpers

pkg_config = configuration_data()
# Get R_HOME
r_home_cmd = run_command('bash', '-c', 'Rscript -e \'R.home()\' | sed \'s/\\[1\\] \\"\\(.*\\)\\"/\\1/\'')
r_home = r_home_cmd.stdout().strip()
# Set the library extension based on the platform
if host_machine.system() == 'darwin'
  pkg_config.set('LIB_EXT', '.dylib')
else
  pkg_config.set('LIB_EXT', '.so')
endif
pkg_config.set('RLIB_PATH', join_paths(r_home, 'lib'))
pkg_config.set('MESON_BDIR', join_paths(meson.current_build_dir(),
                                        'subprojects', 'potlib', 'CppCore'))
pkg_config.set('INC_PATH', join_paths(meson.current_source_dir(),
                                     'subprojects', 'potlib',
                                     'CppCore'))
# Generate Makevars from the template
configure_file(input : 'src/Makevars.in',
               output : 'Makevars',
               configuration : pkg_config)
