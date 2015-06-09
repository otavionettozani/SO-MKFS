//
//  inode.c
//  mkfs
//
//  Otavio Netto Zani - RA:103697
//  Flavio Matheus Muniz Ribeiro da Silva - RA:146098

#include "inode.h"

void setBit(byte* destination, byte bit){
	*destination = *destination | bit;
	return;
}

void resetBit(byte* destination, byte bit){
	*destination = *destination & (~bit);
	return;
}
