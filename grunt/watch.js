module.exports = {
    script: {
        files: ['src/client/css/*.css', 'src/client/ts/*.ts'],
        tasks: 'buildClient'
    },
    server: {
        files: ['src/server/**/*.cpp', 'src/server/**/*.c', 'src/server/**/*.h'],
        tasks: 'buildServer'
    },
    html: {
        options: {
            livereload: true
        },
        files: 'src/client/index.html',
        tasks: 'copy:index'
    }
};