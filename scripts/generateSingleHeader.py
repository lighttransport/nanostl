# Based on scripts in Catch unit test framework.
 
#!/usr/bin/env python

from __future__ import print_function

import os
import sys
import re
import datetime
import string

#from scriptCommon import catchPath

basePath = "../"

def generate(v):
    includesParser = re.compile( r'\s*#\s*include\s*"(.*)"' )
    guardParser = re.compile( r'\s*#.*NANOSTL_.*_H_')
    defineParser = re.compile( r'\s*#define')
    ifParser = re.compile( r'\s*#ifndef NANOSTL_.*_H_')
    endIfParser = re.compile( r'\s*#endif // NANOSTL_.*_H_')
    ifImplParser = re.compile( r'\s*#ifdef NANOSTL_CONFIG_RUNNER' )
    commentParser1 = re.compile( r'^\s*/\*')
    commentParser2 = re.compile( r'^ \*')
    blankParser = re.compile( r'^\s*$')
    seenHeaders = set([])
    rootPath = os.path.join( basePath, 'include/' )
    outputPath = os.path.join( basePath, 'single_include/nanostl.h' )


    globals = {
        'ifdefs'     :  0,
        'implIfDefs' : -1,
        'includeImpl': True
    }

    for arg in sys.argv[1:]:
        arg = string.lower(arg)
        if arg == "noimpl":
            globals['includeImpl'] = False
            print( "Not including impl code" )
        else:
            print( "\n** Unrecognised argument: " + arg + " **\n" )
            exit(1)

    out = open( outputPath, 'w' )

    def write( line ):
        if globals['includeImpl'] or globals['implIfDefs'] == -1:
            out.write( line )

    def parseFile( path, filename ):
        f = open( path + filename, 'r' )
        blanks = 0
        for line in f:
            if ifParser.match( line ):
                globals['ifdefs'] += 1
            elif endIfParser.match( line ):
                globals['ifdefs'] -= 1
                if globals['ifdefs'] == globals['implIfDefs']:
                    globals['implIfDefs'] = -1
            m = includesParser.match( line )
            if m:
                header = m.group(1)
                headerPath, sep, headerFile = header.rpartition( "/" )
                if not headerFile in seenHeaders:
                    if headerFile != "tbc_text_format.h" and headerFile != "clara.h":
                        seenHeaders.add( headerFile )
                    write( "// #included from: {0}\n".format( header ) )
                    if headerPath == "internal" and path.endswith("internal/"):
                        headerPath = ""
                        sep = ""
                    if os.path.exists( path + headerPath + sep + headerFile ):
                        parseFile( path + headerPath + sep, headerFile )
                    else:
                        parseFile( rootPath + headerPath + sep, headerFile )
            else:
                if ifImplParser.match(line):
                    globals['implIfDefs'] = globals['ifdefs']
                if (not guardParser.match( line ) or defineParser.match( line ) ) and not commentParser1.match( line )and not commentParser2.match( line ):
                    if blankParser.match( line ):
                        blanks = blanks + 1
                    else:
                        blanks = 0
                    if blanks < 2:
                        write( line.rstrip() + "\n" )


    out.write( "/*\n" )
    out.write( " * NanoSTL v{0}\n".format( v ) )
    out.write( " * Generated: {0}\n".format( datetime.datetime.now() ) )
    out.write( " * ----------------------------------------------------------\n" )
    out.write( " * The MIT License (MIT)\n" )
    out.write( " *\n" )
    out.write( " * Copyright (c) 2017 Light Transport Entertainment, Inc.\n" )
    out.write( " *\n") 
    out.write( " * Permission is hereby granted, free of charge, to any person obtaining a copy\n" )
    out.write( " * of this software and associated documentation files (the \"Software\"), to deal\n" )
    out.write( " * in the Software without restriction, including without limitation the rights\n" )
    out.write( " * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n" )
    out.write( " * copies of the Software, and to permit persons to whom the Software is\n" )
    out.write( " * furnished to do so, subject to the following conditions:\n" )
    out.write( " *\n" )
    out.write( " * The above copyright notice and this permission notice shall be included in\n" )
    out.write( " * all copies or substantial portions of the Software.\n" )
    out.write( " *\n" )
    out.write( " * THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n" )
    out.write( " * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n" )
    out.write( " * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n" )
    out.write( " * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n" )
    out.write( " * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n" )
    out.write( " * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN\n" )
    out.write( " * THE SOFTWARE.\n" )
    out.write( " */\n" )
    out.write( "#ifndef NANOSTL_SINGLE_INCLUDE_H_\n" )
    out.write( "#define NANOSTL_SINGLE_INCLUDE_H_\n" )

    parseFile( rootPath, 'nanostl.h' )

    out.write( "#endif // NANOSTL_SINGLE_INCLUDE_H_\n\n" )
    out.close()
    print ("Generated single include for NanoSTL v{0}\n".format( v ) )

if __name__ == '__main__':
    #from releaseCommon import Version
    generate("0.1.0")
