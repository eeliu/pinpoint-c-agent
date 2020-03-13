from plugins.PinpointCommon import *
import pinpoint
import threading

class PinpointCommonPlugin(Candy):

    def onBefore(self,*args, **kwargs):
        super().onBefore(*args, **kwargs)
        ###############################################################
        pinpoint.add_clue(FuncName,self.getFuncUniqueName())
        pinpoint.add_clue(ServerType,PYTHON_METHOD_CALL)
        arg = self.get_arg(*args, **kwargs)
        pinpoint.add_clues(PY_ARGS, arg)
        ###############################################################
        # print( threading.currentThread().ident)

    def onEnd(self,ret):
        ###############################################################
        pinpoint.add_clues(PY_RETURN,str(ret))
        ###############################################################
        super().onEnd(ret)
        return ret

    def onException(self, e):
        pinpoint.add_clue('EXP',str(e))

    def get_arg(self, *args, **kwargs):
        args_tmp = {}
        j = 0

        for i in args:
            args_tmp["arg["+str(j)+"]"] = (str(i))
            j = j + 1
            print(j)

        for k in kwargs:
            args_tmp[k] = kwargs[k]

        return str(args_tmp)
