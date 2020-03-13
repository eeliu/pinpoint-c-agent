#!/usr/bin/env python
# -*- coding: UTF-8 -*-


class Method(object):

    @staticmethod
    def static_method(a, b):
        return a + b

    @classmethod
    def class_method(cls, a, b):
        return cls.__name__ + a + b
