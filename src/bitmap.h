/* 
 * File:   bitmap.h
 * Author: cancian
 *
 * Created on 23 November 2013, 11:20
 */

#ifndef BITMAP_H
#define	BITMAP_H

#include "block.h"
#include "resourceAllocator.h"

/**
 * Class to manage disk space. Set blocks as free or occupied.
 * In fact, the class manage block numbers. The external world
 * still needs a RAMDisk or similar to do the actual block allocation.
 *
 * Since the bitmap is a structure that is stored in the disk itself,
 * it needs some space in the disk to do the allocation - at least
 * blockAmount/(8*blockSize) blocks. Will be assumed that this is
 * the amount of items in the passed array.
 *
 * Since the bitmap is a persistent structure, the blocks passed
 * to store the bitmap will be treated as containing useful data -
 * that is, these are the same blocks used in previous 
 * initializations of the bitmap.
 *
 * And since the bitmap does not physically allocate nothing, it
 * does not need to know of what file the blocks refer to. In fact,
 * the storage blocks could be in anoter disk.
 *
 * The block size will be deduced from the first block of the array
 * and assumed to be the same for the entire array.
 *
 * The number of blocks is assumed to be a multiple of 8 for the functions
 * getFreeResourcesCount() and allocate() (avoid rounding errors).
 *
 * @param storageBlocs  blocks that will be used to store the bitmap.
 * @param blockAmount   number of blocks to be managed.
 */
class Bitmap: public ResourceAllocator<Block::number> {
public:
    Bitmap(Block ** storageBlocks, Block::number blockAmount );

    /* Overriden methods */
    virtual unsigned int getFreeResourcesCount() override;
    virtual Block::number allocate() override;
    virtual void deallocate(Block::number) override;
private:

    /** Treat all the storageBlocks as a single unsigned char array
     * and returns the byte with the specified index.
     *
     * No test will be done to assure that index is in range.
     *
     * @param index byte to be read.
     * @return      byte whose index is num.
     */
    unsigned char readByte(size_t index);

    /** Treat all the storageBlocks as a single unsigned char array
     * and stores the byte in the specified index.
     *
     * No test will be done to assure that index is in range.
     *
     * @param index position of byte to be stored.
     * @param byte  value to be stored.
     */
    void writeByte(size_t index, unsigned char byte);

    /** Blocks that will store permanently the bit map. */
    Block ** storageBlocks;

    /** Size of each block used to store the bit map. */
    size_t blockSize;

    /** Number of blocks that will be used to store the bitmap. */
    Block::number storageCount;

    /** Number of blocks that we need to control. */
    Block::number blockAmount;
};

#endif	/* BITMAP_H */

