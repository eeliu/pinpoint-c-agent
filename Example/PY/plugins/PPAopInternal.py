import requests
from plugins.NextSpanPlugin import NextSpanPlugin

## aop requests.get
new_request_get = NextSpanPlugin('requests','')
setattr(requests, "get", new_request_get(requests.get))

## aop requests.post
new_request_post = NextSpanPlugin('requests','')
setattr(requests, "post", new_request_post(requests.post))

