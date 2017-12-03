#!/usr/bin/python

import os, sys
import zipfile
from lxml import html



# Look for JS files/script elements in ePubs
for arg in sys.argv[1:]:
#	print arg
	with zipfile.ZipFile(arg) as z:
		for filename in z.namelist():
			if not os.path.isdir(filename):
				# read the file
				if filename.endswith(".js"):
					print "Book '%s' - has JS file '%s'" % (arg, filename)
				elif filename.endswith(".xhtml"):
					with z.open(filename, "r") as f:
						tree = html.fromstring(f.read())
						scripts = tree.xpath('//script')
						if len(scripts) > 0:
							print "Book '%s' - file '%s' has scripts" % (arg, filename)
