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

# Commandline arguments
* ``-e file_name``: extract from ``file_name``.
* ``-a file_list``: make archive from ``file_list``. Must specify ``-o``.
* ``-d directory``: make archive from ``directory``. Must specify ``-o``.
* ``-o file_name``: save archive as ``file_name``.
* ``--chunksize``: specify the chunk size.
* ``-o path`` (when combined with ``-e``): extract to ``path``.

