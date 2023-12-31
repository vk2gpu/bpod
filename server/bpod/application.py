
import os
import sys
import string
import binascii
from urllib.parse import parse_qs
from bsides2023.token import Token
from bsides2023.scoredb import ScoreDb
from bsides2023.html import HtmlTable

with open(os.path.join(os.path.dirname(__file__), 'static', 'update.html'), 'rb') as handle:
    UPDATE_HTML_DATA = handle.read()
TEMPLATE_HTML_DATA = b''
TEMPLATE_HTML_DATA += UPDATE_HTML_DATA[:UPDATE_HTML_DATA.find(b'</header>')] + b'</header>'
TEMPLATE_HTML_DATA += b'HTML_CONTENT'
TEMPLATE_HTML_DATA += b'<br /><br/>' + UPDATE_HTML_DATA[UPDATE_HTML_DATA.find(b'<footer>'):]
with open(os.path.join(os.path.dirname(__file__), 'static', 'githash.txt'), 'rb') as handle:
    GITHASH_DATA = handle.read().decode('ascii').strip().encode('ascii')
with open(os.path.join(os.path.dirname(__file__), 'static', 'md5hash.txt'), 'rb') as handle:
    MD5HASH_DATA = handle.read().decode('ascii').strip().encode('ascii')


def debug_mode():
    try:
        if not 'BPOD_DEBUG' in os.environ:
            return False
        if os.environ['BPOD_DEBUG'].lower() != 'true':
            return False
    except:
        return False
    return True

def get_version():
    return int(os.environ['BPOD_DB_VERSION'])

def get_key():
    try:
        key = binascii.a2b_hex(os.environ['BPOD_KEY'])
        assert len(key) == 16
        assert isinstance(key, bytes)
    except:
        return None
    return key

def get_key_2():
    try:
        key = binascii.a2b_hex(os.environ['BPOD_KEY_2'])
        assert len(key) == 16
        assert isinstance(key, bytes)
    except:
        return None
    return key

def get_query_string(environ, param):
    try:
        value = parse_qs(environ['QUERY_STRING'])[param][0]
    except:
        return None
    return value

def get_score_row_count(environ):
    return int(os.environ['BPOD_SCORE_ROWS'])

def format_score_board(table):
    new_table = list()
    i = 0
    for row in table:
        ipsn = row[0]
        name = row[1]
        pos = '#{}'.format(i + 1)
        name = ipsn if not name else '{} [{}]'.format(ipsn, name)
        score = str(row[2]) + ' pts'
        new_row = [pos, name, score]
        new_table.append(new_row)
        i += 1
    table = new_table
    return table

def sanitize_name(name_unsafe):
    # these are the characters we will allow for names
    ok_chars = string.ascii_letters + string.digits + '_- '
    # no name, no name
    if not name_unsafe:
        return None
    # deal with ascii characters only (and hence make it bytes first)
    name = name_unsafe.encode('ascii')
    # no longer than 8 characters
    name = name[:8]
    # remove all the bad ones
    name_safe = ''
    for value in name:
        if chr(value) in ok_chars:
            name_safe += chr(value)
    # don't return an empty name
    if len(name_safe) == 0:
        return None
    return name_safe

def error_app(environ, start_response, err='bpod'):
    data = err.encode('ascii')
    start_response('200 OK', [('Content-Type','text/html'), ('Content-Length',str(len(data)))])
    return [data]

def score_board_app(environ, start_response):
    text = ''
    text += '<div class="bpod score-table">'
    with ScoreDb(version=get_version()) as score:
        g1 = '<h1>{}</h1>'.format('Snake')
        g1 += HtmlTable(format_score_board(score.score_board('snake', count=get_score_row_count(environ)))).html()
        g2 = '<h1>{}</h1>'.format('Tetris')
        g2 += HtmlTable(format_score_board(score.score_board('tetris', count=get_score_row_count(environ)))).html()
        text += HtmlTable([[g1, g2]]).html().replace('</h1><table><tbody>', '</h1><table class="score-box"><tbody>')
    text += '</div>'
    data = TEMPLATE_HTML_DATA[:].replace(b'HTML_CONTENT', text.encode('ascii'))
    start_response('200 OK', [('Content-Type','text/html'), ('Content-Length',str(len(data)))])
    return [data]

def submit_name_form_app(environ, start_response, qs_token, token):
    text = ''
    text += '<div class="bpod">'
    text += '<h1>{}</h1><p>{} {} pts</p>'.format(token.ipsn, token.game, token.score)
    text += '<form id="name_form" "/index.html" method="get">'
    text += '<input type="hidden" name="s" value="{}" />'.format(qs_token)
    text += '<label for="name">Your name:</label>'
    text += '<input id="name" type="text" name="n" value="" />'
    text += '<button type="submit" form="name_form" value="Submit">Submit</button>'
    text += '</form>'
    text += '</div>'
    data = TEMPLATE_HTML_DATA[:].replace(b'HTML_CONTENT', text.encode('ascii'))
    start_response('200 OK', [('Content-Type','text/html'), ('Content-Length',str(len(data)))])
    return [data]

def bsides2023_app(environ, start_response):
    key = get_key()
    key2 = get_key_2()
    qs_token_unsafe = get_query_string(environ, 'o')
    qs_save_unsafe = get_query_string(environ, 's')
    qs_name_unsafe = get_query_string(environ, 'n')
    if environ['PATH_INFO'].endswith('reset.html'):
        # only allow a reset in debug mode
        if debug_mode():
            with ScoreDb(version=get_version()) as score:
                score.reset_db()
    if environ['PATH_INFO'].endswith('update.html'):
        # return update page
        data = UPDATE_HTML_DATA[:]
        githash = GITHASH_DATA[:]
        md5hash = MD5HASH_DATA[:]
        data = data.replace(b'GITHASH', githash)
        data = data.replace(b'MD5HASH', md5hash)
        start_response('200 OK', [('Content-Type','text/html'), ('Content-Length',str(len(data)))])
        return [data]
    if qs_token_unsafe:
        # uploading a token
        token = Token.decode(key, key2, qs_token_unsafe)
        # if no error parsing the token, its considered safe
        qs_token_safe = qs_token_unsafe
        # add the score
        with ScoreDb(version=get_version()) as score:
            score.add_score(token.ipsn, token.game, token.score)
        # return a form for the user to set their name
        return submit_name_form_app(environ, start_response, qs_token_safe, token)
    elif qs_save_unsafe:
        # parse the token to get the user ipsn
        token = Token.decode(key, key2, qs_save_unsafe)
        # second time (with a name field) change the name
        safe_name = sanitize_name(qs_name_unsafe)
        if safe_name:
            with ScoreDb(version=get_version()) as score:
                score.add_name(token.ipsn, safe_name)
        # fall through to score board
    return score_board_app(environ, start_response)

def application(environ, start_response):
    err = ''
    try:
        return bsides2023_app(environ, start_response)
    except Exception as error:
        # application failed
        import traceback
        if debug_mode():
            # show the stack trace in debug mode
            err = str(traceback.format_exc())
        else:
            # show the score board otherwise
            try:
                return score_board_app(environ, start_response)
            except:
                # the score board failed, display nothing
                err = ''
    return error_app(environ, start_response, err)
