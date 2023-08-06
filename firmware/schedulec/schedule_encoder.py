import os
import sys
import string
import struct
import argparse
import io
import shutil


class HexDataHeader(object):

    def __init__(self):
        self.data = None
        self.var_namespace = None
        self.var_header_guard = None
        self.var_data_name = None
        self.var_data_size = None
        self.metadata = dict()

    def pack(self):
        # indent
        indent = 0

        # source code text
        text = ''

        # header
        if not self.var_header_guard:
            text += '#pragma once\n'
        else:
            text += '#ifndef {}\n'.format(self.var_header_guard)
            text += '#define {}\n'.format(self.var_header_guard)
        text += '\n'

        # includes
        if not self.var_namespace:
            text += '#include <stdint.h>\n'
        else:
            text += '#include <cstdint>\n'
        text += '\n'

        # arduino
        text += '#include <Arduino.h>'
        text += '\n'

        # namesapce
        if self.var_namespace:
            indent += 4
            for namespace in self.var_namespace.split('::'):
                text += 'namespace {} {}\n'.format(namespace, '{')
            text += '\n'

        # metadata
        for var_name, value in self.metadata:
            if isinstance(value, str):
                var_type = 'char *'
                var_value = '"{}";'.format(value)
            elif isinstance(value, int):
                var_type = 'size_t'
                var_value = '{};  // 0x{:08x}'.format(value, value)
            else:
                raise NotImplementedError()
            text += '{}const {} {} = {}\n'.format(' ' * indent, var_type, var_name, var_value)

        # size
        text += '{}const size_t {} = {};  // 0x{:08x}\n'.format(' ' * indent, self.var_data_size, len(self.data), len(self.data))

        # data
        col = 0
        text += '{}const char {}[{}] PROGMEM  = {}\n'.format(' ' * indent, self.var_data_name, len(self.data), '{')
        indent += 4
        text += ' ' * indent
        ascii_text = ''
        printable = bytes(string.printable, 'ascii')
        not_printable = bytes('\n\t\r\m\x0b', 'ascii')
        lines = list()
        for data_byte in self.data:
            if data_byte in printable and data_byte not in not_printable and data_byte < 0x7f and data_byte >= 0x20:
                ascii_text += chr(data_byte)
            else:
                ascii_text += '.'
            line = "'\\x{:02x}',".format(data_byte)
            col += 1
            if col == 16:
                col = 0
                line += '// {}\n{}'.format(ascii_text, ' ' * indent)
                ascii_text = ''
            lines.append(line)
        text += ''.join(lines)
        if not text.endswith('\n'):
            if len(ascii_text) != 0:
                text += '// {}\n'.format(ascii_text)
            else:
                text += '\n'
        text += '};\n'
        text += '\n'

        # namesapce
        if self.var_namespace:
            for namespace in self.var_namespace.split('::'):
                text += '{} // namespace {}\n'.format('}', namespace)
            text += '\n'

        # footer
        if self.var_header_guard:
            text += '#endif //{}\n'.format(self.var_header_guard)
        return text.encode('ascii')

class ScheduleItem(object):

    def __init__(self):
        self.day = None
        self.time = None
        self.title = None
        self.presenter = None
        self.abstract = None
        self.bio = None

    @classmethod
    def value_count(cls):
        return 6

    @property
    def day_offset(self):
        return 0

    @property
    def day_data(self):
        return self.day.encode('ascii')

    @property
    def day_size(self):
        return len(self.day_data)

    @property
    def time_offset(self):
        return self.day_offset + self.day_size

    @property
    def time_data(self):
        return self.time.encode('ascii')

    @property
    def time_size(self):
        return len(self.time_data)

    @property
    def title_offset(self):
        return self.time_offset + self.time_size

    @property
    def title_data(self):
        return self.title.encode('ascii')

    @property
    def title_size(self):
        return len(self.title_data)

    @property
    def presenter_offset(self):
        return self.title_offset + self.title_size

    @property
    def presenter_data(self):
        if not self.presenter:
            return b''
        return self.presenter.encode('ascii')

    @property
    def presenter_size(self):
        return len(self.presenter_data)

    @property
    def abstract_offset(self):
        return self.presenter_offset + self.presenter_size

    @property
    def abstract_data(self):
        if self.abstract and len(self.abstract.strip()) > 0:
            return self.abstract.encode('ascii')
        if not self.presenter:
            return b''
        if not self.abstract:
            return b''
        return self.abstract.encode('ascii')

    @property
    def abstract_size(self):
        return len(self.abstract_data)

    @property
    def bio_offset(self):
        return self.abstract_offset + self.abstract_size

    @property
    def bio_data(self):
        if not self.presenter:
            return b''
        if not self.abstract:
            return b''
        if not self.bio:
            return b''
        return self.bio.encode('ascii')

    @property
    def bio_size(self):
        return len(self.bio_data)

    def pack(self):
        data = b''
        data += self.day_data
        data += self.time_data
        data += self.title_data
        data += self.presenter_data
        data += self.abstract_data
        data += self.bio_data
        assert len(data) == self.bio_offset + self.bio_size
        return data

