# Components of the File System

### Inode
- File name (char[NAME_LENGTH])
- File size (uint32_t)
- UserID (uint32_t)
- GroupID (uint32_t)
- mode (uint32_t)
- mtime (uint32_t)
- atime (uint32_t)
- ctime (uint32_t)
- indexFirstDataBlock (uint32_t)
- used(bool)
