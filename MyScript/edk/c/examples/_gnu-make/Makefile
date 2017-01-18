#
# edk/c/examples/_gnu-make/Makefile
# Copyright (c) MyScript
#

.PHONY: all clean

# extensions: eg, use make extensions="text" to build MyScript Text examples only
extensions:=engine text inksearch shape math analyzer prediction document writeradaptation ink music
override extensions:=$(sort $(extensions))

# if not specified form command line, build both debug and release configurations
configurations:=debug release
override configurations:=$(sort $(configurations))

# compiler and linker flags
debug-cflags:=-g3 -O0
release-cflags:=-g -O2
debug-ldflags:=-ldl -lm
release-ldflags:=-ldl -lm

# validate configurations
define toLowercase
$(shell echo $(1) | tr '[:upper:]' '[:lower:]')
endef

define equals
$(if $(subst $(1),,$(2))$(subst $(2),,$(1)),,1)
endef

define validate_configuration
$(if $(or $(call equals,$(1),debug), $(call equals,$(1),release)),,$(error invalid configuration, must be "debug" or "release" but was "$(2)"))
endef

$(foreach configuration,$(configurations),$(call validate_configuration,$(call toLowercase,$(configuration)),$(configuration)))

# directories
mydir:=$(realpath $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST)))))
edkdir:=$(realpath $(mydir)/../../..)
edk_cdir:=$(edkdir)/c
edk_c_examplesdir:=$(edk_cdir)/examples
srcdir:=$(edk_c_examplesdir)
bindir:=$(edk_c_examplesdir)/bin

debug-dirsuffix:=-Debug
release-dirsuffix:=

# platform detection
uname_s:=$(shell sh -c 'uname -s 2>/dev/null || echo unknown')
uname_m:=$(shell sh -c 'uname -m 2>/dev/null || echo unknown')

ifeq ($(uname_s),Linux)
  platform:=linux
  platformdir:=lin
endif
ifeq ($(uname_s),Darwin)
  platform:=mac
endif

define pathsearch
$(firstword $(wildcard $(addsuffix /$(1),$(subst :, ,$(PATH)))))
endef

ifeq ($(platform),linux)
  CC-linux:=$(CC)
  CC-linux-x86:=$(CC-linux) -m32
  CC-linux-x64:=$(CC-linux) -m64

  # detect multilib support
  arch:=$(shell echo $(uname_m) | sed 's/.*86/x86/')
  ifeq ($(arch),x86)
    archs:=x86
    ifeq ($(shell $(CC) --print-multi-lib | sed -rn 's/.+;@m64/m64/p'),m64)
      archs+=x64
    endif
  endif
  ifeq ($(arch),x86_64)
    override arch:=x64
    archs:=x64
    ifeq ($(shell $(CC) --print-multi-lib | sed -rn 's/.+;@m32/m32/p'),m32)
      archs+=x86
    endif
  endif
endif
ifeq ($(platform),mac)
  CC-mac:=$(CC)
  CC-mac-x86+x64:=$(CC-mac) -arch i386 -arch x86_64
  CC-mac-x86:=$(CC-mac) -arch i386
  CC-mac-x64:=$(CC-mac) -arch x86_64
  arch:=x86+x64
  archs:=$(arch)
endif

platform?=$(call toLowercase,$(uname_s))
platformdir?=$(platform)
archs?=$(uname_m)

# targets
examples:=$(foreach extension,$(extensions),$(subst $(srcdir)/,,$(wildcard $(srcdir)/$(extension)/*.c)))
targets:=$(foreach arch,$(archs),$(foreach configuration,$(configurations),$(addprefix $(bindir)/$(platformdir)-$(arch)$($(configuration)-dirsuffix)/,$(subst /,_,$(examples:%.c=%)))))
all: $(targets)

MyCertificate.c:=$(srcdir)/certificates/MyCertificate.c
MyCertificate.h:=$(srcdir)/certificates/MyCertificate.h

#compilation rules
cflags:=-I$(edk_cdir)/include -DVO_MSE_FUNC_THROUGH_POINTERS -Werror=implicit-function-declaration

common:=$(srcdir)/common/common.c $(srcdir)/common/common.h $(srcdir)/common/config.h $(srcdir)/common/parseInk.c $(srcdir)/common/parseInk.h
edk_c_src:=$(filter-out %MyScript-whereami.c,$(filter-out %MyScriptEngine-dlHelper.c,$(wildcard $(edk_cdir)/src/*.c)))

define build
CC-$(platform)-$(2)?=$(CC-$(platform))

$(bindir)/$(platformdir)-$(2)$($(3)-dirsuffix)/$(1)_%: $(srcdir)/$(1)/%.c $(MyCertificate.c) $(edk_c_src) $(common)
	mkdir -p $$(@D)
	$$(CC-$(platform)-$(2)) $(cflags) $($(3)-cflags) $(CFLAGS) -o $$@ $$(filter-out %.h,$$^) $($(3)-ldflags) $(LDFLAGS)
endef

$(foreach extension,$(extensions),$(foreach arch,$(archs),$(foreach configuration,$(configurations),$(eval $(call build,$(extension),$(arch),$(configuration))))))

# cleanup
clean:
	rm -f $(targets)
