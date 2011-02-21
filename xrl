#!/usr/bin/python

import sys
import urllib

api = 'http://metamark.net/api/rest/simple'

for url in sys.argv[1:]:
    print urllib.urlopen(api, urllib.urlencode({'long_url': url})).read()
