#!/usr/bin/env python
# -*- coding: UTF-8 -*-


def fact(n):
    if n==1:
        return 1
    return n * fact(n - 1)