import datetime
import os
import subprocess

# Custom configuration for the Sphinx documentation builder.
# All configuration specific to your project should be done in this file.
#
# The file is included in the common conf.py configuration file.
# You can modify any of the settings below or add any configuration that
# is not covered by the common conf.py file.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

############################################################
### Project information
############################################################

# Product name
project = 'Mir'
author = 'Canonical Group Ltd.'

# Uncomment if your product uses release numbers
release = subprocess.check_output(["git", "describe", "--always"], encoding="utf-8")

# The default value uses the current year as the copyright year
copyright = '%s, %s' % (datetime.date.today().year, author)

## Open Graph configuration - defines what is displayed in the website preview
# The URL of the documentation output
ogp_site_url = 'https://canonical-mir.readthedocs-hosted.com/'
# The documentation website name (usually the same as the product name)
ogp_site_name = project
# An image or logo that is used in the preview
ogp_image = 'https://assets.ubuntu.com/v1/253da317-image-document-ubuntudocs.svg'

# Update with the favicon for your product (default is the circle of friends)
html_favicon = '.sphinx/_static/favicon.png'

# (Some settings must be part of the html_context dictionary, while others
#  are on root level. Don't move the settings.)
html_context = {

    # Change to the link to your product website (without "https://")
    'product_page': 'mir-server.io',

    # Add your product tag to ".sphinx/_static" and change the path
    # here (start with "_static"), default is the circle of friends
    'product_tag': '_static/tag.png',

    # Change to the discourse instance you want to be able to link to
    # using the :discourse: metadata at the top of a file
    # (use an empty value if you don't want to link)
    'discourse': 'https://discourse.ubuntu.com/c/mir/15',

    # Change to the GitHub info for your project
    'github_url': 'https://github.com/MirServer/mir',

    # Change to the branch for this version of the documentation
    #
    # https://docs.readthedocs.io/en/stable/reference/environment-variables.html#envvar-READTHEDOCS_GIT_IDENTIFIER
    'github_version': os.environ.get('READTHEDOCS_GIT_IDENTIFIER', os.environ.get('GITHUB_REF', 'main')),

    # Change to the folder that contains the documentation
    # (usually "/" or "/docs/")
    'github_folder': '/doc/',

    # Change to an empty value if your GitHub repo doesn't have issues enabled.
    # This will disable the feedback button and the issue link in the footer.
    'github_issues': 'enabled'
}

# If your project is on documentation.ubuntu.com, specify the project
# slug (for example, "lxd") here.
slug = ""

############################################################
### Redirects
############################################################

# Set up redirects (https://documatt.gitlab.io/sphinx-reredirects/usage.html)
# For example: 'explanation/old-name.html': '../how-to/prettify.html',

redirects = {}

############################################################
### Link checker exceptions
############################################################

# Links to ignore when checking links

linkcheck_ignore = [
    'http://127.0.0.1:8000'
    ]

############################################################
### Additions to default configuration
############################################################

## The following settings are appended to the default configuration.
## Use them to extend the default functionality.

# Add extensions
custom_extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.doctest',
    'sphinx.ext.mathjax',
    'sphinx.ext.viewcode',
    'sphinx.ext.imgmath',
    'sphinx.ext.todo',
    'breathe',
    'exhale',
    'sphinx.ext.graphviz',
]

# Add files or directories that should be excluded from processing.
custom_excludes = []

# Add CSS files (located in .sphinx/_static/)
custom_html_css_files = []

# Add JavaScript files (located in .sphinx/_static/)
custom_html_js_files = []

## The following settings override the default configuration.

# Specify a reST string that is included at the end of each file.
# If commented out, use the default (which pulls the reuse/links.txt
# file into each reST file).
# custom_rst_epilog = ''

# By default, the documentation includes a feedback button at the top.
# You can disable it by setting the following configuration to True.
disable_feedback_button = False

############################################################
### Additional configuration
############################################################

## Add any configuration that is not covered by the common conf.py file.

primary_domain = 'cpp'

highlight_language = 'cpp'

# Setup the exhale extension
exhale_args = {
    # These arguments are required
    "containmentFolder":     "./api",
    "rootFileName":          "library_root.rst",
    "doxygenStripFromPath":  "..",
    # Heavily encouraged optional argument (see docs)
    "rootFileTitle":         "Mir API",
    # Suggested optional arguments
    "createTreeView":        False,
    "exhaleExecutesDoxygen": True,
    "exhaleUseDoxyfile":     True,
    "contentsDirectives":    False,
}

# Setup the breathe extension
breathe_projects = {"Mir": "./doc/xml/"}
breathe_default_project = "Mir"
breathe_default_members = ('members', 'undoc-members')
breathe_order_parameters_first = True
