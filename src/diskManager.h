/** Facade to the dynamic duo RAMDisk-bitmap.
 *
 * Correctly retransmits the calls between the objects.
 *
 * The diskManager is not a disk just because it cannot format
 * itself.
 *
 * @param disk      Disk that will be used to get/release blocks.
 * @param allocator Object that is able to allocate/dealocate blocks.
 */

#ifndef DISK_MANAGER_H
#define DISK_MANAGER_H

#include "block.h"
class RAMDisk;
template <typename Resource> class ResourceAllocator;

class DiskManager {
public:
    DiskManager(RAMDisk * disk, ResourceAllocator<Block::number> * allocator);

    Block * readBlock(Block::number, Block::flushInterval);
    void writeBlock(Block*);
    Block * allocateBlock(Block::flushInterval);
    void releaseBlock(Block*);
    size_t getBlockSize() const;
    Block::number getBlockAmount() const;

    /**
     * Encapsulates the passed RAMDisk in a diskManager, together with
     * some block allocator.
     * Currently, the choosen block allocator is the Bitmap.
     *
     * The bitmap will store its contents in blocks inside the disk.
     * The starting block is start; we'll assume that there is enough
     * blocks for the allocator.
     *
     * @param disk  Disk to be encapsulated.
     * @param start First of the blocks to be used by the allocator.
     * @returns     a disk manager.
     */
    static DiskManager * encapsulate(RAMDisk * disk, Block::number start);

private:
    RAMDisk * disk;
    ResourceAllocator<Block::number> * allocator;
};

#endif // DISK_MANAGER_H
