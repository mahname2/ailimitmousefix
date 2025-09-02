// The target pattern E82A851D00 corresponds to the instruction
//     call Utility$UpdateLerpFix
// in CameraManager$$CameraFollow()
//
// Utility$UpdateLerpFix multiplies the camera movement by 50*frametime, resulting in frametime 
// dependent mouse sensitivity.
// By replacing the pattern with 0F1F440000 (a 5-byte NOP instruction), this call is prevented
// In other words, after the fix the camera sensitivity will be equal to a constant 50 fps
//
// The game demo updated the camera only 50 times per second (rather than every frame), which
// means that camera sensitivity will be the same as in the demo (but updated every frame)

#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE* f;
	
	printf("Checking for AI-LIMIT.exe\n");
	f = fopen("./AI-LIMIT.exe", "rb");
	if (f == NULL) {
		printf("Error: Error: Couldn't find AI-LIMIT.exe. Is the patcher located in the wrong folder?\n");
		return 7;
	}
	fclose(f);
	
	
	printf("Opening GameAssembly.dll\n");
	f = fopen("./GameAssembly.dll", "rb");
	if (f == NULL) {
		printf("Error: Couldn't open GameAssembly.dll. Is the patcher located in the wrong folder?\n");
		return 1;
	}
	
	fseek(f, 0L, SEEK_END);
	long size = ftell(f);
	fseek(f, 0L, SEEK_SET);
	
	unsigned char *buffer = malloc(size);
	if (buffer == NULL) {
		printf("Error: failed to allocate file buffer.\n");
		return 5;
	}
	
	fread(buffer, size, 1, f);
	fclose(f);
	
	unsigned char target_pattern[5] = {0xE8, 0x2A, 0x85, 0x1D, 0x00};
	unsigned char replacement_pattern[5] = {0x0F, 0x1F, 0x44, 0x00, 0x00};
	
	printf("Searching for binary pattern...\n");
	long match_location = -1;
	for (long i=0; i<size-4; i++) {
		int match = 1;
		for (int j=0; j<5; j++) {
			if (buffer[i+j] != target_pattern[j]) {
				match = 0;
				break;
			}
		}
		if (match) {
			if (match_location != -1) {
				printf("Error: Found too many matching binary patterns. The mod may be incompatible with this game version.\n");
				return 3;
			}
			match_location = i;
		}
	}
	
	if (match_location == -1) {
		printf("Error: Couldn't find binary pattern. The mod may be incompatible with this game version, or the game may already be patched.\n");
		return 2;
	}
	
	printf("Creating backup of old GameAssembly.dll.\n");
	f = fopen("./GameAssembly.dll.old", "wb");
	if (f == NULL) {
		printf("Error: Couldn't open GameAssembly.dll.old.\n");
		return 6;
	}
	fwrite(buffer, size, 1, f);
	fclose(f);
	
	printf("Saving patched GameAssembly.dll.\n");
	for (long i=0; i<5; i++) {
		buffer[match_location+i] = replacement_pattern[i];
	}
	
	f = fopen("./GameAssembly.dll", "wb");
	if (f == NULL) {
		printf("Error: Failed to write patched GameAssembly.dll. Make sure the game is not running.\n");
		return 4;
	}
	fwrite(buffer, size, 1, f);
	fclose(f);
	
	free(buffer);
	
	printf("Game patched successfully.\n");
	
	return 0;
}