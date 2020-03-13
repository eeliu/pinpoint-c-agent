#!/usr/bin/env python
# -*- coding: UTF-8 -*-

class Special(object):

    def __init__(self, name, age):
        self.name = name
        self.age = age

    def common_func(self):
        return "%s is %d years old." % (self.name, self.age)
