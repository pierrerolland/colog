# colog
C program that provides syntax coloration to Symfony logs

## Installation
`gcc *.c -o colog && sudo mv colog /usr/local/bin/`

## Usage
`cat app/logs/dev.log | colog`

`tail app/logs/dev.log | colog`

`colog app/logs/dev.log`

### Waits at the end of the file for new logs
`colog -w app/logs/dev.log`

### Displays help
`colog -h`
