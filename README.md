# MikiArchiver
This is a simple file archiver (no compression) written in C++ for fun.

# Archive file information
* Extension: ``mka`` (stands for **M**i**k**i**A**rchive)
* Structure:
    * First ``short``: number of files being archived. Temporarily referred as ``n``.
    * Next ``short``: chunk size.
    * Within next ``n`` blocks:
        * First ``short``: size of the filename. Referred as ``m``.
        * Next ``m`` bytes: the filename.
        * Next ``size_t``: the filesize. Referred as ``l``.
        * Next ``l`` bytes: file content.