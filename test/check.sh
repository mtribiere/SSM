#!/bin/bash
output=$(cd .. && make test)
echo $output
