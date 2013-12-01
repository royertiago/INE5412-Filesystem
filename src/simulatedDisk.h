/* 
 * File:   simulatedDisk.h
 * Author: cancian
 *
 * Created on 22 November 2013, 15:00
 */

#ifndef SIMULATED_DISK_H
#define	SIMULATED_DISK_H

#include "block.h"
#include "storage.h"

/**
 * Implementation of abstract class @class Storage that represents a
 * simulated disk.
 *
 * This implementation simulates a disk by using a simple file in the
 * real HD of this machine.
 *
 * @param filename Name of the file to be used to simulate the disk.
 */
class SimulatedDisk: public Storage {
public:
    SimulatedDisk(const char * fileName, size_t blockSize,
            Block::number blockAmount);

    virtual void writeBlock(Block *) override;
    virtual Block * readBlock(Block::number, Block::flushInterval) override;
    virtual void format() override;
    virtual void format(Block::number blockAmount) override;
    virtual size_t getBlockSize() const override;
    virtual Block::number getBlockAmount() const override;

private:
    /** Name of the file used to simulate the disk. */
    char * fileName;

    /** Size, in bytes, of a single block in the disk. */
    size_t blockSize;

    /** Number of blocks of the disk. */
    Block::number blockAmount;

    /** Size of the disk (blockSize * blockAmount).
     * This field is used to consistently reformat the disk with
     * variable block sizes. */
    unsigned long long int diskSize;
};

#endif	/* SIMULATED_DISK_H */
