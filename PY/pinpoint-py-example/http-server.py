import random
from http.server import BaseHTTPRequestHandler, HTTPServer
import socketserver
import time

from plugins.BaseHTTPRequestPlugins import BaseHTTPRequestPlugins
from plugins.PinpointCommonPlugin import PinpointCommonPlugin
class SimpleWebServer(BaseHTTPRequestHandler):
    @PinpointCommonPlugin('SimpleWebServer',__name__)
    def getAgent(self,headers):
        time.sleep(1)
        if 'User-Agent' in headers:
            print(headers['User-Agent'])
            return headers['User-Agent']
        else:
            return 'null'

    @BaseHTTPRequestPlugins('SimpleWebServer',__name__)
    def do_GET(self):
        # print(self.headers)
        self.getAgent(self.headers)
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()
        self.wfile.write(bytes('true',encoding='utf-8'))

    @BaseHTTPRequestPlugins('SimpleWebServer',__name__)
    def do_POST(self):
        content_length = int(self.headers['Content-Length'])  
        post_data = self.rfile.read(content_length)
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()
        print(self.headers)
        print(post_data)
        self.wfile.write(bytes('false',encoding='utf-8'))

PORT = 9000

with socketserver.TCPServer(("", PORT), SimpleWebServer) as httpd:
    print("start at:", PORT)
    httpd.serve_forever()