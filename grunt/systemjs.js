module.exports = {
    options: {
        sfx: true,
        minify: true,
        baseURL: "",
        sourceMaps: false,
        configFile: 'systemConfig.js',
        build: {
            mangle: false
        }
    },
    ts: {
        src: 'main.js',
        dest: '.build/js/main.min.js'
    }
};