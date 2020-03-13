#!/usr/bin/env python
# -*- coding: UTF-8 -*-

class Animal(object):

    def animal_func(self):
        return " and I am an animal"


class ThinkMixIn(object):

    def think_func(self):
        return " and I can think"


class Human(Animal, ThinkMixIn):

    def human_func(self):
        return "I am a human"