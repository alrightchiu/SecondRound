#!/usr/bin/env python
# -*- coding: utf-8 -*- #
from __future__ import unicode_literals

AUTHOR = u'Chiu CC'
SITENAME = u'Ordinary Days'
SITEURL = ''

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
USE_FOLDER_AS_CATEGORY = False

# Theme
THEME = 'theme/Flex'

# Static path
STATIC_PATHS = ['fig']	# must set this variable to show the logo

# Flex setting
SITETITLE = SITENAME
SITESUBTITLE = u'Hey'
#SITEDESCRIPTION = u'Not Bad Not Bed'
SITELOGO = u'http://alrightchiu.github.io/SecondRound/fig/antiwar.jpg'	# 也弄太久了！！！



MENUITEMS = [
    ('Archives', 'http://alrightchiu.github.io/SecondRound/archives.html'),
    ('Categories', 'http://alrightchiu.github.io/SecondRound/categories.html'),
    ('Tags', '/tags.html'),
]

# Blogroll
# LINKS = (
#  	('About', '#'),
#     ('Project', '#'),
# )


# Social widget
SOCIAL = (
	('facebook', '#'),
    ('github', '#'),
)

DEFAULT_PAGINATION = 10

# Uncomment following line if you want document-relative URLs when developing
#RELATIVE_URLS = True
