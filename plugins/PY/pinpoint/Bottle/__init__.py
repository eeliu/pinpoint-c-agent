#!/usr/bin/env python
# -*- coding: UTF-8 -*-
# Created by eeliu at 11/9/20

from pinpoint.common import *
from .RequestPlugin import RequestPlugin

def PinPointMiddleWare(application):
    def handler(environ,start_response):
        plugin = RequestPlugin("pyweb-request")
        plugin.onBefore(environ,start_response)
        response = application(environ,start_response)
        plugin.onEnd(response)
        return response
    return handler