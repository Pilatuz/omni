# please use one of the following commands:
# make PLATFORM=linux
# make PLATFORM=OSX
# make                    (default)
#
# also the linking option available:
# make LINKING=shared
# make LINKING=static     (default)
#
# build variant as:
# make VARIANT=debug
# make VARIANT=release    (default)
#
# platform as:
# make PLATFORM={OSX|linux}

# detect platform
ifdef PLATFORM
  ifeq '${PLATFORM}' 'OSX'
    platform:=OSX
  else ifeq '${PLATFORM}' 'linux'
    platform:=linux
  else
    $(error '${PLATFORM}' is unknown platform, expected: OSX or linux)
  endif
else
  # platform auto detection
  uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')
  ifeq '${uname_S}' 'Darwin'
    platform:=OSX
  else ifeq '${uname_S}' 'Linux'
    platform:=linux
  else
    $(error '${uname_S}' is unknown platform)
  endif
endif


# build variant
variant:=release
ifdef VARIANT
  ifeq '${VARIANT}' 'release'
    variant:=release
  else ifeq '${VARIANT}' 'debug'
    variant:=debug
  else
    $(error '${VARIANT}' is unknown variant, expected: release or debug)
  endif
endif


# lib linking
linking:=static
ifdef LINKING
  ifeq '${LINKING}' 'shared'
    linking:=shared
  else ifeq '${LINKING}' 'static'
    linking:=static
  else
    $(error '${LINKING}' is unknown linking, expected: shared or static)
  endif
endif


home_path:=.
include_dirs:=-I${home_path}/src

defines:=
shared_libs:=

ifeq '${variant}' 'debug'
  defines+=-D_DEBUG -g
else # default
  defines+=-DNDEBUG
endif


CXXFLAGS:=-Wall -O2 -Os ${include_dirs} ${defines}
CXXFLAGS+=-fdata-sections -ffunction-sections
LDFLAGS:=-L${env_path}/lib -Wl,--gc-sections

test_out:=test.exe


# omni source files
omni_src+=${home_path}/src/omni/calc.cpp
omni_src+=${home_path}/src/omni/conf.cpp
omni_src+=${home_path}/src/omni/util.cpp


# PCH header files
PCH_sources+=${home_path}/src/omni/defs.hpp
PCH_objects:=${PCH_sources:.hpp=.hpp.gch}


# expands to list of object files
test_sources+=${home_path}/src/test/test.cpp
test_sources+=${omni_src} ${home_path}/main_test.cpp
test_objects:=${test_sources:.cpp=.o}

all: ${test_out}
	@strip -x ${test_out}

${test_out}: ${test_objects}
	@ echo " [LN] $@"
	@${CXX} -o $@ ${test_objects} ${LDFLAGS} ${shared_libs}

PCH: ${PCH_objects}

# rule to build gch files
%.hpp.gch: %.hpp
	@echo "[CXX] $<"
	@$(CXX) ${CXXFLAGS} -c $< -o $@
%.h.gch: %.h
	@echo " [CC] $<"
	@$(CC) ${CXXFLAGS} -c $< -o $@

# rules to build c/cpp files
%.o: %.cpp
	@echo "[CXX] $<"
	@${CXX} ${CXXFLAGS} -c $< -o $@
%.o: %.c
	@echo " [CC] $<"
	@${CC} ${CXXFLAGS} -c $< -o $@


clean:
	@rm -f ${test_objects} ${test_out}
	@rm -f ${PCH_objects}

version:
	@echo "${variant} ${platform} ${linking}"
	@echo ${test_objects}

.PHONY: clean PCH version
