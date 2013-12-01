/** RAM Disk - a disk that resides in ram.
 *
 * This class is a memory buffer to a storage class that is
 * contained in a non-volatile media.
 *
 * The disk is assumed to be "dumb"; that is, it does not do any
 * form of free space control neither guarantee that every block
 * in the disk is represented by a single Block object (it does not
 * own Block pointers).
 *
 * TODO: The only class that should call crateBlock and releaseBlock
 *       is the DiskManager.
 *
 * Warning: calling readBlock twice (or after a crateBlock) with different
 * Block::flushInterval values causes the RAMDisk ignores the second
 * value.
 *
 * Calling format (both versions) invalidate any block 
 * buffered in memory.
 */

#include <unordered_map>

#include "block.h"
#include "storage.h"

class RAMDisk : public Storage {
public:
    /** Creates a RAMDisk to cache the refered disk in memory.
     * Uses the specified allocator to select free blocks.
     *
     * @param disk Storage to be used as non-volatile media.
     */
    RAMDisk(Storage * disk);

    /* Overriden functions */
    virtual void writeBlock(Block *) override;
    virtual Block * readBlock(Block::number, Block::flushInterval) override;
    virtual void format() override;
    virtual void format(Block::number blockAmount) override;
    virtual size_t getBlockSize() const override;
    virtual Block::number getBlockAmount() const override;

    /**
     * Creates a new block an treat it as if it is a free block in the
     * disk.
     *
     * Warning: passing createBlock() a block number whose corresponding
     * disk position is not free may cause loss of data.
     *
     * @param n Phyisical block number associated with the created block.
     * @param i Flush interval of the block.
     * @return  A empty block.
     */
    virtual Block * createBlock(Block::number n, Block::flushInterval i);

    /**
     * Discard the refered block as a newly freed block.
     * The block can be deleted by the RAMDisk.
     *
     * Warning: passing releaseBlock() a block which is not free may
     * cause loss of data.
     *
     * @param b Block to be freed.
     */
    virtual void releaseBlock(Block *);
    
private:

    Storage * disk;

    std::unordered_map<Block::number, Block*> blocks;
};
