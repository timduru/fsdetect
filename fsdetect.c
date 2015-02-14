#include <stdio.h>
#include <fcntl.h>

// see libblkid
#define F2FS_MAGIC "\x10\x20\xF5\xF2"
#define EXT4_MAGIC "\x53\xef"
#define EXT4_OFFSET 0x38

enum FSTYPES { UNKNOWN, EXT4, F2FS };
static const char* FSVALS[] = {"-", "ext4", "f2fs"}; 

int magic_match(char* devdata, char* magic, int offset)
{ return !memcmp(devdata+1024+offset, magic, strlen(magic)); } 

int getfstype(const char *path)
{
	char devdata[2048];
	memset(devdata, 0, sizeof(devdata));

	int devfs;
	devfs = open(path, O_RDONLY);
	if (devfs == -1) return UNKNOWN;

	int res = read(devfs, devdata, sizeof(devdata));
	close(devfs);
	if(!res) return UNKNOWN;

	if (magic_match(devdata, F2FS_MAGIC, 0)) return F2FS;
	if (magic_match(devdata, EXT4_MAGIC, EXT4_OFFSET)) return EXT4;
	return UNKNOWN;
}

int main(int argc, char *argv[])
{
	int fstype;

	if(argc == 1) 	fstype = getfstype("/dev/block/mmcblk0p8");
	else 		fstype = getfstype(argv[1]);

	if(argc == 3 && fstype == F2FS) rename("fstab.cardhu.f2fs","fstab.cardhu" ); // rename fstab if f2fs
	//TODO add other use cases for inverted storage...
	else printf(FSVALS[fstype]); // just print type
	return 0;
}
