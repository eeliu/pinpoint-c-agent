import requests
from plugins.NextSpanPlugin import NextSpanPlugin


new_next = NextSpanPlugin('requests','')
setattr(requests, "get", new_next(requests.get))
