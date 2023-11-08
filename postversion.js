const fs = require('fs');

const package = JSON.parse(fs.readFileSync('package.json', { encoding: 'utf8' }));

[ 'main', 'browser' ].forEach(target => {
  package[target] = package[target].replace('dist/', '')
});

fs.writeFileSync('./dist/package.json', JSON.stringify(package, null, 2));

fs.copyFileSync('./LICENSE', './dist/LICENSE');
fs.copyFileSync('./README.md', './dist/README.md');
