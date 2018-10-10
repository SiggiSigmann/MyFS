# MyFS

### Features
- [x] Testrahmen
- [x] Auf Blockdevice schreiben
- [x] Vom Blockdevice lesen
- [x] Superblock definieren
- [x] Superblock schreiben
- [x] Superblock lesen
- [x] Superblock in Klasse extrahieren

### Ongoing
Julian:
- [ ] beliebige Structs schreiben
- [ ] beliebige Structs lesen

Tobi:

Henning:
- [x] DMAP definieren
- [ ] DMAP schreiben
- [ ] DMAP lesen

### Todo
- [ ] Inodes definieren
- [ ] Inodes schreiben
- [ ] Inodes lesen
- [ ] Inodes in Klasse extrahieren
- [ ] Rootblock anlegen
- [ ] FAT definieren
- [ ] FAT schreiben
- [ ] FAT lesen
- [ ] IMAP definieren
- [ ] IMAP schreiben
- [ ] IMAP lesen
- [ ] (leere) Dateien anlegen
- [ ] Dateien umbenennen
- [ ] Metadaten von Dateien verändern
- [ ] Dateien mit Daten befüllen
- [ ] Dateien überschreiben
- [ ] Dateien löschen
- [ ] Dateien kopieren


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
