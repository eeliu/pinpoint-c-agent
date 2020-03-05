import pinpoint
import random

PYTHON = 1700
PYTHON_METHOD_CALL=1701
PYTHON_REMOTE_METHOD = 9900

###############################################################

# user should set below before use
APP_ID ='python-app-id' # application id
APP_NAME ='python-app-name' # application name 


###############################################################
PINPOINT_PSPANID = 'HTTP_PINPOINT_PSPANID'
PINPOINT_SPANID = 'HTTP_PINPOINT_SPANID'
PINPOINT_TRACEID = 'HTTP_PINPOINT_TRACEID'
PINPOINT_PAPPNAME ='HTTP_PINPOINT_PAPPNAME'
PINPOINT_PAPPTYPE ='HTTP_PINPOINT_PAPPTYPE'
PINPOINT_HOST = 'HTTP_PINPOINT_HOST'
NGINX_PROXY = 'Pinpoint-ProxyNginx'
APACHE_PROXY = 'HTTP_PINPOINT_PROXYAPACHE'
SAMPLED = 'Pinpoint-Sampled'
pinpoint.set_collector(collector_host='unix:/tmp/collector-agent.sock')


class Candy(object):

    def onBefore(self,*args, **kwargs):
        pinpoint.start_trace()
        pinpoint.add_clue('appname',APP_NAME)
        pinpoint.add_clue('appid', APP_ID)
    def onEnd(self,ret):
        pinpoint.end_trace()

    def onException(self,e):
        raise NotImplementedError()

    def __call__(self, func):
        def pinpointTrace(*args, **kwargs):
            ret = None
            try:
                self.onBefore(*args, **kwargs)
                ret = func(*args, **kwargs)
            except Exception as e:
                self.onException(e)
            finally:
                return self.onEnd(ret)
        return pinpointTrace

    def generateTid(self):
        return ('%s^%s^%s') % (APP_ID,str(pinpoint.start_time()), str(pinpoint.unique_id()))
    def generateSid(self):
        return random.randint(0,2147483647)


if __name__ == '__main__':

    @Candy()
    def run():
        print("run")

    run()

    run()