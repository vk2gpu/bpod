import os
import sys
import time
import json
import zlib
import base64
import shutil
import hashlib
import argparse
import subprocess

ROOT = os.path.realpath(os.path.dirname(__file__))

# check for python virtual environment (required by esp-idf)
try:
    import venv
except:
    raise Exception("Please install python virtual environment e.g. 'sudo apt install python3-venv'")

# check for python serial
try:
    import serial
except:
    raise Exception("Please install python serial e.g. 'sudo apt install python3-serial'")

# check for cmake
try:
    p = subprocess.Popen(['cmake', '--version'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    p.communicate()
    if 0 != p.returncode:
        raise FileNotFoundError
except FileNotFoundError:
    raise Exception("Please install cmake e.g. 'sudo apt install cmake'")


def git_clone(url, branch, out):
    out = os.path.realpath(out)
    if not os.path.exists(out):
        try:
            if not os.path.exists(os.path.dirname(out)):
                os.makedirs(os.path.dirname(out))
            p = subprocess.Popen(['git', 'clone', '-b', branch, url, out])
            p.communicate()
            assert 0 == p.returncode
            p = subprocess.Popen(['git', 'submodule', 'update', '--init', '--recursive'], cwd=out)
            p.communicate()
            assert 0 == p.returncode
        except:
            if os.path.exists(out):
                shutil.rmtree(out)
    assert os.path.exists(out)


def esp_idf_download_and_install(args):
    '''
    git clone -b v4.4.3 https://github.com/espressif/esp-idf.git
    git submodule update --init --recursive
    cd esp-idf
    ./install.sh
    '''
    if not os.path.exists(args.esp_idf):
        try:
            git_clone('https://github.com/espressif/esp-idf.git', 'v4.4.3', args.esp_idf)
            p = subprocess.Popen([os.path.join(args.esp_idf, 'install.sh')], cwd=args.esp_idf)
            p.communicate()
            assert 0 == p.returncode
            p = subprocess.Popen(['python3', '-m', 'pip', 'install', 'pillow'],
                cwd=args.firmware,
                env=esp_idf_environ(args))
            stdout, stderr = p.communicate()
            assert 0 == p.returncode
        except:
            if os.path.exists(args.esp_idf):
                shutil.rmtree(args.esp_idf)
    assert os.path.exists(args.esp_idf)


def esp_idf_environ(args):
    '''
    source ~/esp-idf/export.sh
    '''
    environ = dict()
    environ.update(os.environ)
    environ['IDF_PATH'] = args.esp_idf
    environ['IDF_TARGET'] = args.target
    p = subprocess.Popen(['. {}'.format(os.path.join(args.esp_idf, 'export.sh'))],
        shell=True, cwd=args.esp_idf,
        stdout=subprocess.PIPE, stderr=subprocess.PIPE,
        env=environ)
    stdout, stderr = p.communicate()
    if 0 != p.returncode:
        sys.stdout.write(stdout.decode())
        sys.stderr.write(stderr.decode())
        assert 0 == p.returncode
    path_variables = list()
    for line in stdout.decode().split('\n'):
        line = line.strip()
        if not line.startswith('/'):
            continue
        if line.find('esptool') == -1:
            continue
        if not line.endswith(environ['PATH']):
            continue
        path_variables.append(line)
    if len(path_variables) != 1:
        sys.stdout.write(stdout.decode())
        sys.stderr.write(stderr.decode())
        raise Exception("Developer error: my trick for finding PATH variable in stdout broke")
    environ['PATH'] = path_variables[0]
    return environ


def esp_idf_cmake_generate(args):
    '''
    idf.py --build-dir ../out set-target esp32
    '''
    p = subprocess.Popen(['idf.py', '--build-dir', args.out, 'set-target', args.target],
        cwd=args.firmware,
        env=args.esp_idf_environ)
    stdout, stderr = p.communicate()
    assert 0 == p.returncode


def esp_idf_build(args):
    '''
    idf.py build
    '''
    p = subprocess.Popen(['idf.py', '--build-dir', args.out, 'build'],
        cwd=args.firmware,
        env=args.esp_idf_environ)
    stdout, stderr = p.communicate()
    assert 0 == p.returncode


def esp_idf_flash(args):
    '''
    idf.py -p /dev/ttyUSB0 flash
    '''
    p = subprocess.Popen(['idf.py', '--build-dir', args.out, '-p', args.device, 'flash'],
        cwd=args.firmware,
        env=args.esp_idf_environ)
    stdout, stderr = p.communicate()
    assert 0 == p.returncode


def esp_idf_menuconfig(args):
    '''
    idf.py menuconfig
    '''
    p = subprocess.Popen(['idf.py', '--build-dir', args.out, 'menuconfig'],
        cwd=args.firmware,
        env=args.esp_idf_environ)
    stdout, stderr = p.communicate()
    assert 0 == p.returncode


def download(args):
    esp_idf_download_and_install(args)
    git_clone('https://github.com/espressif/arduino-esp32.git', '2.0.6', os.path.join(args.firmware, 'components', 'arduino-esp32'))
    git_clone('https://github.com/adafruit/Adafruit_BusIO.git', '1.14.1', os.path.join(args.adafruit, 'Adafruit_BusIO'))
    git_clone('https://github.com/adafruit/Adafruit-GFX-Library.git', '1.11.3', os.path.join(args.adafruit, 'Adafruit-GFX-Library'))
    git_clone('https://github.com/adafruit/Adafruit-ST7735-Library.git', '1.9.3', os.path.join(args.adafruit, 'Adafruit-ST7735-Library'))


def generate(args):
    if not args.esp_idf_environ:
        args.esp_idf_environ = esp_idf_environ(args)
    esp_idf_cmake_generate(args)


def menuconfig(args):
    if not args.esp_idf_environ:
        args.esp_idf_environ = esp_idf_environ(args)
    esp_idf_menuconfig(args)


def package(args):
    script_version_string = 'v1'
    out = args.out + '_bpod_flash_{}.py'.format(script_version_string)

    # read esptool.py script text
    esptool_path = os.path.join(args.esp_idf, 'components', 'esptool_py', 'esptool', 'esptool.py')
    assert os.path.exists(esptool_path)
    with open(esptool_path, 'rt') as handle:
        text = handle.read()

    # read flasher args json file
    flasher_args_path = os.path.join(args.out, 'flasher_args.json')
    with open(flasher_args_path, 'rt') as handle:
        flasher_args = json.loads(handle.read())

    # replace lines
    serial_help_line_found = False
    main_call_found = False
    lines = list()
    for line in text.split('\n'):
        if line.startswith('    print("Pyserial is not installed'):
            serial_help_line_found = True
            line = "    print('{}'.format(sys.executable))".format('REQUIRED: please install pyserial using line "{} -m pip install pyserial"')
        if line.startswith("if __name__ == '__main__':"):
            main_call_found = True
            break
        lines.append(line)
    assert serial_help_line_found
    assert main_call_found
    text = '\n'.join(lines)
    assert text.find('_main()') != -1

    # embed flashing files in script
    file_list = list()
    for offset_string, relpath_bin in flasher_args['flash_files'].items():
        path_bin = os.path.join(args.out, relpath_bin)
        name = os.path.splitext(os.path.basename(path_bin))[0].replace('-', '_')
        with open(path_bin, 'rb') as handle:
            data = handle.read()
        assert len(data) > 0
        md5 = hashlib.md5(data).hexdigest().upper()
        data = zlib.compress(data)
        data = base64.b64encode(data).decode('ascii')
        data_split = []
        count = 0
        chars_per_line = 160
        for i in range(0, len(data), chars_per_line):
            data_split.append(data[i: i + chars_per_line])
            count += chars_per_line
        if count < len(data):
            data_split.append(data[count: ])
        text += '{}_DATA = zlib.decompress(base64.b64decode(b"""\\\n'.format(name.upper())
        text += '\\\n'.join(data_split) + '\\\n'
        text += '"""))\n'
        text += "{}_MD5 = '{}'\n".format(name.upper(), md5)
        text += 'assert hashlib.md5({}_DATA).hexdigest().upper() == {}_MD5\n'.format(name.upper(), name.upper())
        file_list.append((os.path.basename(path_bin), offset_string, '{}_DATA'.format(name.upper()), '{}_MD5'.format(name.upper())))
    text += 'FILE_LIST = [\n'
    for items in file_list:
        text += "    ('{}', '{}', {}, {}),\n".format(items[0], items[1], items[2], items[3])
    text += ']\n'
    text += 'assert len(FILE_LIST) == {}\n'.format(len(file_list))

    # arguments for esptool.py
    FLASHER_ARGS = []
    for name, value in flasher_args['extra_esptool_args'].items():
        if not isinstance(value, str):
            continue
        assert isinstance(name, str)
        FLASHER_ARGS.extend(['--{}'.format(name), value])
    FLASHER_ARGS.append('write_flash')
    FLASHER_ARGS.extend(flasher_args['write_flash_args'])
    for offset_string, path in flasher_args['flash_files'].items():
        assert isinstance(offset_string, str)
        assert isinstance(path, str)
        path = os.path.basename(path)
        FLASHER_ARGS.extend([offset_string, path])
    text += 'FLASHER_ARGS = {}\n'.format(FLASHER_ARGS)

    # function to extract file
    text += """
def extract_files():
    for path, _, data, md5 in FILE_LIST:
        write_file = True
        if os.path.exists(path):
            with open(path, 'rb') as handle:
                file_data = handle.read()
            if hashlib.md5(file_data).hexdigest().upper() == md5:
                write_file = False
        if write_file:
            print('Extracting {}'.format(path))
            with open(path, 'wb') as handle:
                handle.write(data)
    for path, _, _, md5 in FILE_LIST:
        print('Checking {} md5 equals {}'.format(path, md5))
        with open(path, 'rb') as handle:
            check_data = handle.read()
        assert hashlib.md5(check_data).hexdigest().upper() == md5
"""

    # redirect esptool.py to do my custom command
    text += """

def flash_main():
    if len(sys.argv) <= 1:
        print('<FILE_NAME_REPLACE> <device>  # e.g. /dev/ttyACM0 e.g. COM11')
        if len(list_ports.comports()) == 0:
            print('(no serial ports found)')
        for p in list_ports.comports():
            print(p)
        return
    device = sys.argv[1]
    print('<FILE_NAME_REPLACE> {}'.format(device))
    extract_files()
    args = sys.argv[:1]
    args.extend(['-p', device])
    args.extend(['-b', '460800'])
    args.extend(FLASHER_ARGS)
    sys.argv = args
    print(' '.join(sys.argv))
    _main()

if __name__ == '__main__':
    flash_main()
""".replace('<FILE_NAME_REPLACE>', os.path.basename(out))

    with open(out, 'wt') as handle:
        handle.write(text)


def build(args):
    if not args.esp_idf_environ:
        args.esp_idf_environ = esp_idf_environ(args)
    esp_idf_build(args)
    package(args)


def prep_serial_device(args):
    if not os.path.exists(args.device) and os.path.exists('/mnt/c/Windows/system32'):
        p = subprocess.Popen(['cmd.exe', '/c', 'usbipd', 'wsl', 'list'],
            stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = p.communicate()
        busid = None
        for line in stdout.decode().split('\n'):
            if line.count('-') < 1 or line.count(':') < 1:
                continue
            if line.upper().find('COM') == -1:
                continue
            busid = line.strip().split(' ')[0]
            break
        if busid:
            p = subprocess.Popen(['cmd.exe', '/c', 'usbipd', 'wsl', 'attach', '--busid', busid],
                stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            stdout, stderr = p.communicate()
            if p.returncode != 0:
                print(' '.join(['usbipd', 'wsl', 'attach', '--busid', busid]))
                sys.stdout.write(stdout.decode())
                sys.stderr.write(stderr.decode())
            assert p.returncode == 0
            time.sleep(3)
    if (os.lstat(args.device).st_mode & 0o666) != 0o666:
        p = subprocess.Popen(['sudo', 'chmod', '666', args.device])
        stdout, stderr = p.communicate()
        assert 0 == p.returncode

def flash(args):
    prep_serial_device(args)
    if not args.esp_idf_environ:
        args.esp_idf_environ = esp_idf_environ(args)
    esp_idf_flash(args)


def monitor(args):
    prep_serial_device(args)
    s = serial.Serial(port=args.device, baudrate=115600)
    try:
        while True:
            line = s.readline()
            line = line.decode().rstrip('\r\n')
            print(line)
    finally:
        s.close()


def minicom(args):
    prep_serial_device(args)
    s = serial.Serial(port=args.device, baudrate=115600)
    os.system('minicom -D {}'.format(args.device))


def main():
    parser = argparse.ArgumentParser(prog='bPod', description = 'Build, run, debug bPod firmware')
    parser.add_argument('--download', default=False, action='store_true', help='Download required files')
    parser.add_argument('--menuconfig', default=False, action='store_true', help='ESP-IDF menuconfig')
    parser.add_argument('--build', default=False, action='store_true', help='Build firmware')
    parser.add_argument('--flash', default=False, action='store_true', help='Flash firmware')
    parser.add_argument('--monitor', default=False, action='store_true', help='Monitor serial output of device')
    parser.add_argument('--minicom', default=False, action='store_true', help='Connect minicom to device')
    parser.add_argument('--target', default='esp32s2', choices=['esp32', 'esp32s2'], help='Target CPU')
    parser.add_argument('--device', default='/dev/ttyACM0', type=str, help='Path to USB serial device for flashing e.g. /dev/ttyUSB0')
    args = parser.parse_args()
    if not args.device:
        if args.device == 'esp32s2':
            args.device = '/dev/ttyACM0'
        else:
            args.device = '/dev/ttyUSB0'
    args.out = os.path.join(ROOT, 'out', args.target)
    args.firmware = os.path.join(ROOT, 'firmware', args.target)
    args.esp_idf = os.path.join(ROOT, 'firmware', 'esp-idf')
    args.adafruit = os.path.join(ROOT, 'firmware', 'adafruit')
    args.esp_idf_environ = None  # lazy init when needed
    if args.build:
        args.download = True
    if args.download:
        download(args)
    if args.menuconfig:
        menuconfig(args)
    if args.build:
        build(args)
    if args.flash:
        flash(args)
    if args.monitor:
        monitor(args)
    if args.minicom:
        minicom(args)


if __name__ == '__main__':
    main()
