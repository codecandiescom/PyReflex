# $Id: setup.py,v 1.3 2005/03/01 13:32:33 jacek Exp $
from distutils.core import setup, Extension
import os
import sys
#####################################################################
import commands
seal = commands.getoutput('scram b echo_LOCALTOP').split()[-1]
seal_src = seal + '/src'
seal_includeLIST = [seal_src+'/Dictionary/Reflex']

arch = os.getenv('SCRAM_ARCH')
seal_lib = '%s/%s/lib' % (seal, arch)
#####################################################################
# Autogeneration of C++ code.

# Distutils integration is done by a manual hack for now; find better
# solution.

# This consists of checking the timestamps of the generator and any
# auxiliary files, and regenerating the code if it is older than its
# sources.
generated_reflex_location = 'gensrc/%s_generated_reflex.cpp' % (arch)
full_generated_reflex_location = '%s/%s' % (os.getcwd(), generated_reflex_location)

import stat
def timestamp(filename):
    return os.stat(filename)[stat.ST_MTIME] # Why does distutils use ST_MODE ?

times = map(timestamp, ['gensrc/'+ filename
                        for filename in ('Caux.h',
                                         'Converters.h',
                                         'DispatcherTuple.h',
                                         'DyCast.h',
                                         'Field.h',
                                         'GenericFunction.h',
                                         'ProxyObject.h',
                                         'generate_reflex.py',
                                         'godel.py')])
latest = reduce(max, times)

if (not os.path.exists(generated_reflex_location)) or timestamp(generated_reflex_location) < latest:
    os.system('cd gensrc; %s generate_reflex.py where %s' % (sys.executable, full_generated_reflex_location))
#####################################################################
reflex = Extension('python.pyreflex.reflex',
                   [generated_reflex_location],
                   include_dirs = seal_includeLIST,
                   library_dirs = [seal_lib],
                   libraries = ['lcg_Reflex'],
                   extra_compile_args = ['-g']
                   )

setup( name = "Pyreflex",
       version = '0.1',
       description = 'Automatic generation of Python interfaces to C++ code, using Reflex.',
       author = "Massimo Marino, Jacek Generowicz",
       author_email = "jacek.generowicz@cern.ch",
       url = "http://cern.ch/seal",
       package_dir = {'pyreflex':'python/pyreflex'},
       py_modules = ['python.pyreflex.__init__',
                     'python.pyreflex.adapt_reflex',
                     'python.pyreflex.Paux',
                     'python.pyreflex.pyreflex',],
       ext_modules = [reflex,
                      ],
       #data_files = []
       )


