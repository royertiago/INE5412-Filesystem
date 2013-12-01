#include "bitmap.h"
#include "block.h"

/** Population count.
 * Returns the number of non-zero bits in the passed number.
 * @param c Character whose population is to be counted.
 * @return  The number of non-zero bits in the passed number.
 */
unsigned int popcount(unsigned char c) {
    unsigned int count = 0;
    while( c != 0 ) {
        count++;
        c &= c-1; //Trick from K&R "The C Programming Language".
    }
    return count;
}

/** Extracts the refered bit, treating the bit zero as the least
 * significant one.
 * @param c     Number whose bit is to be extracted.
 * @param bit   Number of the bit that is to be extracted.
 * @return      Zero if the refered bit is zero, pow(2,bit) otherwise.
 */
bool extractBit(unsigned char c, unsigned int bit) {
    return c & (1 << bit);
}

/** Sets the refered bit in the specified number to one.
 * @param c     Number whose bit is to be set.
 * @param bit   Number of bit that is to be set.
 */
void setBit(unsigned char & c, unsigned int bit) {
    c = c | (1 << bit);
}

/** Sets the refered bit in the specified number to zero.
 * @param c     Number whose bit is to be set.
 * @param bit   Number of bit that is to be set.
 */
void unsetBit(unsigned char & c, unsigned int bit) {
    c = c & ~(1 << bit);
}

/** Extracts the first zero bit of the specified byte.
 * If the position of the first zero bit is n, this function
 * returns pow(2, n).
 * @param c Number whose first zero bit is to be extracted.
 * @return  The first zero bit.
 */
unsigned char extractFirstZeroBit(unsigned char c) {
    return (~c) & (c+1);
}

/** Returns the integer part of the binary logarithm (logarithm
 * of base 2) of the passed number.
 * This function returns the exact binary log when c is a perfect
 * power of 2.
 * @param c number whose binary logarithm is to be computed.
 * @return  log_2(c).
 */
unsigned int binaryLog(unsigned char c) {
    unsigned int log = 0;
    while( (c >>= 1) != 0 )
        log++;
    return log;
}

/** Represents a byte whose all bits are set to 1. */
extern const unsigned char fullByte = 0xFF;



Bitmap::Bitmap(Block ** storageBlocks, Block::number blockAmount) :
    storageBlocks(storageBlocks),
    blockSize(storageBlocks[0]->size),
    storageCount(blockAmount/(8 * this->blockSize)),
    blockAmount(blockAmount)
{}

unsigned char Bitmap::readByte(size_t index) {
    size_t block = index/blockSize;
    size_t offset = index % blockSize;
    //printf( "Read %ld - block = %ld, offset = %ld\n", index, block, offset );
    return storageBlocks[block]->readByte(offset);
}

void Bitmap::writeByte(size_t index, unsigned char byte) {
    size_t block = index/blockSize;
    size_t offset = index % blockSize;
    //printf( "Write 0x%X to %ld - block = %ld, offset = %ld\n", 
    //        byte, index, block, offset );
    storageBlocks[block]->writeByte(offset, byte);
}

unsigned int Bitmap::getFreeResourcesCount() {
    unsigned int count = 0;
    for(Block::number i = 0; i < blockAmount/8; i++)
        count += popcount(readByte(i));
    return count;
    /* We had to divide by 8 due to a byte being capable
     * of hold information for eight blocks. */
}

Block::number Bitmap::allocate() {
    for(Block::number i = 0; i < blockAmount/8; i++)
        if( readByte(i) != fullByte) {
            unsigned char byte = readByte(i);
            //printf( "byte is 0x%X - ", (int)byte );
            // We'll allocate a block here.
            unsigned char bit = extractFirstZeroBit(byte);
            byte |= bit;
            //printf( "adding 0x%X becomes 0x%X - ", (int) bit, (int) byte );
            // Now, the first zero bit of byte was set to 1.
            writeByte(i, byte);
            //printf("Allocated at %i - i = %i, log = %i\n",
            //        8*i + binaryLog(bit), i, binaryLog(bit) );
            return 8*i + binaryLog(bit);
        }
    return Block::number(0);
}

void Bitmap::deallocate(Block::number n) {
    unsigned char byte = readByte(n/8);
    unsetBit(byte, n % 8);
    writeByte(n/8, byte);
    //printf( "Deallocate at byte %i, bit %i\n", n/8, n%8);
}
