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

# def wapper(fun):
#     def wappered(*args, **kwargs):
#         print(args)
#         ret =  fun(*args, **kwargs)
#         print("wapper end")
#         return ret
#     return wappered
#
#
#
# def testSelf(foo):
#     foo.new_a ='123'
#     foo.new_b = '12'
#
#
# class Foo(object):
#     @wapper
#     def output(self,a):
#         print("method output")
#         testSelf(self)
#         # return a
#
# def output(a):
#     print("func output")
#     # return a
#
# # print(output("a"))
# f = Foo()
# print(f.output(334))
#
# print(f.new_a,f.new_b)

def wappered(*args, **kwargs):
    print(args)
    print(kwargs)

wappered(a=10,b="asdfadf")