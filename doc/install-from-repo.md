# Installing from upstream package repository - Snoopy Command Logger

Supported distributions:
* [AlmaLinux OS](#almalinux-os)
* [Arch Linux](#arch-linux)
* [CentOS](#centos)
* [Debian](#debian)
* [openSUSE Leap](#opensuse-leap)
* [Ubuntu](#ubuntu)

Supported architectures:
* x86_64 (amd64)

Prerequisites:
* Installed package: `curl`

Release channels:
* stable
* [testing](#repositories-with-non-stable-builds-testing)



## AlmaLinux OS

List of supported AlmaLinux OS versions is [here](https://github.com/a2o/snoopy-packages/tree/master/repo/almalinux).

AlmaLinux 9 [search-key:almalinux-9]:
```shell
curl -o snoopy-packages-key.pub https://a2o.github.io/snoopy-packages/snoopy-packages-key.pub &&
rpm --import snoopy-packages-key.pub &&

echo "[snoopy-stable]
name=Snoopy Upstream Stable Repository
baseurl=https://a2o.github.io/snoopy-packages/repo/almalinux/9/stable/
enabled=1
gpgcheck=1
" > /etc/yum.repos.d/snoopy-stable.repo &&

yum install snoopy
```

AlmaLinux 8 [search-key:almalinux-8]:
```shell
curl -o snoopy-packages-key.pub https://a2o.github.io/snoopy-packages/snoopy-packages-key.pub &&
rpm --import snoopy-packages-key.pub &&

echo "[snoopy-stable]
name=Snoopy Upstream Stable Repository
baseurl=https://a2o.github.io/snoopy-packages/repo/almalinux/8/stable/
enabled=1
gpgcheck=1
" > /etc/yum.repos.d/snoopy-stable.repo &&

yum install snoopy
```



## Arch Linux

NOTICE: Arch Linux packaging is currently not supported.

The reason is a bug that causes `fakeroot` tool (used internally by Arch's packaging toolset)
to hang indefinitely when run inside a Docker container. Since Snoopy Logger is using Docker
(via Github Actions, or locally) for creating native binary packages, this bug is a showstopper
and binary packages for Arch will not be provided until this bug is resolved.

Reference: https://github.com/docker/for-mac/issues/7331



## CentOS

List of supported CentOS versions is [here](https://github.com/a2o/snoopy-packages/tree/master/repo/centos).

CentOS 9 Stream [search-key:centos-9]:
```shell
curl -o snoopy-packages-key.pub https://a2o.github.io/snoopy-packages/snoopy-packages-key.pub &&
rpm --import snoopy-packages-key.pub &&

echo "[snoopy-stable]
name=Snoopy Upstream Stable Repository
baseurl=https://a2o.github.io/snoopy-packages/repo/centos/9/stable/
enabled=1
gpgcheck=1
" > /etc/yum.repos.d/snoopy-stable.repo &&

yum install snoopy
```

CentOS 8 Stream [search-key:centos-8]:
```shell
curl -o snoopy-packages-key.pub https://a2o.github.io/snoopy-packages/snoopy-packages-key.pub &&
rpm --import snoopy-packages-key.pub &&

echo "[snoopy-stable]
name=Snoopy Upstream Stable Repository
baseurl=https://a2o.github.io/snoopy-packages/repo/centos/8/stable/
enabled=1
gpgcheck=1
" > /etc/yum.repos.d/snoopy-stable.repo &&

yum install snoopy
```



## Debian

List of supported Debian versions is [here](https://github.com/a2o/snoopy-packages/tree/master/repo/debian/dists).

Debian 12 (Bookworm) [search-key:debian-12]:
```shell
curl https://a2o.github.io/snoopy-packages/snoopy-packages-key.pub | apt-key add - &&
echo "deb https://a2o.github.io/snoopy-packages/repo/debian bookworm stable" > /etc/apt/sources.list.d/snoopy-stable.list &&
apt update &&
apt install snoopy
```

Debian 11 (Bullseye) [search-key:debian-11]:
```shell
curl https://a2o.github.io/snoopy-packages/snoopy-packages-key.pub | apt-key add - &&
echo "deb https://a2o.github.io/snoopy-packages/repo/debian bullseye stable" > /etc/apt/sources.list.d/snoopy-stable.list &&
apt update &&
apt install snoopy
```

Debian 10 (Buster) [search-key:debian-10]:
```shell
curl https://a2o.github.io/snoopy-packages/snoopy-packages-key.pub | apt-key add - &&
echo "deb https://a2o.github.io/snoopy-packages/repo/debian buster stable" > /etc/apt/sources.list.d/snoopy-stable.list &&
apt update &&
apt install snoopy
```



## openSUSE Leap

List of supported openSUSE Leap versions [here](https://github.com/a2o/snoopy-packages/tree/master/repo/opensuse-leap).

openSUSE Leap 15.6 [search-key:opensuse-leap-15.6]:
```shell
curl -o snoopy-packages-key.pub https://a2o.github.io/snoopy-packages/snoopy-packages-key.pub &&
rpm --import snoopy-packages-key.pub &&
zypper addrepo -n "Snoopy Upstream Stable Repository" -g -e https://a2o.github.io/snoopy-packages/repo/opensuse-leap/15.6/stable/ snoopy-stable &&
zypper install snoopy
```

openSUSE Leap 15.5 [search-key:opensuse-leap-15.5]:
```shell
curl -o snoopy-packages-key.pub https://a2o.github.io/snoopy-packages/snoopy-packages-key.pub &&
rpm --import snoopy-packages-key.pub &&
zypper addrepo -n "Snoopy Upstream Stable Repository" -g -e https://a2o.github.io/snoopy-packages/repo/opensuse-leap/15.5/stable/ snoopy-stable &&
zypper install snoopy
```



## Ubuntu

Packages are provided for active Ubuntu LTS versions only.
List of supported Ubuntu LTS versions is [here](https://github.com/a2o/snoopy-packages/tree/master/repo/ubuntu/dists).

Ubuntu 24.04 LTS (Noble Numbat) [search-key:ubuntu-24.04]:
```shell
curl https://a2o.github.io/snoopy-packages/snoopy-packages-key.pub | apt-key add - &&
echo "deb https://a2o.github.io/snoopy-packages/repo/ubuntu noble stable" > /etc/apt/sources.list.d/snoopy-stable.list &&
apt update &&
apt install snoopy
```

Ubuntu 22.04 LTS (Jammy Jellyfish) [search-key:ubuntu-22.04]:
```shell
curl https://a2o.github.io/snoopy-packages/snoopy-packages-key.pub | apt-key add - &&
echo "deb https://a2o.github.io/snoopy-packages/repo/ubuntu jammy stable" > /etc/apt/sources.list.d/snoopy-stable.list &&
apt update &&
apt install snoopy
```

Ubuntu 20.04 LTS (Focal Fossa) [search-key:ubuntu-20.04]:
```shell
curl https://a2o.github.io/snoopy-packages/snoopy-packages-key.pub | apt-key add - &&
echo "deb https://a2o.github.io/snoopy-packages/repo/ubuntu focal stable" > /etc/apt/sources.list.d/snoopy-stable.list &&
apt update &&
apt install snoopy
```



## Respositories with non-stable builds - testing

Instructions above contain steps to install **stable** Snoopy releases only.
Sometimes, non-stable Snoopy packages are released too, but not through the stable repositories.
To install a **testing** Snoopy version, add a _testing_ repository:
- Use the same steps as above, but
- In all steps, replace the word `stable` with `testing`, i.e.:
  - `https://a2o.github.io/snoopy-packages/repo/ubuntu jammy stable` becomes
  - `https://a2o.github.io/snoopy-packages/repo/ubuntu jammy testing`
