import os
import sys
import subprocess
from wsgiref.util import setup_testing_defaults
from wsgiref.simple_server import make_server
sys.path.append(os.path.join(os.path.dirname(__file__), 'bpod'))
from application import application

def simple_app(environ, start_response):
    if environ['PATH_INFO'] in ['', '/'] or environ['PATH_INFO'].endswith('.html'):
        return application(environ, start_response)
    relpath = environ['PATH_INFO'][len('/static/'):].replace('/', '').replace('\\', '')
    while relpath.startswith('/'):
        relpath = relpath[1:]
    data = b''
    path = os.path.realpath(os.path.join(os.path.dirname(__file__), 'bpod', 'static', relpath))
    assert path.startswith(os.path.dirname(__file__))
    try:
        with open(path, 'rb') as handle:
            data = handle.read()
    except:
        pass
    if path.endswith('.png'):
        content_type = 'image/png'
    elif path.endswith('.svg'):
        content_type = 'image/svg+xml'
    elif path.endswith('.css'):
        content_type = 'text/css'
    else:
        content_type = 'text/plain; charset=utf-8'
    status = '200 OK'
    headers = [('Content-type', content_type), ('Content-Length',str(len(data)))]
    start_response(status, headers)
    return [data]


def ip_addr():
    p = subprocess.Popen(['ip', 'addr'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = p.communicate()
    assert 0 == p.returncode
    stdout = stdout.decode()
    for line in stdout.split('\n'):
        line = line.strip()
        if not line.startswith('inet '):
            continue
        if line.find('127.') != -1:
            continue
        line = line[len('inet '):].split('/')[0].strip()
        assert line.count('.') == 3
        return line
    raise Exception("shitty ip finder failed")


def main():
    from bsides2023.token import Token
    os.environ['BPOD_DEBUG'] = 'true'
    os.environ['BPOD_DB_VERSION'] = '1'
    os.environ['BPOD_SCORE_ROWS'] = '20'
    with open(os.path.join(os.path.dirname(__file__), '..', 'keys', 'master.hex'), 'rt') as handle:
        os.environ['BPOD_KEY'] = handle.read().strip()
    with open(os.path.join(os.path.dirname(__file__), '..', 'keys', 'master2.hex'), 'rt') as handle:
        os.environ['BPOD_KEY_2'] = handle.read().strip()
    os.environ['RDS_HOSTNAME'] = '127.0.0.1'
    os.environ['RDS_PORT'] = '3306'
    os.environ['RDS_USERNAME'] = 'root'
    os.environ['RDS_PASSWORD'] = 'password'
    os.environ['RDS_DB_NAME'] = 'bpod'
    with make_server('', 8000, simple_app) as httpd:
        print("http://{}:{}".format(ip_addr(), 8000))
        httpd.serve_forever()

if __name__ == '__main__':
    main()
