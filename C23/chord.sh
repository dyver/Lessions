#!/bin/bash

# for n in E2 A2 D3 G3 B3 E4
# do
#     play -n synth 4 pluck $n repeat 2
# done

play -n synth pl G2 pl B2 pl D3 pl G3 pl D4 pl G4 \
                   delay 0 .05 .1 .15 .2 .25 remix - fade 0 4 .1 norm -1
