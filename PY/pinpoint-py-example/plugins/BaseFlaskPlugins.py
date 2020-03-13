#!/usr/bin/env python
# -*- coding: UTF-8 -*-
# Created by eeliu at 3/5/20

# ------------------------------------------------------------------------------
#  Copyright  2020. NAVER Corp.
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
# ------------------------------------------------------------------------------


import pinpoint
from plugins.PinpointCommon import *
from flask import Flask,Request
import threading



class BaseFlaskPlugins(Candy):
    def __init__(self,class_name,module_name):
        super().__init__(class_name,module_name)
        self.isLimit = False

    def onBefore(self,*args, **kwargs):
        super().onBefore(*args, **kwargs)
        ###############################################################
        print(threading.currentThread().ident)
        print("------------------- call before -----------------------")
        # insFlask = args[0]
        # assert isinstance(insFlask, Flask)
        request = Request(args[1])
        pinpoint.add_clue('name', 'BaseFlaskrequest')
        pinpoint.add_clue('uri', request.path)
        pinpoint.add_clue('client',request.remote_addr)
        pinpoint.add_clue('server', request.host)
        pinpoint.add_clue('stp', PYTHON)

        if PINPOINT_PSPANID in request.headers:
            pinpoint.add_clue('psid', request.headers[PINPOINT_PSPANID])
            print("PINPOINT_PSPANID:", request.headers[PINPOINT_PSPANID])
        
        if PINPOINT_SPANID in request.headers:
            self.sid = request.headers[PINPOINT_SPANID]
        else:
            self.sid = self.generateSid()
        pinpoint.set_special_key('sid', self.sid)

        if PINPOINT_TRACEID in request.headers:
            self.tid = request.headers[PINPOINT_TRACEID]
        else:
            self.tid = self.generateTid()
        pinpoint.set_special_key('tid',self.tid)

        if PINPOINT_PAPPNAME in request.headers:
            self.pname = request.headers[PINPOINT_PAPPNAME]
            pinpoint.set_special_key('pname',self.pname)
            pinpoint.add_clue('pname',self.pname)

        if PINPOINT_PAPPTYPE in request.headers:
            self.ptype = request.headers[PINPOINT_PAPPTYPE]
            pinpoint.set_special_key('ptype',self.ptype)
            pinpoint.add_clue('ptype',self.ptype)

        if PINPOINT_HOST in request.headers:
            self.Ah = request.headers[PINPOINT_PAPPTYPE]
            pinpoint.set_special_key('Ah',self.Ah)
            pinpoint.add_clue('Ah',self.Ah)
        
        if NGINX_PROXY in request.headers:
            pinpoint.add_clue('NP',request.headers[NGINX_PROXY])
        
        if APACHE_PROXY in request.headers:
            pinpoint.add_clue('AP',request.headers[APACHE_PROXY])

        if SAMPLED in request.headers:
            if request.headers[SAMPLED] == 's0':
                self.isLimit = True
                pinpoint.drop_trace()
        else:
            # self.isLimit = pinpoint.check_tracelimit()
            print(self.isLimit)
        pinpoint.add_clue('tid',self.tid)
        pinpoint.add_clue('sid',self.sid)
        ###############################################################

    def onEnd(self,ret):
        ###############################################################
        if self.isLimit:
            pinpoint.drop_trace()
        print("------------------- call end -----------------------")
        ###############################################################
        super().onEnd(ret)
        self.isLimit = False
        return ret

    def onException(self, e):
        pinpoint.add_clue('EXP',e)
        raise e
        # do something