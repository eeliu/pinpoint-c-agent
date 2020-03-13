#!/usr/bin/env python
# -*- coding: UTF-8 -*-


class Private(object):

    def __private_func(self, arg):
        return "Private called by " + arg

    def common_func(self, name):
        r = self.__private_func(name)
        return r
