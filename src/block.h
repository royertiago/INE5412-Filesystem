/* 
 * File:   diskBlock.h
 * Author: cancian
 *
 * Created on 22 November 2013, 15:09
 */

#ifndef DISK_BLOCK_H
#define	DISK_BLOCK_H

#include <cstddef> // size_t

/**
 * A raw data block, consisting of a simple array of @type{unsigned char}. 
 * A block disk resides in RAM and has to be flushed to a physical disk
 * or other non-volatile media.
 *
 * The only class that should create disk blocks is the storage itself.
 *
 * @param data      The data to be hold by this block. This object will own the
 *                  pointer and delete[] it afterwards.
 * @param blockSize Size of the data held by this block, in bytes.
 * @param source    Physical block number from which the data of this block
 *                  comes from or is to be written back.
 * @param interval  Time interval, in miliseconds, in which the block should
 *                  be rewritten from the memory to the disk.
 *
 * TODO: flushInterval machinery still not implemented
 *
 */
class Block {
public:
    /** Number of the block in the disk.
     * The number is related to the physical localization of the block
     * insite the disk. */
    typedef unsigned int number;

    /** Time interval, in miliseconds, in which the block should be
     * rewritten from the memory to the disk.
     *
     * A flushInterval of zero means "as soon as possible". For
     * example, a file table block will have flush interval of zero
     * due to the importance of the file table to the consistence
     * of the file system.
     * On the other side, an user file block may have a flush interval
     * of a few hundred miliseconds - there are more important blocks
     * to be assured consistency.
     */
    typedef unsigned int flushInterval;

    /** Delete[] the rawData pointer. */
    ~Block();

    /** Read data from the block and write in @param buffer.
     *
     * Returns the number of bytes written in the buffer.
     * The buffer is assumed being able to hold at least @param length bytes.
     * No checking will be done to guarantee that first + lenght < blockSize.
     *
     * @param first  Frist byte to be readed from this block.
     * @param length Number of bytes to be readed from this block.
     * @param buffer Buffer in which will be written the readed bytes.
     * @return       The number of readed bytes. */
    size_t readBytes(size_t first, size_t length,
            unsigned char * buffer) const;

    /** Write data to the block from @param buffer.
     *
     * Returns the number of bytes written to the block.
     * No checking will be done to guarantee that first + lenght < blockSize.
     *
     * @param first  First byte to be written in this block.
     * @param length Number of bytes to from the buffer be written in this block.
     * @param buffer Source of the bytes being written.
     * @return       The number of written bytes. */
    size_t writeBytes(size_t first, size_t length, 
            const unsigned char * buffer);

    /** Read the specified byte.
     *
     * No checking will be done to guarantee that byte < blockSize.
     * 
     * @param index Index of the byte to be read.
     * @return      Readed byte.
     */
    unsigned char readByte(size_t index);

    /** Write to the specified byte.
     *
     * No checking will be done to guarantee that byte < blockSize.
     *
     * @param index Index of byte to be written.
     * @param byte  Value of byte to be written.
     */
    void writeByte(size_t index, unsigned char byte);

    /** Physical block from which the data of this object was extracted. */
    const number physicalSource;

    /** Amount of memory held by rawData. */
    const size_t size;

private:
    /** The only class that is able to construct and manipulate blocks
     * is the Storage. Using Storage protected static class members,
     * derived classes can also manipulate blocks. */
    friend class Storage;

    Block(unsigned char * data, size_t size, Block::number source,
            Block::flushInterval interval);

    /** Data being hold by this structure.
     * This object owns this pointer. */
    unsigned char* rawData;

    /** Even though is an entire byte, this is set true whenever any byte
     * of the block was been touched.
     * Can be used to optimize block writes. */
    bool dirtyBit;

    /** Time interval, in miliseconds, in which the block should be
     * rewritten from the memory to the disk.
     */
    flushInterval interval;
};

#endif	/* DISKBLOCK_H */
