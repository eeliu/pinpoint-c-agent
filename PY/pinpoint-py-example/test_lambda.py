#!/usr/bin/env python
# -*- coding: UTF-8 -*-


def in_lambda(b):
    return b * b

def return_lambda(a):
    return lambda: in_lambda(a)

