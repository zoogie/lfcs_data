# lfcs_data

Dumps high 3 octets of 3DS MAC and lfcs/deviceID with LSB replaced with 0x80 as an average. The dumpfile location is sdmc:/lfcsdata_XXXXXXXX.bin with the XXXXXXXX being random hex. You need latest [Luma3DS](https://github.com/LumaTeam/Luma3DS/releases/latest) (10.2.1+) to run this successfully since it needs ps:ps, a highly privileged service.

This collected data from volunteers will be used to improve bruteforcing the lfcs much how msed_data was used to improve bruteforcing of the full movable.sed.
