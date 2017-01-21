#!/bin/bash

URL="https://developer.blender.org/maniphest/project/2/type/Bug/query/open/"

value=`curl -s "$URL" | grep -o -E '>[0-9]+\stotal tasks' | sed -r 's/>([0-9]+).*/\1/'`

#sudo `dirname $0`/nixie-send $value
`dirname $0`/nixie-send $value
