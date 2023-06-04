import os
import sys
import subprocess
from wsgiref.util import setup_testing_defaults
from wsgiref.simple_server import make_server
sys.path.append(os.path.join(os.path.dirname(__file__), 'bpod'))
from application import application

def simple_app(environ, start_response):
    if environ['PATH_INFO'].startswith('/static/'):
        relpath = environ['PATH_INFO'][len('/static/'):].replace('/', '').replace('\\', '')
        text = ''
        path = os.path.join(os.path.dirname(__file__), 'bpod', 'static', relpath)
        try:
            with open(path, 'rt') as handle:
                text = handle.read()
        except:
            pass
        data = text.encode('ascii')
        status = '200 OK'
        headers = [('Content-type', 'text/plain; charset=utf-8'), ('Content-Length',str(len(data)))]
        start_response(status, headers)
        return [data]
    return application(environ, start_response)


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
