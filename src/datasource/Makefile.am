### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Include
#
AM_CFLAGS += -I$(top_srcdir)/src



### Create one library with all Snoopy data sources collected in it
#
noinst_LTLIBRARIES = libsnoopy_datasources_all.la



### Data sources collection is made of the following files
#
# Please maintain alphabetical order, equal to what `ls` would do.
#
libsnoopy_datasources_all_la_SOURCES = \
	noop.c \
	noop.h
libsnoopy_datasources_all_la_LDFLAGS =



# Data source: cmdline
#
if DATASOURCE_ENABLED_cmdline
libsnoopy_datasources_all_la_SOURCES += \
	cmdline.c \
	cmdline.h
endif

# Data source: cwd
#
if DATASOURCE_ENABLED_cwd
libsnoopy_datasources_all_la_SOURCES += \
	cwd.c \
	cwd.h
endif

# Data source: datetime
#
if DATASOURCE_ENABLED_datetime
libsnoopy_datasources_all_la_SOURCES += \
	datetime.c \
	datetime.h
endif

# Data source: domain
#
if DATASOURCE_ENABLED_domain
libsnoopy_datasources_all_la_SOURCES += \
	domain.c \
	domain.h
endif

# Data source: egid
#
if DATASOURCE_ENABLED_egid
libsnoopy_datasources_all_la_SOURCES += \
	egid.c \
	egid.h
endif

# Data source: egroup
#
if DATASOURCE_ENABLED_egroup
libsnoopy_datasources_all_la_SOURCES += \
	egroup.c \
	egroup.h
endif

# Data source: env
#
if DATASOURCE_ENABLED_env
libsnoopy_datasources_all_la_SOURCES += \
	env.c \
	env.h
endif

# Data source: env_all
#
if DATASOURCE_ENABLED_env_all
libsnoopy_datasources_all_la_SOURCES += \
	env_all.c \
	env_all.h
endif

# Data source: euid
#
if DATASOURCE_ENABLED_euid
libsnoopy_datasources_all_la_SOURCES += \
	euid.c \
	euid.h
endif

# Data source: eusername
#
if DATASOURCE_ENABLED_eusername
libsnoopy_datasources_all_la_SOURCES += \
	eusername.c \
	eusername.h
endif

# Data source: filename
#
if DATASOURCE_ENABLED_filename
libsnoopy_datasources_all_la_SOURCES += \
	filename.c \
	filename.h
endif

# Data source: gid
#
if DATASOURCE_ENABLED_gid
libsnoopy_datasources_all_la_SOURCES += \
	gid.c \
	gid.h
endif

# Data source: group
#
if DATASOURCE_ENABLED_group
libsnoopy_datasources_all_la_SOURCES += \
	group.c \
	group.h
endif

# Data source: hostname
#
if DATASOURCE_ENABLED_hostname
libsnoopy_datasources_all_la_SOURCES += \
	hostname.c \
	hostname.h
endif

# Data source: login
#
if DATASOURCE_ENABLED_login
libsnoopy_datasources_all_la_SOURCES += \
	login.c \
	login.h
endif

# Data source: pid
#
if DATASOURCE_ENABLED_pid
libsnoopy_datasources_all_la_SOURCES += \
	pid.c \
	pid.h
endif

# Data source: ppid
#
if DATASOURCE_ENABLED_ppid
libsnoopy_datasources_all_la_SOURCES += \
	ppid.c \
	ppid.h
endif

# Data source: rpname
#
if DATASOURCE_ENABLED_rpname
libsnoopy_datasources_all_la_SOURCES += \
	rpname.c \
	rpname.h
endif

# Data source: sid
#
if DATASOURCE_ENABLED_sid
libsnoopy_datasources_all_la_SOURCES += \
	sid.c \
	sid.h
endif

# Data source: snoopy_literal
#
if DATASOURCE_ENABLED_snoopy_literal
libsnoopy_datasources_all_la_SOURCES += \
	snoopy_literal.c \
	snoopy_literal.h
endif

# Data source: snoopy_threads
#
if THREAD_SAFETY_ENABLED
if DATASOURCE_ENABLED_snoopy_threads
libsnoopy_datasources_all_la_SOURCES += \
	snoopy_threads.c \
	snoopy_threads.h
endif
endif

# Data source: snoopy_version
#
if DATASOURCE_ENABLED_snoopy_version
libsnoopy_datasources_all_la_SOURCES += \
	snoopy_version.c \
	snoopy_version.h
endif

# Data source: tid
#
if DATASOURCE_ENABLED_tid
libsnoopy_datasources_all_la_SOURCES += \
	tid.c \
	tid.h
# This one seems to be added automatically recently.
libsnoopy_datasources_all_la_LDFLAGS += -lpthread
endif

# Data source: tid_kernel
#
if DATASOURCE_ENABLED_tid_kernel
libsnoopy_datasources_all_la_SOURCES += \
	tid_kernel.c \
	tid_kernel.h
endif

# Data source: timestamp
#
if DATASOURCE_ENABLED_timestamp
libsnoopy_datasources_all_la_SOURCES += \
	timestamp.c \
	timestamp.h
endif

# Data source: timestamp_ms
#
if DATASOURCE_ENABLED_timestamp_ms
libsnoopy_datasources_all_la_SOURCES += \
	timestamp_ms.c \
	timestamp_ms.h
endif

# Data source: timestamp_us
#
if DATASOURCE_ENABLED_timestamp_us
libsnoopy_datasources_all_la_SOURCES += \
	timestamp_us.c \
	timestamp_us.h
endif

# Data source: tty
#
if DATASOURCE_ENABLED_tty
libsnoopy_datasources_all_la_SOURCES += \
	tty.c \
	tty.h
endif

# Data source: tty_uid
#
if DATASOURCE_ENABLED_tty_uid
libsnoopy_datasources_all_la_SOURCES += \
	tty_uid.c \
	tty_uid.h
endif

# Data source: tty_username
#
if DATASOURCE_ENABLED_tty_username
libsnoopy_datasources_all_la_SOURCES += \
	tty_username.c \
	tty_username.h
endif

# Data source: uid
#
if DATASOURCE_ENABLED_uid
libsnoopy_datasources_all_la_SOURCES += \
	uid.c \
	uid.h
endif

# Data source: username
#
if DATASOURCE_ENABLED_username
libsnoopy_datasources_all_la_SOURCES += \
	username.c \
	username.h
endif
