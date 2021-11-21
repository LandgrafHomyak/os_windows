# API for windows

---

## `COMiC_OS`

field    | name          | comment
---------|---------------|-----------------------
`HANDLE` | mutex_handler | native mutex handler

## `COMiC_OS_Thread`

field               | name           | comment
--------------------|----------------|-----------------------------
`COMiC_OS_Thread *` | _prev          | _prev thread in dl-list
`COMiC_OS_Thread *` | _next          | _next thread in dl-list
`HANDLE`            | thread_handler | native thread handler
`COMiC_OS *`        | os_instance    | pointer to owner OS instance
`COMiC_Error`       | error          | error storage

## `COMiC_OS`

field               | name        | comment
--------------------|-------------|-------------------------------
`COMiC_OS *`        | _next       | _next OS instance in dl-list
`COMiC_OS *`        | _prev       | _prev OS instance in dl-list
`HANDLE`            | heap        | local heap
`COMiC_size`        | heap_size   | local heap max size
`COMiC_OS_Thread`   | root_thread | thread owned this instance
`COMiC_OS_Thread *` | thread_list | dl-list for bind threads
`COMiC_Lock`        | global_lock | mutex for accessing this instance