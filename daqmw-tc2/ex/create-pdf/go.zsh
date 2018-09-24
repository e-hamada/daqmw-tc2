#!/bin/zsh

setopt extendedglob
setopt nonomatch

last_ex=14

export PATH=/usr/local/texlive/2014/bin/x86_64-linux:$PATH

rm -f ex??.tex
rm -f ex.tex
rm -f *.aux
rm -f *.dvi
rm -f *.log

pandoc -t latex ../README.md -o ex.tex

for i in {01..$last_ex}; do
    pandoc -t latex ../ex${i}/README.md -o ex${i}.tex
done

for i in {01..$last_ex}; do
    if [ -f ../ex${i}/Makefile ]; then
        echo                                                 >> ex${i}.tex
        echo -E "\subsection*{Source Code: ex${i}/Makefile}" >> ex${i}.tex
        echo -E "\VerbatimInput{../ex${i}/Makefile}"         >> ex${i}.tex
    fi

    # we have to some work to include file which has _ (underscore) in filename
    for f in ../ex${i}/*.h; do
        if [ -f $f ]; then
            file=$(basename $f | sed -e 's|_|\\_|g')
            echo                                              >> ex${i}.tex
            echo -E "\subsection*{Source Code: ex${i}/$file}" >> ex${i}.tex
            echo -E '\begingroup'                             >> ex${i}.tex
            echo -E '\catcode`\_=12\relax'                    >> ex${i}.tex
            echo -E "\VerbatimInput{$f}"                      >> ex${i}.tex
            echo -E '\endgroup'                               >> ex${i}.tex
        fi
    done

    for f in ../ex${i}/*.cpp; do
        if [ -f $f ]; then
            file=$(basename $f | sed -e 's|_|\\_|g')
            echo                                              >> ex${i}.tex
            echo -E "\subsection*{Source Code: ex${i}/$file}" >> ex${i}.tex
            echo -E '\begingroup'                             >> ex${i}.tex
            echo -E '\catcode`\_=12\relax'                    >> ex${i}.tex
            echo -E "\VerbatimInput{$f}"                      >> ex${i}.tex
            echo -E '\endgroup'                               >> ex${i}.tex
        fi
    done
done

platex daqmw-tc-ex
dvipdfmx daqmw-tc-ex
