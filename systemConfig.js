SystemJS.config({
    baseURL: '.build/js/',
    packages: {
        app: {
            format: 'register',
            defaultExtension: 'js'
        }
    },
    transpiler: 'typescript'
});
