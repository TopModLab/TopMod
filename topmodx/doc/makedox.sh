#!/bin/bash

echo "Running Doxygen..."
#run doxygen using Doxyfile for configuration
#in order to run doxygen on OS/X, install it using 
# 'sudo port install doxygen'
doxygen 

echo "Zipping up html directory..."
#zip up the contents of the html directory so it can be uploaded to the server
tar -czvf doc.tar.gz ./html

#send to server using scp
echo "Uploading to server, it should ask for the server password..."
scp doc.tar.gz unclehor@unclehorrid.com:/home/unclehor/public_html/ends170/topmod/doc/

echo "cd public_html/ends170/topmod/doc && tar -xzvf doc.tar.gz ./ && mv html/* ./ && exit"

# login to server and go to the doc directory 
ssh unclehor@unclehorrid.com


# cd public_html/ends170/topmod/doc

# untar it into the ./html directory 
# tar -xzvf doc.tar.gz ./
# move the files up a directory
# not sure if there is a way to do this thing 
# without needing to do that every time..
# mv html/* ./

# end the ssh session
# exit


# the script stops once you get into the ssh session
# not sure how to fix this but for now just copy paste this line:
# cd public_html/ends170/topmod/doc && tar -xzvf doc.tar.gz ./ && mv html/* ./ && exit

