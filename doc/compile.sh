#!/bin/sh
#gosh scm2texi \
#--output-directory="." \
#--title="Gauche-FUSE binding" \
#--description="Enabling you to write filesystems in the Scheme language." \
#--prefix="fuse" \
#--title-ja="Gauche-FUSE binding" \
#--description-ja="Scheme言語でファイルシステムを書けます。" \
#--author="Teruaki Gemma" \
#--mail="teruakigemma@gmail.com" \
#fuse.scm

gosh extract -en fuse-ref.texi > fuse-refe.texi
texi2html fuse-refe.texi
gosh extract -jp fuse-ref.texi > fuse-refj.texi
texi2html fuse-refj.texi
