set project=lfcs_data
make clean
make
bannertool makebanner -i "cia/banner.png" -a "cia/audio.wav" -o "cia/banner.bnr"
bannertool makesmdh -i "cia/icon.png" -l "data for lfcs bruteforce" -s %project% -p "zoogie" -o "cia/icon.icn"
makerom -f cia -o %project%.cia -rsf cia/template.rsf -target t -elf %project%.elf -icon cia/icon.icn -banner cia/banner.bnr -exefslogo
cp %project%.cia f:/CIA/%project%.cia
pause