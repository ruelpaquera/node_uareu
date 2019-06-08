# This file is generated by gyp; do not edit.

TOOLSET := target
TARGET := uareuprint
DEFS_Debug := \
	'-DNODE_GYP_MODULE_NAME=uareuprint' \
	'-DUSING_UV_SHARED=1' \
	'-DUSING_V8_SHARED=1' \
	'-DV8_DEPRECATION_WARNINGS=1' \
	'-DV8_DEPRECATION_WARNINGS' \
	'-DV8_IMMINENT_DEPRECATION_WARNINGS' \
	'-D_LARGEFILE_SOURCE' \
	'-D_FILE_OFFSET_BITS=64' \
	'-DOPENSSL_THREADS' \
	'-DBUILDING_NODE_EXTENSION' \
	'-DDEBUG' \
	'-D_DEBUG' \
	'-DV8_ENABLE_CHECKS'

# Flags passed to all source files.
CFLAGS_Debug := \
	-fPIC \
	-pthread \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-m64 \
	-g \
	-O0

# Flags passed to only C files.
CFLAGS_C_Debug :=

# Flags passed to only C++ files.
CFLAGS_CC_Debug := \
	-fno-rtti \
	-fno-exceptions \
	-std=gnu++1y

INCS_Debug := \
	-I/usr/lib/node_modules/node_uareu/.node-gyp/12.4.0/include/node \
	-I/usr/lib/node_modules/node_uareu/.node-gyp/12.4.0/src \
	-I/usr/lib/node_modules/node_uareu/.node-gyp/12.4.0/deps/openssl/config \
	-I/usr/lib/node_modules/node_uareu/.node-gyp/12.4.0/deps/openssl/openssl/include \
	-I/usr/lib/node_modules/node_uareu/.node-gyp/12.4.0/deps/uv/include \
	-I/usr/lib/node_modules/node_uareu/.node-gyp/12.4.0/deps/zlib \
	-I/usr/lib/node_modules/node_uareu/.node-gyp/12.4.0/deps/v8/include \
	-I$(srcdir)/node_modules/nan \
	-I$(srcdir)/. \
	-I/usr/lib \
	-I/opt/Crossmatch/urusdk-linux/Include

DEFS_Release := \
	'-DNODE_GYP_MODULE_NAME=uareuprint' \
	'-DUSING_UV_SHARED=1' \
	'-DUSING_V8_SHARED=1' \
	'-DV8_DEPRECATION_WARNINGS=1' \
	'-DV8_DEPRECATION_WARNINGS' \
	'-DV8_IMMINENT_DEPRECATION_WARNINGS' \
	'-D_LARGEFILE_SOURCE' \
	'-D_FILE_OFFSET_BITS=64' \
	'-DOPENSSL_THREADS' \
	'-DBUILDING_NODE_EXTENSION'

# Flags passed to all source files.
CFLAGS_Release := \
	-fPIC \
	-pthread \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-m64 \
	-O3 \
	-fno-omit-frame-pointer

# Flags passed to only C files.
CFLAGS_C_Release :=

# Flags passed to only C++ files.
CFLAGS_CC_Release := \
	-fno-rtti \
	-fno-exceptions \
	-std=gnu++1y

INCS_Release := \
	-I/usr/lib/node_modules/node_uareu/.node-gyp/12.4.0/include/node \
	-I/usr/lib/node_modules/node_uareu/.node-gyp/12.4.0/src \
	-I/usr/lib/node_modules/node_uareu/.node-gyp/12.4.0/deps/openssl/config \
	-I/usr/lib/node_modules/node_uareu/.node-gyp/12.4.0/deps/openssl/openssl/include \
	-I/usr/lib/node_modules/node_uareu/.node-gyp/12.4.0/deps/uv/include \
	-I/usr/lib/node_modules/node_uareu/.node-gyp/12.4.0/deps/zlib \
	-I/usr/lib/node_modules/node_uareu/.node-gyp/12.4.0/deps/v8/include \
	-I$(srcdir)/node_modules/nan \
	-I$(srcdir)/. \
	-I/usr/lib \
	-I/opt/Crossmatch/urusdk-linux/Include

OBJS := \
	$(obj).target/$(TARGET)/src/main.o \
	$(obj).target/$(TARGET)/src/identify.o \
	$(obj).target/$(TARGET)/src/selection.o \
	$(obj).target/$(TARGET)/src/helpers.o

# Add to the list of files we specially track dependencies for.
all_deps += $(OBJS)

# CFLAGS et al overrides must be target-local.
# See "Target-specific Variable Values" in the GNU Make manual.
$(OBJS): TOOLSET := $(TOOLSET)
$(OBJS): GYP_CFLAGS := $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))  $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_C_$(BUILDTYPE))
$(OBJS): GYP_CXXFLAGS := $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))  $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_CC_$(BUILDTYPE))

# Suffix rules, putting all outputs into $(obj).

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(srcdir)/%.cpp FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

# Try building from generated source, too.

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(obj).$(TOOLSET)/%.cpp FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(obj)/%.cpp FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

# End of this set of suffix rules
### Rules for final target.
LDFLAGS_Debug := \
	-pthread \
	-rdynamic \
	-m64

LDFLAGS_Release := \
	-pthread \
	-rdynamic \
	-m64

LIBS := \
	 \
	-L/opt/Crossmatch/urusdk-linux/Linux/lib \
	/usr/lib/libdpfpdd.so

$(obj).target/uareuprint.node: GYP_LDFLAGS := $(LDFLAGS_$(BUILDTYPE))
$(obj).target/uareuprint.node: LIBS := $(LIBS)
$(obj).target/uareuprint.node: TOOLSET := $(TOOLSET)
$(obj).target/uareuprint.node: $(OBJS) FORCE_DO_CMD
	$(call do_cmd,solink_module)

all_deps += $(obj).target/uareuprint.node
# Add target alias
.PHONY: uareuprint
uareuprint: $(builddir)/uareuprint.node

# Copy this to the executable output path.
$(builddir)/uareuprint.node: TOOLSET := $(TOOLSET)
$(builddir)/uareuprint.node: $(obj).target/uareuprint.node FORCE_DO_CMD
	$(call do_cmd,copy)

all_deps += $(builddir)/uareuprint.node
# Short alias for building this executable.
.PHONY: uareuprint.node
uareuprint.node: $(obj).target/uareuprint.node $(builddir)/uareuprint.node

# Add executable to "all" target.
.PHONY: all
all: $(builddir)/uareuprint.node

