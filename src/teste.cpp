/** Pra quê teste de unidade se temos printf? */

#include <cstdio>
#include <cstring>
#include "block.h"
#include "storage.h"
#include "simulatedDisk.h"
#include "bitmap.h"
#include "RAMDisk.h"
#include "diskManager.h"

using namespace std; //Já que é o main mesmo...

//Esta função assume que Storage::constructBlock é publico.
/*void testarDriverEBlocos() {
    Block::number blockAmount = 2048;
    size_t blockSize = 1024;
    SimulatedDisk disco("meuDiscoFofinho.txt", blockSize*2, blockAmount/2);
    disco.format();
    disco.format(blockAmount);

    unsigned char * data = new unsigned char[blockSize];
    strcpy( (char*) data, "abc" );
    Block * b = Storage::constructBlock( data, blockSize, 1, 0 );

    disco.writeBlock(b);

    delete b;

    Block * d = disco.readBlock(1, 2);    

    printf("Bytes = %c%c%c\n", data[0], data[1], data[2]); //abc
    d->readBytes(1, 2, data);
    printf("Bytes = %c%c%c\n", data[0], data[1], data[2]); //bcc
    d->writeBytes(2, 2, data); //ab|bc
    d->readBytes(0, 4, data);
    printf("Bytes = %c%c%c%c\n", data[0], data[1], data[2], data[3]); //abbc
    disco.writeBlock(d); 
    delete d;
}*/

/* Testar as funções do bitmap.cpp */
unsigned int popcount(unsigned char c);
bool extractBit(unsigned char c, unsigned int bit);
void setBit(unsigned char & c, unsigned int bit);
void unsetBit(unsigned char & c, unsigned int bit);
unsigned char extractFirstZeroBit(unsigned char c);
unsigned int binaryLog(unsigned char c);
extern const unsigned char fullByte;

/*void testarBruxariasBits() {
    unsigned char x = 0x0F;
    setBit(x, 4);
    setBit(x, 5);
    setBit(x, 6);
    setBit(x, 7);
    if( x == fullByte )
        printf( "x is a full byte.\n" );
    else
        printf( "Problem - x = %#X\n", (int) x );
    unsetBit(x, 0);
    unsetBit(x, 1);
    unsetBit(x, 2);
    unsetBit(x, 3);
    printf( "Now x is %#X\n", x ); //0xF
    printf( "Bit 2 = %#X - Bit 6 = %#X\n", (int)extractBit(x,2),
            (int)extractBit(x, 6) ); // 0 / 0x1
    printf( "popcount = %i\n", (int)popcount( x ) );
    printf( "first zero bit = %#X\n", (int) extractFirstZeroBit( x ) ); //0x1
    printf( "first zero bit = %#X\n", (int) extractFirstZeroBit(~x ) ); //0x10
    printf( "binaryLog = %i, binaryLog(zeroBit(~x)) = %i\n", //7 / 4
            (int) binaryLog( x ), (int) binaryLog(extractFirstZeroBit(~x)));
}*/

//Assume que Storage::constructBlock() é público
/*void testarAlocador() {
    Block::number blockAmount = 32;
    size_t blockSize = 2;
    unsigned char * data1 = new unsigned char[blockSize]();
    unsigned char * data2 = new unsigned char[blockSize]();
    setBit(data1[0], 0);
    data1[1] = fullByte; //Ver se o alocador passa por cima do byte 1
    Block * block1 = Storage::constructBlock(data1, blockSize, 0, 0);
    Block * block2 = Storage::constructBlock(data2, blockSize, 0, 0);
    Block * blocks[2] = {block1, block2};
    Bitmap map( blocks, blockAmount );

    Block::number b1 = map.allocate();
    Block::number b2 = map.allocate();
    map.allocate();
    map.allocate();
    Block::number b3 = map.allocate();
    map.allocate();
    Block::number b4 = map.allocate();
    printf( "Allocated blocks: %i %i %i %i\n", b1, b2, b3, b4 ); //1 2 5 7
    map.deallocate(b4);
    map.deallocate(b3);
    map.allocate();
    b4 = map.allocate();
    b3 = map.allocate();
    printf( "Allocated blocks: %i %i %i %i\n", b1, b2, b3, b4 ); //1 2 7 16

    for( unsigned i = 17; i < blockAmount - 2; i++ )
        printf( "Just allocated %i\n", map.allocate() );

    b1 = map.allocate();
    b2 = map.allocate();
    b3 = map.allocate();
    printf( "Allocated blocks (block amount = %i): %i %i %i\n", blockAmount,
            b1, b2, b3 ); //blockAmount - 2, blockAmount - 1, 0.
    map.deallocate(23);
    b4 = map.allocate();
    printf( "Last block: %i\n", b4 ); //34
}*/

