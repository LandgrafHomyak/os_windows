# API for windows

---

## `COMiC_OS`

field              | name       | comment
-------------------|------------|-----------------------
`CRITICAL_SECTION` | cs_storage | native mutex storage

## `COMiC_OS_Thread`

field                  | name           | comment
-----------------------|----------------|-----------------------------
`COMiC_OS_Thread *`    | _prev          | _prev thread in dl-list
`COMiC_OS_Thread *`    | _next          | _next thread in dl-list
`HANDLE`               | thread_handler | native thread handler
`_COMiC_OS_Instance *` | os_instance    | pointer to owner OS instance
`COMiC_Error`          | error          | error storage

## `_COMiC_OS_Instance`

field                  | name          | comment
-----------------------|---------------|-------------------------------
`_COMiC_OS_Instance *` | _next         | _next OS instance in dl-list
`_COMiC_OS_Instance *` | _prev         | _prev OS instance in dl-list
`HANDLE`               | heap          | local heap
`COMiC_size`           | heap_max_size | local heap max size
`COMiC_OS_Thread *`    | thread_list   | dl-list for bind threads