# Get the directory of this patch script
# PATCH_COMMAND runs with the source directory as working directory
file(READ "src/lib/libdwarf/CMakeLists.txt" content)
message("PATCHING LIBDWARF")
string(REPLACE
        "  TARGETS dwarf"
        "  TARGETS dwarf zlib"
        patched_content
        "${content}"
)

file(WRITE "src/lib/libdwarf/CMakeLists.txt" "${patched_content}")