# Werk
Werk is a C++ library of components for developing performance-sensitive,
real-time applications. Each Werk component is available a-la-carte (albeit
possibly with some dependencies, such as logging or configuration). A
lightweight framework that combines the most common components is also
[available](#framework).

# Design
Although Werk is not a framework and does not enforce a particular threading
model, it does have several high level design rules in order to meet its
performance requirements:
* I/O and most other system calls should be deferred from any critical path
thread into a background thread, and done asynchronously. Components that
require I/O should be designed to support this, while still allowing synchronous
execution when possible.
* In the same vein, `new` generally should not be called after initialization.
This has advantages besides avoiding a possible `sbrk()` call; for example, it
guarantees that the application will not fail after startup by running out of
memory.

<a name="components"></a>
# Components
Components below are presented in roughly the order that they depend upon each
other. For a standard set of basic components, look at the `ApplicationContext`
class.

### Math
To enable analytics (e.g. for profiling), a variety of signal processing and
statistical components are included:
* `ContinuousEma`: Calculate an EMA with a continuous value.
* `DiscreteDistribution`: Calculates statistics about a weighted set of ordered
values, including probability information.
* `DiscreteEma`: Calculate an EMA with discrete steps.
* `OrderStatistics`: Calculate fractiles and other order statistics on a set of
samples.
* `SimpleLinearRegression`: Calculate a simple linear regression from two sets
of samples.
* `SummaryStatistics` and `RangedSummaryStatistics`: Calculate basic summary
statistics (count, average, std-dev, and optionally min/max).
* `WeightedSummaryStatistics`: Calculate summary statistics on a weighted set
of samples.

### Profiling
In order to maintain performant code and identify outliers which require
remediation, a light-weight profiling implementation is provided.

`Profile`s keep track of the start and end times of a given sample, and then
insert the delta into an `OrderStatistics` instance. Once that instance has a
certain number of samples (default=100), the `OrderStatistics` is sampled at
various fractiles into a set `SummaryStatistics` instances, then cleared. This
allows for an O(1) implementation while still collecting information about the
50th, 75th, 90th, 95th, and 99th percentile times.

`Profile`s are named and held by a `ProfileManager` class which allows exporting
the summary statistics to JSON for upstream analysis.

### Background Thread
To keep latency low on the main application thread(s), a `BackgroundThread`
class offers a place to defer I/O from other components. A set of `Action`s is
run at a configurable interval. Although actions do not have a latency budget,
they must return as all multitasking is cooperative. To ensure performance,
background actions are automatically profiled by providing a `ProfileManager`
to the `BackgroundThread` upon instantiation.

Built-in actions include:
* `ActionQueue`: Queues other `Action`s to be run a single time -- useful for
queueing actions to be run on another thread.
* `ConditionalAction`: Executes another `Action` only if a `Latch` is set.
* `CounterAction`: Counts the number of times the action is executed.
* `IncrementCounterAction`: Increments a `Counter` (appropriate when a more
complex counter is needed than the simple one in `CounterAction`).
* `NullAction`: Does nothing (useful as a placeholder or for testing).
* `ResetAction`: Resets any component that has a `void reset()` method (`Latch`,
`Counter`, `Watchdog`, etc.).
* `SetLatchAction`: Sets a flag.
* `Watchdog`: Watches a flag and executes another `Action` if it does not get
before a predefined interval expires. This allows for deadlock detection and
many other behaviors to be run on the background thread.

### Logging
Logs are written in a machine-readable [JSON lines](http://jsonlines.org/)
format. They can either be written synchronously with `SyncLog` or
asynchronously with `AsyncLog` on a `BackgroundThread`.

### Configuration
A reloadable configuration system which can read from multiple `ConfigSource`s
(allowing integration with standard formats as well as custom and proprietary
ones) and propagate updates to a set of `Configurable` objects. As `Config`s are
read, the values are logged, allowing for future reconstruction.

### Commands
To allow for easy configuration of actions, and also user input, string command
lines can be accepted and parsed by a `CommandManager`, which will forward
execution to the appropriate `Command` instance. Command managers optionally
start with a set of default, standard commands.

Built-in commands include:
* `echo`, `warning`, `error` (`EchoCommand`): Logs the arguments at a certain
log level.
* `null` (`NullCommand`): Does nothing (useful as a placeholder for testing).
* `?`, `help`: Logs information about commands.
* `version`: Logs the version of Werk the application is running.
* `redo`: Runs the last command again.

A `CommandAction` helper class allows a `Command` to be run whenever an `Action`
is needed, opening up many opportunities for connecting components.

### Console
Consoles allow users to connect to a real time run and enter commands. Currently
the only available console is the `IpcConsoleServer` and `IpcConsoleClient` pair
which send commands over an IPC queue. The client application is built as
`bin/build/IpcConsoleClient`.

<a name="framework"></a>
# Framework
Althought Werk does not force the use of any particular threading model or set
of components, it does offer a default combination of the most common components
via the `ApplicationContext` class. Bootstrapping from a background thread and a
logger that goes to `stdout`, it loads configs, creates a file log, sets up
subsystems like commands and profiling, then runs some startup commands. An
optional main loop may then be run.

## Startup
On startup, the following configuration options are read:
* `Application.ConfigPaths`: Only available in the primary config, this is a
comma-separated list of other configs to load. Default none.
* `Application.LogPath`: Path to the actual log file. If not present or cannot
be opened, logs will go to stderr. Required.
* `Application.InstanceID`: ID of this instance of the application, for audit
trail purposes.
* `Application.ProfilesPath`: Path to profiling information JSON, written on
shutdown. Default none.
* `Application.BackgroundThreadInterval`:  The interval of the background thread,
in nanoseconds. Default 10ms.
* `Application.Debug`: Boolean indicating whether the application is in debug
mode (causing it to output additional information). Default false.
* `Application.Simulation`: Boolean indicating whether the application is a
simulation. Default false.
* `Application.RealTime`: Boolean indicating whether the application is running
in real time. Default true.
* `Application.StartupCommands`: Semicolon delimited list of command lines to
run on startup.
* `Application.IpcConsoleName`: The name of the IPC queue used for a console.
Default none.

## Run Time
If the optional main loop is used by calling `ApplicationContext::run()`, first
some configurations are read:
* `Application.WatchdogInterval`: The interval of the foreground thread watchdog.
* `Application.WatchdogAllowedMisses`: The number of times the foreground thread
can miss resetting the application watchdog.

Then a main loop is started which updates the curren time, runs a main `Action`,
then executes any defered `Action`s in the foreground queue and resets the
watchdog.

### Commands
The application context adds additional default commands:
* `app`: Logs information about the high level state of the application.
* `bg`: Logs information about background tasks.
* `logs`: Logs information about what logs are available.
* `reload`: Reloads the configuration.
* `quit`: Quits the application cleanly, running shutdown commands and actions.

## Shutdown
On shutdown it runs shutdown commands (read from `Application.ShutdownCommands`),
then shutdown actions which may be registered by any component, ensuring a clean
shutdown.