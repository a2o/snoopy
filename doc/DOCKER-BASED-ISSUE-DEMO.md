# Docker-based issue demo - Snoopy Command Logger

To simplify the transfer of context when reporting an issue, create a
Docker-based demonstration of the Snoopy anomaly you discovered.


## What YOU need to do

You need to:
- Create a Dockerfile that sets up the environment in which the issue occurs (see instructions below)
- Provide a `docker run ...` command that correctly starts the container
- Provide a command to run inside the container that triggres the anomaly (if needed)
- Provide a description of how the anomaly manifests itself in the container

The work you do here will make sure that the anomaly can be quickly reproduced
(and thus quickly investigated) by the maintainer.


## What will the maintaner do

The maintainer will:
- Use your Dockerfile to build an image and run it
- Use your command to trigger the anomaly (if provided)


## How to create a Dockerfile

So, how to create a Dockerfile that represents your environment and manifests the anomaly?

Well, this one is really up to you. :)

But, to get you started, [here is an example Dockerfile](DOCKER-BASED-ISSUE-DEMO-Dockerfile)
that creates a Ubuntu 20.04 image which uses systemd as an entry point (to
roughly simulate a non-containerized system) and has the latest version of
Snoopy installed.


## How to build and test the image

To build an image based on your Dockerfile:

    docker build -f my-snoopy-issue-demo-Dockerfile -t my-snoopy-issue-demo-i .

To run the container based on the newly built image (the `--tmpfs` and `-v` flags may not be needed by your Dockerfile, but they are needed by the `jrei/systemd-ubuntu:20.04` image on which our [example Dockerfile](DOCKER-BASED-ISSUE-DEMO-Dockerfile) is based on):

    docker run -d --name my-snoopy-issue-demo-c --tmpfs /tmp --tmpfs /run --tmpfs /run/lock -v /sys/fs/cgroup:/sys/fs/cgroup:ro my-snoopy-issue-demo-i

To enter the running container:

    docker exec -t -i my-snoopy-issue-demo-c bash

Once inside the container, let's confirm that Snoopy is actually working:

    root@3c8649d7a514:/# journalctl -xe | tail
    ...
    Apr 15 00:10:24 3c8649d7a514 snoopy[45]: [uid:0 sid:39 tty:/dev/pts/1 cwd:/ filename:/usr/bin/groups]: groups
    Apr 15 00:10:24 3c8649d7a514 snoopy[46]: [uid:0 sid:39 tty:/dev/pts/1 cwd:/ filename:/usr/bin/dircolors]: dircolors -b
    Apr 15 00:10:25 3c8649d7a514 snoopy[48]: [uid:0 sid:39 tty:(none) cwd:/ filename:/usr/bin/tail]: tail
    Apr 15 00:10:25 3c8649d7a514 snoopy[47]: [uid:0 sid:39 tty:/dev/pts/1 cwd:/ filename:/usr/bin/journalctl]: journalctl -xe
    root@3c8649d7a514:/#
