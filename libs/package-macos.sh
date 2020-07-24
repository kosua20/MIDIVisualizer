# sh script.sh path/to/executable.app path/to/src path/to/package.zip
APP_NAME_MID=MIDIVisualizer.app
BIN_PATH_MID=${APP_NAME_MID}/Contents/MacOS/MIDIVisualizer

cp -r $1 ${APP_NAME_MID}
cp ${BIN_PATH_MID} ./MIDIVisualizer-cli

# Zip both the app and the raw executable.
zip -r $3 ${APP_NAME_MID} MIDIVisualizer-cli
rm -r ${APP_NAME_MID} MIDIVisualizer-cli