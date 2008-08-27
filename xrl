#!/usr/bin/python

import sys
import urllib

api = 'http://metamark.net/api/rest/simple'
args = urllib.urlencode({'long_url': sys.argv[1]})
print urllib.urlopen(api, args).read()