class Schedule(object):

    def __init__(self):
        self.items = list()

    @property
    def count(self):
        return len(self.items)

    @property
    def value_count(self):
        return 6

    @property
    def text_offset(self):
        return 0

    @property
    def text_data(self):
        data = b''
        for item in self.items:
            data += item.pack()
        return data

    @property
    def text_size(self):
        return len(self.text_data)

    @property
    def offset_table_offset(self):
        return self.text_offset + self.text_size

    @property
    def offset_table_data(self):
        base = self.text_offset
        data = b''
        for item in self.items:
            data += struct.pack('>H', base + item.day_offset)
            data += struct.pack('>H', base + item.time_offset)
            data += struct.pack('>H', base + item.title_offset)
            data += struct.pack('>H', base + item.presenter_offset)
            data += struct.pack('>H', base + item.abstract_offset)
            data += struct.pack('>H', base + item.bio_offset)
            base += len(item.pack())
        return data

    @property
    def offset_table_size(self):
        return len(self.offset_table_data)

    @property
    def size_table_offset(self):
        return self.offset_table_offset + self.offset_table_size

    @property
    def size_table_data(self):
        data = b''
        for item in self.items:
            data += struct.pack('>H', item.day_size)
            data += struct.pack('>H', item.time_size)
            data += struct.pack('>H', item.title_size)
            data += struct.pack('>H', item.presenter_size)
            data += struct.pack('>H', item.abstract_size)
            data += struct.pack('>H', item.bio_size)
        return data

    @property
    def size_table_size(self):
        return len(self.size_table_data)

    def pack(self):
        data = b''
        data += self.text_data
        data += self.offset_table_data
        data += self.size_table_data
        return data

    @classmethod
    def cleanup_text(cls, text):
        if not text:
            return None
        while text.find('\r\n') != -1:
            text = text.replace('\r\n', '\n')
        while text.find('\r') != -1:
            text = text.replace('\r', '\n')
        text = text.strip('\n').strip().strip('\n').strip()
        if text == '':
            return None
        while text.find('\n ') != -1:
            text = text.replace(' ')
        while text.find('\n\n\n') != -1:
            text = text.replace('\n\n')
        return text

    @classmethod
    def parse(cls, path):
        with open(path, 'rt') as handle:
            text = handle.read()
        text = text.encode('ascii', errors='ignore').decode('ascii')
        day = None
        time = None
        title = None
        presenter = None
        in_abstract = False
        in_bio = False
        abstract = None
        bio = None
        schedule = list()
        for line in text.split('\n'):
            if line.startswith('# '):
                if title:
                    abstract = cls.cleanup_text(abstract)
                    bio = cls.cleanup_text(bio)
                    schedule.append((day, time, title, presenter, abstract, bio))
                line = line[2:]
                day = line.strip()
                time = None
                title = None
                presenter = None
                in_bio = False
                in_abstract = False
                abstract = None
                bio = None
                continue
            if line.startswith('## '):
                if title:
                    abstract = cls.cleanup_text(abstract)
                    bio = cls.cleanup_text(bio)
                    schedule.append((day, time, title, presenter, abstract, bio))
                time = None
                title = None
                presenter = None
                in_bio = False
                in_abstract = False
                abstract = None
                bio = None
                line = line[3:]
                items = line.split('-')
                assert len(items) > 1
                time = items[0]
                title = items[1]
                presenter = None
                if len(items) > 2:
                    presenter = items[-1].strip()
                    title = '-'.join(items[1:-1])
                time = time.strip().lower()
                title = title.strip()
                if title.startswith('"') and title.endswith('"'):
                    title = title[1:-1]
                time = time.replace('.', ':')
                assert time.endswith('am') or time.endswith('pm')
                assert time.count(':') == 1
                in_abstract = True
                abstract = ''
                bio = None
                continue
            if line.startswith('### '):
                line = line[4:]
                assert line.lower().startswith('bio')
                in_abstract = False
                in_bio = True
                bio = ''
                continue
            if in_abstract:
                abstract += line + '\n'
                continue
            if in_bio:
                bio += line + '\n'
                continue
            raise NotImplementedError
        if title:
            abstract = cls.cleanup_text(abstract)
            bio = cls.cleanup_text(bio)
            schedule.append((day, time, title, presenter, abstract, bio))
        new_schedule = cls()
        for talk in schedule:
            item = ScheduleItem()
            item.day = talk[0]
            item.time = talk[1]
            item.title = talk[2]
            item.presenter = talk[3]
            item.abstract = talk[4]
            item.bio = talk[5]
            new_schedule.items.append(item)
        schedule = new_schedule
        return schedule


def obufscate(data):
    enc_data = b''
    for i in range(0, len(data)):
        value = data[i] ^ (((i & 0x1f) ^ 0xd5) & 0xff)
        enc_data += struct.pack('<B', value)
    assert len(data) == len(enc_data)
    return enc_data


def main():
    parser = argparse.ArgumentParser(description='Encode schedule for the bsides badge as source files')
    parser.add_argument('--in-schedule-md', required=True, type=str, help='Input schedule MD file')
    parser.add_argument('--out-h', required=True, type=str, help='Output C header file')
    args = parser.parse_args()

    if not os.path.exists(os.path.dirname(args.out_h)):
        try:
            os.makedirs(os.path.dirname(args.out_h))
        except:
            pass

    schedule = Schedule.parse(args.in_schedule_md)

    header = HexDataHeader()
    header.data = obufscate(schedule.pack())
    header.var_header_guard = '_H_GEN_SCHEDULE_H_'
    header.var_data_name = 'schedule_data'
    header.var_data_size = 'schedule_size'
    header.metadata = [
        ('schedule_count', schedule.count),
        ('schedule_value_count', schedule.value_count),
        ('schedule_offset_table_offset', schedule.offset_table_offset),
        ('schedule_offset_table_size', schedule.offset_table_size),
        ('schedule_size_table_offset', schedule.size_table_offset),
        ('schedule_size_table_size', schedule.size_table_size),
        ('schedule_text_offset', schedule.text_offset),
        ('schedule_text_size', schedule.text_size),
    ]

    data = header.pack()

    with open(args.out_h, 'wb') as handle:
        handle.write(data)


if __name__ == '__main__':
    main()
