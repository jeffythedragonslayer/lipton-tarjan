#!/bin/sh
fdp -Tpng $1 -o out_graph.png
xdg-open out_graph.png
