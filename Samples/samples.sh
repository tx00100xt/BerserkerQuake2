#!/bin/bash
echo '----------------------------------------------------'
echo 'You can see 9 sample maps: map1, map2, ...and map9. '
echo 'Sources (.maps) and comments (.txt) present.        '
echo '----------------------------------------------------'

# Info
GAME_NAME="Berserker Quake2"

# Directory
CURRENT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Initialization
cd "${CURRENT_DIR}"

./berserkerq2 +set game Samples

