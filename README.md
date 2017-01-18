# Werk
Werk is a C++ library of components for developing performance-sensitive,
real-time applications. Rather than forcing an overarching framework which may
not fit the users' needs, each Werk component is available a-la-carte (albeit
possibly with some dependencies, such as logging or configuration). Most output
is in JSON to be easily machine-readable.

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

# Components
Components below are presented in roughly the order they would need to be
instantiated; that is, components lower on the list may depend upon those above
them. For a standard set of basic components, look at the `ApplicationContext`
class.

### Background Thread
To keep latency low on the main application thread(s), a `BackgroundThread`
class offers a place to defer I/O from other components. A set of `Action`s is
run at a configurable interval.

Built-in actions include:
* `CounterAction`: Counts the number of times the action is executed.
* `LatchAction`: Sets a flag executed.
* `NullAction`: Does nothing (useful as a placeholder or for testing).
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
* `EchoCommand`: Logs the arguments at a certain log level.
* `HelpCommand`: Logs help for all other registered commands.
* `NullCommand`: Does nothing (useful as a placeholder for testing).
* `ReloadConfigCommand`: Flags a configuration to be reloaded.
* `QuitCommand`: Quits the program cleanly.

A `CommandAction` helper class allows a `Command` to be run whenever an `Action`
is needed, opening up many opportunities for connecting components.

### Profiling
In order to maintain performant code and identify outliers which require
remediation, a light-weight profiling implementation is provided. `Profile`s
keep track of the start and end times of a given sample, and then insert the
delta into an `OrderStatistics` instance. Once that instance has a certain
number of samples (default=100), the `OrderStatistics` is sampled at various
fractiles into a set `SummaryStatistics` instances, then cleared. This allows
for an O(1) implementation while still collection some fractile information.

`Profile`s are named and held by a `ProfileManager` class which allows exporting
the summary statistics to JSON for upstream analysis.

# Utilities

### Math
A wide variety of math components are included:
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
