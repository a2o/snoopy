# F.A.Q. - Snoopy Command Logger



### Was legacy Snoopy faster than current implementation?

This test was performed with Snoopy 2.3.0.

We compared the following three implementations:
- current implementation, version 2.3.0, with enabled config file (missing), filtering, and with dynamic log message generation
- legacy Snoopy, version 1.9.0, where message is generated in single pass (no function calling)
- legacy Snoopy with output directly to console (to avoid syslog())

Results (loop with 2000 iterations, on Macbook Air 6.2 with i5-4250U):
- 2.3.0: 3.6s (with config file and filtering enabled)
- 2.3.0: 3.5s
- 1.9.0: 3.2s
- 1.9.0: 3.0s (output to console)

This 10% difference seems insignificant, at least for general workloads.
It also seems that forking is the most expensive operation, which is not unexpected.



### I see no Snoopy output after initial user login

Your user probably has LD_PRELOAD environmental variable set to something non-
empty in their profile script. Remove it, or add `libsnoopy.so` to LD_PRELOAD, like this (UNTESTED):

    export LD_PRELOAD="/path/to/libsnoopy.so /path/to/otherlib.so"



### How do I go about developing new data source/filter/output?

Consult the [HACKING-OVERVIEW.md](HACKING-OVERVIEW.md) document for more information.



### I want to contribute - how should I do it?

Contributions are welcome.
Consult [.github/CONTRIBUTING.md](../.github/CONTRIBUTING.md) for further information.



### I do not see any non-root Snoopy entries in my logs

If you have configured `file` output provider, make sure the target file has proper permissions set for writing into it by all users.
This usually means changing file permissions to world-writable `0666`, which is a fairly insecure setting.
You may want to reconfigure Snoopy to use `devlog` output instead.
