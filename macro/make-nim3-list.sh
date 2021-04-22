#!/bin/bash

FN_OUT=list_nim3.txt

find /pnfs/e1039/persistent/users/kenichi/jobsub_script/job_20210305 -name data_nim3.root | sort >$FN_OUT
