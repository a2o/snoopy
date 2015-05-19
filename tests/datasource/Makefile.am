### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Which tests to run
#
SCRIPT_PREFIX = datasource
TESTS  =
XFAIL_TESTS  =
TESTS += $(SCRIPT_PREFIX)_cmdline.sh
TESTS += $(SCRIPT_PREFIX)_cwd.sh
TESTS += $(SCRIPT_PREFIX)_datetime.sh

# This one is failing because there seems to be a bug in snoopy datasource implementation
TESTS += $(SCRIPT_PREFIX)_domain.sh
XFAIL_TESTS += $(SCRIPT_PREFIX)_domain.sh

TESTS += $(SCRIPT_PREFIX)_egid.sh
TESTS += $(SCRIPT_PREFIX)_egroup.sh
TESTS += $(SCRIPT_PREFIX)_env_all.sh
TESTS += $(SCRIPT_PREFIX)_env-set.sh
TESTS += $(SCRIPT_PREFIX)_env-unset.sh
TESTS += $(SCRIPT_PREFIX)_euid.sh
TESTS += $(SCRIPT_PREFIX)_eusername.sh
TESTS += $(SCRIPT_PREFIX)_filename.sh
TESTS += $(SCRIPT_PREFIX)_gid.sh
TESTS += $(SCRIPT_PREFIX)_group.sh
TESTS += $(SCRIPT_PREFIX)_hostname.sh
TESTS += $(SCRIPT_PREFIX)_login.sh
TESTS += $(SCRIPT_PREFIX)_pid.sh
TESTS += $(SCRIPT_PREFIX)_ppid.sh
#TESTS += $(SCRIPT_PREFIX)_rpname.sh
TESTS += $(SCRIPT_PREFIX)_sid.sh
TESTS += $(SCRIPT_PREFIX)_snoopy_version.sh
TESTS += $(SCRIPT_PREFIX)_timestamp.sh
TESTS += $(SCRIPT_PREFIX)_timestamp_ms.sh
TESTS += $(SCRIPT_PREFIX)_timestamp_us.sh
TESTS += $(SCRIPT_PREFIX)_tty.sh
TESTS += $(SCRIPT_PREFIX)_tty_uid.sh
TESTS += $(SCRIPT_PREFIX)_tty_username.sh
TESTS += $(SCRIPT_PREFIX)_uid.sh
TESTS += $(SCRIPT_PREFIX)_username.sh
