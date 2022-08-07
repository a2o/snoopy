#
# Snoopy Command Logger - RPM builds
#
# This file is designed to work from within Snoopy's git repository working directory.
# It should not be invoked directly. Rather, use the following script to build a .rpm
# package:
#
#       ./dev-tools/build-package-rpm.sh
#
Name:           snoopy
Version:        %snoopy_rpm_package_version
Release:        %snoopy_rpm_release_tag
Summary:        Snoopy Command Logger is a wrapper around execve() that captures all executed commands by all users and all processes and sends the data to syslog.
License:        GPLv2+
URL:            https://github.com/a2o/snoopy
Packager:       Bostjan Skufca Jese <bostjan@skufca.si>


BuildRequires:  automake
BuildRequires:  libtool
BuildRequires:  gcc
BuildRequires:  make
# For tests
BuildRequires:  %{_bindir}/hostname
BuildRequires:  %{_bindir}/ps
BuildRequires:  %{_bindir}/socat


%description
Snoopy Command Logger is a shared library that interposes itself
between dynamic executables and libc's execve() function.
Once loaded, programs' calls to execve() are intercepted and logged.
System administrators may find Snoopy Command Logger useful in tasks
such as system diagnostics, tracking other administrators' actions
as well as getting a good 'feel' of what's going on on their system
(i.e. what CGI scripts are being launched by Apache).


%build
%configure --enable-everything
%make_build check


%install
%make_install
rm %{buildroot}%{_libdir}/libsnoopy.la


%files
%doc ../../../README.md
%doc ../../../ChangeLog
%doc ../../../COPYING
%license ../../../COPYING
%{_libdir}/*
%{_sbindir}/*
%{_sysconfdir}/*
%config(noreplace) %{_sysconfdir}/snoopy.ini


%post
/usr/sbin/snoopyctl enable
%preun
/usr/sbin/snoopyctl disable


%changelog
* Sat Aug 6 2022 Bostjan Skufca Jese <bostjan@skufca.si> %snoopy_rpm_package_version
- NOTICE: This is not a real changelog.
- This .rpm package has been provided by Snoopy maintainers.
- Snoopy's git repository is the actual changelog for this package,
  at https://github.com/a2o/snoopy/.
- NOTICE: While the intent of Snoopy maintainers is to keep their
  .rpm packages in sync and interchangeable with distribution-provided
  Snoopy packages, the packaging efforts are not synchronized and thus
  incompatible differences may occur.
- NOTICE: To build the .rpm package yourself, use the provided
  ./dev-tools/build-package-rpm.sh helper script.
- NOTICE: To install the required software for building the .rpm
  package, use the provided ./dev-tools/install-packaging-software.sh
  helper script.
