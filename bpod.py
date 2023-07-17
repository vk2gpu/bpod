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
            p = subprocess.Popen(['python3', '-m', 'pip', 'install', 'pycryptodome'],
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


def package_flash_image(args):
    out = os.path.join(os.path.dirname(args.out), 'pcbway_flash_bpod_2mb.bin')

    # read flasher args json file
    flasher_args_path = os.path.join(args.out, 'flasher_args.json')
    with open(flasher_args_path, 'rt') as handle:
        flasher_args = json.loads(handle.read())

    # create blank 2MB image
    image = b'\xff' * 2 * 1024 * 1024
    image_size = len(image)
    count = 0
    for offset_string, relpath_bin in flasher_args['flash_files'].items():
        count += 1
        path_bin = os.path.join(args.out, relpath_bin)
        name = os.path.splitext(os.path.basename(path_bin))[0].replace('-', '_')
        with open(path_bin, 'rb') as handle:
            data = handle.read()
        offset = int(offset_string, 0)
        size = len(data)
        assert size > 0
        assert (size & 0xf) == 0
        image = image[:offset] + data + image[offset + size:]
        assert len(image) == image_size
    assert count == 3
    with open(out, 'wb') as handle:
        handle.write(image)

    # verify
    image = None
    with open(out, 'rb') as handle:
        image = handle.read()
    assert len(image) == image_size
    assert len(image) == 2 * 1024 * 1024
    count = 0
    for offset_string, relpath_bin in flasher_args['flash_files'].items():
        count += 1
        path_bin = os.path.join(args.out, relpath_bin)
        name = os.path.splitext(os.path.basename(path_bin))[0].replace('-', '_')
        with open(path_bin, 'rb') as handle:
            data = handle.read()
        offset = int(offset_string, 0)
        assert image.find(data) == offset
    assert count == 3


def package_script(args):
    out = os.path.join(os.path.dirname(args.out), 'bPodUpdater.py')

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

    # embed stripped bpod.elf in updater (to help with CTF)
    remove_these = [
        '/firmware/',
        '/home',
        '.xt.prop',
        '.xt.lit',
    ]
    remove_these = [v.encode('ascii') for v in remove_these]
    bpod_elf_path = os.path.join(args.out, 'bpod.elf')
    assert os.path.exists(bpod_elf_path)
    bpod_stripped_elf_path = os.path.join(args.out, 'bpod-stripped.elf')
    if os.path.exists(bpod_stripped_elf_path):
        os.remove(bpod_stripped_elf_path)
    assert not os.path.exists(bpod_stripped_elf_path)
    with open(bpod_elf_path, 'rb') as handle:
        dont_use_data = handle.read()
    for remove_this in remove_these:
        assert dont_use_data.find(remove_this) != -1, remove_this    # found
    shutil.copyfile(bpod_elf_path, bpod_stripped_elf_path)
    p = subprocess.Popen(['xtensa-esp32s2-elf-strip', bpod_stripped_elf_path], 
        env=args.esp_idf_environ)
    p.communicate()
    assert p.returncode == 0
    p = subprocess.Popen(['xtensa-esp32s2-elf-strip', '--remove-section=*.xt*', bpod_stripped_elf_path],
        env=args.esp_idf_environ)
    p.communicate()
    assert p.returncode == 0
    assert os.path.exists(bpod_stripped_elf_path)
    with open(bpod_stripped_elf_path, 'rb') as handle:
        stripped_data = handle.read()
    for remove_this in remove_these:
        assert stripped_data.find(remove_this) == -1, remove_this    # not found
    assert stripped_data.count(b'bpod') == 1
    assert stripped_data.count(b'BPOD') == 0
    assert stripped_data.count(b'bPod') == 0
    data = None
    data_split = None
    stripped_data = zlib.compress(stripped_data)
    stripped_data = base64.b64encode(stripped_data).decode('ascii')
    stripped_data_split = []
    count = 0
    chars_per_line = 160
    for i in range(0, len(stripped_data), chars_per_line):
        stripped_data_split.append(stripped_data[i: i + chars_per_line])
        count += chars_per_line
    if count < len(stripped_data):
        stripped_data_split.append(stripped_data[count: ])
    text += 'BPOD_ELF_DATA = zlib.decompress(base64.b64decode(b"""\\\n'.format(name.upper())
    text += '\\\n'.join(stripped_data_split) + '\\\n'
    text += '"""))\n'

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

    # add githash
    p = subprocess.Popen(['git', 'rev-parse', 'HEAD'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, _ = p.communicate()
    githash_hex = '(custom)'
    try:
        githash_hex = stdout.decode('ascii').strip()
    except:
        pass
    text += "GITHASH = '{}'\n".format(githash_hex)

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
    print('GIT: {}'.format(GITHASH))
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
""".replace('<FILE_NAME_REPLACE>', os.path.basename(out))

    # PCBWAY flash function
    text += """

def flash_many_main():
    STATE_NO_DEVICE = 0
    STATE_DEVICE_NOT_IN_FLASH_MODE = 1
    STATE_DEVICE_FOUND = 2
    STATE_DEVICE_FLASHING = 3
    STATE_DEVICE_RESET_NEEDED = 4
    STATE_DEVICE_WAIT_FOR_BOOT = 5
    STATE_DEVICE_FLASHED_OK = 6
    STATE_DEVICE_FLASHED_ERROR = 7
    STATE_DEVICE_RESET_TIMEOUT = 8
    STATE_DEVICE_BOOT_TIMEOUT = 9
    ESPRESSIF_VID = 0x303a

    device = None
    reset_retry = None
    boot_retry = None
    state = STATE_NO_DEVICE
    new_state = -1
    while True:
        if state in [STATE_NO_DEVICE,  STATE_DEVICE_NOT_IN_FLASH_MODE]:
            device = None
            tmp_state = STATE_NO_DEVICE
            for p in list_ports.comports():
                if p.vid != ESPRESSIF_VID:
                    continue
                tmp_state = STATE_DEVICE_NOT_IN_FLASH_MODE
                if p.serial_number.strip() != '0':
                    continue
                device = p.device[:]
                tmp_state = STATE_DEVICE_FOUND
                break
            if tmp_state != state:
                new_state = tmp_state
        elif state == STATE_DEVICE_FOUND:
            new_state = STATE_DEVICE_FLASHING
        elif state == STATE_DEVICE_FLASHING:
            print('')
            print('GIT: {}'.format(GITHASH))
            extract_files()
            args = sys.argv[:1]
            args.extend(['-p', device])
            args.extend(['-b', '460800'])
            args.extend(['no_reset' if arg == 'hard_reset' else arg for arg in FLASHER_ARGS])
            sys.argv = args
            error = False
            try:
                main()
            except:
                import traceback
                print(traceback.format_exc())
                error = True
            print('')
            reset_retry = 10
            if error:
                new_state = STATE_DEVICE_FLASHED_ERROR
            else:
                new_state = STATE_DEVICE_RESET_NEEDED
                time.sleep(1)
        elif state == STATE_DEVICE_RESET_NEEDED:
            tmp_state = STATE_NO_DEVICE
            for p in list_ports.comports():
                if p.vid != ESPRESSIF_VID:
                    continue
                tmp_state = STATE_DEVICE_NOT_IN_FLASH_MODE
                if p.serial_number.strip() != '0':
                    continue
                device = p.device[:]
                tmp_state = STATE_DEVICE_FOUND
                break
            if tmp_state == STATE_DEVICE_FOUND:
                if reset_retry > 0:
                    print('(waiting for reset {}...)'.format(reset_retry))
                    reset_retry -= 1
                    time.sleep(1)
                else:
                    new_state = STATE_DEVICE_RESET_TIMEOUT
            else:
                new_state = STATE_DEVICE_WAIT_FOR_BOOT
            boot_retry = 10
        elif state == STATE_DEVICE_WAIT_FOR_BOOT:
            tmp_state = STATE_NO_DEVICE
            for p in list_ports.comports():
                if p.vid != ESPRESSIF_VID:
                    continue
                tmp_state = STATE_DEVICE_NOT_IN_FLASH_MODE
                if p.serial_number.strip() != '0':
                    continue
                device = p.device[:]
                tmp_state = STATE_DEVICE_FOUND
                break
            if tmp_state == STATE_NO_DEVICE:
                if boot_retry > 0:
                    print('(waiting for boot {}...)'.format(boot_retry))
                    boot_retry -= 1
                    time.sleep(1)
                else:
                    new_state = STATE_DEVICE_BOOT_TIMEOUT
            elif tmp_state == STATE_DEVICE_FOUND:
                new_state = STATE_DEVICE_FLASHED_ERROR
            elif tmp_state == STATE_DEVICE_NOT_IN_FLASH_MODE:
                new_state = STATE_DEVICE_FLASHED_OK
            else:
                raise Exception('UNHANDLED POST BOOT STATE: {}'.format(tmp_state))
        elif state in [STATE_DEVICE_FLASHED_ERROR, STATE_DEVICE_FLASHED_OK, STATE_DEVICE_RESET_TIMEOUT, STATE_DEVICE_BOOT_TIMEOUT]:
            # wait for unplug
            tmp_state = STATE_NO_DEVICE
            for p in list_ports.comports():
                if p.vid != ESPRESSIF_VID:
                    continue
                tmp_state = STATE_DEVICE_NOT_IN_FLASH_MODE
                if p.serial_number.strip() != '0':
                    continue
                device = p.device[:]
                tmp_state = STATE_DEVICE_FOUND
                break
            if tmp_state != STATE_NO_DEVICE:
                pass
            else:
                # go back to start and do the next one
                new_state = STATE_NO_DEVICE
        else:
            raise Exception('UNHANDLED STATE: {}'.format(state))
        if state == new_state:
            continue
        if new_state == -1:
            new_state = STATE_NO_DEVICE
        state = new_state
        if state == STATE_NO_DEVICE:
            print('=' * 20)
            print("PLEASE CONNECT DEVICE VIA USB, THEN PRESS 'FLASH' BUTTON")
        elif state == STATE_DEVICE_NOT_IN_FLASH_MODE:
            print('=' * 20)
            print("PLEASE PRESS 'FLASH' BUTTON")
        elif state == STATE_DEVICE_FOUND:
            print('=' * 20)
            print("FOUND DEVICE {}".format(device))
        elif state == STATE_DEVICE_FLASHING:
            print('=' * 20)
            print("FLASHING DEVICE {}".format(device))
        elif state == STATE_DEVICE_RESET_NEEDED:
            print('=' * 20)
            print("FLASHING COMPLETED, PLEASE PRESS 'RESET' BUTTON")
        elif state == STATE_DEVICE_WAIT_FOR_BOOT:
            print('=' * 20)
            print("WAITING FOR DEVICE TO BOOT")
        elif state == STATE_DEVICE_FLASHED_ERROR:
            print('=' * 20)
            print('  -- ERROR FLASHING --')
            print('  -- ERROR FLASHING --')
            print('  -- ERROR FLASHING --')
            print("FLASHING DIDN'T SEEM TO WORK, PLEASE FLASH AGAIN")
            print('(please unplug and replug USB)')
        elif state == STATE_DEVICE_FLASHED_OK:
            print('=' * 20)
            print("DEVICE FLASHED:")
            print("  -- PLEASE CHECK IF GREEN LIGHT IS ON --")
            print("  -- PLEASE CHECK IF GREEN LIGHT IS ON --")
            print("  -- PLEASE CHECK IF GREEN LIGHT IS ON --")
            print("IF GREEN LIGHT IS ON, DEVICE FLASHED SUCCESS")
            print('(please unplug)')
        elif state == STATE_DEVICE_RESET_TIMEOUT:
            print('=' * 20)
            print('  -- ERROR FLASHING RESET TIMEOUT --')
            print('  -- ERROR FLASHING RESET TIMEOUT --')
            print('  -- ERROR FLASHING RESET TIMEOUT --')
            print("TOO LONG FOR RESET, PLEASE FLASH AGAIN")
            print('(please unplug and replug USB)')
        elif state == STATE_DEVICE_BOOT_TIMEOUT:
            print('=' * 20)
            print('  -- ERROR FLASHING BOOT TIMEOUT --')
            print('  -- ERROR FLASHING BOOT TIMEOUT --')
            print('  -- ERROR FLASHING BOOT TIMEOUT --')
            print("TOO LONG FOR BOOT, PLEASE FLASH AGAIN")
            print('(please unplug and replug USB)')
        else:
            raise Exception('UNHANDLED NEW STATE: {}'.format(state))
"""

    # call main function
    text += """

if __name__ == '__main__':
    <FLASH_FUNCTION>()
""".replace('<FLASH_FUNCTION>', 'flash_main')

    with open(out, 'wt') as handle:
        handle.write(text)


def package_pcbway_script(args):
    src = os.path.join(os.path.dirname(args.out), 'bPodUpdater.py')
    assert os.path.exists(src)
    out = os.path.join(os.path.dirname(args.out), 'pcbway_flash_bpod.py')
    with open(src, 'rt') as handle:
        text = handle.read()
    find_str = '    flash_main()'
    replace_str = '    flash_many_main()'
    assert text.count(find_str) == 1
    assert text.count(replace_str) == 0
    text = text.replace(find_str, replace_str)
    assert text.count(find_str) == 0
    assert text.count(replace_str) == 1
    with open(out, 'wt') as handle:
        handle.write(text)
    assert os.path.exists(out)


def ctf_verify(args):
    with open(os.path.join(args.out, 'bpod.bin'), 'rb') as handle:
        data = handle.read()
    flags = list()
    keep_flags = list()
    offset = 0
    plain_text_flags = ['cybears{h0w_l0ng_1s_a_p1ec3_0f_str1ng_ch33s3}']
    while True:
        index = data[offset:].find(b'cybears{')
        if index == -1:
            break
        offset += index
        size = data[offset:].find(b'}')
        assert size != -1
        size += 1
        flag = data[offset: offset + size].decode('ascii')
        if flag not in plain_text_flags:
            print("FLAG PLAIN TEXT: '{}'".format(flag))
            flags.append(flags)
        else:
            keep_flags.append(flag)
        offset += size
    for ok in plain_text_flags:
        if ok not in keep_flags:
            raise Exception("CTF plain text flag not found '{}'".format(ok))
    if len(flags) != 0:
        raise Exception("CTF flags in plain text in binary!")
    with open(os.path.join(args.out, 'firmware/stringdb/stringdb_macro.h'), 'rt') as handle:
        text = handle.read()
    start = None
    end = None
    for line in text.split('\n'):
        if line.find('SDB_START_CTF_FLAG_CHEESY_STRINGS_II') != -1:
            start = int(line.split('(size_t)')[1].split(')')[0], 10)
        if line.find('SDB_END_CTF_FLAG_CHEESY_STRINGS_II') != -1:
            end = int(line.split('(size_t)')[1].split(')')[0], 10)
    if not start and not end:
        raise Exception('Cheesey Strings II CTF flag not found in stringdb')
    size = end - start
    if size != len('cybears{h0w_g0uda_1s_th1s_flag_can_y0u_camemb3rt_1t}'):
        raise Exception("Cheesey Strings II CTF flag size doesn't look right")
    with open(os.path.join(args.music, 'u2_snippet.wav'), 'rb') as handle:
        u2_song_data = handle.read()
    md5 = hashlib.md5(u2_song_data).hexdigest()
    assert md5.lower() == '196b901fbb5bd93923727addec8ad4ce'.lower()
    assert data.find(u2_song_data) != -1


def package_server(args):
    if os.name != 'posix':
        print('Server package only produced on linux systems')
        return

    # package up static server files
    bpod_server = os.path.join(args.server, 'bpod')
    assert os.path.exists(bpod_server)
    path = os.path.realpath(os.path.join(args.out, '..', 'bpod-server.zip'))
    if os.path.exists(path):
        os.remove(path)
    assert not os.path.exists(path)
    p = subprocess.Popen(['zip', '-r', path, '.'], cwd=bpod_server)
    p.communicate()
    assert 0 == p.returncode
    assert os.path.exists(path)

    # package up build server files
    p = subprocess.Popen(['git', 'rev-parse', 'HEAD'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, _ = p.communicate()
    githash_hex = '(custom)'
    try:
        githash_hex = stdout.decode('ascii').strip()
    except:
        pass
    out = os.path.dirname(args.out)
    with open(os.path.join(out, 'bPodUpdater.py'), 'rb') as handle:
        data = handle.read()
    md5hash_hex = hashlib.md5(data).hexdigest().lower()
    static_path = os.path.join(out, 'static')
    os.makedirs(static_path)
    assert os.path.exists(static_path)
    try:
        shutil.copyfile(os.path.join(out, 'bPodUpdater.py'), os.path.join(static_path, 'bPodUpdater.py'))
        with open(os.path.join(static_path, 'githash.txt'), 'wb') as handle:
            handle.write(githash_hex.encode('ascii'))
        with open(os.path.join(static_path, 'md5hash.txt'), 'wb') as handle:
            handle.write(md5hash_hex.encode('ascii'))
        p = subprocess.Popen(['zip', '-u', path,
            'static/githash.txt',
            'static/md5hash.txt',
            'static/bPodUpdater.py',
        ], cwd=out)
        p.communicate()
    finally:
        shutil.rmtree(static_path)
    assert 0 == p.returncode
    assert os.path.exists(path)
    assert not os.path.exists(static_path)
    print('bPod server package created {}'.format(path))


def build(args):
    if not args.esp_idf_environ:
        args.esp_idf_environ = esp_idf_environ(args)
    esp_idf_build(args)
    ctf_verify(args)
    package_flash_image(args)
    package_script(args)
    package_pcbway_script(args)
    package_server(args)


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


def mount_serial(args):
    prep_serial_device(args)


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
    parser.add_argument('--mount', default=False, action='store_true', help='Mount serial device and set permissions')
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
    args.server = os.path.join(ROOT, 'server')
    args.esp_idf = os.path.join(ROOT, 'firmware', 'esp-idf')
    args.adafruit = os.path.join(ROOT, 'firmware', 'adafruit')
    args.music = os.path.join(ROOT, 'music')
    args.esp_idf_environ = None  # lazy init when needed
    if args.build:
        args.download = True
    if args.download:
        download(args)
    if args.menuconfig:
        menuconfig(args)
    if args.build:
        build(args)
    if args.mount:
        mount_serial(args)
    if args.flash:
        flash(args)
    if args.monitor:
        monitor(args)
    if args.minicom:
        minicom(args)


if __name__ == '__main__':
    main()
