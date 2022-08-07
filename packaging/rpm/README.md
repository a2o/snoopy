# Snoopy Command Logger - RPM packaging

## Info

The provided [snoopy.spec](snoopy.spec) file is not configured to be used directly, nor in a standalone fashion.
It only works when used within this Git repository, and with the provided tooling.



## How to build a RPM package

Steps:
- Clone this Git repository onto a supported system,
- Install the required development and packaging tools, and
- Use the package building script.

To clone this repository:
```
git clone https://github.com/a2o/snoopy
cd snoopy
```

To install the required software (this only needs to be done once):
```bash
./dev-tools/install-dev-software.sh
./dev-tools/install-packaging-software.sh
```

To build a RPM package:
```bash
./dev-tools/build-package-rpm.sh
```

Done.
