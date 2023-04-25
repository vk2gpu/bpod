import os
import binascii

def main():
    path = os.path.join(os.path.dirname(__file__), 'master.hex')
    with open(path, 'wb') as handle:
        handle.write(binascii.b2a_hex(os.urandom(16)))
    path = os.path.join(os.path.dirname(__file__), 'master2.hex')
    with open(path, 'wb') as handle:
        handle.write(binascii.b2a_hex(os.urandom(16)))

if __name__ == '__main__':
    main()
