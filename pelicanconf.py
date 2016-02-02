#!/usr/bin/env python
# -*- coding: utf-8 -*- #
from __future__ import unicode_literals

AUTHOR = u'Chiu CC'
SITENAME = u'Second Round'
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
# THEME = 'theme/Flex'
THEME = 'theme/pelican-clean-blog'

# 使用pelican-plugins
PLUGIN_PATHS = ['pelican-plugins']

#顯示數學式
# PLUGINS = [ 'render_math']

# Static path
STATIC_PATHS = ['fig']
# STATIC_PATHS = ['fig','extra/custom.css']	# 在 content/ 下建立 extra/custom.css
# EXTRA_PATH_METADATA = {						# 這行我也看不懂, 但是有用
#     'extra/custom.css': {'path': 'static/custom.css'},
# }
# CUSTOM_CSS = 'static/custom.css'			# 這行也很重要, 我也不知道為什麼
# 但是上面幾行可以忽略了，因為可以直接改 theme/Flex/ 內部的所有file，再回到Flex/ 執行 $ git add. & git commit
# 接著回到 SecondRound/ 執行add. , commit, push, make github

# Flex setting
SITETITLE = SITENAME
SITESUBTITLE = u'Hey'
SITEDESCRIPTION = u'Not Bad Not Bed'
SITELOGO = u'http://alrightchiu.github.io/SecondRound/fig/antiwar.jpg'	# 也弄太久了！！！

# for code blocks, 增加 line number 
# MD_EXTENSIONS = ['fenced_code', 'codehilite(css_class=highlight, linenums=True)', 'extra']

# setting for plig-in: better_codeblock_line_numbering
# MD_EXTENSIONS = ['fenced_code', 'codehilite(css_class=highlight, linenums=False)', 'extra' ]

# PLUGINS = ['better_codeblock_line_numbering', 'render_math']
PLUGINS = ['render_math']

TYPOGRIFY = True




MENUITEMS = [	# 別人直接用 /archives.html應該是因為 user pages, 總之我要用URL才行, 至少是搞定了
    ('Archives', 'http://alrightchiu.github.io/SecondRound/archives.html'),	
    ('Categories', 'http://alrightchiu.github.io/SecondRound/categories.html'),
    ('Tags', 'http://alrightchiu.github.io/SecondRound/tags.html'),
]

# Blogroll
LINKS = (
 	#('About', '#'),
    # ('Project', '#'),
)

CC_LICENSE = {
    'name': 'Creative Commons Attribution-ShareAlike',
    'version': '4.0',
    'slug': 'by-sa'
}

# Social widget
SOCIAL = (
	#('facebook', '#'),
    ('github', 'https://github.com/alrightchiu'),
)

DEFAULT_PAGINATION = 10

# Uncomment following line if you want document-relative URLs when developing
#RELATIVE_URLS = True
