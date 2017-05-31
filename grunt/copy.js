module.exports =
{
    index:
    {
        files:
        [{
            expand: true,
            cwd: 'src/client',
            src: 'index.html',
            dest: '.build/'
        }]
    },
    libs:
    {
        files:
        [{
            expand: true,
            cwd: 'node_modules/systemjs/dist',
            src: 'system.js',
            dest: '.build/js/'
        },
        ]
    },
    filesDirectory:
    {
        files:
        [{
            expand: true,
            cwd: 'src/client',
            src: 'files/**',
            dest: '.build/'
        }]
    },
    styles:
    {
        files:
        [{
            expand: true,
            cwd: 'src/client/css',
            src: '*.css',
            dest: '.build/css/'
        }]
    },
    server:
    {
        files:
        [{
            expand: true,
            cwd: 'src/server/Release',
            src: 'server.exe',
            dest: '.build/'
        }]
    }
};