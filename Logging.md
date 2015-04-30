# Debug levels #

The debug level may be specified from the command prompt. It defines the minimum set of levels recognized by the system. The debug levels are defined as follows:

| **Parameter value** | **Rank** | **Internal log level** | **Description** |
|:--------------------|:---------|:-----------------------|:----------------|
| `debugall` | 1 | `logDEBUG2` | The DEBUG2 level has the lowest possible rank and is intended to turn on all logging. |
| `debug` | 2 | `logDEBUG` |  The DEBUG level designates fine-grained informational events that are most useful to debug the application. |
| `info` | 3 | `logINFO` | The INFO level designates informational messages that highlight the progress of the application at coarse-grained level. (Default) |
| `warning` | 4 | `logWARNING` | If the debug state is WARNING, only errors and warnings will be filed.  |
| `error` | 5 | `logERROR` | The ERROR level has the highest possible rank and is intended to turn off logging. |


# Implementation #
All debug messages are outputted through a Logger object.
The Logger object is used as follows:

```
LOG( logINFO ) << "Log message at debug level INFO";
```

Any time the Logger object is told to log something at a debug level that is above the global debug level, it ignores it.