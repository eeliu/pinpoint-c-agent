from distutils.core import setup, Extension
from distutils.command.install import install
import os,subprocess

class CommonInstall(install):
      
    def build_common(self):
          
        if not os.path.exists('build'):
            os.makedirs('build')

        comm_path = os.path.abspath('common')
        subprocess.check_call(['cmake',comm_path],cwd='build')
        subprocess.check_call(['make'],cwd='build')

    def run(self):
        self.build_common()
        install.run(self)

setup(name='pinpoint',
      ext_modules=[
        Extension('pinpointPy',
          ['src/PY/pinpoint_py.c'],
          include_dirs = ['common/include'],
          # define_macros = [('FOO','1')],
          # undef_macros = ['BAR'],
          library_dirs = ['common/lib'],
          libraries = ['pinpoint_common','jsoncpp', 'rt', 'stdc++']
          )
        ],
        cmdclass={'install': CommonInstall}
)