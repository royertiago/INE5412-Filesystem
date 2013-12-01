#include <cstdio>
#include <cstring>

#include "storage.h"
#include "block.h"
#include "simulatedDisk.h"

using std::strlen;
using std::strcpy;
using std::fopen;
using std::fseek;
using std::fclose;
using std::fread;

SimulatedDisk::SimulatedDisk(const char * file_name, size_t
        blockSize, Block::number blockAmount)
:
    fileName( new char[strlen(file_name) + 1] ),
    blockSize(blockSize),
    blockAmount(blockAmount),
    diskSize(blockSize * blockAmount)
{
    strcpy( fileName, file_name );
}

Block * SimulatedDisk::readBlock(Block::number n, Block::flushInterval i) {
    FILE * discoFofinho = fopen(fileName, "rb");
    fseek(discoFofinho, n * blockSize, SEEK_SET);

    unsigned char * data = new unsigned char[blockSize];

    fread(data, 1, blockSize, discoFofinho);
    fclose(discoFofinho);

    return Storage::constructBlock(data, blockSize, n, i);
}

void SimulatedDisk::writeBlock(Block* block) {
    FILE * discoFofinho = fopen(fileName, "rb+");
    Block::number position = block->physicalSource;
    fseek(discoFofinho, position * blockSize, SEEK_SET);

    unsigned char * data = Storage::extractData(block);
    fwrite(data, 1, blockSize, discoFofinho);
    fclose(discoFofinho);
}

void SimulatedDisk::format() {
    FILE * discoFofinho = fopen(fileName, "wb" );
    for( unsigned i = 0; i < diskSize; i++ )
        fputc(0, discoFofinho);

    fclose(discoFofinho);
}

void SimulatedDisk::format(Block::number newBlockAmount) {
    blockAmount = newBlockAmount;
    blockSize = diskSize / newBlockAmount;

    format();
}

size_t SimulatedDisk::getBlockSize() const {
    return blockSize;
}

Block::number SimulatedDisk::getBlockAmount() const {
    return blockAmount;
}
