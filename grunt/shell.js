module.exports =
{
    spellChecker:
    {
        command: 'cspell src/client/ts/*.ts'
    },
    buildServer:
    {
        command: 'buildServer.bat'
    },
    startServer:
    {
        command: 'start server.exe',
        options:
        {
            async: true,
            execOptions:
            {
                cwd: '.build'
            }
        }
    },
    closeServer:
    {
        command: 'start taskkill /im server.exe'
    }
};
