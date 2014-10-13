#!/usr/bin/env python
##
## Copyright (C) 2014  Vishesh Handa <me@vhanda.in>
##
## This library is free software; you can redistribute it and/or
## modify it under the terms of the GNU Lesser General Public
## License as published by the Free Software Foundation; either
## version 2.1 of the License, or (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public
## License along with this library; if not, write to the Free Software
## Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
##
##

from guessit import guess_file_info
from optparse import OptionParser
import json

def fetchData(filename):
    info = ['filename']
    guess = guess_file_info(filename, info)
    guess = dict(guess)
    guess.pop("unidentified", None)
    return guess

parser = OptionParser()
options, args = parser.parse_args()

for filename in args:
    data = fetchData(filename)
    print json.dumps(data)
