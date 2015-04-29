# The exclude_spawns_of filter

## Purpose of the filter

When Snoopy is installed, it will generate a log message for every executable
that runs. This includes the programs invoked by cron and other system
daemons. All these messages create noise in the logs and tends to drown useful
information. So filtering out messages created by cron jobs is therefore a
useful feature. That's what the exclude_spawns_of filter does.

## Terms used

In this page, we use the following terms:

- Process tree: this is the graph of processes running on your system, starting at init (process 1) at the top, and going down from parent to child processes.
- Ancestor: A process _A_ is the ancestor of process _P_ if you can start from _P_ in the process tree and go up one or more levels until you find _A_.
- Descendant: A process _D_ is a descendant of process _P_ if _P_ is the ancestor of _D_. A synonym for _spawn_.


## Setting up the filter

The filter takes a comma-separated list of program names as a parameter. If the current process is a descendant of one of the named programs, the filter will drop the message.

So if you want to avoid logging programs executed by cron jobs or their descendants, just put

    filter_chain = "exclude_spawns_of:cron"

in the `snoopy.ini` file.

The list of program names after the `exclude_spawns_of:` is called the _black list_. It is a list of ancestors for which the log message should be dropped.

Of course, your filter chain can include other filters. See snoopy.ini for examples.

If you also want to avoid logging the executables launched by `mydaemon`, you can add it to the black list:

    filter_chain = "exclude_spawns_of:cron,mydaemon"

Do not put spaces around the colon or the commas!

## The gory details

The filter walks up the process tree and examines the ancestors of the current process, starting from the parent of the current process. For each ancestor, the filter compares the name of the executable with the black list. If there is a match, the filter drops the message.

A name listed in the black list can be a binary name or a script name. If you have a doubt, find the program that you want to add in the list and run the command

    cat /proc/nnnn/stat

where _nnnn_ is the PID of the program. The output of the command will list a name between parentheses in the second word. That name (without the parentheses!) is what you need to add in the black list.

If the filter encounters an error, it will pass the message.

## Installing and testing the filter

### Compiling

If you install from source, make sure that the following options are passed to `./configure`:

    --enable-filtering
    --enable-config-file

For debian, these options are in `debian/rules`.

### Installing

The filter is installed along with the rest of Snoopy, there is no separate package.

### Setting up

We assume you are running as root or sudoing as needed.

If you installed from a package that was correctly configured, you should see the file `/etc/snoopy.ini` in your system. If this file is missing, it means that the version of Snoopy you installed was compiled without the `--enable-config-file` option. It cannot be used here. Please install a different version.

 We are going to modify the ini file to make sure the filter works.

Change the default message format in `/etc/snoopy.ini`. For example, we add the PID of the process:

    message_format =  "[pid:%{pid} uid:%{uid} sid:%{sid} tty:%{tty} cwd:%{cwd} filename:%{filename}]: %{cmdline}"

Next, we will create an easy-to-test black list. Add the following line in  `/etc/snoopy.ini`:

    filter_chain = "exclude_spawns_of:cron,sh"

We specified that Snoopy should not log programs spawned by cron and sh. We assume here that you run bash as the standard shell. You can verify that this is the case with the command:

    getent passwd $USER

The last string in the command output is your login shell, `bash` by default.

### Verifying that Snoopy reads the ini file

Let's enable Snoopy with the command:

    snoopy-enable

You can check that the `/etc/ld.so.preload` file now contains something like `/lib/libsnoopy.so`.

Open another window and run `tail -f /var/log/auth.log` to check that Snoopy works.

Snoopy works only for processes started after it's enabled, so let's invoke a new shell with the command

    bash

Within that new shell, run ls. You will see a message in `auth.log` that looks like this:
```
Apr 24 21:41:57 fred-vm-ub1 snoopy[2320]: [pid:2320 uid:1000 sid:1872 tty:/dev/pts/0 cwd:/home/fmora filename:/bin/ls]: ls
```

This shows that Snoopy read the ini file and parsed the `message_format` parameter.

### Testing the filter

Remember that your default shell is bash, and that you added the `sh` shell to the black list. So let's run the command `sh` to start an instance of this shell. You should see that Snoopy records it in the log (since its ancestors are not blacklisted).

However, type `ls` or other commands within the new `sh` shell. You should see that Snoopy does not log these commands, since they are a descendant of the blacklisted `sh`.

Start a bash shell within sh by typing `bash`. Within bash, type `ls` or other commands. Snoopy does not log them. This is because these commands are still descendants from the blacklisted `sh`.

This shows that the filter works. You can now set the black list as you need.


