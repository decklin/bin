#!/usr/bin/python

import sys
import urllib

post_data = urllib.urlencode({'long_url': sys.argv[1]})
response = urllib.urlopen("http://metamark.net/api/rest/simple", post_data)
print response.read()
