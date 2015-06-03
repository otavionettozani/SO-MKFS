//
//  main.c
//  mkfs
//
//  Created by Otávio Netto Zani on 25/05/15.
//  Copyright (c) 2015 Otávio Netto Zani. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "inode.h"
#include <time.h>


#define FILE_SIZE (24*1024*1024)


FILE* createFile(char* name){
	
	FILE* file;
	file = fopen(name, "w");
	byte zero[1024] = {0};
	
	word i;
	for (i=0; i<FILE_SIZE/1024; i++) {
		fwrite(zero, sizeof(byte), sizeof(zero), file);
	}
	
	return file;
	
}


void createInodes(inode* inodes, word size){
	
	int i=0;
	
	for (i=0; i<size; i++){
		inodes[i].id = i;
	}
	
	setBit(&inodes[0].metadata.flags, FlagIsDir);
	setBit(&inodes[0].metadata.flags, FlagPmExec);
	setBit(&inodes[0].metadata.flags, FlagPmRead);
	setBit(&inodes[0].metadata.flags, FlagPmWrite);
	inodes[0].metadata.time = (word)time(NULL);
	
	return;
}

int main(int argc, char * argv[]) {
	
	halfWord blockSize;
	char* fileName;
	char* sizeNumbers;
	char* sizeType;
	
	//---------------- Processing arguments-------------------//
	if(argc != 4){
		printf("Error: block size was not defined");
		return 0;
	}
	
	//process arguments
	//first argument is a -b
	if(!strcmp("-b", argv[1])){
		int j =0, jmax=0;
		while (argv[2][j]>='0' && argv[2][j]<='9') {
			j++;
		}
		sizeNumbers = (char*) malloc((j+1)*sizeof(char));
		jmax = j;
		
		if(jmax == 0){
			printf("Error: block size must be a integer between 512B and 4KB");
			return 0;
		}
		
		for (; j>=0; j--) {
			sizeNumbers[j] = argv[2][j];
		}
		sizeNumbers[jmax] = 0;
		sizeType = &argv[2][jmax];
		fileName = argv[3];
		
		if(!strcmp("B", sizeType)){
			//is a byte block
			blockSize = atoi(sizeNumbers);
			if(blockSize<512 || blockSize>4*1024){
				printf("Error: block size must be a integer between 512B and 4KB");
				return 0;
			}
		}else if(!strcmp("KB", sizeType)){
			//is a KiloByte block
			blockSize = atoi(sizeNumbers);
			if(blockSize>4){
				printf("Error: block size must be a integer between 512B and 4KB");
				return 0;
			}
			blockSize*=1024;
		}else{
			printf("Error: block size must be a integer between 512B and 4KB");
			return 0;
		}
		
	} else
		//second argument is a -b
	if(!strcmp("-b", argv[2])){
		int j =0, jmax=0;
		while (argv[3][j]>='0' && argv[3][j]<='9') {
			j++;
		}
		sizeNumbers = (char*) malloc((j+1)*sizeof(char));
		jmax = j;
		
		if(jmax == 0){
			printf("Error: block size must be a integer between 512B and 4KB");
			return 0;
		}
		
		for (; j>=0; j--) {
			sizeNumbers[j] = argv[3][j];
		}
		sizeNumbers[jmax] = 0;
		sizeType = &argv[3][jmax];
		fileName = argv[1];
		
		if(!strcmp("B", sizeType)){
			//is a byte block
			blockSize = atoi(sizeNumbers);
			if(blockSize<512 || blockSize>4*1024){
				printf("Error: block size must be a integer between 512B and 4KB");
				return 0;
			}
		}else if(!strcmp("KB", sizeType)){
			//is a KiloByte block
			blockSize = atoi(sizeNumbers);
			if(blockSize>4){
				printf("Error: block size must be a integer between 512B and 4KB");
				return 0;
			}
			blockSize*=1024;
		}else{
			printf("Error: block size must be a integer between 512B and 4KB");
			return 0;
		}
		
	}else
	//argument -b does not make sense or does not exists
	{
		printf("Error: Couldn't process arguments");
		return 0;
	}
	
	
	//------------------- Creating the file with its size filled with 0s---------------------//
	
	FILE* file = createFile(fileName);
	
	
	
	//------------------- Create superBlock -----------------------//
	
	superBlock sBlock;
	
	sBlock.inodesDirectories = 1;
	sBlock.inodesFiles = 0;
	sBlock.usedBlocks = 0;
	sBlock.magicNumber = blockSize;
	sBlock.rootInode = 0; //all of the offsets will be calculated via the Sections enum at inode.h
	
	//------------------- Create the inodes bitmap ---------------------//
	
	byte inodeBitmap[InodeBitmapSize] = {0};
	//first inode is the root inode, so, it is used by default
	setBit(&inodeBitmap[0], 0x1);
	
	//------------------- Create the blocks bitmap --------------------//
	//All of the blocks are free at the start of the ufs, so, it will be a region of zeros;
	
	
	//------------------- Create the inodes region --------------------//
	inode inodes[1024] = {0};
	
	createInodes(inodes, 1024);
	
	
	//------------------- write everything to the file ----------------//
	
	
	//put cursor at start of file
	fseek(file, 0, SEEK_SET);
	//copy superblock
	fwrite(&sBlock, sizeof(superBlock), 1, file);
	//copy inode bitmap
	fwrite(inodeBitmap, sizeof(inodeBitmap), 1, file);
	//jump to the inodes section
	fseek(file, SectionInodes, SEEK_SET);
	//copy inodes
	fwrite(inodes, sizeof(inode), 1024, file);
	
	fclose(file);
	
	
	halfWord maxNumberOfBlocks = (FILE_SIZE-SectionDataBlocks)/blockSize;
	halfWord incompleteBlockSize = (FILE_SIZE - SectionDataBlocks - blockSize*maxNumberOfBlocks);

	printf("The File System was created succefully!\nOverhead:%g%%\n",(100.*SectionDataBlocks/FILE_SIZE));
	printf("Unused Space (last incomplete block): %d B\n",incompleteBlockSize);
	printf("System supports %g%% of data\n",100.-(100.*(SectionDataBlocks+incompleteBlockSize)/FILE_SIZE));
	
}
