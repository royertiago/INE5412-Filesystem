/* 
 * File:   resourceAllocator.h
 * Author: Tiago Royer
 */

#ifndef RESOURCE_ALLOCATOR_H
#define RESOURCE_ALLOCATOR_H

/**
 * Interface that is able to do some form of resource allocation.
 *
 * Ideally, the resource allocator does not know what it is allocating,
 * but only some kind of key. For instance, in memory allocation,
 * the ResourceAllocator would know only the number of the block in
 * memory. The job of returning the pointer is of another class.
 */
template <typename Resource>
class ResourceAllocator {
public:
    ResourceAllocator() = default;
    virtual ~ResourceAllocator() = default;

    /** 
     * Returns the number of free resources in the allocator.
     * For instance, a disk space allocator would return the number
     * of free blocks in the disk.
     *
     * @return The number of free resources in the allocator.
     */
    virtual unsigned int getFreeResourcesCount() = 0;

    /**
     * Select a free resource to be allocated.
     * The resource shall be marked as occupied.
     * If there is no free resource, Resource(0) is returned.
     *
     * @return The index of a free block. 
     */
    virtual Resource allocate() = 0;

    /**
     * Mark the refered resource as free.
     * @param r Resource to be marked as free.
     */
    virtual void deallocate(Resource r) = 0;
};

#endif // RESOURCE_ALLOCATOR_H
