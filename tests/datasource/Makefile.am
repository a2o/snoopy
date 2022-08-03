### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Which tests to run
#
TESTS       =
XFAIL_TESTS =

if DATASOURCE_ENABLED_cgroup
    TESTS += datasource_cgroup-name.sh
    TESTS += datasource_cgroup-name-first.sh
    TESTS += datasource_cgroup-name-last.sh
    TESTS += datasource_cgroup-name-mid.sh
    TESTS += datasource_cgroup-name-notfound.sh
    TESTS += datasource_cgroup-number.sh
    TESTS += datasource_cgroup-number-notfound.sh
endif

if DATASOURCE_ENABLED_cmdline
    TESTS += datasource_cmdline.sh
endif

if DATASOURCE_ENABLED_cwd
    TESTS += datasource_cwd.sh
endif

if DATASOURCE_ENABLED_datetime
    TESTS += datasource_datetime.sh
    TESTS += datasource_datetime-fmt-epoch.sh
    TESTS += datasource_datetime-fmt-static.sh
    TESTS += datasource_datetime-fmt-too-long.sh
    XFAIL_TESTS += datasource_datetime-fmt-too-long.sh
    TESTS += datasource_datetime-fmt-ymd.sh
endif

#if DATASOURCE_ENABLED_domain
#    TESTS += datasource_domain.sh
#endif

if DATASOURCE_ENABLED_egid
    TESTS += datasource_egid.sh
endif

if DATASOURCE_ENABLED_egroup
    TESTS += datasource_egroup.sh
endif

if DATASOURCE_ENABLED_env_all
    TESTS += datasource_env_all.sh
endif
if DATASOURCE_ENABLED_env
    TESTS += datasource_env-set.sh
    TESTS += datasource_env-unset.sh
endif

if DATASOURCE_ENABLED_euid
    TESTS += datasource_euid.sh
endif

if DATASOURCE_ENABLED_eusername
    TESTS += datasource_eusername.sh
endif

TESTS += datasource_failure.sh

if DATASOURCE_ENABLED_filename
    TESTS += datasource_filename.sh
endif

if DATASOURCE_ENABLED_gid
    TESTS += datasource_gid.sh
endif

if DATASOURCE_ENABLED_group
    TESTS += datasource_group.sh
endif

if DATASOURCE_ENABLED_hostname
    TESTS += datasource_hostname.sh
endif

if DATASOURCE_ENABLED_ipaddr
    TESTS += datasource_ipaddr.sh
endif

if DATASOURCE_ENABLED_login
    TESTS += datasource_login.sh
endif

if DATASOURCE_ENABLED_pid
    TESTS += datasource_pid.sh
endif

if DATASOURCE_ENABLED_ppid
    TESTS += datasource_ppid.sh
endif

if DATASOURCE_ENABLED_rpname
    TESTS += datasource_rpname.sh
endif

if DATASOURCE_ENABLED_sid
    TESTS += datasource_sid.sh
endif

if DATASOURCE_ENABLED_snoopy_configure_command
    TESTS += datasource_snoopy_configure_command.sh
endif

if DATASOURCE_ENABLED_snoopy_literal
    TESTS += datasource_snoopy_literal.sh
endif

if THREAD_SAFETY_ENABLED
if DATASOURCE_ENABLED_snoopy_threads
    TESTS += datasource_snoopy_threads.sh
endif
endif

if DATASOURCE_ENABLED_snoopy_version
    TESTS += datasource_snoopy_version.sh
endif

if DATASOURCE_ENABLED_systemd_unit_name
    TESTS += datasource_systemd_unit_name.sh
endif

if DATASOURCE_ENABLED_tid
    TESTS += datasource_tid.sh
endif

if DATASOURCE_ENABLED_tid_kernel
    TESTS += datasource_tid_kernel.sh
endif

if DATASOURCE_ENABLED_timestamp
    TESTS += datasource_timestamp.sh
endif

if DATASOURCE_ENABLED_timestamp_ms
    TESTS += datasource_timestamp_ms.sh
endif

if DATASOURCE_ENABLED_timestamp_us
    TESTS += datasource_timestamp_us.sh
endif

if DATASOURCE_ENABLED_tty
    TESTS += datasource_tty.sh
endif

if DATASOURCE_ENABLED_tty_uid
    TESTS += datasource_tty_uid.sh
endif

if DATASOURCE_ENABLED_tty_username
    TESTS += datasource_tty_username.sh
endif

if DATASOURCE_ENABLED_uid
    TESTS += datasource_uid.sh
endif

if DATASOURCE_ENABLED_username
    TESTS += datasource_username.sh
endif



### Include tests in distribution archive
#
EXTRA_DIST  = _bootstrap.sh
EXTRA_DIST += $(TESTS)
