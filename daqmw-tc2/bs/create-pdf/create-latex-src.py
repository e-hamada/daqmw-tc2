#!/usr/bin/env python

import os
import sys
import time # for sleep

def get_latex_head():
    src = r"""\documentclass[11pt]{jsarticle}
%\usepackage{graphicx}
%\usepackage{hyperref}
%\usepackage{enumerate}
\usepackage{fancyvrb}
\fvset{
    tabsize=4,
    baselinestretch=0.55,
    numbers=left,
    frame=single,
    framesep=5pt,
    numbersep=5pt,
    fontsize=\small,
    % fontfamily=courier,
    % frame=topline,
    % frame=bottomline,
    % frame=lines,
    % frame=single,
    % firstnumber=100,
    % firstnumber=last,
    % comment
}

\begin{document}
"""
    return src

def get_latex_tail():
    src = r'\end{document}'
    return src

def get_subsection_part(title, path):
    subsection_part = r"""\subsection*{%s}
\begingroup
\catcode`\_=12\relax
\VerbatimInput{%s}
\endgroup
\clearpage

""" % (title, path)
    return subsection_part

def main():
    if len(sys.argv) == 1:
        sys.exit('Usage: create-latex-src filename.tex dir0 dir1 ...')
    
    output = sys.argv[1]

    try:
        fh = open(output, 'w')
    except IOError, e:
        sys.exit(e)

    fh.writelines(get_latex_head())

    for d in sys.argv[2:]:
        for (root, dirs, files) in os.walk(d):
            files.sort(key=lambda f: os.path.splitext(f)[1], reverse=True)
            for f in files:
                ext = os.path.splitext(f)[1]
                if f == 'Makefile' or ext == '.cpp' or ext == '.h':
                    file_path = os.path.join(root, f)
                    title = file_path.replace('_', '\_').replace('../', '')
                    title = os.path.join('bs', title)
                    fh.writelines(get_subsection_part(title, file_path))

    fh.writelines(get_latex_tail())

if __name__ == '__main__':
    main()
