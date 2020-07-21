# sh script.sh path/to/executable path/to/src path/to/package.zip

APP_NAME_MID=MIDIVisualizer.app
BASE_PATH_MID=${APP_NAME_MID}/Contents/
BIN_PATH_MID=${APP_NAME_MID}/Contents/MacOS/
RSC_PATH_MID=${APP_NAME_MID}/Contents/Resources/

mkdir -p ${BIN_PATH_MID}
mkdir -p ${RSC_PATH_MID}

cp $1 ${BIN_PATH_MID}
cp $1 ./MIDIVisualizer-cli
cp $2/resources/icon/MIDIVisualizer.icns ${RSC_PATH_MID}

chmod +x ${BIN_PATH_MID}/MIDIVisualizer

cat <<EOF > ${BASE_PATH_MID}/Info.plist
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>CFBundleDevelopmentRegion</key>
	<string>English</string>
	<key>CFBundleGetInfoString</key>
	<string>MIDIVisualizer</string>
	<key>CFBundleName</key>
	<string>MIDIVisualizer</string>
	<key>CFBundleExecutable</key>
	<string>MIDIVisualizer</string>
	<key>CFBundleInfoDictionaryVersion</key>
	<string>6.0</string>
	<key>CFBundleLongVersionString</key>
	<string></string>
	<key>CFBundlePackageType</key>
	<string>APPL</string>
	<key>CFBundleSignature</key>
	<string>????</string>
	<key>CFBundleSupportedPlatforms</key>
	<array>
		<string>MacOSX</string>
	</array>
	<key>LSMinimumSystemVersion</key>
	<string>10.12</string>
	<key>NSHumanReadableCopyright</key>
  	<string>Simon Rodriguez</string>
  	<key>NSHighResolutionCapable</key>
  	<true/>
  	<key>CFBundleIconFile</key>
  	<string>MIDIVisualizer.icns</string>
</dict>
</plist>

EOF

# Zip both the app and the raw executable.
zip -r $3 ${APP_NAME_MID} MIDIVisualizer-cli
rm -r ${APP_NAME_MID} MIDIVisualizer-cli