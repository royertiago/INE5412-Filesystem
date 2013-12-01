#include "block.h"

Block::Block(unsigned char * data, size_t size, 
        Block::number source, Block::flushInterval interval)
:
    physicalSource(source),
    size(size),
    rawData(data),
    dirtyBit(false),
    interval(interval)
{}

Block::~Block() {
    delete [] rawData;
}

size_t Block::readBytes(size_t first, size_t length,
        unsigned char * buffer) const
{
    size_t b; // bytes lidos
    for(b = 0; b < length; b++)
        buffer[b] = rawData[first + b];

    return b;
}

size_t Block::writeBytes(size_t first, size_t length,
        const unsigned char * buffer)
{
    dirtyBit = true;
    size_t b; // bytes lidos
    for(b = 0; b < length; b++)
        rawData[b + first] = buffer[b];

    return b;
}

unsigned char Block::readByte(size_t num) {
    return rawData[num];
}

void Block::writeByte(size_t num, unsigned char byte) {
    dirtyBit = true;
    rawData[num] = byte;
}
