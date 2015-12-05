#!/usr/bin/env python
# -*- coding: utf-8 -*- #
from __future__ import unicode_literals

AUTHOR = u'Chiu CC'
SITENAME = u'Ordinary Days'
SITEURL = 'http://alrightchiu.github.io/SecondRound/'

PATH = 'content'

TIMEZONE = 'Asia/Taipei'

#DEFAULT_LANG = u'en'
DEFAULT_LANG = u'zh-Hant'
DEFAULT_DATE = 'fs'

DEFAULT_DATE_FORMAT = u'%b %d, %Y'
OG_LOCALE = u'zh_TW'

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
STATIC_PATHS = ['fig','extra/custom.css']
EXTRA_PATH_METADATA = {
    'extra/custom.css': {'path': 'static/custom.css'},
}
CUSTOM_CSS = 'static/custom.css'

# Flex setting
SITETITLE = SITENAME
SITESUBTITLE = u'Hey'
SITEDESCRIPTION = u'Not Bad Not Bed'
SITELOGO = u'http://alrightchiu.github.io/SecondRound/fig/antiwar.jpg'	# 也弄太久了！！！



MENUITEMS = [	# 別人直接用 /archives.html應該是因為 user pages, 總之我要用URL才行, 至少是搞定了
    ('Archives', 'http://alrightchiu.github.io/SecondRound/archives.html'),	# 也弄太久了！
    ('Categories', 'http://alrightchiu.github.io/SecondRound/categories.html'),
    ('Tags', 'http://alrightchiu.github.io/SecondRound/tags.html'),
]

# Blogroll
LINKS = (
 	#('About', '#'),
    ('Project', '#'),
)


# Social widget
SOCIAL = (
	('facebook', '#'),
    ('github', '#'),
)

DEFAULT_PAGINATION = 3

# Uncomment following line if you want document-relative URLs when developing
#RELATIVE_URLS = True
