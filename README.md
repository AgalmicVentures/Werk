
# Werk
Werk is a C++ library/framework for developing performance sensitive real-time
applications.

## Components
Rather than forcing an overarching framework which may not fit the users' needs,
each Werk component is available a-la-carte (albeit with some dependencies,
such as logging or configuration).

### Background Thread
To keep latency low on the main application thread(s), a `BackgroundThread`
class offers a place to defer I/O from other components. A set of `Action`s is
run at a configurable interval.

Pre-built actions include:
* `CounterAction`: Counts the number of times the action is executed.
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

Pre-built commands include:
* `EchoCommand`: Logs the arguments at a certain log level.
* `NullCommand`: Does nothing (useful as a placeholder for testing).

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

## Utilities
A variety of utilities are used to build the above components.

### Math
A wide variety of math components are included:
* `ContinuousEma`: Calculate an EMA with a continuous value.
* `DiscreteEma`: Calculate an EMA with discrete steps.
* `OrderStatistics`: Calculate fractiles and other order statistics on a set of
samples.
* `SimpleLinearRegression`: Calculate a simple linear regression from two sets
of samples.
* `SummaryStatistics` and `RangedSummaryStatistics`: Calculate basic summary
statistics (count, average, std-dev, and optionally min/max).
