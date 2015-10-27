### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Which tests to run
#
SCRIPT_PREFIX = datasource
TESTS  =
XFAIL_TESTS  =

if DATASOURCE_ENABLED_cmdline
    TESTS += $(SCRIPT_PREFIX)_cmdline.sh
endif

if DATASOURCE_ENABLED_cwd
    TESTS += $(SCRIPT_PREFIX)_cwd.sh
endif

if DATASOURCE_ENABLED_datetime
    TESTS += $(SCRIPT_PREFIX)_datetime.sh
endif

#if DATASOURCE_ENABLED_domain
#    TESTS += $(SCRIPT_PREFIX)_domain.sh
#endif

if DATASOURCE_ENABLED_egid
    TESTS += $(SCRIPT_PREFIX)_egid.sh
endif

if DATASOURCE_ENABLED_egroup
    TESTS += $(SCRIPT_PREFIX)_egroup.sh
endif

if DATASOURCE_ENABLED_env_all
    TESTS += $(SCRIPT_PREFIX)_env_all.sh
endif
if DATASOURCE_ENABLED_env
    TESTS += $(SCRIPT_PREFIX)_env-set.sh
    TESTS += $(SCRIPT_PREFIX)_env-unset.sh
endif

if DATASOURCE_ENABLED_euid
    TESTS += $(SCRIPT_PREFIX)_euid.sh
endif

if DATASOURCE_ENABLED_eusername
    TESTS += $(SCRIPT_PREFIX)_eusername.sh
endif

if DATASOURCE_ENABLED_filename
    TESTS += $(SCRIPT_PREFIX)_filename.sh
endif

if DATASOURCE_ENABLED_gid
    TESTS += $(SCRIPT_PREFIX)_gid.sh
endif

if DATASOURCE_ENABLED_group
    TESTS += $(SCRIPT_PREFIX)_group.sh
endif

if DATASOURCE_ENABLED_hostname
    TESTS += $(SCRIPT_PREFIX)_hostname.sh
endif

if DATASOURCE_ENABLED_login
    TESTS += $(SCRIPT_PREFIX)_login.sh
endif

if DATASOURCE_ENABLED_pid
    TESTS += $(SCRIPT_PREFIX)_pid.sh
endif

if DATASOURCE_ENABLED_ppid
    TESTS += $(SCRIPT_PREFIX)_ppid.sh
endif

if DATASOURCE_ENABLED_rpname
    TESTS += $(SCRIPT_PREFIX)_rpname.sh
endif

if DATASOURCE_ENABLED_sid
    TESTS += $(SCRIPT_PREFIX)_sid.sh
endif

if DATASOURCE_ENABLED_snoopy_literal
    TESTS += $(SCRIPT_PREFIX)_snoopy_literal.sh
endif

if THREAD_SAFETY_ENABLED
if DATASOURCE_ENABLED_snoopy_threads
    TESTS += $(SCRIPT_PREFIX)_snoopy_threads.sh
endif
endif

if DATASOURCE_ENABLED_snoopy_version
    TESTS += $(SCRIPT_PREFIX)_snoopy_version.sh
endif

if DATASOURCE_ENABLED_tid
    TESTS += $(SCRIPT_PREFIX)_tid.sh
endif

if DATASOURCE_ENABLED_tid_kernel
    TESTS += $(SCRIPT_PREFIX)_tid_kernel.sh
endif

if DATASOURCE_ENABLED_timestamp
    TESTS += $(SCRIPT_PREFIX)_timestamp.sh
endif

if DATASOURCE_ENABLED_timestamp_ms
    TESTS += $(SCRIPT_PREFIX)_timestamp_ms.sh
endif

if DATASOURCE_ENABLED_timestamp_us
    TESTS += $(SCRIPT_PREFIX)_timestamp_us.sh
endif

if DATASOURCE_ENABLED_tty
    TESTS += $(SCRIPT_PREFIX)_tty.sh
endif

if DATASOURCE_ENABLED_tty_uid
    TESTS += $(SCRIPT_PREFIX)_tty_uid.sh
endif

if DATASOURCE_ENABLED_tty_username
    TESTS += $(SCRIPT_PREFIX)_tty_username.sh
endif

if DATASOURCE_ENABLED_uid
    TESTS += $(SCRIPT_PREFIX)_uid.sh
endif

if DATASOURCE_ENABLED_username
    TESTS += $(SCRIPT_PREFIX)_username.sh
endif



### Include tests in distribution archive
#
EXTRA_DIST  = _bootstrap.sh
EXTRA_DIST += $(TESTS)
