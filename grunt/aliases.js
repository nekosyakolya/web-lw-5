module.exports = 
{ 
'buildClient': ['clean:oldHashes', 'shell:spellChecker', 'tslint', 'ts', 'copy:libs', 
'copy:filesDirectory', 'copy:styles', 'systemjs', 'hashres', 'copy:index'], 
'buildServer': ['shell:closeServer', 'shell:buildServer', 'copy:server', 'shell:startServer'], 
'default': ['clean', 'buildServer', 'buildClient', 'open', 'watch'] 
};