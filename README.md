# colog
C program that provides syntax coloration to Symfony logs

## Installation
`gcc main.c -o colog && sudo mv colog /usr/local/bin/`

## Usage
`cat app/logs/dev.log | colog`

`tail app/logs/dev.log | colog`
