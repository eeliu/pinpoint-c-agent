#!/usr/bin/env python
# -*- coding: UTF-8 -*-

from plugins.BaseFlaskPlugins import BaseFlaskPlugins

class MyMidWare():

    def __init__(self, app):
        self.app = app

    @BaseFlaskPlugins("Flask Web App", __name__)
    def __call__(self, environ, start_response):
        return self.app(environ, start_response)
