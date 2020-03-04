from unittest import TestCase
import unittest

# class CommonMethodPlugins(object):
#     def onBefore(self):
#         pass

#     def onEnd(self):
#         pass

#     def onException(self):
#         pass
#     def __call__(self, *args, **kwargs):
        

# class BaseHTTPRequestPlugins(object):

#     def onBefore(self):
#         pass

#     def onEnd(self):
#         pass


# class TestAgent(TestCase):
#     pass



# if __name__ == '__main__':
#     unittest.main()

def wapper(fun):
    def wappered(*args, **kwargs):
        print(args)
        ret =  fun(*args, **kwargs)
        print("wapper end")
        return ret
    return wappered



class Foo(object):
    @wapper
    def output(self,a):
        print("method output")
@wapper
def output(a):
    print("func output")

output("a")
f = Foo()
f.output(334)