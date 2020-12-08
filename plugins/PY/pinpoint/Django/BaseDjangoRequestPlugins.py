#!/usr/bin/env python
# -*- coding: UTF-8 -*-

# ------------------------------------------------------------------------------
#  Copyright  2020. NAVER Corp.                                                -
#                                                                              -
#  Licensed under the Apache License, Version 2.0 (the "License");             -
#  you may not use this file except in compliance with the License.            -
#  You may obtain a copy of the License at                                     -
#                                                                              -
#   http://www.apache.org/licenses/LICENSE-2.0                                 -
#                                                                              -
#  Unless required by applicable law or agreed to in writing, software         -
#  distributed under the License is distributed on an "AS IS" BASIS,           -
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.    -
#  See the License for the specific language governing permissions and         -
#  limitations under the License.                                              -
# ------------------------------------------------------------------------------

# Created by eeliu at 3/5/20

from pinpoint.libs import monkey_patch_for_pinpoint
monkey_patch_for_pinpoint()

from pinpoint.common import *
import pinpointPy

class BaseDjangoRequestPlugins(Candy):
    def __init__(self,name):
        super().__init__(name)

    def onBefore(self,*args, **kwargs):
        super().onBefore(*args, **kwargs)
        pinpointPy.add_clue(PP_APP_NAME,APP_NAME)
        pinpointPy.add_clue(PP_APP_ID, APP_ID)
        ###############################################################
        request = args[1]
        headers = request.META
        
        # assert isinstance(request,BaseHTTPRequestHandler)
        pinpointPy.add_clue(PP_INTERCEPTOR_NAME, 'BaseDjangoRequest request')
        pinpointPy.add_clue(PP_REQ_URI,request.path)
        # print(headers)
        pinpointPy.add_clue(PP_REQ_CLIENT,headers['REMOTE_ADDR'])
        pinpointPy.add_clue(PP_REQ_SERVER,request.get_host())
        pinpointPy.add_clue(PP_SERVER_TYPE,PYTHON)

        # nginx add http
        if PP_HTTP_PINPOINT_PSPANID in headers:
            pinpointPy.add_clue(PP_PARENT_SPAN_ID, headers[PP_HTTP_PINPOINT_PSPANID])

        if PP_HTTP_PINPOINT_SPANID in headers:
            self.sid = headers[PP_HTTP_PINPOINT_SPANID]
        elif PP_HEADER_PINPOINT_SPANID in headers:
            self.sid = headers[PP_HEADER_PINPOINT_SPANID]
        else:
            self.sid = generateSid()
        pinpointPy.set_context_key(PP_SPAN_ID,self.sid)


        if PP_HTTP_PINPOINT_TRACEID in headers:
            self.tid = headers[PP_HTTP_PINPOINT_TRACEID]
        elif PP_HEADER_PINPOINT_TRACEID in headers:
            self.tid = headers[PP_HEADER_PINPOINT_TRACEID]
        else:
            self.tid = generateTid()
        pinpointPy.set_context_key(PP_TRANSCATION_ID,self.tid)

        if PP_HTTP_PINPOINT_PAPPNAME in headers:
            self.pname = headers[PP_HTTP_PINPOINT_PAPPNAME]
            pinpointPy.set_context_key(PP_PARENT_NAME,self.pname)
            pinpointPy.add_clue(PP_PARENT_NAME,self.pname)

        if PP_HTTP_PINPOINT_PAPPTYPE in headers:
            self.ptype = headers[PP_HTTP_PINPOINT_PAPPTYPE]
            pinpointPy.set_context_key(PP_PARENT_TYPE,self.ptype)
            pinpointPy.add_clue(PP_PARENT_TYPE,self.ptype)

        if PP_HTTP_PINPOINT_HOST in request.headers:
            self.Ah = headers[PP_HTTP_PINPOINT_HOST]
            pinpointPy.set_context_key(PP_PARENT_HOST, self.Ah)
            pinpointPy.add_clue(PP_PARENT_HOST, self.Ah)

        if PP_HTTP_PINPOINT_HOST in headers:
            pinpointPy.add_clue(PP_PARENT_SPAN_ID, headers[PP_HEADER_PINPOINT_PSPANID])

        if PP_HEADER_PINPOINT_PAPPNAME in headers:
            self.pname = headers[PP_HEADER_PINPOINT_PAPPNAME]
            pinpointPy.set_context_key(PP_PARENT_NAME, self.pname)
            pinpointPy.add_clue(PP_PARENT_NAME, self.pname)

        if PP_HEADER_PINPOINT_PAPPTYPE in headers:
            self.ptype = headers[PP_HEADER_PINPOINT_PAPPTYPE]
            pinpointPy.set_context_key(PP_PARENT_TYPE, self.ptype)
            pinpointPy.add_clue(PP_PARENT_TYPE, self.ptype)

        if PP_HEADER_PINPOINT_HOST in headers:
            self.Ah = headers[PP_HEADER_PINPOINT_HOST]
            pinpointPy.set_context_key(PP_PARENT_HOST, self.Ah)
            pinpointPy.add_clue(PP_PARENT_HOST, self.Ah)

        
        if PP_NGINX_PROXY in headers:
            pinpointPy.add_clue(PP_APACHE_PROXY,headers[PP_NGINX_PROXY])

        if PP_APACHE_PROXY in headers:
            pinpointPy.add_clue(PP_APACHE_PROXY,headers[PP_APACHE_PROXY])

        pinpointPy.set_context_key("Pinpoint-Sampled", "s1")
        if (PP_HTTP_SAMPLED in headers and headers[PP_HTTP_SAMPLED] == PP_NOT_SAMPLED) or pinpointPy.check_tracelimit():
            if headers[PP_HTTP_SAMPLED] == PP_NOT_SAMPLED:
                pinpointPy.drop_trace()
                pinpointPy.set_context_key("Pinpoint-Sampled", "s0")



        pinpointPy.add_clue(PP_TRANSCATION_ID,self.tid)
        pinpointPy.add_clue(PP_SPAN_ID,self.sid)
        pinpointPy.set_context_key(PP_TRANSCATION_ID, self.tid)
        pinpointPy.set_context_key(PP_SPAN_ID, self.sid)

        ###############################################################
        return args, kwargs

    def onEnd(self,ret):
        ###############################################################
        
        ###############################################################
        super().onEnd(ret)
        return ret

    def onException(self, e):
        pinpointPy.mark_as_error(e,"",0)
        raise e



