#include "block.h"
#include "RAMDisk.h"
#include "resourceAllocator.h"
#include "bitmap.h"
#include "diskManager.h"

DiskManager::DiskManager(RAMDisk * disk, 
        ResourceAllocator<Block::number>* allocator)
:
    disk(disk),
    allocator(allocator)
{}

Block * DiskManager::readBlock(Block::number n, Block::flushInterval i) {
    return disk->readBlock(n, i);
}

void DiskManager::writeBlock(Block * b) {
    disk->writeBlock(b);
}

Block * DiskManager::allocateBlock(Block::flushInterval i) {
    Block::number n = allocator->allocate();
    return disk->createBlock(n, i);
}

void DiskManager::releaseBlock(Block * b) {
    allocator->deallocate(b->physicalSource);
    disk->releaseBlock(b);
}

size_t DiskManager::getBlockSize() const {
    return disk->getBlockSize();
}

Block::number DiskManager::getBlockAmount() const {
    return disk->getBlockAmount();
}

DiskManager * DiskManager::encapsulate( RAMDisk * disk, Block::number start ) {
    Block::number numberOfBlocks = 
            disk->getBlockAmount() / (8*disk->getBlockSize());
    Block ** blocks = new Block*[numberOfBlocks];

    for( Block::number i = 0; i < numberOfBlocks; i++ )
        blocks[i] = disk->readBlock( start + i, 0 );

    Bitmap * map = new Bitmap( blocks, disk->getBlockAmount() );

    return new DiskManager( disk, map );
}
