#!/usr/bin/env python
# -*- coding: utf-8 -*- #
from __future__ import unicode_literals

AUTHOR = u'Chiu CC'
SITENAME = u'Ordinary Days'
SITEURL = 'http://localhost:8000'

PATH = 'content'

TIMEZONE = 'Asia/Taipei'

DEFAULT_LANG = u'en'

# Feed generation is usually not desired when developing
FEED_ALL_ATOM = None
CATEGORY_FEED_ATOM = None
TRANSLATION_FEED_ATOM = None
AUTHOR_FEED_ATOM = None
AUTHOR_FEED_RSS = None

MAIN_MENU = True
USE_FOLDER_AS_CATEGORY = True

# Theme
THEME = 'theme/Flex'

# Static path
STATIC_PATHS = ['fig']

# Flex setting
SITETITLE = SITENAME
SITESUBTITLE = u'Hey'
SITEDESCRIPTION = u'Not Bad Not Bed'
SITELOGO = u'http://alrightchiu.github.io/SecondRound/fig/antiwar.jpg'

#https://github.com/alrightchiu/SecondRound/blob/master/content/fig/antiwar.jpg
#https://github.com/Xion/xion.github.io/blob/master/logo.jpeg
#http://xion.io/logo.jpeg


MENUITEMS = [
    ('Archives', '/archives.html'),
    ('Categories', '/categories.html'),
    ('Tags', '/tags.html'),
]

# Blogroll
LINKS = (
 	('Pelican', 'http://getpelican.com/'),
    ('Python.org', 'http://python.org/'),
    ('Jinja2', 'http://jinja.pocoo.org/'),
)


# Social widget
SOCIAL = (
	('facebook', '#'),
    ('github', '#'),
)

DEFAULT_PAGINATION = 10

# Uncomment following line if you want document-relative URLs when developing
#RELATIVE_URLS = True
