NAME=parallax

DBIN_DIR=bin\debug
RBIN_DIR=bin\release

DBUILD_DIR=build\debug
RBUILD_DIR=build\release

SRC_DIR=src

CFLAGS=/c /EHsc /std:c++20 /W4 /nologo /I "C:\Users\Alter\Documents\cpp-libraries\include"
CDFLAGS=/Od /Zi /Fd$(DBUILD_DIR)\ /Fo$(DBUILD_DIR)\ /MTd
CRFLAGS=/DNDEBUG /O2 /Fo$(RBUILD_DIR)\ /MT

S_LIBS=glfw3_mt.lib User32.lib Gdi32.lib Shell32.lib
LFLAGS=/NOLOGO /LIBPATH:"C:\Users\Alter\Documents\cpp-libraries\lib"
LDFLAGS=/DEBUG /ILK:$(DBUILD_DIR)\$(NAME).ilk /PDB:$(DBUILD_DIR)\ /OUT:$(DBIN_DIR)\$(NAME).exe
LRFLAGS=/OUT:$(RBIN_DIR)\$(NAME).exe

# List of .obj files that will need to be created
OBJ=main.obj

DOBJ=$(patsubst %.obj,$(DBUILD_DIR)\\%.obj,$(OBJ))
ROBJ=$(patsubst %.obj,$(RBUILD_DIR)\\%.obj,$(OBJ))

# Link .obj to debug executable
$(DBIN_DIR)\$(NAME).exe: $(DOBJ)
	IF NOT EXIST bin MD bin
	IF NOT EXIST bin\debug MD bin\debug

	LINK $(LFLAGS) $(LDFLAGS) $(S_LIBS) $**

# Compile source files to debug .obj files
{$(SRC_DIR)\}.cpp{$(DBUILD_DIR)\}.obj:
	IF NOT EXIST build MD build
	IF NOT EXIST build\debug MD build\debug
	
	CL $(CFLAGS) $(CDFLAGS) $<

# Clean all builds
clean:
	IF EXIST bin RD /s /q bin
	IF EXIST build RD /s /q build

# Build optimized executable
release: $(RBIN_DIR)\$(NAME).exe

# Link .obj to optimized executable
$(RBIN_DIR)\$(NAME).exe: $(ROBJ)
	IF NOT EXIST bin MD bin
	IF NOT EXIST bin\release MD bin\release

	LINK $(LFLAGS) $(LRFLAGS) $(S_LIBS) $**

# Compile source files to optimized .obj files
{$(SRC_DIR)\}.cpp{$(RBUILD_DIR)\}.obj:
	IF NOT EXIST build MD build
	IF NOT EXIST build\release MD build\release
	
	CL $(CFLAGS) $(CRFLAGS) $<