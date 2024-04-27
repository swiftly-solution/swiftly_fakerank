#include <swiftly/swiftly.h>
#include <swiftly/server.h>
#include <swiftly/database.h>
#include <swiftly/commands.h>
#include <swiftly/configuration.h>
#include <swiftly/logger.h>
#include <swiftly/timers.h>
#include <swiftly/gameevents.h>

Server *server = nullptr;
PlayerManager *g_playerManager = nullptr;
std::map<int, int> ranksTab;
Database *db = nullptr;
Commands *commands = nullptr;
Configuration *config = nullptr;
Logger *logger = nullptr;
Timers *timers = nullptr;
int enabled = 0;
int type = 0;

void OnProgramLoad(const char *pluginName, const char *mainFilePath)
{
    Swiftly_Setup(pluginName, mainFilePath);

    server = new Server();
    g_playerManager = new PlayerManager();
    commands = new Commands(pluginName);
    config = new Configuration();
    logger = new Logger(mainFilePath, pluginName);
    timers = new Timers();
}

int FetchPoints(Player *player)
{
    return player->vars->Get<int>("points");
}

void OnPluginStart()
{

    int pluginstate = GetPluginState("swiftly_rank");
    if (pluginstate == 0)
    {
        server->ExecuteCommand("sw plugins unload swiftly_fakerank");
    }

    ranksTab = {
        {config->Fetch<int>("swiftly_ranks.Ranks.Silver1"), 1},
        {config->Fetch<int>("swiftly_ranks.Ranks.Silver2"), 2},
        {config->Fetch<int>("swiftly_ranks.Ranks.Silver3"), 3},
        {config->Fetch<int>("swiftly_ranks.Ranks.Silver4"), 4},
        {config->Fetch<int>("swiftly_ranks.Ranks.Silver5"), 5},
        {config->Fetch<int>("swiftly_ranks.Ranks.SEM"), 6},
        {config->Fetch<int>("swiftly_ranks.Ranks.GN2"), 7},
        {config->Fetch<int>("swiftly_ranks.Ranks.GN3"), 8},
        {config->Fetch<int>("swiftly_ranks.Ranks.GN4"), 9},
        {config->Fetch<int>("swiftly_ranks.Ranks.GN4"), 10},
        {config->Fetch<int>("swiftly_ranks.Ranks.MG1"), 11},
        {config->Fetch<int>("swiftly_ranks.Ranks.MG2"), 12},
        {config->Fetch<int>("swiftly_ranks.Ranks.MGE"), 13},
        {config->Fetch<int>("swiftly_ranks.Ranks.DMG"), 14},
        {config->Fetch<int>("swiftly_ranks.Ranks.LE"), 15},
        {config->Fetch<int>("swiftly_ranks.Ranks.LEM"), 16},
        {config->Fetch<int>("swiftly_ranks.Ranks.SMFC"), 17},
        {config->Fetch<int>("swiftly_ranks.Ranks.Global"), 18}};

    enabled = config->Fetch<int>("swiftly_ranks.FakeRank.Enabled");
    type = config->Fetch<int>("swiftly_ranks.FakeRank.Type");
}

bool OnPlayerPrePostThink(Player *player)
{
    return true;
}

void OnPlayerPostThink(Player *player)
{
    int points = FetchPoints(player);
    int rank = 0;

    for (auto it = ranksTab.rbegin(); it != ranksTab.rend(); ++it)
    {
        if (points >= it->first)
        {
            rank = it->second;
            break;
        }
    }

    if (enabled == 1 && type == 1)
    {
        player->SetCompetitiveRankType(12);
    }
    else if (enabled == 1 && type == 2)
    {
        player->SetCompetitiveRankType(11);
    }
    else if (enabled == 1 && type == 3)
    {
        player->SetCompetitiveRankType(7);
    }

    player->SetCompetitiveRanking(rank);
    player->SetCompetitiveWins(11);
}

void OnPluginStop()
{
}

const char *GetPluginAuthor()
{
    return "";
}

const char *GetPluginVersion()
{
    return "1.0.0";
}

const char *GetPluginName()
{
    return "swiftly_fakerank";
}

const char *GetPluginWebsite()
{
    return "";
}