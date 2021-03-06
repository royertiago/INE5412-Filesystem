/* 
 * File:   operatingSystem1_FileSystemDevelopmentClassWork.h
 * Author: cancian
 *
 * Created on 23 November 2013, 12:04
 */

#ifndef INE5412_CLASSWORK_H
#define	INE5412_CLASSWORK_H

#include "fileSystem.h"


/**
 * The main class of this homework. It just creates the file system and then uses it. 
 * Thats where main code for tests should be placed in. 
 * One should format the disk, create some directories, navigate on them, create, write and close some files, reading existing
 * files and trying to read non existing ones, deleting files and stuff like that. Be creative.
 */
class INE5412_ClassWork {
public:
    INE5412_ClassWork();
public:
    void useFileSystem();
private:
    void createFileSystem();
private:
    FileSystem fs;
};

#endif	/* INE5412_CLASSWORK_H */