/*void testarDisco() {
    size_t blockSize = 8;
    Block::number blockAmount = 256; //Precisamos de 4 blocos para o bitmap

    unsigned char str1[] = "asdf";
    unsigned char str2[] = "qwer";
    unsigned char str3[] = "zxcv";
    unsigned char str4[] = "alce";
    unsigned char * clear = new unsigned char[blockSize]();

    SimulatedDisk disco("meuDiscoFofinho.txt", blockSize, blockAmount);

    RAMDisk ram( &disco );

    ram.format();

    Block * blocos[5];
    for( Block::number i = 0; i < 5; i++ ) {
        blocos[i] = ram.createBlock(i, 0);
        blocos[i]->writeBytes(0, blockSize, clear);
        //todos os blocos vem com lixo de memória.
    }

    blocos[1]->writeByte(0, 0x1F);

    Bitmap map( blocos + 1, blockAmount );
    printf( "%x %x %x %x\n", (int)blocos[1]->readByte(0), 
            (int)blocos[1]->readByte(1),
            (int)blocos[1]->readByte(2), 
            (int)blocos[1]->readByte(3) );

    DiskManager disk( &ram, &map );

    printf( "Ponteiros para o bloco zero: %p %p\n", (void*) blocos[0],
            (void*) disk.readBlock(0, 1) );

    Block * b1 = disk.allocateBlock( 1 );
    Block * b2 = disk.allocateBlock( 1 );
    Block * b3 = disk.allocateBlock( 1 );
    b1->writeBytes(0, blockSize, clear); 
    b2->writeBytes(0, blockSize, clear); 
    b3->writeBytes(0, blockSize, clear); 
    printf( "%x %x %x %x\n", (int)blocos[1]->readByte(0), 
            (int)blocos[1]->readByte(1),
            (int)blocos[1]->readByte(2), 
            (int)blocos[1]->readByte(3) );

    b1->writeBytes(0, 4, str1);
    b2->writeBytes(0, 4, str2);
    b3->writeBytes(0, 4, str3);
    b3->writeBytes(4, 4, str4);

    disk.writeBlock( b1 );
    disk.writeBlock( b3 );
    printf( "Posições = %i %i %i\n", b1->physicalSource, b2->physicalSource,
            b3->physicalSource ); //5, 6, 7
    printf( "Ponteiros = %p %p %p\n", (void*) b1, (void*)b2, (void*)b3 );

    disk.releaseBlock( b2 );
    b2 = disk.allocateBlock( 2 );
    printf( "Posição de b2 = %i - ponteiro = %p\n", b2->physicalSource,
            (void*) b2); //6

    printf( "%x %x %x %x\n", (int)blocos[1]->readByte(0), 
            (int)blocos[1]->readByte(1),
            (int)blocos[1]->readByte(2), 
            (int)blocos[1]->readByte(3) );
    disk.writeBlock( blocos[1] );
}*/

/* Esta função assume que meuDiscoFofinho.txt seja igual a 
 * meuDiscoFofinho.txt.bak - este arquivo possui os bits do bitmap
 * já configurados, ou seja, não é um disco vazio. */
void testarEncapsulador() {
    size_t blockSize = 8;
    Block::number blockAmount = 256; //Precisamos de 4 blocos para o bitmap
    SimulatedDisk disco("meuDiscoFofinho.txt", blockSize, blockAmount);
    RAMDisk ram( &disco );
    DiskManager* disk = DiskManager::encapsulate( &ram, 1 );

    unsigned char str1[] = "asdf";
    unsigned char str2[] = "qwer";
    unsigned char str3[] = "zxcv";
    unsigned char str4[] = "alce";
    unsigned char * clear = new unsigned char[blockSize]();

    Block * b1 = disk->allocateBlock( 1 );
    Block * b2 = disk->allocateBlock( 1 );
    Block * b3 = disk->allocateBlock( 1 );
    b1->writeBytes(0, blockSize, clear); 
    b2->writeBytes(0, blockSize, clear); 
    b3->writeBytes(0, blockSize, clear); //os blocos vem com lixo de memória

    b1->writeBytes(0, 4, str1);
    b2->writeBytes(0, 4, str2);
    b3->writeBytes(0, 4, str3);
    b3->writeBytes(4, 4, str4);

    disk->writeBlock( b1 );
    disk->writeBlock( b3 );

    printf( "Posições = %i %i %i\n", b1->physicalSource, b2->physicalSource,
            b3->physicalSource ); //5, 6, 7
    printf( "Ponteiros = %p %p %p\n", (void*) b1, (void*)b2, (void*)b3 );

    disk->releaseBlock( b2 );
    b2 = disk->allocateBlock( 2 );
    printf( "Posição de b2 = %i - ponteiro = %p\n", b2->physicalSource,
            (void*) b2); //6
}

/*int main() {
    testarEncapsulador();
    return 0;
}*/
