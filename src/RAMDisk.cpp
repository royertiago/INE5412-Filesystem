#include <unordered_map>
#include <cstddef>
using std::unordered_map;

#include "block.h"
#include "RAMDisk.h"

RAMDisk::RAMDisk(Storage * disk) :
    disk(disk)
{}

void RAMDisk::writeBlock(Block * block) {
    disk->writeBlock(block);
}

Block * RAMDisk::readBlock(Block::number n, Block::flushInterval i) {
    auto pair = blocks.find(n);
    if( pair != blocks.end() )
        return pair->second;
    // The block is not buffered. Let's read it from disk:
    Block * block = disk->readBlock(n, i);
    blocks[n] = block;
    return block;
}

void RAMDisk::format() {
    for(auto it : blocks)
        delete it.second;
    blocks.clear();
    disk->format();
}

void RAMDisk::format(Block::number blockAmount) {
    for(auto it : blocks)
        delete it.second;
    blocks.clear();
    disk->format(blockAmount);
}

size_t RAMDisk::getBlockSize() const {
    return disk->getBlockSize();
}

Block::number RAMDisk::getBlockAmount() const {
    return disk->getBlockAmount();
}

Block * RAMDisk::createBlock(Block::number n, Block::flushInterval i) {
    size_t size = disk->getBlockSize();
    unsigned char * data = new unsigned char[size];
    Block * newBlock = Storage::constructBlock( data, size, n, i );
    blocks[n] = newBlock;
    return newBlock;
}

void RAMDisk::releaseBlock(Block * b) {
    blocks.erase(b->physicalSource);
    delete b;
}
