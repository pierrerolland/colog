# colog
**This software provides syntax coloration to Symfony logs.**

_Written in C._

## Installation

#### Debian-based (Ubuntu, etc)

`sudo dpkg -i colog.deb`
_(requires dpkg >= 1.15.6)_

#### Other systems
`gcc src/*.c -Iinc -o colog && sudo mv colog /usr/local/bin/`

## Usage
`cat app/logs/dev.log | colog`

`tail app/logs/dev.log | colog`

`colog app/logs/dev.log`

#### Waits at the end of the file for new logs
`colog -w app/logs/dev.log`

#### Colors the whole line instead of keywords
`colog -l app/logs/dev.log`

#### Displays help
`colog -h`
