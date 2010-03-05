#!/bin/sh
# Copyright 2006 David Johnson
# The author grants unlimited permission to
# copy, distribute and modify this script

APPNAME=TopMod

### get system configuration ########################################

# as long as we can find qmake, we don't need QTDIR
FWPATH=`qmake -query QT_INSTALL_LIBS`
if [ ! -d $FWPATH/QtGui.framework ] ; then
    echo "ERROR: cannot find the Qt frameworks. Make sure Qt is installed"
    echo "and qmake is in your environment path."
    exit
fi

### get required user input #########################################

if [ -z $APPNAME ] ; then
    echo
    echo "This script prepares a Qt application bundle for deployment. It will"
    echo "copy over the required Qt frameworks and sets the installation"
    echo "identifications. Please see the \"Deploying an Application on Qt/Mac\""
    echo "page in the Qt documentation for more information."
    echo
    echo "This script assumes you have already built the application bundle."
    echo
    echo  -n "What is the name of the application? "
    read userinput
    APPNAME=$userinput
fi

BUNDLE=$APPNAME.app

if [ ! -d $BUNDLE ] ; then
    echo "ERROR: cannot find application bundle \"$BUNDLE\" in current directory"
    exit
fi

if [ ! -x $BUNDLE/Contents/MacOS/$APPNAME ] ; then
    echo "ERROR: cannot find application in bundle. Did you forget to run make?"
    exit
fi

echo "application: $APPNAME"
echo "bundle:      $BUNDLE"

### query binary for frameworks #####################################

for n in `otool -L $BUNDLE/Contents/MacOS/$APPNAME | grep Qt` ; do
    path=`echo $n | grep Qt`
    if [ $path ] ; then
	name=`basename $path`
	FRAMEWORKS="$FRAMEWORKS $name"
	# sanity check
	if [ "$path" != "$FWPATH/$name.framework/Versions/4/$name" ] ; then
	    echo "ERROR: problem with framework paths. Perhaps this script has already been run?"
	    exit
	fi
    fi
done

echo -n "Using frameworks"
for n in $FRAMEWORKS ; do
    echo -n " $n"
done
echo

### make install ####################################################

# assumes install target populates the application bundle
echo "Running make install"
if [ -e Makefile.Release ] ; then
    make -f Makefile.Release install
else
    make install
fi
strip $BUNDLE/Contents/MacOS/$APPNAME

### copy over frameworks ############################################

mkdir -p $BUNDLE/Contents/Frameworks
for framework in $FRAMEWORKS ; do
    if [ ! -d $FWPATH/$framework.framework ] ; then
        echo "ERROR: cannot find $FWPATH/$framework.framework"
        exit
    fi
    echo "Copying $framework framework"
    cp -fR $FWPATH/$framework.framework $BUNDLE/Contents/Frameworks
    # strip libs (-x is max allowable for shared libs)
    strip -x $BUNDLE/Contents/Frameworks/$framework.framework/Versions/4/$framework
done

# copy over the 3dx framework
# cp -fR /Library/Frameworks/3DconnexionClient.framework $BUNDLE/Contents/Frameworks

# remove unwanted parts
find $BUNDLE/Contents/Frameworks | egrep "debug|Headers" | xargs rm -rf

### set the identification names for frameworks #####################

echo -n "Setting framework IDs..."

for framework in $FRAMEWORKS ; do
	echo -n " $framework"
	install_name_tool \
				-id @executable_path/../Frameworks/$framework.framework/Versions/4/$framework \
        $BUNDLE/Contents/Frameworks/$framework.framework/Versions/4/$framework
done
echo

# 3Dx framework
# framework="3DconnexionClient"
# install_name_tool \
			# -id @executable_path/../Frameworks/$framework.framework/Versions/A/$framework \
			# $BUNDLE/Contents/Frameworks/$framework.framework/Versions/A/$framework

### change framework location #######################################

echo -n "Changing framework paths..."
for framework in $FRAMEWORKS ; do
    echo -n " $framework"
    install_name_tool \
	-change $FWPATH/$framework.framework/Versions/4/$framework \
        @executable_path/../Frameworks/$framework.framework/Versions/4/$framework \
        $BUNDLE/Contents/MacOS/$APPNAME
done
echo

# 3Dx framework
# framework="3DconnexionClient"
# install_name_tool \
# -change /Library/Frameworks/$framework.framework/Versions/A/$framework \
      # @executable_path/../Frameworks/$framework.framework/Versions/A/$framework \
      # $BUNDLE/Contents/MacOS/$APPNAME

### change location for bundled frameworks #########################

echo -n "Fixing bundled frameworks..."
for framework in $FRAMEWORKS ; do
    echo -n " $framework"
    fwdeps=""
    bundledfw="$BUNDLE/Contents/Frameworks/$framework.framework/Versions/4/$framework"
    # get framework dependencies
    for n in `otool -LX $bundledfw | grep Qt` ; do
			path=`echo $n | grep Qt`
			if [ $path ] ; then
			    name=`basename $path`
			    fwdeps="$fwdeps $name"
			fi
    done
    # fix dependency location
    for dep in $fwdeps ; do
			if [ "$dep" != "$framework" ] ; then
		            install_name_tool \
				-change $FWPATH/$dep.framework/Versions/4/$dep \
				@executable_path/../Frameworks/$dep.framework/Versions/4/$dep \
				$bundledfw
			fi
    done
done
# echo

# 3Dx framework
# framework="3DconnexionClient"
# echo -n " $framework"
# fwdeps=""
# bundledfw="$BUNDLE/Contents/Frameworks/$framework.framework/Versions/A/$framework"
# # get framework dependencies
# for n in `otool -LX $bundledfw | grep 3DConnexion` ; do
# 	path=`echo $n | grep 3DConnexion`
# 	if [ $path ] ; then
# 	    name=`basename $path`
# 	    fwdeps="$fwdeps $name"
# 	fi
# done
# # fix dependency location
# for dep in $fwdeps ; do
# 	if [ "$dep" != "$framework" ] ; then
# 		install_name_tool \
# 			-change $FWPATH/$dep.framework/Versions/4/$dep \
# 			@executable_path/../Frameworks/$dep.framework/Versions/4/$dep \
# 			$bundledfw
# 	fi
# done

### create disk image ###############################################

# echo "Creating disk image"
# imagedir="/tmp/$APPNAME.$$"
# mkdir $imagedir
# cp -R $BUNDLE $imagedir
# 
# # \todo  copy over additional files, if any
# hdiutil create -ov -srcfolder $imagedir -format UDBZ -volname "$APPNAME" "$APPNAME.dmg"
# rm -rf $imagedir

echo "Done"