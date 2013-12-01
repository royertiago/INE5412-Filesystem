#ifndef BLOCK_MANAGER_H
#define	BLOCK_MANAGER_H

#include "block.h"

/**
 * Class that represents any non-volatile media that is able
 * to do low(er) level software I/O operations.
 *
 * Objects of this class are rather dumb and only know how to
 * read and write blocks in themselves. For a "smart" version,
 * that do effective space allocation and block management,
 * see @class RAMDisk.
 *
 * Since the constructor and all the fields of the Block are
 * private, this class provides some static member functions
 * that can extract their data and create new blocks.
 */
class Storage {
public:

    virtual ~Storage() = default;

    /**
     * Writes a block @param block into RAM structures.
     * To guarantee that the block will be written in disk, use flush().
     *
     * @param block A pointer to the disk block.
     */
    virtual void writeBlock(Block* block) = 0;
    
    /**
     * Reads a block from the disk.
     *
     * The pointer should not be deleted.
     *
     * @param n The number of the block to be read
     * @param i Time interval, in miliseconds, in which the block should be
     *          rewritten from the memory to the disk.
     * @return   A pointer to the block.
     */
    virtual Block * readBlock(Block::number n, Block::flushInterval i) = 0;
    
    /**
     * Set every bit in the disk to zero.
     * The previous configurations of blockSize and blockAmount shall
     * not be changed.
     */
    virtual void format() = 0;

    /**
     * Set every bit in the disk to zero and change the physical block
     * arrangement.
     *
     * The number of blocks in the disk will be @param blockAmount.
     * The size of each block will be computed accordingly.
     *
     * @param blockAmount Number of blocks that the disk will have
     *                    after the method call.
     */
    virtual void format(Block::number blockAmount) = 0;

    /**
     * Returns the number of bytes in one disk block.
     * @return The number of bytes per block.
     */
    virtual size_t getBlockSize() const = 0;
    
    /**
     * Returns the number of blocks in the disk.
     * @return The number of blocks in the disk.
     */  
    virtual Block::number getBlockAmount() const = 0;

//protected:

    /** Constructs a block in the memory and give away the
     * pointer.
     * All the attributes will be simply forwarded to the constructor
     * of the block.
     */
    template <typename ... Parameters>
    static Block * constructBlock( Parameters ... );

    /** Returns a pointer to the raw data held by this block.
     * Using this pointer, the caller function can modify the block's
     * internal data.
     * @param b Block whose data is to be extracted
     * @return  The data of the block.
     */
    static unsigned char * extractData(Block* b);

    /** Returns a reference to the Block::dirtyBit class field.
     * Since it is a reference, the caller function can modify the
     * block's internal data.
     * @param b Block whose dirty bit is to be extracted.
     * @return  The dirty bit of the block.
     */
    static bool& extractDirtyBit(Block * b);

    /** Returns a reference to the Block::interval class field.
     * Since it is a reference, the caller function can modify the
     * block's internal data.
     * @param b Block whose flush interval is to be extracted.
     * @return  The flush interval of the block.
     */
    static Block::flushInterval& extractFlushInterval(Block * b);
};

template <typename ... Parameters>
inline Block * Storage::constructBlock( Parameters ... parameters ) {
    return new Block( parameters... );
}

inline unsigned char * Storage::extractData(Block* b) {
    return b->rawData;
}

inline bool& Storage::extractDirtyBit(Block * b) {
    return b->dirtyBit;
}

inline Block::flushInterval& Storage::extractFlushInterval(Block * b) {
    return b->interval;
}
#endif	/* DISK_H */
