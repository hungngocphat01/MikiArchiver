# MikiArchiver
This is a simple file archiver (no compression) written in C++ in my free time just for fun (almost not practically useful).<br>
Named after my favourite character in iDOLM@STER Hoshii Miki.

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

# Examples
```bash
# Make an archive named myarchive.mka from file1.docx, file2.exe and file3.cpp
$ mk -a file1.docx file2.exe file3.cpp -o myarchive.mka
# Make an archive named myarchive2.mka from mydirectory
$ mk -d mydirectory p -o myarchive2.mka
# Extract myarchive.mka to myoutputdir
$ mk -e myarchive.mka -o myoutputdir
```

# Building
* Simply run ``make`` command in the repository directory.
* Works fine on Unix-like OSes. Tested on macOS Mojave.
