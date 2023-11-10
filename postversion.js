/*
Check the following issue to avoid always being one version late in
dist/package.json (annoyingly, the one being published) :
https://github.com/sindresorhus/np/issues/516
Problem : package.json was copied into dist (from make dist) before the version
was bumped.
Here is an implementation of the fix suggested by will-stone at the end of the
issue, triggered by the postversion npm script lifecycle hook.
As specified by np's author, this workaround should be removed in the future and
the (soon-to-be-supported ?) 'exports' field from package.json should be used
instead to address this issue.
*/

const fs = require('fs');

const package = JSON.parse(fs.readFileSync('package.json', { encoding: 'utf8' }));

[ 'main', 'browser' ].forEach(target => {
  package[target] = package[target].replace('dist/', '')
});

fs.writeFileSync('./dist/package.json', JSON.stringify(package, null, 2));

fs.copyFileSync('./LICENSE', './dist/LICENSE');
fs.copyFileSync('./README.md', './dist/README.md');
