
#include <boost/test/unit_test.hpp>

#include "Werk/Commands/CommandManager.hpp"
#include "Werk/Logging/Logger.hpp"

BOOST_AUTO_TEST_SUITE(CommandManagerTest)

BOOST_AUTO_TEST_CASE(TestDefaultCommands)
{
    werk::Clock c;
    werk::SyncLogger log(&c);
    werk::CommandManager commandManager(&log);

    BOOST_REQUIRE(commandManager.execute("null"));
    BOOST_REQUIRE(commandManager.execute("echo Echo  test"));

    BOOST_REQUIRE(!commandManager.execute("asdf"));
}

BOOST_AUTO_TEST_SUITE_END()
