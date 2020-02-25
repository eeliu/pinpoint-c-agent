from distutils.core import setup, Extension
import os

c_path = os.path.split(os.path.realpath(__file__))[0]

_include_dirs = c_path+'/depends/include'
_library_dirs = c_path+'/depends/lib/'
_static_libraries  = ['rt', 'stdc++']
for lib in ['libpinpoint_common.a','libjsoncpp.a']:
    _static_libraries.append(_library_dirs+lib)

setup(name='pinpoint',
      ext_modules=[
        Extension('pinpoint',
          ['pinpoint_py.c'],
          include_dirs = [_include_dirs],
          # define_macros = [('FOO','1')],
          # undef_macros = ['BAR'],
          library_dirs = [_library_dirs],
          # libraries =_static_libraries
          libraries = ['pinpoint_common','jsoncpp', 'rt', 'stdc++']

                  #   libraries = ['pinpoint_common.a libjsoncpp.a -lrt -lstdc++']
                  )
        ]
)