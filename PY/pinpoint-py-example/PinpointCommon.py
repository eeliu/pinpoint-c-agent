import pinpoint

PYTHON = 1700
PYTHON_METHOD_CALL=1701
PYTHON_REMOTE_METHOD = 9900




class Candy(object):
    def onBefore(self,*args, **kwargs):
        pinpoint.start_trace()

    def onEnd(self,ret):
        pinpoint.end_trace()
        return ret

    def onException(self,e):
        pass

    def __call__(self, func):
        def pinpointTrace(*args, **kwargs):
            ret = None
            try:
                self.onBefore(*args, **kwargs)
                ret = func(*args, **kwargs)
            except Exception as e:
                self.onException(e)
                ret = None
            finally:
                self.onEnd(ret)
        return pinpointTrace
        

class BaseHTTPRequestPlugins(object):

    def onBefore(self):
        pass

    def onEnd(self):
        pass
