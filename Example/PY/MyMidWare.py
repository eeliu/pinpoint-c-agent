#!/usr/bin/env python
# -*- coding: UTF-8 -*-

from plugins.BaseFlaskPlugins import BaseFlaskPlugins
import  pinpointPy
class MyMidWare():

    def __init__(self, app,wsgi_app):
        self.app = app
        self.wsgi_app = wsgi_app
        # todo if in process mode , try to force flush span with timeout

        @app.teardown_request
        def force_flush_span(exc):
            pinpointPy.force_flush_trace(5)
            print("-----------------------------------------------------")

    @BaseFlaskPlugins("Flask Web App", __name__)
    def __call__(self, environ, start_response):
        return self.wsgi_app(environ, start_response)

