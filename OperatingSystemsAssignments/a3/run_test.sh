#!/bin/bash

make

for i in {1..20}; do ./simgen 1000 3152 | ./rrsim --quantum 50 --dispatch 0 --file results; done
for i in {1..20}; do ./simgen 1000 3152 | ./rrsim --quantum 50 --dispatch 5 --file results; done
for i in {1..20}; do ./simgen 1000 3152 | ./rrsim --quantum 50 --dispatch 10 --file results; done
for i in {1..20}; do ./simgen 1000 3152 | ./rrsim --quantum 50 --dispatch 15 --file results; done
for i in {1..20}; do ./simgen 1000 3152 | ./rrsim --quantum 50 --dispatch 20 --file results; done
for i in {1..20}; do ./simgen 1000 3152 | ./rrsim --quantum 50 --dispatch 25 --file results; done

for i in {1..20}; do ./simgen 1000 3152 | ./rrsim --quantum 100 --dispatch 0 --file results; done
for i in {1..20}; do ./simgen 1000 3152 | ./rrsim --quantum 100 --dispatch 5 --file results; done
for i in {1..20}; do ./simgen 1000 3152 | ./rrsim --quantum 100 --dispatch 10 --file results; done
for i in {1..20}; do ./simgen 1000 3152 | ./rrsim --quantum 100 --dispatch 15 --file results; done
for i in {1..20}; do ./simgen 1000 3152 | ./rrsim --quantum 100 --dispatch 20 --file results; done
for i in {1..20}; do ./simgen 1000 3152 | ./rrsim --quantum 100 --dispatch 25 --file results; done

for i in {1..20}; do ./simgen 1000 3152 | ./rrsim --quantum 250 --dispatch 0 --file results; done
for i in {1..20}; do ./simgen 1000 3152 | ./rrsim --quantum 250 --dispatch 5 --file results; done
for i in {1..20}; do ./simgen 1000 3152 | ./rrsim --quantum 250 --dispatch 10 --file results; done
for i in {1..20}; do ./simgen 1000 3152 | ./rrsim --quantum 250 --dispatch 15 --file results; done
for i in {1..20}; do ./simgen 1000 3152 | ./rrsim --quantum 250 --dispatch 20 --file results; done
for i in {1..20}; do ./simgen 1000 3152 | ./rrsim --quantum 250 --dispatch 25 --file results; done

for i in {1..20}; do ./simgen 1000 3152 | ./rrsim --quantum 500 --dispatch 0 --file results; done
for i in {1..20}; do ./simgen 1000 3152 | ./rrsim --quantum 500 --dispatch 5 --file results; done
for i in {1..20}; do ./simgen 1000 3152 | ./rrsim --quantum 500 --dispatch 10 --file results; done
for i in {1..20}; do ./simgen 1000 3152 | ./rrsim --quantum 500 --dispatch 15 --file results; done
for i in {1..20}; do ./simgen 1000 3152 | ./rrsim --quantum 500 --dispatch 20 --file results; done
for i in {1..20}; do ./simgen 1000 3152 | ./rrsim --quantum 500 --dispatch 25 --file results; done