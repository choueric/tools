#!/usr/bin/python

import sys
import subprocess
import re
import time
from termcolor import colored
import math
import pygments
from pygments.lexers import CppLexer, PythonLexer
from pygments.formatters import TerminalFormatter


cpp_lexer = CppLexer()
python_lexer = PythonLexer()

terminal_formatter = TerminalFormatter()


def colorize_cpp(line):
    return pygments.highlight(code, cpp_lexer, terminal_formatter).rstrip("\r\n")


def colorize_python(line):
    return pygments.highlight(code, python_lexer, terminal_formatter).rstrip("\r\n")


def number_of_lines_in_file(file_name):
    with open(file_name) as f:
        return sum(1 for _ in f)


reload(sys)
sys.setdefaultencoding('utf8')

file_name = sys.argv[1]
fname = file_name.lower()

blame_output = subprocess.check_output(
    ['git', 'blame', '--line-porcelain', file_name])


author_color = {}
commit_color = {}

colors = ['grey', 'red', 'green',  'yellow',
          'blue', 'magenta', 'cyan', 'white']

commit_color_index = 0
color_index = 0
line_number = 1
max_author = 0
line_number_chars = int(math.log10(number_of_lines_in_file(file_name))) + 1

lines = blame_output.splitlines()


for line in lines:
    m = re.search("^author\s+(.*)$", line)
    if m:
        author = m.group(1)
        author = author.strip().decode()
        if len(author) > max_author:
            max_author = len(author)


for line in lines:
    m = re.search("^[0-9a-f]{40}", line)
    if m:
        commit_id = m.group(0)[0:8]
        if not commit_id in commit_color:
            commit_color_index = (commit_color_index + 1) % len(colors)
            commit_color[commit_id] = colors[commit_color_index]
        continue

    m = re.search("^author\s+(.*)$", line)
    if m:
        author = m.group(1)
        author = author.strip().decode()

        if not author in author_color:
            color_index = (color_index + 1) % len(colors)
            author_color[author] = colors[color_index]

        continue

    m = re.search("^author-time\s+(.*)$", line)
    if m:
        author_time = m.group(1)
        author_time = time.ctime(int(author_time))
        author_time = author_time.encode('utf-8')
        continue

    m = re.search("^\t(.*)$", line)
    if m:
        code = m.group(1)

        if (fname.endswith('.cpp') or fname.endswith('.c') or fname.endswith('.cc') or
                fname.endswith('.h') or fname.endswith('.hpp') or fname.endswith('.hh')):
            code = colorize_cpp(code)
        elif fname.endswith('.py'):
            code = colorize_python(code)

        print(str.format("{0} {1} {2}{3}: {4}",
            colored(commit_id, commit_color[commit_id]),
            author_time,
            colored(author.ljust(max_author + 1), author_color[author]),
            str(line_number).rjust(line_number_chars), code))
        line_number += 1
        continue
