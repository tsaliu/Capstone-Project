#!/bin/bash

system("./start_camd.sh");
if pgrep -x "./start_camd.sh" >/dev/null
then
	system("./do_caputure.sh");
		if pgrep -x "./do_caputure.sh" >/dev/null
		then
		
		else
else 