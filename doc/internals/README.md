# Snoopy Logger Internals



## How does Snoopy actually work?

Snoopy installs itself as a preloaded library that provides a wrapper
around execv() and execve() syscall. Logging is done via syslog.

Snoopy should be completely transparent to users and to applications.



## Architectural note

execv() calls are now explicitly logged.   Although,  according to the
man page for execv(),  it is supposed to call execve().   To this date
the reason why execv()  calls weren't being logged is unknown,  but we
are working to find out why.



## Further information

* Read the code.
* Start at [../../src/eventsource/execve_wrapper.c](../../src/eventsource/execve_wrapper.c).
    This is essential.
    Here the execv()/execve() syscalls are intercepted and forwarded to internal
    Snoopy functions. Once Snoopy is done, execution is forwarded back to
    real execv()/execve() syscall.
* Continue with [../../src/log.c](../../src/log.c). Here you can learn how
    Snoopy log message is generated (using data sources) and dispatched to
    configured output.
* Configuration file parsing code is here: [../../src/configfile.c](../../src/configfile.c).
* Data sources are located here: [../../src/datasource/](../../src/datasource/).
* Filters are located here: [../../src/filter/](../../src/filter/).
* Outputs are located here: [../../src/output/](../../src/output/).



## Data storage list

When thread safety is DISABLED (default), data is stored in the following global
variables:
- snoopy_configuration_data
- snoopy_inputdatastorage_data
These variables must not be accessed directly, but rather by their corresponding
snoopy_*_get() functions. These functions, when thread safety is enabled, return
pointers to thread-specific instances of these variables.


When thread-safety is enabled, all configuration data is stored in single global
registry, and its structure looks something like this:

- *tsrm_threadRepo (of type List ptr):
    - main threadRepo/list data structure
    - the only production global variable (there is another for testing purposes - altConfigFilePath)
    - malloced in tsrm_ctor(), if necessary, with mutex
    - freed in tsrm_dtor(), if necessary, with mutex
    - *firstNode (null or ListNode ptr)
    - *lastNode  (null or ListNode ptr): pointer to tsrm_threadRepo's last node
    - ListNode structure:
        - *prevNode
        - *nextNode
        - *value: pointer to malloced tsrm_threadData_t
        - (*value structure):
            - (malloced in tsrm_createNewThreadData())
            - tid (id of thread for this threadData
            - *configuration   : malloced in tsrm_createNewThreadData()
            - *inputdatastorage: malloced in tsrm_createNewThreadData()
            - (*configuration structure):
                - defaults in configuration_setDefaults()
                - see structure in configuration.h
            - (*inputdatastorage structure):
                - defaults in inputdatastorage_setDefaults()
                - see structure in inputdatastorage.h
                    - *argv
                    - *envp
