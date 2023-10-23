# TBTREE

[![Build](https://img.shields.io/github/actions/workflow/status/pavelxdd/tbtree/meson.yml?branch=master&style=flat)](https://github.com/pavelxdd/tbtree/actions)
[![License](https://img.shields.io/github/license/pavelxdd/tbtree?style=flat&color=blue)](https://github.com/pavelxdd/tbtree/blob/master/UNLICENSE)

TBTREE is a single header intrusive double threaded binary tree.

It is inspired by `<sys/tree.h>` from `FreeBSD`.

````bash
meson setup build -Dprefix=/usr/local
meson install -Cbuild
````

Reference: https://en.wikipedia.org/wiki/Threaded_binary_tree
