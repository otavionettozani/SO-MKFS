//
//  inode.h
//  mkfs
//
//  Created by Otávio Netto Zani on 25/05/15.
//  Copyright (c) 2015 Otávio Netto Zani. All rights reserved.
//

#ifndef __mkfs__inode__
#define __mkfs__inode__

#include <stdio.h>
#include <stdint.h>

#endif /* defined(__mkfs__inode__) */

//generic definitions
typedef uint8_t byte;
typedef uint32_t word;
typedef uint16_t halfWord;





//----------super block definitions ------------//

typedef struct SuperBlock{
	//we will use the magic number to save the block size selected by the user
	word magicNumber;
	halfWord rootInode;
	halfWord usedBlocks;
	halfWord inodesQuantity;
} superBlock;

//---------inode definitions-----------//

//location of flags in the metadata
typedef enum MetaFlags{
	FlagIsDir = 0x1<<0,
	FlagPmWrite = 0x1<<1,
	FlagPmRead = 0x1<<2,
	FlagPmExec = 0x1<<3
} metaFlags;


//the metadata itself
typedef struct Meta{
	byte name[256];
	byte flags;
	halfWord parent;
	word time;
} metaData;


//the inode itself
typedef struct Inode{
	halfWord id;
	metaData metadata;
	halfWord blocks[1024];
	
} inode;


//------------auxiliar functions---------------//

void setBit(byte* destination, byte bit);
void resetBit(byte* destination, byte bit);



//---------sections definitions---------//
#define InodeBitmapSize 128
#define DataBitmapSize 6144
typedef enum Sections{
	SectionSuperBlock = 0,
	SectionInodeBitmap = sizeof(superBlock),
	SectionDataBitmap = sizeof(superBlock)+InodeBitmapSize,
	SectionInodes = sizeof(superBlock)+InodeBitmapSize+DataBitmapSize,
	SectionDataBlocks =  sizeof(superBlock)+InodeBitmapSize+DataBitmapSize+1024*sizeof(inode)
} sections;



