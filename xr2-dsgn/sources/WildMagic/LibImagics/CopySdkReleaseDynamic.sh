#!/bin/tcsh
mkdir -p ../SDK/Include
mkdir -p ../SDK/Library
mkdir -p ../SDK/Library/ReleaseDLL

set DIRS = `ls`
foreach dir (${DIRS})
    if (-d $dir && $dir != LibImagics.xcodeproj && $dir != build) then
        echo $dir
        cd $dir
        set CURDIR = `pwd`
        set HEADERS = `ls *.h *.inl`
        foreach header (${HEADERS})
            cp -fp "${CURDIR}"/$header ../../SDK/Include
        end
        cd ..
    endif
end

set RHEADERS = `ls *.h`
foreach rheader (${RHEADERS})
    cp -fp "${SRCROOT}"/$rheader ../SDK/Include
end

cp -fp "${SRCROOT}"/build/Default/libWm4Imagics.dylib ../SDK/Library/ReleaseDLL/libWm4Imagics.dylib 
