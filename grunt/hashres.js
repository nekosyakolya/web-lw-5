module.exports = {
    options: {
        fileNameFormat: '${name}[${hash}].${ext}'
    },
    prod: {
        src: ['.build/js/*.js', '.build/css/*.css'],
        dest: 'src/client/index.html'
    }
};