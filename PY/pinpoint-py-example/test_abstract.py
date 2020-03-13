#!/usr/bin/env python
# -*- coding: UTF-8 -*-


import abc


class AllFle(metaclass=abc.ABCMeta):
    all_type='file'

    @abc.abstractmethod
    def read(self):
        pass


class Txt(AllFle):
    def read(self):
        return "Reading txt!"
