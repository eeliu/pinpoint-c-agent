from plugins.PinpointCommon import *
import pinpoint

class PinpointCommonPlugin(Candy):

    def onBefore(self,*args, **kwargs):
        super().onBefore(*args, **kwargs)
        ###############################################################
        pinpoint.add_clue(FuncName,self.getFuncUniqueName())
        pinpoint.add_clue(ServerType,PYTHON_METHOD_CALL);
        pinpoint.add_clues(PY_ARGS,'special your input values')
        ###############################################################

    def onEnd(self,ret):
        ###############################################################
        pinpoint.add_clues(PY_RETURN,str(ret))
        ###############################################################
        super().onEnd(ret)
        return ret

    def onException(self, e):
        pinpoint.add_clue('EXP',e)
        raise e
