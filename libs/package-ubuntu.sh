# sh script.sh path/to/executable path/to/src path/to/package.zip 

APP_NAME_MID=MIDIVisualizer_temp
BIN_PATH_MID=${APP_NAME_MID}/

mkdir -p ${BIN_PATH_MID}
cp $1 ${BIN_PATH_MID}
chmod +x ${BIN_PATH_MID}/MIDIVisualizer

cat <<EOF > ${BIN_PATH_MID}/MIDIVisualizer.sh
#!/bin/sh
./MIDIVisualizer >> MIDIVisualizer.log 2>&1
EOF

chmod +x ${BIN_PATH_MID}/MIDIVisualizer.sh

zip -r -j $3 ${BIN_PATH_MID}/MIDIVisualizer.sh ${BIN_PATH_MID}/MIDIVisualizer
rm -r ${APP_NAME_MID}