# colog
**This software provides syntax coloration to your log files.**

_Written in C._

## Installation

#### Debian-based (Ubuntu, etc)

`sudo dpkg -i colog.deb`
_(requires dpkg >= 1.15.6)_

#### Other systems
`./install.sh`

## Usage

The -d option is required, to specify the log definition colog will use. Log definitions are stored in the configuration file (default /etc/colog/colog.yml)

You can override this configuration file by using the -c option

`cat app/logs/dev.log | colog -d sf`

`tail app/logs/dev.log | colog -d sf`

`colog -d sf app/logs/dev.log`

#### Waits at the end of the file for new logs
`colog -d sf -w app/logs/dev.log`

#### Colors the whole line instead of keywords
`colog -d sf -l app/logs/dev.log`

#### Displays help
`colog -h`

## Definitions

The colog.yml file currently contains one definition : Symfony 2+ logs.
