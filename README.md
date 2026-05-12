# WrplReplayParser
Warthunder .wrpl Replayer Parser written in cpp with python bindings

a feature filled wrpl replay parser that has python bindings (and will be publish on pypi soon)
this parser lets you access battle messages, chat messages, various global object data (like player information and team information), and entity construction information plus aircraft and tank positions (see FMSync and GMSync functions)
I have completed most of my inital goals, and will work to clean up the codebase as time goes on. this include reorganization / refactoring plus adding of probably sorely needed utility functions.

currently supported replay version: 0x18bfe10

This attempts to be buildable for:
Windows: using minGW, MSVC
Linux (WSL2): using gcc
# TODO
work on new blk internals. this one will be a mix of a more advanced version of the current one plus a 'static' version that is expressly for loading a binary datablock with speed. will be a static version. idea derived from gaijin

setup loading of char.vromfs.bin and proper translation code

overhaul directory structure

work on reducing certain memory usage, vromfs are rather expensive to load. maybe make a vromfs repackager?

ADD COMMENTS

ensure LICENCE is correctly setup
