from distutils.core import setup, Extension
import os

c_path = os.path.split(os.path.realpath(__file__))[0]

_include_dirs = c_path+'/depends/include'
_library_dirs = c_path+'/depends/lib/'


setup(name='pinpoint',
      ext_modules=[
        Extension('pinpoint',
          ['pinpoint_py.c'],
          include_dirs = [_include_dirs],
          # define_macros = [('FOO','1')],
          # undef_macros = ['BAR'],
          library_dirs = [_library_dirs],
          libraries = ['pinpoint_common','jsoncpp', 'rt', 'stdc++']
          )
        ]
)