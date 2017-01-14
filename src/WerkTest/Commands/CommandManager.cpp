
#include <boost/test/unit_test.hpp>

#include "Werk/Commands/CommandManager.hpp"
#include "Werk/Logging/Log.hpp"

BOOST_AUTO_TEST_SUITE(CommandManagerTest)

BOOST_AUTO_TEST_CASE(TestDefaultCommands)
{
    werk::Clock c;
    werk::SyncLog log("stdout", &c);
    werk::CommandManager commandManager(&log);

    BOOST_REQUIRE(commandManager.execute("null"));
    BOOST_REQUIRE(commandManager.execute("echo Echo  test"));

    BOOST_REQUIRE(!commandManager.execute("asdf"));

    werk::CommandAction *action = commandManager.newCommandAction("echoAction", "echo Action!");
    action->execute();
    action->execute();

    BOOST_REQUIRE(commandManager.newCommandAction("missingAction", "asdf") == nullptr);
}

BOOST_AUTO_TEST_SUITE_END()
