#!/bin/bash
for i in `seq 0 199`;
do
	echo "Benjamin ${i}"
	./ssm "test/LECOMTEBenjamin/test${i}.txt"
	echo "Matthieu ${i}"
	./ssm "test/RIBIEREMatthieu/test${i}.txt"
	echo "Thibaut ${i}"
	./ssm "test/MANCEAUThibaut/test${i}.txt"
done
exit 0