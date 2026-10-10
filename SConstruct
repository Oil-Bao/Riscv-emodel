# Global Python imports
import atexit
import itertools
import os
import sys

from os import mkdir, remove, environ, listdir
from os.path import abspath, dirname, expanduser
from os.path import isdir, isfile
from os.path import join, split

import logging
logging.basicConfig()

# SCons imports
import SCons
import SCons.Node
import SCons.Node.FS
import SCons.Tool
from SCons.Errors import UserError as SConsUserError
try:
    # SCons.Errors.SConsEnvironmentError for version > 4.0.0
    from SCons.Errors import SConsEnvironmentError
except ImportError:
    # SCons.Errors.EnvironmentError for version < 4.0.0
    from SCons.Errors import EnvironmentError as SConsEnvironmentError

if getattr(SCons, '__version__', None) in ('3.0.0', '3.0.1'):
    # Monkey patch a fix which appears in version 3.0.2, since we only
    # require version 3.0.0
    def __hash__(self):
        return hash(self.lstr)
    import SCons.Subst
    SCons.Subst.Literal.__hash__ = __hash__


########################################################################
#
# Command line options.
#
########################################################################

linker_options = ('bfd', 'gold', 'lld', 'mold')

AddOption('--no-colors', dest='use_colors', action='store_false',
          help="Don't add color to abbreviated scons output")


# Imports of gem5_scons happen here since it depends on some options which are
# declared above.
from gem5_scons import error, warning, summarize_warnings, parse_build_path
from gem5_scons import TempFileSpawn, EnvDefaults, MakeAction, MakeActionTool
from gem5_scons import kconfig
import gem5_scons
from gem5_scons.builders import ConfigFile, AddLocalRPATH, SwitchingHeaders
from gem5_scons.builders import Blob
from gem5_scons.sources import TagImpliesTool
from gem5_scons.util import compareVersions, readCommand

########################################################################
#
# Set up the main build environment.
#
########################################################################

main = Environment(tools=[
        'default', 'git', TempFileSpawn, EnvDefaults, MakeActionTool,
        ConfigFile, AddLocalRPATH, SwitchingHeaders, TagImpliesTool, Blob
    ])

main.Tool(SCons.Tool.FindTool(['gcc', 'clang'], main))
main.Tool(SCons.Tool.FindTool(['g++', 'clang++'], main))

Export('main')

from gem5_scons.util import get_termcap
termcap = get_termcap()

def makePathAbsolute(path, root=GetLaunchDir()):
    return abspath(os.path.join(root, expanduser(str(path))))
def makePathListAbsolute(path_list, root=GetLaunchDir()):
    return [makePathAbsolute(p, root) for p in path_list]

if BUILD_TARGETS and BUILD_TARGETS[0] in kconfig_actions:
    # The build targets are really arguments for the kconfig action.
    kconfig_args = BUILD_TARGETS[:]
    BUILD_TARGETS[:] = []

    kconfig_action = kconfig_args[0]
    if len(kconfig_args) < 2:
        error(f'Missing arguments for kconfig action {kconfig_action}')
    dir_to_configure = makePathAbsolute(kconfig_args[1])

    kconfig_args = kconfig_args[2:]

    variant_paths = {dir_to_configure}
else:
    # Each target must have 'build' in the interior of the path; the
    # directory below this will determine the build parameters.  For
    # example, for target 'foo/bar/build/X86/arch/x86/blah.do' we
    # recognize that X86 specifies the configuration because it
    # follow 'build' in the build path.

    # The funky assignment to "[:]" is needed to replace the list contents
    # in place rather than reassign the symbol to a new list, which
    # doesn't work (obviously!).
    BUILD_TARGETS[:] = makePathListAbsolute(BUILD_TARGETS)

    # Generate a list of the unique build directories that the collected
    # targets reference.
    variant_paths = set(map(parse_build_path, BUILD_TARGETS))
    kconfig_action = None


########################################################################
#
# Set up various paths.
#
########################################################################

base_dir = Dir('#src').abspath
Export('base_dir')

# the ext directory should be on the #includes path
main.Append(CPPPATH=[Dir('ext')])

# Add shared top-level headers
#main.Prepend(CPPPATH=Dir('include'))
if not GetOption('duplicate_sources'):
    main.Prepend(CPPPATH=Dir('src'))

CXX_version = readCommand([main['CXX'], '--version'], exception=False)

main['GCC'] = CXX_version and CXX_version.find('g++') >= 0
main['CLANG'] = CXX_version and CXX_version.find('clang') >= 0
if main['GCC'] + main['CLANG'] > 1:
    error('Two compilers enabled at once?')

# Find the gem5 binary target architecture (usually host architecture). The
# "Target: <target>" is consistent accross gcc and clang at the time of
# writting this.
bin_target_arch = readCommand([main['CXX'], '--verbose'], exception=False)


