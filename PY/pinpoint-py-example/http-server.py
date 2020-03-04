import random
from http.server import BaseHTTPRequestHandler, HTTPServer
import socketserver

from PinpointCommon import *
class ExampleServer(BaseHTTPRequestHandler):

    def do_GET(self):
        pinpoint.start_trace()
        pinpoint.add_clue("key","value3")
        pinpoint.add_clue('uri','')
        pinpoint.add_clue('client',)
        pinpoint.add_clue('server', )
        pinpoint.add_clue('stp',PYTHON)
        pinpoint.add_clue('name','python request')
        pinpoint.add_clue('appname','test-py')
        pinpoint.add_clue('appid', 'test-py-id')
        tid = ('test-py-id^%s^%s')%(str(pinpoint.start_time()),str(pinpoint.unique_id()))
        pinpoint.add_clue('tid',tid)
        sid =  random.randint(0,99999999999)
        pinpoint.add_clue('sid',sid)

        """Serve a GET request."""
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()
        self.wfile.write(bytes('true',encoding='utf-8'))

        pinpoint.end_trace()

    def do_POST(self):
        content_length = int(self.headers['Content-Length'])  # <--- Gets the size of data
        post_data = self.rfile.read(content_length)  # <--- Gets the data itself
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()
        print(self.headers)
        print(post_data)
        self.wfile.write(bytes('false',encoding='utf-8'))

PORT = 9000

pinpoint.set_collector_host('unix:/tmp/collector-agent.sock')

with socketserver.TCPServer(("", PORT), ExampleServer) as httpd:
    print("serving at port", PORT)
    httpd.serve_forever()