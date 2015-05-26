//
//  inode.c
//  mkfs
//
//  Created by Otávio Netto Zani on 25/05/15.
//  Copyright (c) 2015 Otávio Netto Zani. All rights reserved.
//

#include "inode.h"

void setBit(byte* destination, byte bit){
	*destination = *destination | bit;
	return;
}

void resetBit(byte* destination, byte bit){
	*destination = *destination & (~bit);
	return;
}
