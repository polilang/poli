#!/bin/sh

inotifywait -q -m -e close_write lexer.c parser.c|
while read -r filename event; do
   kill `ps aux | grep [v]algrind | awk '{print $2}' | tail -n1`
   clear
   cc -Wall -g parser.c && valgrind -q --show-leak-kinds=all ./a.out &
done
