# External Libraries

* Located in include/texture/ddspp.hpp
 * A modified version of https://github.com/redorav/ddspp

# Adding New Extractor

1. Create a new folder in *include* and *src*
1. Create a new class that inherits from the Extractor class (in BaseExtractor.h) and fill out the abstract methods
1. Add the new file type(s) to both structs in FileType.hpp
1. Add the new class to the constructor in FileTypeMap.hpp