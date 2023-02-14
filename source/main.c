#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>

void getMac(u8 *m){
	memcpy(m, (u8*)0x1FF81060, 6);
}

int main(int argc, char* argv[])
{
	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);
	
	Result res=0;
	u8 lfcsdata[0x100]={0};
	u8 mac[6]={0};
	char filename[0x100]={0};
	u64 lfcs=0;
	u64 lfcs2=0;
	u8 lfcs_match=0xff;
	u32 device_id=0;
	u64 mask=0x3ffffffff;
	
	//srand(time(NULL));
	res = psInit();
	cfguInit();
	
	printf("lfcs_data v1.0\n\n");  
	
	if(res==0){
		
		getMac(mac);         //funfact: the mac address is also conveniently found in mii qr codes
		
		printf("MAC:        ");    
		for(int i=0;i<6;i++) printf("%02X ", (int)mac[i]);
		printf("\n");
		
		res = PS_GetLocalFriendCodeSeed(&lfcs);
		if(res) printf("getLFCS:    %08X fail\n", (int)res);
		printf("LFCS:       %010llX\n\n", lfcs);
		
		//these 3 printouts are just for academic purposes (9000X blocks are very same-ish and the reason they may differ is still a mystery)
		res = CFGU_GetConfigInfoBlk2(8, 0x90000, &lfcs2);
		if(res) printf("get90000:    %08X fail\n", (int)res);
		printf("90000:      %010llX\n", lfcs2);
		res = CFGU_GetConfigInfoBlk2(8, 0x90001, &lfcs2);
		if(res) printf("get90001:    %08X fail\n", (int)res);
		printf("90001:      %010llX\n", lfcs2);
		res = CFGU_GetConfigInfoBlk2(4, 0x90002, &lfcs2);
		if(res) printf("get90002:    %08X fail\n", (int)res);
		printf("90002:      %08llX\n\n", lfcs2 & 0xffffffff);
		
		//the real lfcs2 we use for data
		res = CFGU_GetConfigInfoBlk2(8, 0x90001, &lfcs2);
		if(res) printf("getLFCS2:    %08X fail\n", (int)res);
		printf("LFCS2:      %010llX\n", lfcs2);
		
		res = PS_GetDeviceId(&device_id);
		if(res) printf("getDevID:   %08X fail\n", (int)res);
		printf("DevID:      %08X\n", (int)device_id);
		
		lfcs_match = ((lfcs & mask) == (lfcs2 & mask)) ? 1:0;
		lfcs=(lfcs & 0xfffffffffffffff0)+0x8;    //truncate unneeded precision
		device_id=(device_id & 0xfffffff0)+0x8;  //""
		
		if(lfcs_match==1) printf("Movable:    Native\n");
		else if(lfcs_match==0) printf("Movable:    Guest\n");
		else printf("Movable:    ???\n");
		
		//printf("Time:       %llX\n", osGetTime());
		
		memcpy(lfcsdata, mac, 3);
		memset(lfcsdata+3, 0xff, 3);
		memcpy(lfcsdata+0x10, &lfcs, 8);
		memcpy(lfcsdata+0x20, &device_id, 4);
		memcpy(lfcsdata+0x30, &lfcs_match, 1);
		
		snprintf(filename, 128, "/lfcsdata_%llX.bin", osGetTime()); //osTime is just to make it easier to throw multiple files in a single input directory for processing
		printf("\ndumping sdmc:%s...\n", filename);
		
		FILE *f=fopen(filename,"wb");
		if(f && fwrite(lfcsdata, 1, 0x100, f)==0x100){
			fclose(f);
			printf("success!\n");
		}
		else{
			printf("filesystem error: lfcsdata dump failed\n");
		}
	
	}
	else{
		printf("psInit: %08X fail\nAre you running the latest Luma3DS? (v10.2.1+)\n", (int)res);
	}
	
	printf("\nPress START to exit\n");

	while (aptMainLoop())
	{
		gspWaitForVBlank();
		gfxSwapBuffers();
		hidScanInput();

		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu
	}

	gfxExit();
	return 0;
}