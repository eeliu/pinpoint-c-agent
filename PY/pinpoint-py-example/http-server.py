import random
from http.server import BaseHTTPRequestHandler, HTTPServer
import socketserver

from plugins.BaseHTTPRequestPlugins import BaseHTTPRequestPlugins

class SimpleWebServer(BaseHTTPRequestHandler):

    def do_GET(self):
        print(self.headers)
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()
        self.wfile.write(bytes('true',encoding='utf-8'))

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