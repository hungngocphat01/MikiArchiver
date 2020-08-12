# MikiArchiver
This is a simple file archiver (no compression) written in C++ in my free time just for fun (almost not practically useful).<br>
Named after my favourite character in iDOLM@STER Hoshii Miki.

## Archive file information
* Extension: ``mka`` (stands for **M**i**k**i**A**rchive)
* Structure:
    * First ``8 bytes``: Signal to validate the file format ``MIKIMIKI``.
    * Next ``2 bytes``: number of files being archived = ``n``.
    * Next ``4 bytes``: chunk size.
    * Within next ``n`` blocks:
        * First ``2 bytes``: size of the filename = ``m``.
        * Next ``m`` bytes: the filename.
        * Next ``8 bytes``: the filesize = ``s``.
        * Next ``s`` bytes: file content.

## Commandline arguments
* ``-e file_name``: extract from ``file_name``.
* ``-a file_list``: make an archive from ``file_list``. Must specify ``-o``.
* ``-d directory``: make an archive from ``directory``. Must specify ``-o``.
* ``-o file_name``: save the archive as ``file_name`` (when used with ``-a`` and ``-d``).
* ``-o directory``: extract to ``directory`` (when used with ``-e``).
* ``--chunksize``: specify the chunk size.

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
* Recommended to use only on \*nix. Tested on macOS Mojave.
